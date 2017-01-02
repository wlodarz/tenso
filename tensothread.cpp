
#include <QtCore>
#include <QtDebug>
#include <QQmlEngine>
#include <QQuickItem>
#include <QList>
#include <QQmlContext>
#include <QTimer>
#include <QApplication>

#include <stdlib.h>

#include <phidget21.h>

#include "config.hpp"

#include "tensosensor.hpp"
#include "tensothread.hpp"
#include "tensometer.hpp"
#include "stepperengine.hpp"
#include "tensoconfig.hpp"

#include "values.hpp"

int TensoThread::init(TensoSensor *sensor)
{
	test_counter = 0;

  	m_config = new TensoConfig();
	m_tensometer = new Tensometer();
	m_stepperengine = new StepperEngine();

	m_sensor = sensor;

	if (loadConfig("config.txt") < 0) {
#if !TEST
		return -1;
#endif
	}

	if (m_stepperengine->init() < 0) {
#if !TEST_ENGINE
		qDebug() << "stepper";
		return -2;
#endif
	}
	if (m_tensometer->init() < 0) {
#if !TEST_TENSOMETER
		qDebug() << "tensometer";
		return -3;
#endif
	}

	configure();
	start();

	return 0;
}

int TensoThread::loadConfig(QString configFile)
{
  if (m_config->init(configFile) < 0) {
	qDebug() << "load config failed";
	return -1;
  }
  qDebug() << "load config ok";
  return 0;
}

int TensoThread::configure()
{
	m_stepperengine->setDirection(m_config->getConfigValueInt(DIRECTION_KEY));
	m_stepperengine->setLoosenDelta(m_config->getConfigValueInt(LENGTH_LOOSEN_KEY));

	return 0;
}

