
#include <QtCore>
#include <QtDebug>
#include <QQmlEngine>
#include <QQuickItem>
#include <QList>
#include <QQmlContext>
#include <QTimer>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QRegExp>

#include <stdlib.h>

#include <phidget21.h>

#include "config.hpp"

#include "tensosensor.hpp"
#include "tensothread.hpp"
#include "tensometer.hpp"
#include "stepperengine.hpp"
#include "tensoconfig.hpp"

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

	if (loadSteps2CmTable("steps2cm.txt") < 0) {
		qDebug() << "Can't load step2cm.txt file";
		return -1;
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

int TensoThread::parseConvertLine(QString line)
{
	QString key, value;
	QRegExp rx("([0-9.]+),([\\-0-9.]+)");

	qDebug() << "parsing line " << line;
	if (rx.indexIn(line) < 0) {
		qDebug() << "line " << line << " not matched";
		return -1;
	}
	key = rx.cap(1);
	value = rx.cap(2);

	m_sensor->addStep2CmTableEntry(key.toInt(), value.toFloat());

	return 0;
}

int TensoThread::loadSteps2CmTable(QString fileFileName)
{
	unsigned int steps = 0;
	float cm = 0.0;

	m_sensor->addStep2CmTableEntry(steps, cm);

	QFile configFile(fileFileName);
	if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) return -1;

	QTextStream in(&configFile);
	QString line = in.readLine();

	while(!line.isNull()) {
		if (parseConvertLine(line) < 0) {
			qDebug() << "bad line " << line;
		}
		line = in.readLine();
	}

	return 0;
}

