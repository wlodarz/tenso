
#include <stdio.h>
#include <phidget21.h>
#include <QDebug>
#include "tensometer.hpp"

int CCONV AttachHandler(CPhidgetHandle phid, void *userptr)
{
        CPhidgetBridgeHandle bridge = (CPhidgetBridgeHandle)phid;

        CPhidgetBridge_setEnabled(bridge, 0, PTRUE);
        CPhidgetBridge_setEnabled(bridge, 1, PFALSE);
        CPhidgetBridge_setEnabled(bridge, 2, PFALSE);
        CPhidgetBridge_setEnabled(bridge, 3, PFALSE);

        CPhidgetBridge_setGain(bridge, 0, PHIDGET_BRIDGE_GAIN_128);
        CPhidgetBridge_setGain(bridge, 1, PHIDGET_BRIDGE_GAIN_16);
        CPhidgetBridge_setGain(bridge, 2, PHIDGET_BRIDGE_GAIN_32);
        CPhidgetBridge_setGain(bridge, 3, PHIDGET_BRIDGE_GAIN_64);
        CPhidgetBridge_setDataRate(bridge, 100);

        printf("Attach handler ran!\n");
        return 0;
}

int CCONV DetachHandler(CPhidgetHandle phid, void *userptr)
{
        printf("Detach handler ran!\n");
        return 0;
}

int CCONV ErrorHandler(CPhidgetHandle phid, void *userptr, int ErrorCode, const char *errorStr)
{
        printf("Error event: %s\n",errorStr);
        return 0;
}

#define FREQS_SIZE 20
double bridges[FREQS_SIZE] = {0};
int CCONV data(CPhidgetBridgeHandle phid, void *userPtr, int index, double val)
{
        CPhidgetBridgeHandle bridge = (CPhidgetBridgeHandle)phid;
        double f, ms;
        int i;
        double K = 160000.0;
        double offset = 0.002325;
        double weight;
	Tensometer *tenso = (Tensometer *)userPtr;

        //qDebug() << "Data Event (" << index << " " << val;

        weight = K * ((val / 5.0) - offset);
        //qDebug() << "Weight " << weight << "g";

	tenso->setLastValue((int)weight);

        return 0;
}


Tensometer::Tensometer()
{
}

int Tensometer::Init()
{
#ifdef TEST
#else
	const char *err;
	int result;
	CPhidgetBridgeHandle bridge;

	m_lastValue = 0;

	CPhidgetBridge_create(&bridge);

	CPhidget_set_OnAttach_Handler((CPhidgetHandle)bridge, AttachHandler, NULL);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)bridge, DetachHandler, NULL);
	CPhidget_set_OnError_Handler((CPhidgetHandle)bridge, ErrorHandler, NULL);

	CPhidgetBridge_set_OnBridgeData_Handler(bridge, data, this);

	CPhidget_open((CPhidgetHandle)bridge, -1);

	//Wait for 10 seconds, otherwise exit
	if(result = CPhidget_waitForAttachment((CPhidgetHandle)bridge, 10000)) {
        	CPhidget_getErrorDescription(result, &err);
		qDebug() << "Problem waiting for attachment:" << err;
		return -1;
	}
#endif
	return 0;
}

Tensometer::~Tensometer() {}

#if 0
int Tensometer::getForceValue()
{
	return rand() % 120;
}
#endif

