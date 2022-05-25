#ifndef __TENSOSENSOR_H__
#define __TENSOSENSOR_H__

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <map>

#include <math.h>

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
		Q_PROPERTY(int measureUpIndex READ measureUpIndexProperty WRITE setMeasureUpIndexProperty NOTIFY measureUpIndexPropertyChanged)
		Q_PROPERTY(int measureHoldIndex READ measureHoldIndexProperty WRITE setMeasureHoldIndexProperty NOTIFY measureHoldIndexPropertyChanged)
		Q_PROPERTY(int measureDownIndex READ measureDownIndexProperty WRITE setMeasureDownIndexProperty NOTIFY measureDownIndexPropertyChanged)

		Q_PROPERTY(int maxForce READ maxForceProperty WRITE setMaxForceProperty NOTIFY maxForcePropertyChanged)
		Q_PROPERTY(int maxLength READ maxLengthProperty WRITE setMaxLengthProperty NOTIFY maxLengthPropertyChanged)

		Q_PROPERTY(float calculatedWorkUp READ calculatedWorkUpProperty WRITE setCalculatedWorkUpProperty NOTIFY calculatedWorkUpPropertyChanged)
		Q_PROPERTY(float calculatedWorkHold READ calculatedWorkHoldProperty WRITE setCalculatedWorkHoldProperty NOTIFY calculatedWorkHoldPropertyChanged)
		Q_PROPERTY(float calculatedWorkDown READ calculatedWorkDownProperty WRITE setCalculatedWorkDownProperty NOTIFY calculatedWorkDownPropertyChanged)
		Q_PROPERTY(int measurePhase READ measurePhaseProperty WRITE setMeasurePhaseProperty NOTIFY measurePhasePropertyChanged)
		Q_PROPERTY(float maxMeasuredLength READ maxMeasuredLengthProperty WRITE setMaxMeasuredLengthProperty NOTIFY maxMeasuredLengthPropertyChanged)
		Q_PROPERTY(float minMeasuredLength READ minMeasuredLengthProperty WRITE setMinMeasuredLengthProperty NOTIFY minMeasuredLengthPropertyChanged)
		Q_PROPERTY(int secondsCounter READ secondsCounterProperty WRITE setSecondsCounterProperty NOTIFY secondsCounterPropertyChanged)
		Q_PROPERTY(float turns READ turnsProperty WRITE setTurnsProperty NOTIFY turnsPropertyChanged)
		Q_PROPERTY(float startingLength READ startingLengthProperty WRITE setStartingLengthProperty NOTIFY startingLengthPropertyChanged)
        	Q_PROPERTY(int savereportflag READ saveReportFlagProperty WRITE setSaveReportFlagProperty NOTIFY saveReportFlagPropertyChanged)
		Q_PROPERTY(QString lotId READ lotIdProperty WRITE setLotIdProperty NOTIFY lotIdPropertyChanged)

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
		m_calculatedWorkUp(0),
		m_calculatedWorkHold(0),
		m_calculatedWorkDown(0),
		m_timerType(TIMER_TYPE_NONE),
		m_measureIndex(0),
		m_measureUpIndex(0),
		m_measureHoldIndex(0),
		m_measureDownIndex(0),
		m_measurePhase(MEASURE_PHASE_NONE),
		m_maxMeasuredLength(0.0),
		m_minMeasuredLength(0.0),
		m_secondsCounter(0),
		m_turns(0),
		m_startingLength(36.0),
		m_saveReportFlag(0),
		m_lotId("testLot")
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

		int measureUpIndexProperty() const { qDebug() << "measureUpIndex get"; return m_measureUpIndex; }
		void setMeasureUpIndexProperty(int val) { m_measureUpIndex = val; emit measureUpIndexPropertyChanged(val); }
		int measureHoldIndexProperty() const { qDebug() << "measureHoldIndex get"; return m_measureHoldIndex; }
		void setMeasureHoldIndexProperty(int val) { m_measureHoldIndex = val; emit measureHoldIndexPropertyChanged(val); }
		int measureDownIndexProperty() const { qDebug() << "measureDownIndex get"; return m_measureDownIndex; }
		void setMeasureDownIndexProperty(int val) { m_measureDownIndex = val; emit measureDownIndexPropertyChanged(val); }

		float calculatedWorkUpProperty() const { qDebug() << "calculatedWorkUp get " << m_calculatedWorkUp; return m_calculatedWorkUp; }
		void setCalculatedWorkUpProperty(float val) { m_calculatedWorkUp = val; emit calculatedWorkUpPropertyChanged(val); }
		float calculatedWorkHoldProperty() const { qDebug() << "calculatedWorkHold get " << m_calculatedWorkHold; return m_calculatedWorkHold; }
		void setCalculatedWorkHoldProperty(float val) { m_calculatedWorkHold = val; emit calculatedWorkHoldPropertyChanged(val); }
		float calculatedWorkDownProperty() const { qDebug() << "calculatedWorkDown get " << m_calculatedWorkDown; return m_calculatedWorkDown; }
		void setCalculatedWorkDownProperty(float val) { m_calculatedWorkDown = val; emit calculatedWorkDownPropertyChanged(val); }
		int secondsCounterProperty() const { qDebug() << "secondsCounter get"; return m_secondsCounter; }
		void setSecondsCounterProperty(int val) { m_secondsCounter = val; emit secondsCounterPropertyChanged(val); }

		float turnsProperty() const { qDebug() << "turns get"; return m_turns; }
		void setTurnsProperty(float val) { m_turns = val; emit turnsPropertyChanged(val); }

		float startingLengthProperty() const { qDebug() << "startingLength get"; return m_startingLength; }
		void setStartingLengthProperty(float val) { m_startingLength = val; emit startingLengthPropertyChanged(val); }

		int saveReportFlagProperty() const { return m_saveReportFlag; }
		void setSaveReportFlagProperty(int val) { m_saveReportFlag = val; emit saveReportFlagPropertyChanged(val); }

		QString lotIdProperty() const { return m_lotId; }
		void setLotIdProperty(QString val) { m_lotId = val; emit lotIdPropertyChanged(val); }

		int timerTypeProperty() const { return m_timerType; }
		void setTimerType(int type) { m_timerType = type; }

		void updateMeasure(int position, int force) {
			switch (m_measurePhase) {
				case MeasurePhases::MEASURE_PHASE_UP:
					m_measureUpIndex++;
					break;
				case MeasurePhases::MEASURE_PHASE_HOLD:
					m_measureHoldIndex++;
					break;
				case MeasurePhases::MEASURE_PHASE_DOWN:
					m_measureDownIndex++;
					break;
			}
			position_values[m_measureIndex] = position;
			force_values[m_measureIndex] = force;
			m_measureIndex++;
		}

		int measurePhaseProperty() const { return m_measurePhase; }
		void setMeasurePhaseProperty(int val) { m_measurePhase = val; emit measurePhasePropertyChanged(val); }

		float maxMeasuredLengthProperty() const { return m_maxMeasuredLength; }
		void setMaxMeasuredLengthProperty(float val) { m_maxMeasuredLength = val; emit maxMeasuredLengthPropertyChanged(val); }
		float minMeasuredLengthProperty() const { return m_minMeasuredLength; }
		void setMinMeasuredLengthProperty(float val) { m_minMeasuredLength = val; emit minMeasuredLengthPropertyChanged(val); }

		float getForceSample(int sample) {
			return force_values[sample];
		}
		float getDistanceSample(int sample) {
			return steps2cm(position_values[sample]);
		}

		float calculateWork(int index) {
			int i;
			float work = 0.0;
			//qDebug() << "calculating " << index;
			for(i=1; i<index; i++) {
				float force_mid = (force_values[i] + force_values[i-1]) / 2.0;
				float position_delta = fabsf(steps2cm(position_values[i])-steps2cm(position_values[i-1]));
				work += (force_mid * position_delta);
				//qDebug() << "work " << work << " force_mid " << force_mid << " position_delta " << position_delta;
			}
			work /= 100.0; // mm->m
			work /= 1000.0; // g -> kg
			qDebug() << "Up Power " << work << "kg*m " << index;
			//setCalculatedWorkUpProperty((int)work);
			return work;
		}

		float steps2cm(int steps) {
			int steps_start = 0, steps_end = 0;
			float cm_start = 0.0, cm_end = 0.0;
			float  cm_delta = 0.0;

			qDebug() << "steps2cm " << steps;

	        	if (steps < 0) steps = 0;

			for(std::map<unsigned int, float>::iterator it = steps2cm_table.begin();
					it != steps2cm_table.end();
					it++) {
				steps_end = it->first;
				cm_end = it->second;

				if (steps < steps_end) {
					qDebug() << "steps_i " << steps_end;
					break;
				}

				steps_start = steps_end;
				cm_start = cm_end;
			}

			float perc = (float)(steps - steps_start) / (float)(steps_end - steps_start);
			cm_delta = cm_start + (cm_end - cm_start) * perc;

			qDebug() << "perc " << perc << "cm_delta " << cm_delta;
			if (perc > 100) {
				qDebug() << "steps " << steps << " steps_start " << steps_start;
				qDebug() << "steps_end " << steps_end << " steps_start " << steps_start;
			}

			return cm_delta;
		}

		float calculateTurns(float const1, float const2, float maxlength, float minlength) {
			float rotates;
			rotates = ((maxlength + m_startingLength) / 2.54) * powf((((minlength+m_startingLength) + const1) / 2.54), 0.5) / const2;
			qDebug() << "startingLength " << m_startingLength << " maxlength " << maxlength << " minlength " << minlength << " const1 " << const1 << " const2 " << const2;
			return rotates;
		}

		enum Operations {
			SENSOR_OPERATION_IDLE = 0,
			SENSOR_OPERATION_STOP,
			SENSOR_OPERATION_MOVE,
			SENSOR_OPERATION_CALIBRATE,
			SENSOR_OPERATION_ZERO,
			SENSOR_OPERATION_GOSTART,
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
			ZERO_SUBOPERATION_TILLMAXFORCE, // 2

			/* MEASURE1 */
			MEASURE1_SUBOPERATION_TILLMAXFORCE, // 3
			MEASURE1_SUBOPERATION_WAIT_1MIN,    // 4
			MEASURE1_SUBOPERATION_TILLMINFORCE, // 5

			/* MEASURE1 */
			MEASURE2_SUBOPERATION_TILLSTARTMEASUREFORCE, // 3
			MEASURE2_SUBOPERATION_TILLMAXFORCE, // 3
			MEASURE2_SUBOPERATION_HOLDMAXFORCE,   // 4
			MEASURE2_SUBOPERATION_TILLMINFORCE, // 5

			/* MEASUREX */
			MEASUREX_SUBOPERATION_X, // 6

			/* GOSTART */
			GOSTART_SUBOPERATION_INPROGRESS, // 7

			/* CALIBRATE */
			CALIBRATE_SUBOPERATION_TILL_FORCE_MAX, // 8
			CALIBRATE_SUBOPERATION_LOOSE, // 9
		};
		Q_ENUMS(Suboperations)

		/* measure phase */
		enum MeasurePhases {
			MEASURE_PHASE_NONE = 0,
			MEASURE_PHASE_UP,
			MEASURE_PHASE_HOLD,
			MEASURE_PHASE_DOWN,
		};
		Q_ENUMS(MeasurePhases)

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
		void measureUpIndexPropertyChanged(int newValue);
		void measureHoldIndexPropertyChanged(int newValue);
		void measureDownIndexPropertyChanged(int newValue);
		void calculatedWorkUpPropertyChanged(float newValue);
		void calculatedWorkHoldPropertyChanged(float newValue);
		void calculatedWorkDownPropertyChanged(float newValue);
		void measurePhasePropertyChanged(int newValue);
		void maxMeasuredLengthPropertyChanged(float newValue);
		void minMeasuredLengthPropertyChanged(float newValue);
		void secondsCounterPropertyChanged(int newValue);
		void turnsPropertyChanged(float newValue);
		void startingLengthPropertyChanged(float newValue);
		void saveReportFlagPropertyChanged(int newValue);
		void lotIdPropertyChanged(QString newValue);

		public slots:
			void onTimeout() {
				//qDebug() << "m_secondsCounter " << m_secondsCounter;
				if (m_secondsCounter > 0)
					setSecondsCounterProperty(m_secondsCounter-1);
				//m_secondsCounter++;
			}

		void onMeasureTimeout() {
			qDebug() << "TensoSensor::onMeasureTimeout";
		}

	public:
		void setZeroingLooseFlag(int loose) { m_zeroingLooseFlag = loose; }
		int getZeroingLooseFlag() { return m_zeroingLooseFlag; }
		void setSubOperationStarted(int started) { m_subOperationStarted = started; }
		int getSubOperationStarted() { return m_subOperationStarted; }
		int getMeasureUpIndex() { return m_measureUpIndex; }
		int getMeasureHoldIndex() { return m_measureHoldIndex; }
		int getMeasureDownIndex() { return m_measureDownIndex; }

		void addStep2CmTableEntry(unsigned int steps, float cm) { steps2cm_table.insert(std::pair<unsigned int, float>(steps, cm)); }

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
		float m_calculatedWorkUp;
		float m_calculatedWorkHold;
		float m_calculatedWorkDown;
		int m_timerType;
		int m_measureIndex;
		int m_measureUpIndex;
		int m_measureHoldIndex;
		int m_measureDownIndex;

		/* flags */
		int m_zeroingLooseFlag;
		int m_subOperationStarted;

		int m_measurePhase;

		float m_maxMeasuredLength;
		float m_minMeasuredLength;
		int m_secondsCounter;
		float m_turns;
		float m_startingLength;
		int m_saveReportFlag;
		QString m_lotId;

		std::map<int, signed int> position_values;
		std::map<int, signed int> force_values;

		std::map<unsigned int, float> steps2cm_table;

/*
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
*/
};

#endif // __TENSOSENSOR_H__

