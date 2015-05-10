#-------------------------------------------------
#
# Project created by QtCreator 2015-04-22T23:42:25
#
#-------------------------------------------------

#androidextras
QT       += core gui multimedia
QMAKE_CXXFLAGS += -std=gnu++1y

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES +=  android/AndroidManifest.xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ManualMatrixEditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    matrix.cpp \
    floatdelegate.cpp \
    buttonwithnum.cpp \
    settings.cpp

HEADERS  += mainwindow.h \
    matrix.h \
    floatdelegate.h \
    buttonwithnum.h \
    settings.h

FORMS    += \
    main.ui

CONFIG += mobility
MOBILITY = 

DISTFILES += \
    MANUAL \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

RESOURCES += \
    resources.qrc

TRANSLATIONS = translations/mme_ru.ts