int TensoThread::configure()
{
	m_stepperengine->setDirection(m_config->getConfigInt(DIRECTION_KEY));
	m_stepperengine->setLoosenDelta(m_config->getConfigInt(LENGTH_LOOSEN_KEY));
	m_stepperengine->setMaxSpeedDiv(m_config->getConfigInt(ENGINE_MAXSPEEDDIV_KEY));
	m_stepperengine->setMinSpeedDiv(m_config->getConfigInt(ENGINE_MINSPEEDDIV_KEY));

	m_sensor->setMaxForceProperty(m_config->getConfigInt(FORCE_MAX_KEY));
	m_sensor->setMaxLengthProperty(m_config->getConfigInt(LENGTH_MAX_KEY));

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
	int v = m_config->getConfigInt(LENGTH_100CM_KEY) / 2;
	//int force = m_stepperengine->getCurrentPosition() * m_config->getConfigInt(FORCE_MAX_KEY) / v;
	int force = test_counter * m_config->getConfigInt(FORCE_MAX_KEY) / v;

	//qDebug() << "test_counter " << test_counter;
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
	m_sensor->setCurrentForceProperty(force);
	int position = m_stepperengine->getCurrentPosition();
	m_sensor->setCurrentLengthProperty(position);

	// draw only if required
	if (m_sensor->measureStartedProperty()) {
		m_sensor->setMeasureUpdatedProperty(1);
		m_sensor->updateMeasure(position, force);
	}

	//qDebug() << "TensoThread::onControlTimeout() " << m_sensor->operationProperty() << " completed " << m_sensor->operationCompletedProperty();
	if (m_sensor->operationCompletedProperty() == 1) return;

	switch (m_sensor->operationProperty()) {
		case TensoSensor::Operations::SENSOR_OPERATION_IDLE:
		{
			//qDebug() << "SENSOR_OPERATION_IDLE";
			m_sensor->setMeasureStartedProperty(0);
		}
		break;

		case TensoSensor::Operations::SENSOR_OPERATION_STOP:
		{
			qDebug() << "TensoThread::onControlTimeout() : forceStopProperty()";
			m_stepperengine->stop();

			m_sensor->setOperationProperty(TensoSensor::Operations::SENSOR_OPERATION_IDLE);
			m_sensor->setSubOperationProperty(TensoSensor::Suboperations::NONE_SUBOPERATION);
			m_sensor->setOperationCompletedProperty(0);

			test_counter = 0;

			float const1 = m_config->getConfigFloat(CONFIG_ROTATION_CONST1_KEY);
			float const2 = m_config->getConfigFloat(CONFIG_ROTATION_CONST2_KEY);
			float max_length = m_sensor->maxMeasuredLengthProperty(); // 310.0;
			float min_length = m_sensor->minMeasuredLengthProperty(); // 33.0;
			float turns = m_sensor->calculateTurns(const1, const2, max_length, min_length);
			qDebug() << "Turns " << turns;
		}
		break;

		case TensoSensor::Operations::SENSOR_OPERATION_MOVE:
		{
			operationMove();
		}
		break;

		case TensoSensor::Operations::SENSOR_OPERATION_ZERO:
		{
			qDebug() << "SENSOR_OPERATION_ZERO";

			/* set current engine position to zero */
			m_stepperengine->setCurrentPosition(0);

			/* go IDLE state */
			m_sensor->setOperationCompletedProperty(1);
			m_sensor->setOperationProperty(TensoSensor::Operations::SENSOR_OPERATION_IDLE);
			m_sensor->setSubOperationProperty(TensoSensor::Suboperations::NONE_SUBOPERATION);
		}
		break;

		case TensoSensor::Operations::SENSOR_OPERATION_MEASURE1:
		{
			operationMeasure1();
		}
		break;

		case TensoSensor::Operations::SENSOR_OPERATION_MEASURE2:
		{
			operationMeasure2();
		}
		break;

		case TensoSensor::Operations::SENSOR_OPERATION_PARK:
		{
			adaptSpeed();

			if (m_sensor->getSubOperationStarted() == 0) {
				// stop should be removed???
				m_stepperengine->stop();
				m_sensor->setSubOperationStarted(1);
				m_stepperengine->setTargetPosition(0);
				// setting velocity should be removed??? - velocity should came from adaptSpeed only!?!?!
				m_stepperengine->setVelocityLimit(50);
			} else {
				if (m_stepperengine->isEngineStopped()) {
					m_sensor->setOperationProperty(TensoSensor::Operations::SENSOR_OPERATION_IDLE);
					m_sensor->setSubOperationProperty(TensoSensor::Suboperations::NONE_SUBOPERATION);
				}
			}
		}
		break;

		case TensoSensor::Operations::SENSOR_OPERATION_CALIBRATE:
		{
			operationCalibrate();
		}
		break;

		default:
		{
			qDebug() << "Unknown operaton"; 
		}
	}
}

void TensoThread::adaptSpeed()
{
	// limit speed according to current force level
	int current_force_level = m_tensometer->getForceValue() * 100;
	current_force_level /= m_config->getConfigInt(FORCE_MAX_KEY);

	if (current_force_level < m_config->getConfigInt(ENGINE_FORCELEVEL1_KEY)) {
		m_stepperengine->setVelocityLimit(m_config->getConfigInt(ENGINE_SPEED1_KEY));
	} else if (current_force_level < m_config->getConfigInt(ENGINE_FORCELEVEL2_KEY)) {
		m_stepperengine->setVelocityLimit(m_config->getConfigInt(ENGINE_SPEED2_KEY));
	} else if (current_force_level <= m_config->getConfigInt(ENGINE_FORCELEVEL3_KEY)) {
		m_stepperengine->setVelocityLimit(m_config->getConfigInt(ENGINE_SPEED3_KEY));
	} else if (current_force_level <= m_config->getConfigInt(ENGINE_FORCELEVEL4_KEY)) {
		m_stepperengine->setVelocityLimit(m_config->getConfigInt(ENGINE_SPEED4_KEY));
	} else if (current_force_level <= m_config->getConfigInt(ENGINE_FORCELEVEL5_KEY)) {
		m_stepperengine->setVelocityLimit(m_config->getConfigInt(ENGINE_SPEED5_KEY));
	}
}

