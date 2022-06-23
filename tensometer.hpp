#ifndef __TENSOMETER_H__
#define __TENSOMETER_H__

#include <phidget22.h>
#include <stdlib.h>

class Tensometer {
public:
	Tensometer();
	~Tensometer();

	int init();
	int waitForHardware();

	/* force values in grams */
	void setForceScale(float scale) { m_forceScale = scale; }
	float getForceValue() { return m_forceValue * m_forceScale; }
	void setForceValue(float val) { m_forceValue = val >= 0 ? val : 0; }

private:
	float m_forceValue;
	float m_forceScale;
	PhidgetVoltageRatioInputHandle voltageRatioInput;
};


#endif // __TENSOMETER_H__

