
#include <QtCore>
#include <QtDebug>
#include <QQmlEngine>
#include <QQuickItem>
#include <QList>
#include <QQmlContext>
#include <QTimer>

#include <stdlib.h>

#include <phidget21.h>

#include "tensosensor.hpp"
#include "tensothread.hpp"
#include "tensometer.hpp"
#include "stepperengine.hpp"

void TensoThread::run()
{
  int startFlag = 0;

  qDebug() << "TensoThread::run";

  m_tensometer = new Tensometer();
  m_stepperengine = new StepperEngine();

  m_tensometer->Init();

  QTimer counting_timer;
  connect(&counting_timer, SIGNAL(timeout()), m_sensor, SLOT(onTimeout()));
  counting_timer.start(1000);

  QTimer measure_timer;
  connect(&measure_timer, SIGNAL(timeout()), this, SLOT(onMeasureTimeout()));
  measure_timer.start(100);

  m_sensor->setStartMeasureProperty(startFlag);
  m_sensor->setTimerType(TIMER_TYPE_NONE);

  exec();
}

void TensoThread::onMeasureTimeout()
{
	int val = m_tensometer->getForceValue();
	m_sensor->setCurrentForceProperty(val);

	if (m_sensor->startMeasureProperty() == 0) return;

	if (val > 5000) val = 5000;
	val *= 4;
	val /= 50;

	m_sensor->checkLimits();

	int step = m_sensor->getNextStep();
	emit m_sensor->forceChanged(step, val);
	m_stepperengine->setMove(1);
}