void TensoThread::operationMove()
{
	int move_val = m_sensor->moveDirectionProperty();
	int move_sign = m_sensor->moveDirectionProperty() >= 0 ? 1 : -1;
	int engine_speed_val = 0;

	qDebug() << "operationMove";

	if (m_sensor->getSubOperationStarted() == 0) {
		m_sensor->m_last_move_val = 0;
		m_sensor->setSubOperationStarted(1);
	}

	if (move_val == m_sensor->m_last_move_val) return;
	qDebug() << "engine_speed changed";

	switch (abs(move_val)) {
		case 1:
			engine_speed_val = m_config->getConfigInt(ENGINE_SPEED5_KEY);
			//engine_speed_val = m_config->getConfigInt(ENGINE_MAXSPEEDDIV_KEY);
			break;
		case 2:
			engine_speed_val = m_config->getConfigInt(ENGINE_SPEED3_KEY);
			//engine_speed_val = m_config->getConfigInt(ENGINE_MAXSPEEDDIV_KEY);
			break;
		case 3:
			engine_speed_val = m_config->getConfigInt(ENGINE_SPEED1_KEY);
			//engine_speed_val = m_config->getConfigInt(ENGINE_MAXSPEEDDIV_KEY);
			break;
		default:
			engine_speed_val = 0;
			break;
	}

	qDebug() << "engine_speed " << engine_speed_val;

	if (engine_speed_val == 0) {
		qDebug() << "STOP";
		m_stepperengine->stop();
	} else if (move_sign > 0) {
		qDebug() << "MOVING till MAX";
		if (m_sensor->m_last_move_val == 0) {
			m_stepperengine->on();
			m_stepperengine->setTargetPosition(m_config->getConfigInt(LENGTH_MAX_KEY));
			m_stepperengine->start();
		}
		m_stepperengine->setVelocityLimit(engine_speed_val);
	} else if (move_sign < 0) {
		qDebug() << "MOVING till ZERO";
		if (m_sensor->m_last_move_val == 0) {
			m_stepperengine->on();
			//m_stepperengine->setTargetPosition(0);
			m_stepperengine->setTargetPosition(-1 * m_config->getConfigInt(LENGTH_MAX_KEY));
			m_stepperengine->start();
		}
		m_stepperengine->setVelocityLimit(engine_speed_val);
	}

	m_sensor->m_last_move_val = move_val;
}

