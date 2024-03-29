
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

QString TensoConfig::getConfigStr(QString key)
{
	if (configHolder.find(key) == configHolder.end()) return QString("");
	return configHolder[key];
}

int TensoConfig::getConfigInt(QString key)
{
	if (configHolder.find(key) == configHolder.end()) return -1;
	return configHolder[key].toInt();
}

float TensoConfig::getConfigFloat(QString key)
{
	if (configHolder.find(key) == configHolder.end()) return -1;
	return configHolder[key].toFloat();
}

int TensoConfig::parseLine(QString line)
{
	QString key, value;
    	QRegExp rx("([a-zA-Z0-9.]+)=([a-zA-Z0-9.-,]+)");
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
    	//QFile configFile("c:/tenso/config.txt");
    	qDebug() << "config filename " << configFileName;
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
	CHECK_CONFIG_ENTRY(LENGTH_LOOSEN_KEY);
	CHECK_CONFIG_ENTRY(FORCE_MAX_KEY);
	CHECK_CONFIG_ENTRY(LENGTH_MAX_KEY);
	CHECK_CONFIG_ENTRY(DIRECTION_KEY);
	CHECK_CONFIG_ENTRY(TENSO_MODE_KEY);
	CHECK_CONFIG_ENTRY(ENGINE_MODE_KEY);
	CHECK_CONFIG_ENTRY(ENGINE_MAXSPEEDDIV_KEY);
	CHECK_CONFIG_ENTRY(ENGINE_MINSPEEDDIV_KEY);
	CHECK_CONFIG_ENTRY(ENGINE_SPEED1_KEY);
	CHECK_CONFIG_ENTRY(ENGINE_SPEED2_KEY);
	CHECK_CONFIG_ENTRY(ENGINE_SPEED3_KEY);
	CHECK_CONFIG_ENTRY(ENGINE_SPEED4_KEY);
	CHECK_CONFIG_ENTRY(ENGINE_SPEED5_KEY);
	CHECK_CONFIG_ENTRY(ENGINE_FORCELEVEL1_KEY);
	CHECK_CONFIG_ENTRY(ENGINE_FORCELEVEL2_KEY);
	CHECK_CONFIG_ENTRY(ENGINE_FORCELEVEL3_KEY);
	CHECK_CONFIG_ENTRY(ENGINE_FORCELEVEL4_KEY);
	CHECK_CONFIG_ENTRY(ENGINE_FORCELEVEL5_KEY);
	CHECK_CONFIG_ENTRY(CALIBRATE_FORCE_0_KEY);
	CHECK_CONFIG_ENTRY(CALIBRATE_LENGTH_LOOSE_KEY);
    	CHECK_CONFIG_ENTRY(CALIBRATE_LENGTH_KEY);
	CHECK_CONFIG_ENTRY(MEASURE_POINTS_KEY);
	CHECK_CONFIG_ENTRY(MEASURE1_HOLDTIME_KEY);
	CHECK_CONFIG_ENTRY(MEASURE2_HOLDTIME_KEY);
	CHECK_CONFIG_ENTRY(MEASURE2_HOLDFORCE_KEY);
    CHECK_CONFIG_ENTRY(MEASURE2_MINLENGTH_KEY);
	CHECK_CONFIG_ENTRY(CONFIG_ROTATION_CONST1_KEY);
	CHECK_CONFIG_ENTRY(CONFIG_ROTATION_CONST2_KEY);
	
	return 0;
}

