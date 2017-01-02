
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QRegExp>
#include <QDebug>

#include "tensoconfig.hpp"


TensoConfig::TensoConfig()
{
}

int TensoConfig::init(QString configFileName)
{
	int ret;

	if ((ret = load(configFileName)) < 0) return ret;
	return validate();
}

QString TensoConfig::getConfigValueStr(QString key)
{
	if (configHolder.find(key) == configHolder.end()) return QString("");
	return configHolder[key];
}

int TensoConfig::getConfigValueInt(QString key)
{
	if (configHolder.find(key) == configHolder.end()) return -1;
	return configHolder[key].toInt();
}

int TensoConfig::parseLine(QString line)
{
	QString key, value;
	QRegExp rx("([a-zA-Z0-9.]+)=([a-zA-Z0-9.]+)");
	//qDebug() << "parsing line " << line;
	if (rx.indexIn(line) < 0) {
		qDebug() << "line " << line << " not matched";
		return -1;
	}
	key = rx.cap(1);
	value = rx.cap(2);

	//qDebug() << "found " << key << " " << value;
	configHolder.insert(key, value);
	
	return 0;
}

int TensoConfig::load(QString configFileName)
{
	QFile configFile(configFileName);
	if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) return -1;

	QTextStream in(&configFile);
	QString line = in.readLine();

	while(!line.isNull()) {
		if (parseLine(line) < 0) {
			qDebug() << "bad line " << line;
		}
		line = in.readLine();
	}

	return 0;
}

int TensoConfig::validate()
{
	if (configHolder.find(LENGTH_LOOSEN_KEY) == configHolder.end()) return -1;
	if (configHolder.find(FORCE_MAX_KEY) == configHolder.end()) return -1;
	if (configHolder.find(LENGTH_20CM_KEY) == configHolder.end()) return -1;
	if (configHolder.find(LENGTH_100CM_KEY) == configHolder.end()) return -1;
	if (configHolder.find(LENGTH_FULL_KEY) == configHolder.end()) return -1;
	if (configHolder.find(DIRECTION_KEY) == configHolder.end()) return -1;

	return 0;
}

