#ifndef __TENSOTHREAD_H__
#define __TENSOTHREAD_H__

#include <QtCore>
#include <QDebug>

#include "tensometer.hpp"
#include "stepperengine.hpp"

class TensoSensor;

class TensoThread : public QThread
{
  Q_OBJECT

public:
  void setTensoSensor(TensoSensor *sensor) { m_sensor = sensor; }

private:
  void run();

public slots:
  void onMeasureTimeout();

private:
  TensoSensor *m_sensor;
  Tensometer *m_tensometer;
  StepperEngine *m_stepperengine;
};

#endif // __TENSOTHREAD_H__

