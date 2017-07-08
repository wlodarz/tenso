
#include <stdio.h>
#include <unistd.h>
#include <phidget21.h>
#include <QDebug>

#include "config.hpp"
#include "tensoconfig.hpp"

#include "stepperengine.hpp"


static int CCONV AttachHandler(CPhidgetHandle stepper, void *userptr)
{
        int serialNo;
        const char *name;
	StepperEngine *engine = (StepperEngine *)userptr;

        CPhidget_getDeviceName (stepper, &name);
        CPhidget_getSerialNumber(stepper, &serialNo);
        printf("%s %10d attached!\n", name, serialNo);
	engine->setConnected(1);
	//engine->setPosition(engine->getCurrentPosition());

        return 0;
}

static int CCONV DetachHandler(CPhidgetHandle stepper, void *userptr)
{
        int serialNo;
        const char *name;
	StepperEngine *engine = (StepperEngine *)userptr;


        CPhidget_getDeviceName (stepper, &name);
        CPhidget_getSerialNumber(stepper, &serialNo);
        printf("%s %10d detached!\n", name, serialNo);
	engine->setConnected(0);
	

        return 0;
}

static int CCONV ErrorHandler(CPhidgetHandle stepper, void *userptr, int ErrorCode, const char *errorStr)
{
	StepperEngine *engine = (StepperEngine *)userptr;

	engine = engine;
	stepper = stepper;
        printf("Error handled. %d - %s\n", ErrorCode, errorStr);

        return 0;
}

int CCONV PositionChangeHandler(CPhidgetStepperHandle stepper, void *userptr, int Index, __int64 Value)
{
	StepperEngine *engine = (StepperEngine *)userptr;
	stepper = stepper;
	Index = Index;
	engine = engine;
	Value = Value;
        //qDebug() << "Motor: " << Index << " Current Position: " << Value;
	//engine->setPosition(Value);

        return 0;
}

StepperEngine::StepperEngine()
{
}

StepperEngine::~StepperEngine() {}

int StepperEngine::init()
{
	const char *err;
	int result;
	//CPhidgetStepperHandle stepper = 0;

	m_position = 0;
	m_connected = 0;
	m_maxspeeddiv = 100000;

	CPhidgetStepper_create(&m_stepper);

	CPhidget_set_OnAttach_Handler((CPhidgetHandle)m_stepper, AttachHandler, this);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)m_stepper, DetachHandler, this);
	CPhidget_set_OnError_Handler((CPhidgetHandle)m_stepper, ErrorHandler, this);
	CPhidgetStepper_set_OnPositionChange_Handler(m_stepper, PositionChangeHandler, this);

	CPhidget_open((CPhidgetHandle)m_stepper, -1);

	//Wait for 10 seconds, otherwise exit
	if((result = CPhidget_waitForAttachment((CPhidgetHandle)m_stepper, 1000))!=0) {
        	CPhidget_getErrorDescription(result, &err);
		qDebug() << "Problem waiting for attachment:" << err;
		return -1;
	}

	CPhidgetStepper_getAccelerationMin(m_stepper, 0, &minAccel);
	CPhidgetStepper_getAccelerationMax(m_stepper, 0, &maxAccel);
	CPhidgetStepper_getVelocityMax(m_stepper, 0, &maxVel);

	//CPhidgetStepper_setAcceleration(m_stepper, 0, minAccel*100);
	//CPhidgetStepper_setAcceleration(m_stepper, 0, maxAccel/250);
	CPhidgetStepper_setAcceleration(m_stepper, 0, maxAccel/10000);
        CPhidgetStepper_setVelocityLimit(m_stepper, 0, maxVel/m_maxspeeddiv);

	CPhidgetStepper_setCurrentPosition(m_stepper, 0, 0);

        CPhidgetStepper_setEngaged(m_stepper, 0, 0);

	double currentMax, currentLimit;
	CPhidgetStepper_getCurrentMax(m_stepper, 0, &currentMax);
	//CPhidgetStepper_getCurrentLimit(m_stepper, 0, &currentLimit);
	currentLimit = 4.0;
	CPhidgetStepper_setCurrentLimit(m_stepper, 0, currentLimit);
	CPhidgetStepper_getCurrentLimit(m_stepper, 0, &currentLimit);
	qDebug() << "CURRENT MAX " << currentMax;
	qDebug() << "CURRENT LIMIT " << currentLimit;
	qDebug() << "MAX Velocity " << maxVel;


