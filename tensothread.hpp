#ifndef __TENSOTHREAD_H__
#define __TENSOTHREAD_H__

#include <QtCore>
#include <QDebug>

#include "tensometer.hpp"
#include "stepperengine.hpp"
#include "tensoconfig.hpp"

class TensoSensor;

class TensoThread : public QThread
{
  Q_OBJECT

public:
  int loadConfig(QString);
  int configure();
  int init(TensoSensor *);

private:
  void run();

  void testFunction();
  void operationMove();
  void operationMeasure1();
  void operationCalibrate();

  void adaptSpeed();

public slots:
  void onTimeout();
  void onControlTimeout();

private:
  TensoConfig *m_config;
  TensoSensor *m_sensor;
  Tensometer *m_tensometer;
  StepperEngine *m_stepperengine;

  int test_counter;
};

#endif // __TENSOTHREAD_H__

