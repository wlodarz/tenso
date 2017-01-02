#ifndef __TENSOCONFIG_HPP__
#define __TENSOCONFIG_HPP__

//#include <QtCore>
//#include <QDebug>
#include <QString>
#include <QMap>


#if 0
#define TENSO_LENGTH_20CM     (1*1000)
#define TENSO_LENGTH_100CM    (2*1000)
#define TENSO_LENGTH_FULL     (5*1000)
#define TENSO_FORCE_MAX	      (10000)
#endif

#define LENGTH_LOOSEN_KEY "length.loosen"
#define FORCE_MAX_KEY     "force.max"
#define FORCE_MIN_KEY     "force.min"
#define LENGTH_20CM_KEY   "length.20cm"
#define LENGTH_100CM_KEY  "length.100cm"
#define LENGTH_FULL_KEY   "length.full"
#define DIRECTION_KEY     "direction"


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

