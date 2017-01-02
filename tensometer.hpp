#ifndef __TENSOMETER_H__
#define __TENSOMETER_H__

#include <stdlib.h>

class Tensometer {
public:
	Tensometer();
	~Tensometer();

	int init();
	int waitForHardware();

	/* force values in grams */
	int getForceValue() { return m_forceValue; }
	void setForceValue(int val) { m_forceValue = val >= 0 ? val : 0; }

private:
	int m_forceValue;
};


#endif // __TENSOMETER_H__

