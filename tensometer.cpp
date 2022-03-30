
#include <stdio.h>
#include <phidget22.h>
#include <QDebug>
#include "tensometer.hpp"

static void CCONV attachHandler(PhidgetHandle phid, void *userptr)
{
	PhidgetReturnCode result;
	const char *err;

	userptr = userptr;

        PhidgetVoltageRatioInputHandle voltageRatioInput = (PhidgetVoltageRatioInputHandle)phid;

        printf("Tensometer attached!\n");
        return;
}

static void CCONV detachHandler(PhidgetHandle phid, void *userptr)
{
	phid = phid;
	userptr = userptr;

        printf("Tensometer detached!\n");
        return;
}

static void CCONV errorHandler(PhidgetHandle phid, void *userptr, Phidget_ErrorEventCode ErrorCode, const char *errorStr)
{
	phid = phid;
	userptr = userptr;
	ErrorCode = ErrorCode;
        printf("Tensometer error event: %s\n",errorStr);
        return;
}

static void CCONV dataHandler(PhidgetVoltageRatioInputHandle ch, void *ctx, double voltageRatio)
{
        //PhidgetBridgeHandle bridge = (PhidgetBridgeHandle)phid;
        //double f;
	//double ms;
        //int i;
        double K = 160000.0;
        double offset = 0.002325;
        double weight;

	ch = ch;

	Tensometer *tenso = (Tensometer *)ctx;

        //qDebug() << "Data Event (" << index << " " << val;

        weight = K * ((voltageRatio * 1000.0 / 5.0) - offset);
        qDebug() << "Weight " << weight << "g";
	//printf("voltageRatio %f weight %f\n", voltageRatio, weight);

	tenso->setForceValue((int)weight);

        return;
}


Tensometer::Tensometer()
{
}

int Tensometer::init()
{
	int res;
	m_forceValue = 0;
	PhidgetReturnCode result;
	const char *err;

	PhidgetVoltageRatioInput_create(&voltageRatioInput);

	res = Phidget_setChannel((PhidgetHandle)voltageRatioInput, 0);
	if (res != EPHIDGET_OK) {
		qDebug() << "Problem setting channel";
		return -1;
	}

        PhidgetVoltageRatioInput_setBridgeEnabled(voltageRatioInput, PTRUE);
        PhidgetVoltageRatioInput_setBridgeGain(voltageRatioInput, BRIDGE_GAIN_128);
        PhidgetVoltageRatioInput_setDataRate(voltageRatioInput, 10);
	PhidgetVoltageRatioInput_setVoltageRatioChangeTrigger(voltageRatioInput, 0.0);

	if((result = Phidget_open((PhidgetHandle)voltageRatioInput))!=0) {
        	Phidget_getErrorDescription(result, &err);
		qDebug() << "Problem waiting for attachment:" << err;
		return -1;
	}

	Phidget_setOnAttachHandler((PhidgetHandle)voltageRatioInput, attachHandler, this);
	Phidget_setOnDetachHandler((PhidgetHandle)voltageRatioInput, detachHandler, this);
	Phidget_setOnErrorHandler((PhidgetHandle)voltageRatioInput, errorHandler, this);

	PhidgetVoltageRatioInput_setOnVoltageRatioChangeHandler(voltageRatioInput, dataHandler, this);

	return 0;
}

Tensometer::~Tensometer() {}

