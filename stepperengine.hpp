#ifndef __STEPPERENGINE_H__
#define __STEPPERENGINE_H__

#include <phidget21.h>

class StepperEngine {
public:
	StepperEngine();
	~StepperEngine();

	int init();

	void start();
	void stop();
	void off();
	void on();
	void setCurrentPosition(__int64 position); // { m_stepValue = step; }
	void setTargetPosition(__int64 position); // { m_stepValue = step; }
	__int64 getCurrentPosition();
	void setVelocity(int velocity); // { m_stepValue = step; }
	void setConnected(int connected) { m_connected = connected; }
	int getConnected() { return m_connected; }
	//void setPosition(__int64 position) { m_position = position; }
	//__int64 getPosition() { return m_position; }
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
	void setVelocityLimit(int limit);

private:
	__int64 m_position;
	int m_velocity;
	int m_connected;			/* 0 - engine not connected, 1 - engine connected */
	int m_loosen_delta;
	int m_direction; // if position is added from start to max, this flag should be 1, otherwise 0
	int m_maxspeeddiv; // div
	CPhidgetStepperHandle m_stepper;

	double minAccel, maxAccel, maxVel;
};


#endif // __STEPPERENGINE_H__

