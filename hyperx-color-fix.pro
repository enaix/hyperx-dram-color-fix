QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


DEFINES += ORGBEXAMPLEPLUGIN_LIBRARY
TEMPLATE = lib


CONFIG += c++17 plugin silent

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Project files

SOURCES += \
    colorcorrection.cpp \
    colorinjector.cpp \
    plugin.cpp \
    settingswidget.cpp

HEADERS += \
    colorcorrection.h \
    colorinjector.h \
    plugin.h \
    settingswidget.h


# OpenRGB

INCLUDEPATH += \
    OpenRGB/   \
    OpenRGB/hidapi_wrapper/   \
    OpenRGB/dependencies/hidapi \
    OpenRGB/RGBController/

HEADERS += \
    OpenRGB/OpenRGBPluginInterface.h    \
    OpenRGB/hidapi_wrapper/hidapi_wrapper.h

unix: {
    CONFIG += link_pkgconfig

    PKGCONFIG +=                                                                                \
        libusb-1.0
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
