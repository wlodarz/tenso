#include <QApplication>
#include <QQuickView>
//#include <QQuickItem>

#include <QObject>
#include <QtQml>

#include "tensothread.hpp"
#include "tensosensor.hpp"

static TensoSensor *tensosensor;

QObject *tensosensor_qobject_singleton_provider(QQmlEngine *engine, QJSEngine *jsengine)
{
    Q_UNUSED(engine);
    Q_UNUSED(jsengine);

    return (QObject *)tensosensor;
}

int main(int argc, char* argv[])
{
    QApplication app(argc,argv);
    QQuickView view;
    int ret;

    tensosensor = new TensoSensor();
    qmlRegisterSingletonType<TensoSensor>("TensoSensor", 1, 0, "TensoSensor", tensosensor_qobject_singleton_provider);

    TensoThread *tensoThread = new TensoThread();
    ret = tensoThread->init(tensosensor);

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    QObject::connect((QObject*)view.engine(), SIGNAL(quit()), &app, SLOT(quit()));

    switch (ret) {
	/* config failed */
	case -1:
    		view.setSource(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + QLatin1String("/error1.qml")));
		break;
	/* missing engine */
	case -2:
    		view.setSource(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + QLatin1String("/error2.qml")));
		break;
	/* missing tensometr */
	case -3:
    		view.setSource(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + QLatin1String("/error3.qml")));
		break;
	default:
    		view.setSource(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + QLatin1String("/tenso.qml")));
    }

    const QString lowerArgument = QString::fromLatin1(argv[1]).toLower();
    if (lowerArgument == QLatin1String("--fullscreen")) {
        view.showFullScreen();
    } else {
        view.show();
    }

    return app.exec();
}
