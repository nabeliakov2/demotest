QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

android {
QT += androidextras
}

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

VERSION = 0.0.0.1  # Major.Minor.Update.Test

DEFINES += APPLICATION_VERSION=\\\"$$VERSION\\\"
DEFINES += DEMO_NAMESPACE=demo

SOURCES += \
    src/core/applicationlauncher.cpp \
    src/core/applicationmediator.cpp \
    src/core/eventprocessingsignalwaiter.cpp \
    src/core/main.cpp \
    src/core/networkmanager.cpp \
    src/core/utility.cpp \
    src/gui/colorcombobox.cpp \
    src/gui/mainwidget.cpp \
    src/gui/settingswidget.cpp

HEADERS += \
    include/core/applicationlauncher.h \
    include/core/applicationmediator.h \
    include/core/customexitcodequiteventloop.h \
    include/core/eventprocessingsignalwaiter.h \
    include/core/networkmanager.h \
    include/core/utility.h \
    include/gui/colorcombobox.h \
    include/gui/mainwidget.h \
    include/gui/settingswidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    android-sources/AndroidManifest.xml \
    android-sources/build.gradle \
    android-sources/gradle.properties \
    android-sources/gradle/wrapper/gradle-wrapper.jar \
    android-sources/gradle/wrapper/gradle-wrapper.properties \
    android-sources/gradlew \
    android-sources/gradlew.bat \
    android-sources/res/values/libs.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources
