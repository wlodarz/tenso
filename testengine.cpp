
#include <stdio.h>
#include <unistd.h>
#include <QDebug>

#include "tensoconfig.hpp"

#include "testengine.hpp"

TestEngine::TestEngine()
{
}

TestEngine::~TestEngine() {}

int TestEngine::init()
{
	const char *err;

	m_started = false;
	m_engaged = false;

	m_currentPosition = 0;
	m_targetPosition = 0;

	m_mode = ENGINE_MODE_IDLE;

	return 0;
}

void TestEngine::start()
{
	int speed_limit = maxVel / getMinSpeedDiv();

	qDebug() << "TestEngine::start() limit " << speed_limit;
	qDebug() << "TestEngine::start() maxVel " << maxVel;
	qDebug() << "TestEngine::start() m_minspeeddiv " << getMinSpeedDiv();

	if (checkConnected() < 0) return;

	m_started = true;
}

void TestEngine::stop()
{
	qDebug() << "TestEngine::stop()";

	if (checkConnected() < 0) return;

	m_started = false;
	m_targetPosition = m_currentPosition;
}

void TestEngine::loose()
{
	int loosen_delta;

	qDebug() << "TestEngine::stop()";

	if (checkConnected() < 0) return;

	if (getDirection()) loosen_delta = getLoosenDelta();
	else loosen_delta = (-1) * getLoosenDelta();
	qDebug() << "loosen_delta " << loosen_delta;
	setTargetPosition(getCurrentPosition()-loosen_delta);
}

void TestEngine::on()
{
	qDebug() << "TestEngine::on()";

	if (checkConnected() < 0) return;

	m_engaged = true;
}

void TestEngine::off()
{
	qDebug() << "TestEngine::off()";

	if (checkConnected() < 0) return;

	m_engaged = false;
}

void TestEngine::setCurrentPosition(qint64 position)
{
	qDebug() << "TestEngine::setCurrentPosition()";

	if (checkConnected() < 0) return;
	m_currentPosition = position;
}

void TestEngine::setTargetPosition(qint64 position)
{
	qDebug() << "TestEngine::setTargetPosition() " << position;

	if (checkConnected() < 0) return;
	m_targetPosition = position;
}

qint64 TestEngine::getCurrentPosition()
{
	//qDebug() << "TestEngine::getCurrentPosition()";
#if 0
	switch (m_mode) {
		case ENGINE_MODE_UP:
			m_currentPosition+=30;
			break;
		case ENGINE_MODE_IDLE:
			break;
		case ENGINE_MODE_DOWN:
			m_currentPosition-=30;
			break;
	}
#endif
	if (m_currentPosition < m_targetPosition) {
		m_currentPosition += 30;
	} else if (m_currentPosition > m_targetPosition) {
		m_currentPosition -= 30;
	}
	return m_currentPosition;
}

int TestEngine::checkConnected()
{
	if (!getConnected()) {
		qDebug() << "TestEngine::checkConnected(): engine not detected";
		return -1;
	}
	return 0;
}

int TestEngine::waitTillEngineStopped()
{
#if 0
	int stopped = 0;

        while(!stopped)
        {
                PhidgetStepper_getIsMoving(m_stepper, &stopped);
                sleep(1);
        }
        PhidgetStepper_setVelocityLimit(m_stepper, 0);

#endif
	return 0;
}

int TestEngine::isEngineStopped()
{
	int stopped = 0;
	stopped = random() % 2;

	return stopped;
}

void TestEngine::setVelocityLimit(int limit)
{
#if 0
	//m_maxspeeddiv = limit;
	setMaxSpeedDiv(limit);
        PhidgetStepper_setVelocityLimit(m_stepper, maxVel / limit);
	//qDebug() << "speed " << maxVel / limit;
#endif
}

