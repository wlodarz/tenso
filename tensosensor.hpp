#ifndef __TENSOSENSOR_H__
#define __TENSOSENSOR_H__

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <map>

#define MAX_POSITION 200
#define LENGTH_TODO 100 /* in cm */
#define FORCE_TODO 300

#define MAX_STATES 0x0010

enum {
	TIMER_TYPE_NONE,
	TIMER_TYPE_PULL,
	TIMER_TYPE_HOLD
};


class TensoSensor : public QObject {
	Q_OBJECT

		Q_PROPERTY(int operation READ operationProperty WRITE setOperationProperty NOTIFY operationPropertyChanged)
		Q_PROPERTY(int suboperation READ subOperationProperty WRITE setSubOperationProperty NOTIFY subOperationPropertyChanged)
		Q_PROPERTY(int operationcompleted READ operationCompletedProperty WRITE setOperationCompletedProperty NOTIFY operationCompletedPropertyChanged)

		Q_PROPERTY(int movedirection READ moveDirectionProperty WRITE setMoveDirectionProperty NOTIFY moveDirectionPropertyChanged)

		Q_PROPERTY(int currentForce READ currentForceProperty WRITE setCurrentForceProperty NOTIFY currentForcePropertyChanged)
		Q_PROPERTY(int currentLength READ currentLengthProperty WRITE setCurrentLengthProperty NOTIFY currentLengthPropertyChanged)

		Q_PROPERTY(int measureUpdated READ measureUpdatedProperty WRITE setMeasureUpdatedProperty NOTIFY measureUpdatedPropertyChanged)
		Q_PROPERTY(int measureStarted READ measureStartedProperty WRITE setMeasureStartedProperty NOTIFY measureStartedPropertyChanged)

		Q_PROPERTY(int measureIndex READ measureIndexProperty WRITE setMeasureIndexProperty NOTIFY measureIndexPropertyChanged)

		Q_PROPERTY(int maxForce READ maxForceProperty WRITE setMaxForceProperty NOTIFY maxForcePropertyChanged)
		Q_PROPERTY(int maxLength READ maxLengthProperty WRITE setMaxLengthProperty NOTIFY maxLengthPropertyChanged)

		Q_PROPERTY(int calculatedPower READ calculatedPowerProperty WRITE setCalculatedPowerProperty NOTIFY calculatedPowerPropertyChanged)

	public:
		TensoSensor(QObject *parent = 0) : QObject(parent),
		m_operation(SENSOR_OPERATION_IDLE),
		m_subOperation(NONE_SUBOPERATION),
		m_operationCompleted(0),
		m_moveDirection(0),
		m_currentForce(0),
		m_currentLength(0),
		m_maxForce(0),
		m_maxLength(0),
		m_measureUpdated(0),
		m_measureStarted(0),
		m_measureIndex(0),
		m_calculatedPower(0),
		m_timerType(TIMER_TYPE_NONE)
	{}
		~TensoSensor() {}

		int operationProperty() const { return m_operation; }
		void setOperationProperty(int val) { m_operation = val; emit operationPropertyChanged(val); }

		int subOperationProperty() const { return m_subOperation; }
		void setSubOperationProperty(int val) { m_subOperation = val; m_subOperationStarted = 0; emit subOperationPropertyChanged(val); }

		int operationCompletedProperty() const { return m_operationCompleted; }
		void setOperationCompletedProperty(int val) { m_operationCompleted = val; emit operationCompletedPropertyChanged(val); }

		int moveDirectionProperty() const { return m_moveDirection; }
		void setMoveDirectionProperty(int val) { m_moveDirection = val; emit moveDirectionPropertyChanged(val); }

		int currentForceProperty() const { return m_currentForce; }
		void setCurrentForceProperty(int val) { m_currentForce = val; emit currentForcePropertyChanged(val); }

		int currentLengthProperty() const { return m_currentLength; }
		void setCurrentLengthProperty(int val) { m_currentLength = val; emit currentLengthPropertyChanged(val); }

