QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    frame.cpp \
    framedetection.cpp \
    image.cpp \
    imagelabel.cpp \
    jsonhelper.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindowviewmodel.cpp \
    utils.cpp

HEADERS += \
    constants.h \
    frame.h \
    framedetection.h \
    image.h \
    imagelabel.h \
    jsonhelper.h \
    mainwindow.h \
    mainwindowviewmodel.h \
    stb_image.h \
    utils.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    TODOs.txt \
    stylesheet.qss
