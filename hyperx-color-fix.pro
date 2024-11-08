QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


DEFINES += ORGBEXAMPLEPLUGIN_LIBRARY
TEMPLATE = lib


CONFIG += c++17 plugin silent

# Build info
GIT_COMMIT_ID = $$system(git --git-dir $$_PRO_FILE_PWD_/.git --work-tree $$_PRO_FILE_PWD_ rev-parse HEAD)
PLUGIN_VERSION = 0.0.1
GIT_COMMIT_DATE = $$system(git log -n 1 --pretty=format:"%ci")

DEFINES += \
    ENABLE_DEBUG \  # Enable or disable debugging (also checks for loglevel at runtime)
    GIT_COMMIT_ID=\\"\"\"$$GIT_COMMIT_ID\\"\"\" \
    VERSION_STRING=\\"\"\"$$PLUGIN_VERSION\\"\"\" \
    GIT_COMMIT_DATE=\\"\"\"$$GIT_COMMIT_DATE\\"\"\" \


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
    logging.h \
    plugin.h \
    settingswidget.h


# OpenRGB

INCLUDEPATH += \
    OpenRGB/   \
    OpenRGB/hidapi_wrapper/   \
    OpenRGB/dependencies/hidapi \
    OpenRGB/RGBController/ \
    OpenRGB/i2c_smbus \
    OpenRGB/dependencies/json/

HEADERS += \
    OpenRGB/OpenRGBPluginInterface.h    \
    OpenRGB/ResourceManager.h \
    OpenRGB/ProfileManager.h \
    OpenRGB/RGBController/RGBController.h \
    OpenRGB/SettingsManager.h \
    OpenRGB/hidapi_wrapper/hidapi_wrapper.h \ 
    OpenRGB/i2c_smbus/i2c_smbus.h \
    OpenRGB/dependencies/json/json.hpp \
    OpenRGB/LogManager.h

SOURCES += \
    OpenRGB/LogManager.cpp

unix:!macos {
    CONFIG += link_pkgconfig

    PKGCONFIG += \
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
