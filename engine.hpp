#ifndef __ENGINE_H__
#define __ENGINE_H__

class Engine {
public:
	virtual int init() = 0;

	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void off() = 0;
	virtual void on() = 0;
	virtual void setCurrentPosition(qint64 position) = 0;
	virtual void setTargetPosition(qint64 position) = 0;
	virtual qint64 getTargetPosition() = 0;
	virtual qint64 getCurrentPosition() = 0;

	virtual void loose() = 0;
	virtual int checkConnected() = 0;
	virtual int waitTillEngineStopped() = 0;
	virtual int isEngineStopped() = 0;
	virtual void setVelocityLimit(int limit) = 0;

	void setConnected(int connected) { m_connected = connected; }
	int getConnected() { return m_connected; }

	void setLoosenDelta(int delta) { m_loosen_delta = delta; }
	int getLoosenDelta() { return m_loosen_delta; }
	void setDirection(int direction) { m_direction = direction; }
	int getDirection() { return m_direction; }
	void setMaxSpeedDiv(int div) { m_maxspeeddiv = div; }
	int getMaxSpeedDiv() { return m_maxspeeddiv; } 
	void setMinSpeedDiv(int div) { m_minspeeddiv = div; }
	int getMinSpeedDiv() { return m_minspeeddiv; } 
	void setPosition(qint64 position) { m_position = position; }
	qint64 getPosition() { return m_position; }

protected:
    	qint64 m_targetPosition;

private:
	qint64 m_position;
        int m_velocity;
        int m_connected;                        /* 0 - engine not connected, 1 - engine connected */
        int m_loosen_delta;
        int m_direction; // if position is added from start to max, this flag should be 1, otherwise 0
        int m_maxspeeddiv; // div
        int m_minspeeddiv; // div
};


#endif // __ENGINE_H__

