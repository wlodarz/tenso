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

#define CONV_TAB_SIZE 14

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
		void setOperationProperty(int val) { qDebug() << "setOperation " << val; m_operation = val; emit operationPropertyChanged(val); }

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
			qDebug() << "calculating " << m_measureIndex;
			for(i=1; i<m_measureIndex; i++) {
				int force_mid = (force_values[i] + force_values[i-1]) / 2;
				int position_delta = steps2mm(position_values[i])-steps2mm(position_values[i-1]);
				power += (force_mid * position_delta);
				qDebug() << "power " << power << " force_mid " << force_mid << " position_delta " << position_delta;
			}
			//m_calculatedPower = power;
			float fpower = power;
			fpower /= 1000; // mm->m
			fpower /= 1000; // g -> kg
			qDebug() << "Power " << fpower << "kg*m";
			setCalculatedPowerProperty(power);
		}
		unsigned int steps2mm(unsigned int steps) {
			int i;
			qDebug() << "steps2mm " << steps;
			for(i=1; i<CONV_TAB_SIZE; i++) {
				if (steps < convtable[i].steps) {
					qDebug() << "steps_i " << convtable[i].steps;
					break;
				}
			}

			float perc = (float)(steps - convtable[i-1].steps) / (float)(convtable[i].steps - convtable[i-1].steps);
			int mm_delta = convtable[i-1].mm + (convtable[i].mm - convtable[i-1].mm) * perc;
			qDebug() << "perc " << perc << "mm_delta " << mm_delta;
			return mm_delta;
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
			ZERO_SUBOPERATION_MOTOROFF, // 1
			ZERO_SUBOPERATION_TILL40KG, // 2

			/* MEASURE1 */
			MEASURE1_SUBOPERATION_20CM, // 3
			MEASURE1_SUBOPERATION_MEASURE_LEN, // 4
			MEASURE1_SUBOPERATION_100CM, // 5

			/* MEASURE2 */
			MEASURE2_SUBOPERATION_TILL40KG, // 6

			/* PARK */
			PARK_SUBOPERATION_INPROGRESS, // 7

			/* CALIBRATE */
			CALIBRATE_SUBOPERATION_TILL_FORCE_MAX, // 8
			CALIBRATE_SUBOPERATION_LOOSE, // 9
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

	public:
		int m_last_move_val;

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

			struct convertable {
				unsigned int steps;
				unsigned int mm;
			} convtable[CONV_TAB_SIZE] = {
				{0, 0},
				{10014, 125},
				{20004, 270},
				{29994, 430},
				{40001, 590},
				{50025, 750},
				{55003, 850},
				{60011, 940},
				{65010, 1030},
				{70001, 1130},
				{74992, 1225},
				{80011, 1320},
				{85011, 1415},
				{90006, 1515}
			};
};

#endif // __TENSOSENSOR_H__

