QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE =lib
TARGET = darkchanneldehazor
DESTDIR = ../lib/Dehazor

DEFINES += DEHAZECODE

HEADERS += \
    darkchanneldehazor.h\
    dehazecode_global.h

SOURCES += \
    darkchanneldehazor.cpp


win32:CONFIG(release, debug|release):LIBS += -LC:/OSGeo4W64/lib/ -lopencv_core248 -lopencv_highgui248 -lopencv_gpu248 -lopencv_imgproc248 -lopencv_features2d248 -lopencv_photo248
else:win32:CONFIG(debug, debug|release):LIBS += -L/C:/OSGeo4W64/lib/ -lopencv_core248d -lopencv_highgui248d
else:unix: LIBS += -LC:/OSGeo4W64/lib/ -lopencv_core -lopencv_highgui

INCLUDEPATH += C:/OSGeo4W64/include
DEPENDPATH += C:/OSGeo4W64/include


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/ColorFactory/ -lColorFactory
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/ColorFactory/ -lColorFactory
else:unix: LIBS += -L$$OUT_PWD/../lib/ColorFactory/ -lColorFactory

INCLUDEPATH += $$PWD/../ColorFactory
DEPENDPATH += $$PWD/../ColorFactory
