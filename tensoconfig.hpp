#ifndef __TENSOCONFIG_HPP__
#define __TENSOCONFIG_HPP__

#include <QString>
#include <QMap>

#define CHECK_CONFIG_ENTRY(x) \
	if (configHolder.find(x) == configHolder.end()) { \
		qDebug() << "can not find config entry " << x; \
		return -1; \
	}

#define LENGTH_LOOSEN_KEY "length.loosen"
#define FORCE_MAX_KEY     "force.max"
#define FORCE_MIN_KEY     "force.min"
#define LENGTH_20CM_KEY   "length.20cm"
#define LENGTH_100CM_KEY  "length.100cm"
#define LENGTH_MAX_KEY   "length.max"
#define DIRECTION_KEY     "direction"
#define TENSO_MODE	  "tenso.mode"
#define ENGINE_MODE	  "engine.mode"
#define ENGINE_MINSPEEDDIV_KEY  "engine.minspeeddiv"
#define ENGINE_MAXSPEEDDIV_KEY  "engine.maxspeeddiv"
#define ENGINE_SPEED1_KEY  "engine.speed1"
#define ENGINE_SPEED2_KEY  "engine.speed2"
#define ENGINE_SPEED3_KEY  "engine.speed3"
#define ENGINE_SPEED4_KEY  "engine.speed4"
#define ENGINE_SPEED5_KEY  "engine.speed5"
#define ENGINE_FORCELEVEL1_KEY  "engine.forcelevel1"
#define ENGINE_FORCELEVEL2_KEY  "engine.forcelevel2"
#define ENGINE_FORCELEVEL3_KEY  "engine.forcelevel3"
#define ENGINE_FORCELEVEL4_KEY  "engine.forcelevel4"
#define ENGINE_FORCELEVEL5_KEY  "engine.forcelevel5"
#define CALIBRATE_FORCE_0_KEY "calibrate.force.0"
#define CALIBRATE_LENGTH_LOOSE_KEY "calibrate.length.loose"
#define MEASURE1_HOLDTIME_KEY "measure1.holdtime"
#define MEASURE2_HOLDTIME_KEY "measure2.holdtime"
#define MEASURE2_HOLDFORCE_KEY "measure2.holdforce"
#define MEASURE2_MINFORCE_KEY "measure2.minforce"
#define CONFIG_ROTATION_CONST1_KEY "config.rotation.const1"
#define CONFIG_ROTATION_CONST2_KEY "config.rotation.const2"
#define GOSTART_OFFSET "gostart.offset"


class TensoConfig //: public QThread
{
//  Q_OBJECT
public:
	TensoConfig();
	~TensoConfig() {}

	int init(QString configFileName);
	QString getConfigStr(QString key);
	int getConfigInt(QString key);
	float getConfigFloat(QString key);

private:
	int load(QString configFileName);
	int validate();
	int parseLine(QString line);

	QMap<QString, QString> configHolder;
};


#endif //__TENSOCONFIG_HPP__

