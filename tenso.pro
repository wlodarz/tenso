TEMPLATE = app

QT += gui qml quick widgets core network

SOURCES += main.cpp tensothread.cpp tensometer.cpp stepperengine.cpp tensoconfig.cpp
HEADERS += tensosensor.hpp tensothread.hpp tensometer.hpp stepperengine.hpp tensoconfig.hpp config.hpp
LIBS += -lphidget21

target.path = /opt/tenso
qml.files = tenso.qml content
qml.path = /opt/tenso
INSTALLS += target qml
