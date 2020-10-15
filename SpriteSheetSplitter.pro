QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    animation.cpp \
    animationwindow.cpp \
    backgroundcolorlistwidget.cpp \
    eventsservice.cpp \
    frame.cpp \
    framedetection.cpp \
    framesdock.cpp \
    frameswindow.cpp \
    image.cpp \
    imagelabel.cpp \
    jsonhelper.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindowviewmodel.cpp \
    qtutils.cpp \
    settings.cpp \
    utils.cpp

HEADERS += \
    animation.h \
    animationwindow.h \
    backgroundcolorlistwidget.h \
    eventsservice.h \
    frame.h \
    framedetection.h \
    framesdock.h \
    frameswindow.h \
    image.h \
    imagelabel.h \
    jsonhelper.h \
    mainwindow.h \
    mainwindowviewmodel.h \
    qtutils.h \
    settings.h \
    stb_image.h \
    utils.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Settings.json \
    TODOs.txt \
    stylesheet.qss
