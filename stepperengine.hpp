#ifndef __STEPPERENGINE_H__
#define __STEPPERENGINE_H__

#include "engine.hpp"
#include <phidget22.h>

class StepperEngine : public Engine {
public:
	StepperEngine();
	~StepperEngine();

	int init();

	void start();
	void stop();
	void off();
	void on();
	void setCurrentPosition(qint64 position);
	void setTargetPosition(qint64 position);
	qint64 getCurrentPosition();
	void loose();
	int checkConnected();
	int waitTillEngineStopped();
	int isEngineStopped();
	void setVelocityLimit(int limit);

private:
	PhidgetStepperHandle m_stepper;
	double minAccel, maxAccel, maxVel;
};


#endif // __STEPPERENGINE_H__

