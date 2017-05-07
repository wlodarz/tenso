#ifndef __TENSOCONFIG_HPP__
#define __TENSOCONFIG_HPP__

//#include <QtCore>
//#include <QDebug>
#include <QString>
#include <QMap>

#define LENGTH_LOOSEN_KEY "length.loosen"
#define FORCE_MAX_KEY     "force.max"
#define FORCE_MIN_KEY     "force.min"
#define LENGTH_20CM_KEY   "length.20cm"
#define LENGTH_100CM_KEY  "length.100cm"
#define LENGTH_MAX_KEY   "length.max"
#define DIRECTION_KEY     "direction"
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


class TensoConfig //: public QThread
{
//  Q_OBJECT
public:
	TensoConfig();
	~TensoConfig() {}

	int init(QString configFileName);
	QString getConfigValueStr(QString key);
	int getConfigValueInt(QString key);

private:
	int load(QString configFileName);
	int validate();
	int parseLine(QString line);

	QMap<QString, QString> configHolder;
};


#endif //__TENSOCONFIG_HPP__

