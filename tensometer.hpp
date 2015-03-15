#ifndef __TENSOMETER_H__
#define __TENSOMETER_H__

#include <stdlib.h>

//#define TEST

class Tensometer {
public:
	Tensometer();
	~Tensometer();

#ifdef TEST
	int getForceValue() { return rand() % 300; }
#else
	int getForceValue() { return m_lastValue; }
#endif
	int waitForHardware();
	int Init();
	void setLastValue(int val) { m_lastValue = val >= 0 ? val : 0; }

private:
	int m_lastValue;
};


#endif // __TENSOMETER_H__

