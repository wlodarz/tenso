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

    tensosensor = new TensoSensor();
    qmlRegisterSingletonType<TensoSensor>("TensoSensor", 1, 0, "TensoSensor", tensosensor_qobject_singleton_provider);

    TensoThread *tenso = new TensoThread();
    tenso->setTensoSensor((TensoSensor *)tensosensor);
    tenso->start();

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + QLatin1String("/tenso.qml")));

    const QString lowerArgument = QString::fromLatin1(argv[1]).toLower();
    if (lowerArgument == QLatin1String("--fullscreen")) {
        view.showFullScreen();
    } else {
        view.show();
    }

    return app.exec();
}
