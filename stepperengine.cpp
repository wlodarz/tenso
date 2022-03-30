
#include <stdio.h>
#include <unistd.h>
#include <phidget22.h>
#include <QDebug>

#include "config.hpp"
#include "tensoconfig.hpp"

#include "stepperengine.hpp"


static void CCONV attachHandler(PhidgetHandle stepper, void *userptr)
{
        int serialNo;
        const char *name;
	StepperEngine *engine = (StepperEngine *)userptr;

        //Phidget_getDeviceName (stepper, &name);
        //Phidget_getSerialNumber(stepper, &serialNo);
        //printf("%s %10d attached!\n", name, serialNo);
	engine->setConnected(1);
	//engine->setPosition(engine->getPosition());

        return;
}

static void CCONV detachHandler(PhidgetHandle stepper, void *userptr)
{
        int serialNo;
        const char *name;
	StepperEngine *engine = (StepperEngine *)userptr;


        //Phidget_getDeviceName (stepper, &name);
        //Phidget_getSerialNumber(stepper, &serialNo);
        //printf("%s %10d detached!\n", name, serialNo);
	engine->setConnected(0);
	

        return;
}

static void CCONV errorHandler(PhidgetHandle stepper, void *userptr, Phidget_ErrorEventCode ErrorCode, const char *errorStr)
{
	StepperEngine *engine = (StepperEngine *)userptr;

	engine = engine;
	stepper = stepper;
        printf("Error handled. %d - %s\n", ErrorCode, errorStr);

        return;
}

void CCONV positionChangeHandler(PhidgetStepperHandle ch, void *ctx, double Value)
{
	StepperEngine *engine = (StepperEngine *)ctx;
	ch = ch;
	Value = Value;
        //qDebug() << "Motor: " << Index << " Current Position: " << Value;
	//engine->setPosition(Value);

        return;
}

StepperEngine::StepperEngine()
{
}

StepperEngine::~StepperEngine() {}

int StepperEngine::init()
{
	const char *err;
	PhidgetReturnCode result;

	m_position = 0;
	m_connected = 0;
	m_maxspeeddiv = 100000;
	double currentPosition;

	PhidgetStepper_create(&m_stepper);
	Phidget_setChannel((PhidgetHandle)m_stepper, PHIDGET_CHANNEL_ANY);

	Phidget_setOnAttachHandler((PhidgetHandle)m_stepper, attachHandler, this);
	Phidget_setOnDetachHandler((PhidgetHandle)m_stepper, detachHandler, this);
	Phidget_setOnErrorHandler((PhidgetHandle)m_stepper, errorHandler, this);
	PhidgetStepper_setOnPositionChangeHandler(m_stepper, positionChangeHandler, this);

	//Wait for 10 seconds, otherwise exit
	if((result = Phidget_openWaitForAttachment((PhidgetHandle)m_stepper, 1000))!=0) {
        	Phidget_getErrorDescription(result, &err);
		qDebug() << "Problem waiting for attachment:" << err;
		return -1;
	}

	PhidgetStepper_getMinAcceleration(m_stepper, &minAccel);
	PhidgetStepper_getMaxAcceleration(m_stepper, &maxAccel);
	PhidgetStepper_getMaxVelocityLimit(m_stepper, &maxVel);

	//PhidgetStepper_setAcceleration(m_stepper, minAccel*100);
	//PhidgetStepper_setAcceleration(m_stepper, maxAccel/250);
	PhidgetStepper_setAcceleration(m_stepper, maxAccel/10000);
        PhidgetStepper_setVelocityLimit(m_stepper, maxVel/m_maxspeeddiv);

	PhidgetStepper_getPosition(m_stepper, &currentPosition);
	PhidgetStepper_addPositionOffset(m_stepper, -1 * currentPosition);

        PhidgetStepper_setEngaged(m_stepper, 0);

	double currentMax, currentLimit;
	currentLimit = 4.0;

	PhidgetStepper_getMaxCurrentLimit(m_stepper, &currentMax);

	PhidgetStepper_setCurrentLimit(m_stepper, currentLimit);
	PhidgetStepper_getCurrentLimit(m_stepper, &currentLimit);
	qDebug() << "CURRENT MAX " << currentMax;
	qDebug() << "CURRENT LIMIT " << currentLimit;
	qDebug() << "MAX Velocity " << maxVel;
	printf("max velocity %f maxAccel %f m_maxspeeddiv %d\n", maxVel, maxAccel, m_maxspeeddiv);
	printf("max current %f current %f\n", currentMax, currentLimit);


#if 0
	PhidgetStepper_setTargetPosition (m_stepper, 0);
	stopped = PFALSE;
        while(!stopped)
        {
                PhidgetStepper_getIsMoving(m_stepper, &stopped);
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
        PhidgetStepper_setVelocityLimit(m_stepper, speed_limit);
}

void StepperEngine::stop()
{
	qDebug() << "StepperEngine::stop()";

	if (checkConnected() < 0) return;

        PhidgetStepper_setVelocityLimit(m_stepper, 0);
	setTargetPosition(getCurrentPosition());
        //PhidgetStepper_setVelocityLimit(m_stepper, maxVel/m_maxspeeddiv);
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
        PhidgetStepper_setEngaged(m_stepper, 1);
}

void StepperEngine::off()
{
	qDebug() << "StepperEngine::off()";

	if (checkConnected() < 0) return;
        PhidgetStepper_setEngaged(m_stepper, 0);
}

void StepperEngine::setCurrentPosition(qint64 position)
{
	double currentPosition;
	qDebug() << "StepperEngine::setCurrentPosition()";

	if (checkConnected() < 0) return;
	PhidgetStepper_getPosition(m_stepper, &currentPosition);
	PhidgetStepper_addPositionOffset(m_stepper, -1 * currentPosition);
}

void StepperEngine::setTargetPosition(qint64 position)
{
	qDebug() << "StepperEngine::setTargetPosition() " << position;

	if (checkConnected() < 0) return;
	PhidgetStepper_setTargetPosition (m_stepper, position);
}

qint64 StepperEngine::getCurrentPosition()
{
	//qDebug() << "StepperEngine::getCurrentPosition()";
#if TEST_ENGINE
	static int position;
	position++;
	position %= 1000;
	return position;
#else
	double curr_pos;

	if (checkConnected() < 0) return -1;

	if(PhidgetStepper_getPosition(m_stepper, &curr_pos) == EPHIDGET_OK)	
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
                PhidgetStepper_getIsMoving(m_stepper, &stopped);
                sleep(1);
        }
        PhidgetStepper_setVelocityLimit(m_stepper, 0);

	return 0;
}

int StepperEngine::isEngineStopped()
{
	int stopped = 0;
	PhidgetStepper_getIsMoving(m_stepper, &stopped);
#if TEST_ENGINE
	stopped = random() % 2;
#endif
	return stopped;
}

void StepperEngine::setVelocityLimit(int limit)
{
	m_maxspeeddiv = limit;
        PhidgetStepper_setVelocityLimit(m_stepper, maxVel / limit);
	//qDebug() << "speed " << maxVel / limit;
}