void TensoThread::operationMeasure1()
{
	switch (m_sensor->subOperationProperty()) {
		case TensoSensor::Suboperations::MEASURE1_SUBOPERATION_TILLMAXFORCE:
			{
				adaptSpeed();

				qDebug() << "Till Max Force";
				if (m_sensor->getSubOperationStarted() == 0) {
					qDebug() << "measure1 - till max force suboperation started";
					m_stepperengine->on();
					m_stepperengine->setTargetPosition(m_config->getConfigInt(LENGTH_MAX_KEY));
					m_stepperengine->start();
					m_sensor->setSubOperationStarted(1);
					m_sensor->setMeasureStartedProperty(1);
					m_sensor->setMeasureIndexProperty(0);
					test_counter = 0;
					m_stepperengine->setVelocityLimit(m_config->getConfigInt(ENGINE_SPEED1_KEY));
					m_sensor->setMeasurePhaseProperty(TensoSensor::MeasurePhases::MEASURE_PHASE_UP);
				} else {
					qDebug() << "measure1 - till max force suboperation already in progress";

					if (m_tensometer->getForceValue() >= m_config->getConfigInt(FORCE_MAX_KEY)
							|| m_stepperengine->getCurrentPosition() >= m_config->getConfigInt(LENGTH_MAX_KEY)) {
						qDebug() << "till max force reached or lenght_full reached";
						m_stepperengine->stop();
						m_sensor->setSubOperationStarted(0);
						m_sensor->setSubOperationProperty(TensoSensor::Suboperations::MEASURE1_SUBOPERATION_WAIT_1MIN);

						float work = m_sensor->calculateWork(m_sensor->getMeasureUpIndex());
						m_sensor->setCalculatedWorkUpProperty(work);
						/*
						qDebug() << "WorkUp = " << m_sensor->calculatedWorkUpProperty();
						qDebug() << "WorkHold = " << m_sensor->calculatedWorkHoldProperty();
						qDebug() << "WorkDown = " << m_sensor->calculatedWorkDownProperty();
						*/
					}
				}
			}
			break;
		case TensoSensor::Suboperations::MEASURE1_SUBOPERATION_WAIT_1MIN:
			{
				qDebug() << "Wait 1min";
				if (m_sensor->getSubOperationStarted() == 0) {
					m_sensor->setSubOperationStarted(1);
					m_sensor->setSecondsCounterProperty(m_config->getConfigInt(MEASURE1_HOLDTIME_KEY));
					m_sensor->setMeasurePhaseProperty(TensoSensor::MeasurePhases::MEASURE_PHASE_HOLD);
				} else {
					if (m_sensor->secondsCounterProperty() == 0) {
						m_sensor->setSubOperationProperty(TensoSensor::Suboperations::MEASURE1_SUBOPERATION_TILLMINFORCE);
						m_sensor->setSubOperationStarted(0);
					}
				}
			}
			break;

		case TensoSensor::Suboperations::MEASURE1_SUBOPERATION_TILLMINFORCE:
			{
				qDebug() << "Till Min Force";

				adaptSpeed();

				if (m_sensor->getSubOperationStarted() == 0) {
					qDebug() << "measure1 - till min force suboperation started";
					m_stepperengine->on();
					m_stepperengine->setTargetPosition(0);
					m_stepperengine->start();
					m_sensor->setSubOperationStarted(1);
					m_sensor->setMeasureIndexProperty(0);
					test_counter = 0;
					m_stepperengine->setVelocityLimit(m_config->getConfigInt(ENGINE_SPEED5_KEY));
					m_sensor->setMeasurePhaseProperty(TensoSensor::MeasurePhases::MEASURE_PHASE_DOWN);
				} else {
					qDebug() << "measure1 - till min force suboperation already in progress";

					if (m_tensometer->getForceValue() <= m_config->getConfigInt(FORCE_MIN_KEY)
							|| m_stepperengine->getCurrentPosition() <= 0) {
						qDebug() << "till min force reached or 0 reached";
						m_stepperengine->stop();
						m_sensor->setOperationCompletedProperty(1);
						m_sensor->setMeasureStartedProperty(0);
						m_sensor->setSubOperationStarted(0);
						m_sensor->setOperationProperty(TensoSensor::Operations::SENSOR_OPERATION_IDLE);
						m_sensor->setSubOperationProperty(TensoSensor::Suboperations::NONE_SUBOPERATION);

						float work = m_sensor->calculateWork(m_sensor->getMeasureDownIndex());
						m_sensor->setCalculatedWorkDownProperty(work);
						qDebug() << "POWER = " << m_sensor->calculatedWorkDownProperty();
					}
				}
			}
			break;

		default:
			{
			}
	}
}

