#ifndef __TESTENGINE_H__
#define __TESTENGINE_H__

#include "engine.hpp"

class TestEngine : virtual public Engine {
public:
	TestEngine();
	~TestEngine();

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
	double minAccel, maxAccel, maxVel;
	bool m_started;
	bool m_engaged;
	qint64 m_currentPosition;
	qint64 m_targetPosition;
	int m_mode;

	enum {
		ENGINE_MODE_DOWN = 1,
		ENGINE_MODE_IDLE = 2,
		ENGINE_MODE_UP = 3
	};
};


#endif // __TESTENGINE_H__

