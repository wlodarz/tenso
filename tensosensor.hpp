#ifndef __TENSOSENSOR_H__
#define __TENSOSENSOR_H__

#include <QObject>
#include <QDebug>
#include <QTimer>

#define MAX_POSITION 200
#define LENGTH_TODO 100 /* in cm */
#define FORCE_TODO 300

#define MAX_STATES 0x0010

enum {
	TIMER_TYPE_NONE,
	TIMER_TYPE_PULL,
	TIMER_TYPE_HOLD
};

enum {
	SENSOR_OPERATION_IDLE = 0,
	SENSOR_OPERATION_ZEROING,
	SENSOR_OPERATION_TEST1,
	SENSOR_OPERATION_TEST2,
	SENSOR_OPERATION_PARKING = 10,
	SENSOR_OPERATION_LAST
};

/* zeroing operation states */
enum {
	NONE_SUBOPERATION = 0,
	ZEROING_SUBOPERATION_MOTOROFF = SENSOR_OPERATION_ZEROING * 0x0010,
	ZEROING_SUBOPERATION_TILL40KG,
	TEST1_SUBOPERATION_20CM = SENSOR_OPERATION_TEST1 * 0x0010,
	//TEST1_SUBOPERATION_STOP,
	TEST1_SUBOPERATION_MEASURE_LEN,
	TEST1_SUBOPERATION_100CM
};

class TensoSensor : public QObject {
	Q_OBJECT

	Q_PROPERTY(int operation READ operationProperty WRITE setOperationProperty NOTIFY operationPropertyChanged)
	Q_PROPERTY(int suboperation READ subOperationProperty WRITE setSubOperationProperty NOTIFY subOperationPropertyChanged)
	Q_PROPERTY(int operationcompleted READ operationCompletedProperty WRITE setOperationCompletedProperty NOTIFY operationCompletedPropertyChanged)
	
	Q_PROPERTY(int parking READ parkingProperty WRITE setParkingProperty NOTIFY parkingPropertyChanged)
	Q_PROPERTY(int forceStop READ forceStopProperty WRITE setForceStopProperty NOTIFY forceStopPropertyChanged)

	Q_PROPERTY(int currentForce READ currentForceProperty WRITE setCurrentForceProperty NOTIFY currentForcePropertyChanged)
	Q_PROPERTY(int currentLength READ currentLengthProperty WRITE setCurrentLengthProperty NOTIFY currentLengthPropertyChanged)

	Q_PROPERTY(int measureUpdated READ measureUpdatedProperty WRITE setMeasureUpdatedProperty NOTIFY measureUpdatedPropertyChanged)
	Q_PROPERTY(int measureStarted READ measureStartedProperty WRITE setMeasureStartedProperty NOTIFY measureStartedPropertyChanged)


public:
	TensoSensor(QObject *parent = 0) : QObject(parent),
					   m_operation(SENSOR_OPERATION_IDLE),
					   m_subOperation(NONE_SUBOPERATION),
					   m_operationCompleted(0),
					   m_parking(0),
					   m_forcestop(0),
					   m_currentForce(0),
					   m_currentLength(0),
					   m_measureUpdated(0),
					   m_measureStarted(0),
					   m_timerType(TIMER_TYPE_NONE)
						 {}
	~TensoSensor() {}

	int operationProperty() const { return m_operation; }
	void setOperationProperty(int val) { m_operation = val; emit operationPropertyChanged(val); }

	int subOperationProperty() const { return m_subOperation; }
	void setSubOperationProperty(int val) { m_subOperation = val; m_subOperationStarted = 0; emit subOperationPropertyChanged(val); }

	int operationCompletedProperty() const { return m_operationCompleted; }
	void setOperationCompletedProperty(int val) { m_operationCompleted = val; emit operationCompletedPropertyChanged(val); }

	int parkingProperty() const { return m_parking; }
	void setParkingProperty(int val) { m_parking = val; emit parkingPropertyChanged(val); }

	int forceStopProperty() const { return m_forcestop; }
	void setForceStopProperty(int val) { qDebug() << "setForceStop"; m_forcestop = val; emit forceStopPropertyChanged(val); }

	int currentForceProperty() const { return m_currentForce; }
	void setCurrentForceProperty(int val) { m_currentForce = val; emit currentForcePropertyChanged(val); }

	int currentLengthProperty() const { return m_currentLength; }
	void setCurrentLengthProperty(int val) { m_currentLength = val; emit currentLengthPropertyChanged(val); }

	int measureUpdatedProperty() const { return m_measureUpdated; }
	void setMeasureUpdatedProperty(int val) { m_measureUpdated = val; emit measureUpdatedPropertyChanged(val); }

	int measureStartedProperty() const { return m_measureStarted; }
	void setMeasureStartedProperty(int val) { m_measureStarted = val; emit measureStartedPropertyChanged(val); }

	int timerTypeProperty() const { return m_timerType; }
	void setTimerType(int type) { m_timerType = type; }


signals:
	void operationPropertyChanged(int newValue);
	void subOperationPropertyChanged(int newValue);
	void operationCompletedPropertyChanged(int newValue);
	void parkingPropertyChanged(int newValue);
	void forceStopPropertyChanged(int newValue);
	void currentForcePropertyChanged(int newValue);
	void currentLengthPropertyChanged(int newValue);
	void measureUpdatedPropertyChanged(int newValue);
	void measureStartedPropertyChanged(int newValue);
	//void startMeasurePropertyChanged(int newValue);

public slots:
	void onTimeout() {
		//qDebug() << "TensoSensor::onTimeout";
		if (m_timerType == TIMER_TYPE_HOLD) {
#if 0
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
#endif
		}
	}

	void onMeasureTimeout() {
		qDebug() << "TensoSensor::onMeasureTimeout";
#if 0
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
#endif
	}

	void setZeroingLooseFlag(int loose) { m_zeroingLooseFlag = loose; }
	int getZeroingLooseFlag() { return m_zeroingLooseFlag; }
	void setSubOperationStarted(int started) { m_subOperationStarted = started; }
	int getSubOperationStarted() { return m_subOperationStarted; }

private:
	int m_operation;
	int m_subOperation;
	int m_operationCompleted;
	int m_parking;
	int m_forcestop;
	int m_currentForce;
	int m_currentLength;
	int m_measureUpdated;
	int m_measureStarted;

	int m_timerType;

	/* flags */
	int m_zeroingLooseFlag;
	int m_subOperationStarted;
};

#endif // __TENSOSENSOR_H__

