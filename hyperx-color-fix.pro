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
    OpenRGB/RGBController/ \
    OpenRGB/i2c_smbus

HEADERS += \
    OpenRGB/OpenRGBPluginInterface.h    \
    OpenRGB/hidapi_wrapper/hidapi_wrapper.h \ 
    OpenRGB/i2c_smbus/i2c_smbus.h

unix: {
    CONFIG += link_pkgconfig

    PKGCONFIG +=                                                                                \
        libusb-1.0
    packagesExist(hidapi-hidraw) {
        PKGCONFIG += hidapi-hidraw

        #---------------------------------------------------------------------------------------#
        # hidapi-hidraw >= 0.10.1 supports USAGE/USAGE_PAGE                                     #
        # Define USE_HID_USAGE if hidapi-hidraw supports it                                     #
        #---------------------------------------------------------------------------------------#
        HIDAPI_HIDRAW_VERSION = $$system($$PKG_CONFIG --modversion hidapi-hidraw)
        if(versionAtLeast(HIDAPI_HIDRAW_VERSION, "0.10.1")) {
            DEFINES += USE_HID_USAGE
        }
    } else {
        packagesExist(hidapi-libusb) {
            PKGCONFIG += hidapi-libusb
        } else {
            PKGCONFIG += hidapi
        }
    }

}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
