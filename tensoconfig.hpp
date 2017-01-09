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
#define ENGINE_MAXSPEEDDIV_KEY  "engine.maxspeeddiv"
#define ENGINE_SPEED1_KEY  "engine.speed1"
#define ENGINE_SPEED2_KEY  "engine.speed2"
#define ENGINE_SPEED3_KEY  "engine.speed3"


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