void TensoThread::run()
{
  qDebug() << "TensoThread::run";

  QTimer counting_timer;
  connect(&counting_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
  counting_timer.start(1000);

  QTimer measure_timer;
  connect(&measure_timer, SIGNAL(timeout()), this, SLOT(onControlTimeout()));
  measure_timer.start(100);

  //m_sensor->setStartMeasureProperty(startFlag);
  m_sensor->setTimerType(TIMER_TYPE_NONE);

  exec();
}

void TensoThread::onTimeout()
{
	//qDebug() << "TensoThread::onTimeout() ";
	m_sensor->onTimeout();
}

void TensoThread::testFunction()
{
	test_counter++;
#if TEST_TENSOMETER
	int v = m_config->getConfigValueInt(LENGTH_100CM_KEY) / 2;
	//int force = m_stepperengine->getCurrentPosition() * m_config->getConfigValueInt(FORCE_MAX_KEY) / v;
	int force = test_counter * m_config->getConfigValueInt(FORCE_MAX_KEY) / v;

	qDebug() << "test_counter " << test_counter;
	m_tensometer->setForceValue(force);
#endif

	return;
}

void TensoThread::onControlTimeout()
{
#if TEST
	testFunction();
#endif
	int force = m_tensometer->getForceValue();
	qDebug() << "force = " << force;
	m_sensor->setCurrentForceProperty(force);
	m_sensor->setCurrentLengthProperty(m_stepperengine->getCurrentPosition());

	// draw only if required
	if (m_sensor->measureStartedProperty()) m_sensor->setMeasureUpdatedProperty(1);

	//qDebug() << "TensoThread::onControlTimeout() ";
	if (m_sensor->operationCompletedProperty() == 1) return;

	if (m_sensor->forceStopProperty() == 1) {
		qDebug() << "TensoThread::onControlTimeout() : forceStopProperty()";
		m_stepperengine->stop();

		m_sensor->setForceStopProperty(0);
		m_sensor->setOperationProperty(SENSOR_OPERATION_IDLE);
		m_sensor->setSubOperationProperty(NONE_SUBOPERATION);
		m_sensor->setOperationCompletedProperty(0);

		test_counter = 0;

		return;
	}

	switch (m_sensor->operationProperty()) {
		case SENSOR_OPERATION_IDLE:
		{
			qDebug() << "SENSOR_OPERATION_IDLE";
			m_sensor->setMeasureStartedProperty(0);
		}
		break;

		case SENSOR_OPERATION_ZEROING:
		{
			qDebug() << "SENSOR_OPERATION_ZEROING";
			switch (m_sensor->subOperationProperty()) {
				case ZEROING_SUBOPERATION_MOTOROFF:
				{
					qDebug() << "State motoroff";
					m_stepperengine->off();
					m_sensor->setOperationCompletedProperty(1);
					m_sensor->setZeroingLooseFlag(0);
				}
				break;
				case ZEROING_SUBOPERATION_TILL40KG:
				{
					int force;

					qDebug() << "State till40kg";
					if (m_sensor->getSubOperationStarted() == 0) {
						qDebug() << "State till40kg - starting";
						m_stepperengine->setCurrentPosition(0);
						m_stepperengine->setTargetPosition(m_config->getConfigValueInt(LENGTH_100CM_KEY));
						m_stepperengine->on();
						m_stepperengine->start();
						m_sensor->setSubOperationStarted(1);
						test_counter = 0;
					} else {


						force = m_tensometer->getForceValue();
						qDebug() << "SENSOR_SUBOPERATION_ZEROING_TILL40KG force " << force << " till " << m_config->getConfigValueInt(FORCE_MAX_KEY);


						if (m_sensor->getZeroingLooseFlag() == 0) {
							qDebug() << "looseFlag = 0";
							if ( force > m_config->getConfigValueInt(FORCE_MAX_KEY)) {
								qDebug() << "FORCE REACHED";
								// stop engine, set target position to current - "2cm"
								m_stepperengine->stop();
								m_stepperengine->loose();
								m_sensor->setZeroingLooseFlag(1);
							}
						} else {
							qDebug() << "looseFlag = 1";
							if (m_stepperengine->isEngineStopped()) {
								qDebug() << "engineStopped";
								m_sensor->setOperationCompletedProperty(1);
								m_stepperengine->stop();
								m_stepperengine->setCurrentPosition(0);
							}
						}
					}
				}
				break;
				default:
					qDebug() << "Unknown State";
			}
		}
		break;


		case SENSOR_OPERATION_TEST1:
		{
			qDebug() << "SENSOR_OPERATION_TEST1";
			switch (m_sensor->subOperationProperty()) {
				case TEST1_SUBOPERATION_20CM:
				{
					qDebug() << "State 20cm";
					if (m_sensor->getSubOperationStarted() == 0) {
						qDebug() << "test1 - 20cm suboperation started";
						m_stepperengine->on();
						m_stepperengine->setTargetPosition(m_config->getConfigValueInt(LENGTH_20CM_KEY));
						m_stepperengine->start();
						m_sensor->setSubOperationStarted(1);
					} else {
						qDebug() << "test1 - 20cm suboperation already in progress";
						if (m_stepperengine->getCurrentPosition() >= m_config->getConfigValueInt(LENGTH_20CM_KEY)) {
							qDebug() << "20CM reached";
							m_stepperengine->stop();
							m_sensor->setOperationCompletedProperty(1);
						}
					}
				}
				break;
				case TEST1_SUBOPERATION_MEASURE_LEN:
				{
					qDebug() << "State measure";
					if (m_sensor->getSubOperationStarted() == 0) {
						qDebug() << "test1 - measure len suboperation started";
						m_stepperengine->setTargetPosition(m_config->getConfigValueInt(LENGTH_100CM_KEY));
						m_stepperengine->start();
						m_sensor->setSubOperationStarted(1);
						// start paint here
						m_sensor->setMeasureStartedProperty(1);
						test_counter = 0;
					} else {
						qDebug() << "test1 - measure len suboperation already in progress";
						if (m_tensometer->getForceValue() > m_config->getConfigValueInt(FORCE_MIN_KEY)) {
							m_stepperengine->stop();
							qDebug() << "test1 - measure len force reached";
							m_sensor->setOperationCompletedProperty(1);
						}
					}
				}
				break;
				case TEST1_SUBOPERATION_100CM:
				{
					qDebug() << "State 100cm";
					if (m_sensor->getSubOperationStarted() == 0) {
						qDebug() << "test1 - go 100cm suboperation started";
						m_stepperengine->setTargetPosition(m_config->getConfigValueInt(LENGTH_100CM_KEY));
						m_stepperengine->start();
						m_sensor->setSubOperationStarted(1);
					} else {
						qDebug() << "test1 - go 100cm suboperation already in progress";
						if (m_stepperengine->getCurrentPosition() >= m_config->getConfigValueInt(LENGTH_100CM_KEY)) {
							m_stepperengine->stop();
							qDebug() << "test1 - measure 100cm reached";
							m_sensor->setOperationCompletedProperty(1);
						}
					}
				}
				break;
				default:
				{
				}
			}
		}
		break;

		case SENSOR_OPERATION_TEST2:
		{
		}
		break;

		case SENSOR_OPERATION_PARKING:
		{
			if (m_sensor->getSubOperationStarted() == 0) {
				m_stepperengine->stop();
				m_sensor->setSubOperationStarted(1);
				m_stepperengine->setTargetPosition(0);
			} else {
				if (m_stepperengine->isEngineStopped()) {
					m_sensor->setParkingProperty(0);
					m_sensor->setOperationProperty(SENSOR_OPERATION_IDLE);
					m_sensor->setSubOperationProperty(NONE_SUBOPERATION);
				}
			}
		}
		break;

		default:
		{
			qDebug() << "Unknown operaton"; 
		}
	}
}

