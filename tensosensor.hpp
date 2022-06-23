#ifndef __TENSOSENSOR_H__
#define __TENSOSENSOR_H__

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QRegularExpression>
#include <map>
#include <vector>

#include <math.h>

#define MAX_POSITION 200
#define LENGTH_TODO 100
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

		Q_PROPERTY(float currentForce READ currentForceProperty WRITE setCurrentForceProperty NOTIFY currentForcePropertyChanged)
		Q_PROPERTY(float currentLength READ currentLengthProperty WRITE setCurrentLengthProperty NOTIFY currentLengthPropertyChanged)

		Q_PROPERTY(int measureUpdated READ measureUpdatedProperty WRITE setMeasureUpdatedProperty NOTIFY measureUpdatedPropertyChanged)
		Q_PROPERTY(int measureLevelsUpdated READ measureLevelsUpdatedProperty WRITE setMeasureLevelsUpdatedProperty NOTIFY measureLevelsUpdatedPropertyChanged)
		Q_PROPERTY(int measureStarted READ measureStartedProperty WRITE setMeasureStartedProperty NOTIFY measureStartedPropertyChanged)

		Q_PROPERTY(int measureIndex READ measureIndexProperty WRITE setMeasureIndexProperty NOTIFY measureIndexPropertyChanged)
		Q_PROPERTY(int measureUpIndex READ measureUpIndexProperty WRITE setMeasureUpIndexProperty NOTIFY measureUpIndexPropertyChanged)
		Q_PROPERTY(int measureHoldIndex READ measureHoldIndexProperty WRITE setMeasureHoldIndexProperty NOTIFY measureHoldIndexPropertyChanged)
		Q_PROPERTY(int measureDownIndex READ measureDownIndexProperty WRITE setMeasureDownIndexProperty NOTIFY measureDownIndexPropertyChanged)
		Q_PROPERTY(int measureLevelsIndex READ measureLevelsIndexProperty WRITE setMeasureLevelsIndexProperty NOTIFY measureLevelsIndexPropertyChanged)

		Q_PROPERTY(float maxForce READ maxForceProperty WRITE setMaxForceProperty NOTIFY maxForcePropertyChanged)
		Q_PROPERTY(int maxPosition READ maxPositionProperty WRITE setMaxPositionProperty NOTIFY maxPositionPropertyChanged)
        	Q_PROPERTY(float maxLength READ maxLengthProperty WRITE setMaxLengthProperty NOTIFY maxLengthPropertyChanged)

		Q_PROPERTY(float calculatedWorkUp READ calculatedWorkUpProperty WRITE setCalculatedWorkUpProperty NOTIFY calculatedWorkUpPropertyChanged)
		Q_PROPERTY(float calculatedWorkHold READ calculatedWorkHoldProperty WRITE setCalculatedWorkHoldProperty NOTIFY calculatedWorkHoldPropertyChanged)
		Q_PROPERTY(float calculatedWorkDown READ calculatedWorkDownProperty WRITE setCalculatedWorkDownProperty NOTIFY calculatedWorkDownPropertyChanged)
		Q_PROPERTY(int measurePhase READ measurePhaseProperty WRITE setMeasurePhaseProperty NOTIFY measurePhasePropertyChanged)
		Q_PROPERTY(float maxMeasuredLength READ maxMeasuredLengthProperty WRITE setMaxMeasuredLengthProperty NOTIFY maxMeasuredLengthPropertyChanged)
		Q_PROPERTY(float minMeasuredLength READ minMeasuredLengthProperty WRITE setMinMeasuredLengthProperty NOTIFY minMeasuredLengthPropertyChanged)
		Q_PROPERTY(int secondsCounter READ secondsCounterProperty WRITE setSecondsCounterProperty NOTIFY secondsCounterPropertyChanged)
		Q_PROPERTY(int secondsHoldCounter READ secondsHoldCounterProperty WRITE setSecondsHoldCounterProperty NOTIFY secondsHoldCounterPropertyChanged)
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
		m_currentForce(0.0),
		m_currentLength(0.0),
		m_maxForce(0.0),
        	//m_maxMeasuredForce(0.0),
        	//m_oneMMeasuredForce(0.0),
		m_maxPosition(0),
        	m_maxLength(0.0),
		m_measureUpdated(0),
		m_measureLevelsUpdated(0),
		m_measureStarted(0),
		m_calculatedWorkUp(0),
		m_calculatedWorkHold(0),
		m_calculatedWorkDown(0),
		m_timerType(TIMER_TYPE_NONE),
		m_measureIndex(0),
		m_measureUpIndex(0),
		m_measureHoldIndex(0),
		m_measureDownIndex(0),
		m_measureLevelsIndex(0),
		m_measurePhase(MEASURE_PHASE_NONE),
		m_maxMeasuredLength(0.0),
		m_minMeasuredLength(0.0),
		m_secondsCounter(0),
		m_secondsHoldCounter(0),
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

		float currentForceProperty() const { return m_currentForce; }
		void setCurrentForceProperty(float val) { m_currentForce = val; emit currentForcePropertyChanged(val); }

		float currentLengthProperty() const { return m_currentLength; }
		void setCurrentLengthProperty(float val) { m_currentLength = val; emit currentLengthPropertyChanged(val); }

		float maxForceProperty() const { return m_maxForce; }
		void setMaxForceProperty(float val) { m_maxForce = val; emit maxForcePropertyChanged(val); }

		int maxPositionProperty() const { return m_maxPosition; }
		void setMaxPositionProperty(int val) { m_maxPosition = val; emit maxPositionPropertyChanged(val); }

        	float maxLengthProperty() const { return m_maxLength; }
        	void setMaxLengthProperty(float val) { m_maxLength = val; emit maxLengthPropertyChanged(val); }

		int measureUpdatedProperty() const { return m_measureUpdated; }
		void setMeasureUpdatedProperty(int val) { m_measureUpdated = val; emit measureUpdatedPropertyChanged(val); }

		int measureLevelsUpdatedProperty() const { return m_measureLevelsUpdated; }
		void setMeasureLevelsUpdatedProperty(int val) { m_measureLevelsUpdated = val; emit measureLevelsUpdatedPropertyChanged(val); }

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
		int measureLevelsIndexProperty() const { return m_measureLevelsIndex; }
		void setMeasureLevelsIndexProperty(int val) { m_measureLevelsIndex = val; emit measureLevelsIndexPropertyChanged(val); }

		float calculatedWorkUpProperty() const { qDebug() << "calculatedWorkUp get " << m_calculatedWorkUp; return m_calculatedWorkUp; }
		void setCalculatedWorkUpProperty(float val) { m_calculatedWorkUp = val; emit calculatedWorkUpPropertyChanged(val); }
		float calculatedWorkHoldProperty() const { qDebug() << "calculatedWorkHold get " << m_calculatedWorkHold; return m_calculatedWorkHold; }
		void setCalculatedWorkHoldProperty(float val) { m_calculatedWorkHold = val; emit calculatedWorkHoldPropertyChanged(val); }
		float calculatedWorkDownProperty() const { qDebug() << "calculatedWorkDown get " << m_calculatedWorkDown; return m_calculatedWorkDown; }
		void setCalculatedWorkDownProperty(float val) { m_calculatedWorkDown = val; emit calculatedWorkDownPropertyChanged(val); }
		int secondsCounterProperty() const { /* qDebug() << "secondsCounter get"; */ return m_secondsCounter; }
		void setSecondsCounterProperty(int val) { m_secondsCounter = val; emit secondsCounterPropertyChanged(val); }
		int secondsHoldCounterProperty() const { /* qDebug() << "secondsHoldCounter get"; */ return m_secondsHoldCounter; }
		void setSecondsHoldCounterProperty(int val) { m_secondsHoldCounter = val; emit secondsHoldCounterPropertyChanged(val); }

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

		void updateMeasure(float length, float force) {
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
			length_values[m_measureIndex] = length;
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
		float getLengthSample(int sample) {
			return length_values[sample];
		}

		float calculateWork(int index) {
			int i;
			float work = 0.0;
			//qDebug() << "calculating " << index;
			for(i=1; i<index; i++) {
				float force_mid = (force_values[i] + force_values[i-1]) / 2.0;
				float length_delta = fabsf(length_values[i]-length_values[i-1]);
				work += (force_mid * length_delta);
				//qDebug() << "work " << work << " force_mid " << force_mid << " position_delta " << position_delta;
			}
			//work /= 100.0; // mm->m
			//work /= 1000.0; // g -> kg
			qDebug() << "Up Power " << work << "kg*m " << index;
			//setCalculatedWorkUpProperty((int)work);
			return work;
		}

		int length2steps(float length) {
			int steps_end = 0;
			float length_end = 0.0;
			int steps_delta = 0;

			std::map<unsigned int, float>::iterator it = steps2length_table.begin();
			steps_end = it->first;
			length_end = it->second;
			
			steps_delta = length * steps_end / length_end;

			//qDebug() << "length2steps " << steps_delta;

			return steps_delta;
		}

		float steps2length(int steps) {
			int steps_end = 0;
			float length_end = 0.0;
			float  length_delta = 0.0;

            		//qDebug() << "steps2length " << steps;

			std::map<unsigned int, float>::iterator it = steps2length_table.begin();
			steps_end = it->first;
			length_end = it->second;
			
			length_delta = steps * length_end / steps_end;
			//qDebug() << "steps2length " << length_delta;

			return length_delta;
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
			MEASURE_LEVELS,
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
		void maxPositionPropertyChanged(int newValue);
        	void maxLengthPropertyChanged(int newValue);
		void measureUpdatedPropertyChanged(int newValue);
		void measureLevelsUpdatedPropertyChanged(int newValue);
		void measureStartedPropertyChanged(int newValue);
		void measureIndexPropertyChanged(int newValue);
		void measureUpIndexPropertyChanged(int newValue);
		void measureHoldIndexPropertyChanged(int newValue);
		void measureDownIndexPropertyChanged(int newValue);
		void measureLevelsIndexPropertyChanged(int newValue);
		void calculatedWorkUpPropertyChanged(float newValue);
		void calculatedWorkHoldPropertyChanged(float newValue);
		void calculatedWorkDownPropertyChanged(float newValue);
		void measurePhasePropertyChanged(int newValue);
		void maxMeasuredLengthPropertyChanged(float newValue);
		void minMeasuredLengthPropertyChanged(float newValue);
		void secondsCounterPropertyChanged(int newValue);
		void secondsHoldCounterPropertyChanged(int newValue);
		void turnsPropertyChanged(float newValue);
		void startingLengthPropertyChanged(float newValue);
		void saveReportFlagPropertyChanged(int newValue);
		void lotIdPropertyChanged(QString newValue);

	public slots:
		void onTimeout() {
			//qDebug() << "m_secondsCounter " << m_secondsCounter;
			if (m_secondsHoldCounter > 0)
				setSecondsHoldCounterProperty(m_secondsHoldCounter-1);
			//m_secondsCounter++;
			setSecondsCounterProperty(m_secondsCounter+1);
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

		void addStep2LengthTableEntry(unsigned int steps, float length) { steps2length_table.insert(std::pair<unsigned int, float>(steps, length)); }

		void setMeasurePoints(QString points_string) {
			m_measurePointsSize = 0;
			QStringList list = points_string.split(',');
			for(int i=0; i<list.size(); i++) {
				measure_points[i] = list.at(i).toFloat();
				m_measurePointsSize++;
			}
		}
		int getMeasurePointsSize() { return m_measurePointsSize; }
		float getMeasurePoint(int index) { return measure_points[index]; }
		void addMeasurePoint(float value) { measure_points[m_measurePointsSize] = value; m_measurePointsSize++; }

		void addMeasuredValue(float length, float force) { m_measuredLength.push_back(length); m_measuredForce.push_back(force); }
		float getMeasuredValueLength(int i) { return m_measuredLength[i]; }
		float getMeasuredValueForce(int i) { return m_measuredForce[i]; }
		int getMeasuredValueSize() { return m_measuredLength.size(); }

	public:
		int m_last_move_val;

	private:
		int m_operation;
		int m_subOperation;
		int m_operationCompleted;
		int m_moveDirection;
		float m_currentForce;
		float m_currentLength;
		float m_maxForce;
		int m_maxPosition;
        	float m_maxLength;
		int m_measureUpdated;
		int m_measureLevelsUpdated;
		int m_measureStarted;
		float m_calculatedWorkUp;
		float m_calculatedWorkHold;
		float m_calculatedWorkDown;
		int m_timerType;
		int m_measureIndex;
		int m_measureUpIndex;
		int m_measureHoldIndex;
		int m_measureDownIndex;
		int m_measureLevelsIndex;

		/* flags */
		int m_zeroingLooseFlag;
		int m_subOperationStarted;

		int m_measurePhase;

		float m_maxMeasuredLength;
		float m_minMeasuredLength;
		int m_secondsCounter;
		int m_secondsHoldCounter;
		float m_turns;
		float m_startingLength;
		int m_saveReportFlag;
		QString m_lotId;

		int m_measurePointsSize;

		std::map<int, float> length_values;
		std::map<int, float> force_values;

		std::map<unsigned int, float> steps2length_table;
		std::map<unsigned int, float> measure_points;
		std::map<unsigned int, float> forceLevels;

		std::vector<float> m_measuredForce;
		std::vector<float> m_measuredLength;
};

#endif // __TENSOSENSOR_H__

