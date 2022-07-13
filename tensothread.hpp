#ifndef __TENSOTHREAD_H__
#define __TENSOTHREAD_H__

#include <QtCore>
#include <QDebug>

#include "tensometer.hpp"
#include "engine.hpp"
#include "tensoconfig.hpp"
#include "encoder.hpp"

class TensoSensor;

class TensoThread : public QThread
{
  Q_OBJECT

public:
  int loadConfig(QString);
  int loadSteps2LengthTable(QString filename);
  int configure();
  int init(TensoSensor *);

private:
  void run();

  void testFunction();
  void operationMove();
  void operationMeasure1();
  void operationMeasure2();
  void operationCalibrate();

  void adaptSpeed();
  int parseConvertLine(QString line);

  void saveReportToFile(QString csvFileName);
  void saveSamplesToFile(QString csvFileName, int numberOfSamples);

  void checkLengthUpLevels();
  void checkLengthDownLevels();

public slots:
  void onTimeout();
  void onControlTimeout();

private:
  TensoConfig *m_config;
  TensoSensor *m_sensor;
  Tensometer *m_tensometer;
  Engine *m_engine;
  Encoder *m_encoder;
};

#endif // __TENSOTHREAD_H__