void TensoThread::operationMeasure2()
{
	switch (m_sensor->subOperationProperty()) {
		case TensoSensor::Suboperations::MEASURE2_SUBOPERATION_TILLSTARTMEASUREFORCE:
			{
				adaptSpeed();

				qDebug() << "Till StartMeasure Force";
				if (m_sensor->getSubOperationStarted() == 0) {
					qDebug() << "measure2 - till start force suboperation started";
					m_stepperengine->on();
					m_stepperengine->setTargetPosition(m_config->getConfigInt(LENGTH_MAX_KEY));
					m_stepperengine->start();
					m_sensor->setSubOperationStarted(1);
					m_sensor->setMeasureIndexProperty(0);
					m_sensor->setMeasureStartedProperty(0);
					test_counter = 0;
					m_stepperengine->setVelocityLimit(m_config->getConfigInt(ENGINE_SPEED1_KEY));
				} else {
					if (m_tensometer->getForceValue() >= m_config->getConfigInt(MEASURE2_MINFORCE_KEY)) {
						m_sensor->setSubOperationStarted(0);
						m_sensor->setSubOperationProperty(TensoSensor::Suboperations::MEASURE2_SUBOPERATION_TILLMAXFORCE);
					}
				}
			}
			break;
		case TensoSensor::Suboperations::MEASURE2_SUBOPERATION_TILLMAXFORCE:
			{
				adaptSpeed();

				qDebug() << "Till Max Force";
				if (m_sensor->getSubOperationStarted() == 0) {
					qDebug() << "measure2 - till max force suboperation started";
					m_sensor->setSubOperationStarted(1);
					m_sensor->setMeasureStartedProperty(1);
					m_sensor->setMeasureIndexProperty(0);
					test_counter = 0;
					m_sensor->setMeasurePhaseProperty(TensoSensor::MeasurePhases::MEASURE_PHASE_UP);
				} else {
					qDebug() << "measure2 - till max force suboperation already in progress";

					if (m_tensometer->getForceValue() >= m_config->getConfigInt(MEASURE2_HOLDFORCE_KEY)
							|| m_stepperengine->getCurrentPosition() >= m_config->getConfigInt(LENGTH_MAX_KEY)) {
						qDebug() << "till max force reached or lenght_full reached";
						m_stepperengine->stop();
						m_sensor->setSubOperationStarted(0);
						m_sensor->setSubOperationProperty(TensoSensor::Suboperations::MEASURE2_SUBOPERATION_HOLDMAXFORCE);

						float work = m_sensor->calculateWork(m_sensor->getMeasureUpIndex());
						m_sensor->setCalculatedWorkUpProperty(work);
						qDebug() << "WorkUp = " << m_sensor->calculatedWorkUpProperty();
					}
				}
			}
			break;
		case TensoSensor::Suboperations::MEASURE2_SUBOPERATION_HOLDMAXFORCE:
			{
				qDebug() << "Hold Max Force";
				if (m_sensor->getSubOperationStarted() == 0) {

					/* start of the operation */
					m_sensor->setSubOperationStarted(1);
					m_sensor->setSecondsCounterProperty(m_config->getConfigInt(MEASURE2_HOLDTIME_KEY));
					m_sensor->setMeasureIndexProperty(0);
					m_sensor->setMeasurePhaseProperty(TensoSensor::MeasurePhases::MEASURE_PHASE_HOLD);
				} else {

					/* operation handler */
					if (m_sensor->secondsCounterProperty() == 0) {

						/* operation finished */
						m_sensor->setSubOperationProperty(TensoSensor::Suboperations::MEASURE2_SUBOPERATION_TILLMINFORCE);
						m_sensor->setSubOperationStarted(0);
						float work = m_sensor->calculateWork(m_sensor->getMeasureHoldIndex());
						m_sensor->setCalculatedWorkHoldProperty(work);
						qDebug() << "WorkHold = " << m_sensor->calculatedWorkHoldProperty();
					} else {
						if (m_tensometer->getForceValue() >= m_config->getConfigInt(MEASURE2_HOLDFORCE_KEY)
								|| m_stepperengine->getCurrentPosition() >= m_config->getConfigInt(LENGTH_MAX_KEY)) {
							/* stop */
							qDebug() << "measure2 - hold max force suboperation stop engine";
							m_stepperengine->stop();

						} else {
							/* try to hold max force */
							qDebug() << "measure2 - hold max force suboperation start engine";
							m_stepperengine->setTargetPosition(m_config->getConfigInt(LENGTH_MAX_KEY));
							m_stepperengine->start();
							m_stepperengine->setVelocityLimit(m_config->getConfigInt(ENGINE_SPEED5_KEY));
						}
					}
				}
			}
			break;

		case TensoSensor::Suboperations::MEASURE2_SUBOPERATION_TILLMINFORCE:
			{
				qDebug() << "Till Min Force";

				adaptSpeed();

				if (m_sensor->getSubOperationStarted() == 0) {
					qDebug() << "measure2 - till min force suboperation started";
					m_stepperengine->on();
					m_stepperengine->setTargetPosition(0);
					m_stepperengine->start();
					m_sensor->setSubOperationStarted(1);
					m_sensor->setMeasureIndexProperty(0);
					test_counter = 0;
					m_stepperengine->setVelocityLimit(m_config->getConfigInt(ENGINE_SPEED5_KEY));
					m_sensor->setMeasurePhaseProperty(TensoSensor::MeasurePhases::MEASURE_PHASE_DOWN);
				} else {
					qDebug() << "measure2 - till min force suboperation already in progress";

					if (m_tensometer->getForceValue() <= m_config->getConfigInt(FORCE_MIN_KEY)
							|| m_stepperengine->getCurrentPosition() <= 0) {
						qDebug() << "till min force reached or 0 reached";
						m_stepperengine->stop();
						m_sensor->setOperationCompletedProperty(1);
						m_sensor->setMeasureStartedProperty(0);
						m_sensor->setSubOperationStarted(0);
						m_sensor->setOperationProperty(TensoSensor::Operations::SENSOR_OPERATION_IDLE);
						m_sensor->setSubOperationProperty(TensoSensor::Suboperations::NONE_SUBOPERATION);

						float work = m_sensor->calculateWork(m_sensor->getMeasureDownIndex());
						m_sensor->setCalculatedWorkDownProperty(work);
						qDebug() << "WorkDown = " << m_sensor->calculatedWorkDownProperty();
					}
				}
			}
			break;

		default:
			{
			}
	}
}