#if 0
	CPhidgetStepper_setTargetPosition (m_stepper, 0, 0);
	stopped = PFALSE;
        while(!stopped)
        {
                CPhidgetStepper_getStopped(m_stepper, 0, &stopped);
                sleep(1);
        }
#endif

	return 0;
}

void StepperEngine::start()
{
	int speed_limit = maxVel / m_minspeeddiv;

	qDebug() << "StepperEngine::start() limit " << speed_limit;
	qDebug() << "StepperEngine::start() maxVel " << maxVel;
	qDebug() << "StepperEngine::start() m_minspeeddiv " << m_minspeeddiv;

	if (checkConnected() < 0) return;
        CPhidgetStepper_setVelocityLimit(m_stepper, 0, speed_limit);
}

void StepperEngine::stop()
{
	qDebug() << "StepperEngine::stop()";

	if (checkConnected() < 0) return;

        CPhidgetStepper_setVelocityLimit(m_stepper, 0, 0);
	setTargetPosition(getCurrentPosition());
        //CPhidgetStepper_setVelocityLimit(m_stepper, 0, maxVel/m_maxspeeddiv);
}

void StepperEngine::loose()
{
	int loosen_delta;

	qDebug() << "StepperEngine::stop()";

	if (checkConnected() < 0) return;

	if (getDirection()) loosen_delta = getLoosenDelta();
	else loosen_delta = (-1) * getLoosenDelta();
	qDebug() << "loosen_delta " << loosen_delta;
	setTargetPosition(getCurrentPosition()-loosen_delta);
}

void StepperEngine::on()
{
	qDebug() << "StepperEngine::on()";

	if (checkConnected() < 0) return;
        CPhidgetStepper_setEngaged(m_stepper, 0, 1);
}

void StepperEngine::off()
{
	qDebug() << "StepperEngine::off()";

	if (checkConnected() < 0) return;
        CPhidgetStepper_setEngaged(m_stepper, 0, 0);
}

void StepperEngine::setCurrentPosition(__int64 position)
{
	qDebug() << "StepperEngine::setCurrentPosition()";

	if (checkConnected() < 0) return;
	CPhidgetStepper_setCurrentPosition (m_stepper, 0, position);
}

void StepperEngine::setTargetPosition(__int64 position)
{
	qDebug() << "StepperEngine::setTargetPosition() " << position;

	if (checkConnected() < 0) return;
	CPhidgetStepper_setTargetPosition (m_stepper, 0, position);
}

__int64 StepperEngine::getCurrentPosition()
{
	//qDebug() << "StepperEngine::getCurrentPosition()";
#if TEST_ENGINE
	static int position;
	position++;
	position %= 1000;
	return position;
#else
	__int64 curr_pos;

	if (checkConnected() < 0) return -1;

	if(CPhidgetStepper_getCurrentPosition(m_stepper, 0, &curr_pos) == EPHIDGET_OK)	
		return curr_pos;
	return -1;
#endif
}

int StepperEngine::checkConnected()
{
	if (!m_connected) {
		qDebug() << "StepperEngine::checkConnected(): engine not detected";
		return -1;
	}
	return 0;
}

int StepperEngine::waitTillEngineStopped()
{
	int stopped = 0;

        while(!stopped)
        {
                CPhidgetStepper_getStopped(m_stepper, 0, &stopped);
                sleep(1);
        }
        CPhidgetStepper_setVelocityLimit(m_stepper, 0, 0);

	return 0;
}

int StepperEngine::isEngineStopped()
{
	int stopped = 0;
	CPhidgetStepper_getStopped(m_stepper, 0, &stopped);
#if TEST_ENGINE
	stopped = random() % 2;
#endif
	return stopped;
}

void StepperEngine::setVelocityLimit(int limit)
{
	m_maxspeeddiv = limit;
        CPhidgetStepper_setVelocityLimit(m_stepper, 0, maxVel / limit);
	//qDebug() << "speed " << maxVel / limit;
}