		int maxForceProperty() const { return m_maxForce; }
		void setMaxForceProperty(int val) { m_maxForce = val; emit maxForcePropertyChanged(val); }

		int maxLengthProperty() const { return m_maxLength; }
		void setMaxLengthProperty(int val) { m_maxLength = val; emit maxLengthPropertyChanged(val); }

		int measureUpdatedProperty() const { return m_measureUpdated; }
		void setMeasureUpdatedProperty(int val) { m_measureUpdated = val; emit measureUpdatedPropertyChanged(val); }

		int measureStartedProperty() const { return m_measureStarted; }
		void setMeasureStartedProperty(int val) { m_measureStarted = val; emit measureStartedPropertyChanged(val); }

		int measureIndexProperty() const { qDebug() << "measureIndex get"; return m_measureIndex; }
		void setMeasureIndexProperty(int val) { m_measureIndex = val; emit measureIndexPropertyChanged(val); }

		int calculatedPowerProperty() const { qDebug() << "calculatedPower get"; return m_calculatedPower; }
		void setCalculatedPowerProperty(int val) { m_calculatedPower = val; emit calculatedPowerPropertyChanged(val); }

		int timerTypeProperty() const { return m_timerType; }
		void setTimerType(int type) { m_timerType = type; }

		void updateMeasure(int position, int force) {
			position_values[m_measureIndex] = position;
			force_values[m_measureIndex] = force;
			m_measureIndex++;
		}
		void calculatePower() {
			int i;
			int power = 0;
			for(i=0; i<m_measureIndex; i++) power += force_values[i];
			m_calculatedPower = power;
		}

		enum Operations {
			SENSOR_OPERATION_IDLE = 0,
			SENSOR_OPERATION_STOP,
			SENSOR_OPERATION_MOVE,
			SENSOR_OPERATION_CALIBRATE,
			SENSOR_OPERATION_ZERO,
			SENSOR_OPERATION_PARK,
			SENSOR_OPERATION_MEASURE1,
			SENSOR_OPERATION_MEASURE2,
			SENSOR_OPERATION_LAST
		};
		Q_ENUMS(Operations)

		/* zeroing operation states */
		enum Suboperations {
			NONE_SUBOPERATION = 0,
			/* ZERO */
			ZERO_SUBOPERATION_MOTOROFF,
			ZERO_SUBOPERATION_TILL40KG,

			/* MEASURE1 */
			MEASURE1_SUBOPERATION_20CM,
			MEASURE1_SUBOPERATION_MEASURE_LEN,
			MEASURE1_SUBOPERATION_100CM,

			/* MEASURE2 */
			MEASURE2_SUBOPERATION_TILL40KG,

			/* PARK */
			PARK_SUBOPERATION_INPROGRESS,
		};
		Q_ENUMS(Suboperations)

signals:
		void operationPropertyChanged(int newValue);
		void subOperationPropertyChanged(int newValue);
		void operationCompletedPropertyChanged(int newValue);
		void moveDirectionPropertyChanged(int newValue);
		void currentForcePropertyChanged(int newValue);
		void currentLengthPropertyChanged(int newValue);
		void maxForcePropertyChanged(int newValue);
		void maxLengthPropertyChanged(int newValue);
		void measureUpdatedPropertyChanged(int newValue);
		void measureStartedPropertyChanged(int newValue);
		void measureIndexPropertyChanged(int newValue);
		void calculatedPowerPropertyChanged(int newValue);

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
		int m_moveDirection;
		int m_currentForce;
		int m_currentLength;
		int m_maxForce;
		int m_maxLength;
		int m_measureUpdated;
		int m_measureStarted;
		int m_calculatedPower;
		int m_timerType;

		/* flags */
		int m_zeroingLooseFlag;
		int m_subOperationStarted;

		int m_measureIndex;

		std::map<int, int> position_values;
		std::map<int, int> force_values;
};

#endif // __TENSOSENSOR_H__

