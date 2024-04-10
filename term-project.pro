QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controller.cpp \
    devicelight.cpp \
    eegheadset.cpp \
    electrode.cpp \
    electrodethread.cpp \
    main.cpp \
    mainwindow.cpp \
    neureset.cpp \
    pcsoftware.cpp \
    pcui.cpp \
    qcustomplot.cpp \
    session.cpp

HEADERS += \
    controller.h \
    defs.h \
    devicelight.h \
    eegheadset.h \
    electrode.h \
    electrodethread.h \
    mainwindow.h \
    neureset.h \
    pcsoftware.h \
    pcui.h \
    qcustomplot.h \
    session.h

FORMS += \
    mainwindow.ui \
    pcui.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
