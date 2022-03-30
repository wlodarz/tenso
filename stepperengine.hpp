#ifndef __STEPPERENGINE_H__
#define __STEPPERENGINE_H__

#include <phidget22.h>

class StepperEngine {
public:
	StepperEngine();
	~StepperEngine();

	int init();

	void start();
	void stop();
	void off();
	void on();
	void setCurrentPosition(qint64 position); // { m_stepValue = step; }
	void setTargetPosition(qint64 position); // { m_stepValue = step; }
	qint64 getCurrentPosition();
	void setVelocity(int velocity); // { m_stepValue = step; }
	void setConnected(int connected) { m_connected = connected; }
	int getConnected() { return m_connected; }
	//void setPosition(qint64 position) { m_position = position; }
	//qint64 getPosition() { return m_position; }
	void loose();
	int checkConnected();
	int waitTillEngineStopped();
	int isEngineStopped();
	void setLoosenDelta(int delta) { m_loosen_delta = delta; }
	int getLoosenDelta() { return m_loosen_delta; }
	void setDirection(int direction) { m_direction = direction; }
	int getDirection() { return m_direction; }
	void setMaxSpeedDiv(int div) { m_maxspeeddiv = div; }
	int getMaxSpeedDiv() { return m_maxspeeddiv; } 
	void setMinSpeedDiv(int div) { m_minspeeddiv = div; }
	int getMinSpeedDiv() { return m_minspeeddiv; } 
	void setVelocityLimit(int limit);

private:
	qint64 m_position;
	int m_velocity;
	int m_connected;			/* 0 - engine not connected, 1 - engine connected */
	int m_loosen_delta;
	int m_direction; // if position is added from start to max, this flag should be 1, otherwise 0
	int m_maxspeeddiv; // div
	int m_minspeeddiv; // div
	PhidgetStepperHandle m_stepper;

	double minAccel, maxAccel, maxVel;
};


#endif // __STEPPERENGINE_H__

