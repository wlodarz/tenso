#ifndef __STEPPERENGINE_H__
#define __STEPPERENGINE_H__


class StepperEngine {
public:
	StepperEngine() {}
	~StepperEngine() {}

	void setMove(int step) { m_stepValue = step; }

private:
	int m_stepValue;
};


#endif // __STEPPERENGINE_H__

