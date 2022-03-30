
#include "encoder.hpp"
#include <phidget22.h>

#include <stdio.h>

void CCONV positionChangeHandler(PhidgetEncoderHandle ch, void *ctx, int positionChange, double timeChange, int indexTriggered) {
	printf("position change %d\n", positionChange);
}

Encoder::Encoder()
{
}

Encoder::~Encoder()
{
}

int Encoder::init()
{
	PhidgetReturnCode returnCode;
	
	returnCode = PhidgetEncoder_create(&m_encoder);
	if (returnCode != EPHIDGET_OK) {
		return -1;
	}

	returnCode = PhidgetEncoder_setOnPositionChangeHandler(m_encoder, positionChangeHandler, this);
	if (returnCode != EPHIDGET_OK) {
		return -2; // Exit in error
	}

	returnCode = Phidget_open((PhidgetHandle)m_encoder);
	if (returnCode != EPHIDGET_OK) {
		return -3; // Exit in error
	}

	return 0;
}

