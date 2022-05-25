TEMPLATE = app

QT += gui qml quick widgets core network

SOURCES += main.cpp tensothread.cpp tensometer.cpp stepperengine.cpp tensoconfig.cpp encoder.cpp testengine.cpp
HEADERS += tensosensor.hpp tensothread.hpp tensometer.hpp stepperengine.hpp tensoconfig.hpp encoder.hpp engine.hpp testengine.hpp
LIBS += -lphidget22

target.path = /opt/tenso
qml.files = tenso.qml Config.qml  Counter.qml  error1.qml  error2.qml  error3.qml  Moving.qml  Report.qml  TensoButton.qml  TensoField.qml  TensoInputField.qml content
qml.path = /opt/tenso
INSTALLS += target qml

CONFIG += force_debug_info