void TensoThread::operationCalibrate()
{
	/* one time initialization code
	*/

	qDebug() << "SENSOR_OPERATION_CALIBRATE";
	switch (m_sensor->subOperationProperty()) {
		case TensoSensor::Suboperations::CALIBRATE_SUBOPERATION_TILL_FORCE_MAX:
			{
				qDebug() << "SENSOR_SUBOPERATION_CALIBRATE_TILL_FORCE_MAX";
				if (m_sensor->getSubOperationStarted() == 0) {

					/* just start the engine with the middle speed
					*/
					int engine_speed_val = m_config->getConfigInt(ENGINE_SPEED2_KEY);
					m_stepperengine->on();
					m_stepperengine->start();
					m_stepperengine->setVelocityLimit(engine_speed_val);
					m_stepperengine->setTargetPosition(m_config->getConfigInt(LENGTH_MAX_KEY));
					m_sensor->setSubOperationStarted(1);
				}

				/* control force
				*/
				int current_force_value = m_tensometer->getForceValue();
				if (current_force_value >= m_config->getConfigInt(CALIBRATE_FORCE_0_KEY)) {
					m_stepperengine->stop();
					m_sensor->setSubOperationProperty(TensoSensor::Suboperations::CALIBRATE_SUBOPERATION_LOOSE);
					m_sensor->setSubOperationStarted(0);
					m_stepperengine->setCurrentPosition(0);
				}
			}
			break;
		case TensoSensor::Suboperations::CALIBRATE_SUBOPERATION_LOOSE:
			{
				qDebug() << "SENSOR_SUBOPERATION_CALIBRATE_LOOSE";
				if (m_sensor->getSubOperationStarted() == 0) {

					/* just start the engine with the middle speed
					*/
					int engine_speed_val = m_config->getConfigInt(ENGINE_SPEED2_KEY);
					int position = m_stepperengine->getCurrentPosition();
					m_stepperengine->start();
					m_stepperengine->setVelocityLimit(engine_speed_val);
					m_stepperengine->setTargetPosition(position - m_config->getConfigInt(CALIBRATE_LENGTH_LOOSE_KEY));
					m_sensor->setSubOperationStarted(1);
				} else {

					if (m_stepperengine->isEngineStopped()) {
						m_sensor->setOperationCompletedProperty(1);
						m_sensor->setOperationProperty(TensoSensor::Operations::SENSOR_OPERATION_IDLE);
						m_sensor->setSubOperationProperty(TensoSensor::Suboperations::NONE_SUBOPERATION);
					}
				}
			}
			break;
		default:
			{
				qDebug() << "DEFAULT";
			}
	}
}

