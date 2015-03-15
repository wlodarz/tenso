#ifndef __TENSOSENSOR_H__
#define __TENSOSENSOR_H__

#include <QObject>
#include <QDebug>
#include <QTimer>

#define MAX_STEPS 200
#define LENGTH_TODO 100 /* in cm */
#define FORCE_TODO 300

enum {
	TIMER_TYPE_NONE,
	TIMER_TYPE_PULL,
	TIMER_TYPE_HOLD
};

class TensoSensor : public QObject {
	Q_OBJECT

	Q_PROPERTY(int holdTime READ holdTimeProperty WRITE setHoldTimeProperty NOTIFY holdTimePropertyChanged)
	Q_PROPERTY(int maxLength READ maxLengthProperty WRITE setMaxLengthProperty NOTIFY maxLengthPropertyChanged)
	Q_PROPERTY(int maxForce READ maxForceProperty WRITE setMaxForceProperty NOTIFY maxForcePropertyChanged)
	Q_PROPERTY(int currentHoldTime READ currentHoldTimeProperty WRITE setCurrentHoldTimeProperty NOTIFY currentHoldTimePropertyChanged)
	Q_PROPERTY(int currentForce READ currentForceProperty WRITE setCurrentForceProperty NOTIFY currentForcePropertyChanged)
	Q_PROPERTY(int currentLength READ currentLengthProperty WRITE setCurrentLengthProperty NOTIFY currentLengthPropertyChanged)
	Q_PROPERTY(int currentTime READ currentTimeProperty WRITE setCurrentTimeProperty NOTIFY currentTimePropertyChanged)
	Q_PROPERTY(int startMeasure READ startMeasureProperty WRITE setStartMeasureProperty NOTIFY startMeasurePropertyChanged)

public:
	TensoSensor(QObject *parent = 0) : QObject(parent),
					   m_holdTime(10),
					   m_maxLength(LENGTH_TODO),
					   m_maxForce(FORCE_TODO),
					   m_currentForce(0),
					   m_currentLength(0),
					   m_currentTime(0),
					   m_currentHoldTime(0),
					   m_startMeasure(0),
					   m_timerType(TIMER_TYPE_NONE)
						 {}
	~TensoSensor() {}

	int holdTimeProperty() const { return m_holdTime; }
	void setHoldTimeProperty(int &val) { m_holdTime = val; emit holdTimePropertyChanged(val); }
	int maxLengthProperty() const { return m_maxLength; }
	void setMaxLengthProperty(int &val) { m_maxLength = val; emit maxLengthPropertyChanged(val); }
	int maxForceProperty() const { return m_maxForce; }
	void setMaxForceProperty(int &val) { m_maxForce = val; emit maxForcePropertyChanged(val); }
	int currentHoldTimeProperty() const { return m_currentHoldTime; }
	void setCurrentHoldTimeProperty(int &val) { m_currentHoldTime = val; emit currentHoldTimePropertyChanged(val); }
	int currentForceProperty() const { return m_currentForce; }
	void setCurrentForceProperty(int &val) { m_currentForce = val; emit currentForcePropertyChanged(val); }
	int currentLengthProperty() const { return m_currentLength; }
	void setCurrentLengthProperty(int &val) { m_currentLength = val; emit currentLengthPropertyChanged(val); }
	int currentTimeProperty() const { return m_currentTime; }
	void setCurrentTimeProperty(int &val) { m_currentTime = val; emit currentTimePropertyChanged(val); }
	int startMeasureProperty() const { return m_startMeasure; }
	void setStartMeasureProperty(int &val) { 
		int val1 = 0;
		m_startMeasure = val; 
		if (val) m_timerType = TIMER_TYPE_PULL;
		else m_timerType = TIMER_TYPE_NONE;
		setCurrentHoldTimeProperty(val1);
		setCurrentForceProperty(val1);
		setCurrentLengthProperty(val1);
		emit startMeasurePropertyChanged(val);
		qDebug() << "setStartMeasureProperty " << val;
		m_step = 0;
	}

	//void resetCurrentHoldTime() { m_currentHoldTime = 0; }
	//void resetCurrentForce() { m_currentForce = 0; }
	//void resetCurrentLength() { m_currentLength = 0; }

	void setTimerType(int type) { m_timerType = type; }

	int getNextStep() {
                if (m_timerType == TIMER_TYPE_PULL && m_step < MAX_STEPS) {
                        int val = m_currentLength+1;
                        setCurrentLengthProperty(val);
                        qDebug() << "onMeasureTimeout: PULLING force " << m_currentForce << " length " << m_currentLength;
			return m_step++;
                }
		return m_step;
	}

	void checkLimits() {
                //qDebug() << "onMeasureTimeout: start";
                if (m_currentForce >= m_maxForce) {
                        qDebug() << "onMeasureTimeout: max force reached";
                        m_timerType = TIMER_TYPE_HOLD;
                }
                if (m_currentLength >= m_maxLength) {
                        qDebug() << "onMeasureTimeout: max length reached";
                        m_timerType = TIMER_TYPE_HOLD;
                }
	}

signals:
	void holdTimePropertyChanged(int newValue);
	void maxLengthPropertyChanged(int newValue);
	void maxForcePropertyChanged(int newValue);
	void currentHoldTimePropertyChanged(int newValue);
	void currentForcePropertyChanged(int newValue);
	void currentLengthPropertyChanged(int newValue);
	void currentTimePropertyChanged(int newValue);
	void startMeasurePropertyChanged(int newValue);
	void forceChanged(int step, int value);

public slots:
	void onTimeout() {
		if (m_timerType == TIMER_TYPE_HOLD) {
			qDebug() << "m_currentHoldTimer " << m_currentHoldTime;
			if (m_currentHoldTime < m_holdTime) {
				int val = m_currentHoldTime+1;
				setCurrentHoldTimeProperty(val);
			} else if (m_currentHoldTime == m_holdTime) {
				int val = 0;
				m_timerType = TIMER_TYPE_NONE;
				setStartMeasureProperty(val);
				qDebug() << "Stop";
			}
		}
		qDebug() << "TensoSensor::onTimeout";
	}

	void onMeasureTimeout() {
		//qDebug() << "onMeasureTimeout: start";
		if (m_currentForce >= m_maxForce) {
			qDebug() << "onMeasureTimeout: max force reached";
			m_timerType = TIMER_TYPE_HOLD;
		}
		if (m_currentLength >= m_maxLength) {
			qDebug() << "onMeasureTimeout: max length reached";
			m_timerType = TIMER_TYPE_HOLD;
		}
		if (m_timerType == TIMER_TYPE_PULL) {
			int val = m_currentLength+1;
			setCurrentLengthProperty(val);
			qDebug() << "onMeasureTimeout: PULLING force " << m_currentForce << " length " << m_currentLength;
		}
	}

private:
	int m_pullTime;  /* */
	int m_holdTime; /* */
	int m_maxLength; /* in cm */
	int m_maxForce; /* */
	int m_currentForce;
	int m_currentLength;
	int m_currentTime;
	int m_startMeasure;
	int m_forceValues[MAX_STEPS];
	int m_currentHoldTime;
	int m_timerType;
	int m_step;
};

#endif // __TENSOSENSOR_H__

