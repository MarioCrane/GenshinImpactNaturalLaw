# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------
QT += widgets network
TEMPLATE = app
TARGET = \345\244\251\347\220\206\347\263\273\347\273\237
DESTDIR = ../x64/Release
CONFIG += release
LIBS += -L"."
DEPENDPATH += .
MOC_DIR += .
OBJECTS_DIR += release
UI_DIR += .
RCC_DIR += .
HEADERS += ./DownloadManager.h \
    ./resource.h \
    ./SettingData.h \
    ./GenshinImpactNaturalLaw.h \
    ./QtWidgetsSetting.h \
    ./UpdataModule.h \
    ./QtWidgetsMessageBox.h
SOURCES += ./DownloadManager.cpp \
    ./QtWidgetsMessageBox.cpp \
    ./QtWidgetsSetting.cpp \
    ./SettingData.cpp \
    ./UpdataModule.cpp \
    ./GenshinImpactNaturalLaw.cpp \
    ./main.cpp
FORMS += ./GenshinImpactNaturalLaw.ui \
    ./QtWidgetsMessageBox.ui \
    ./QtWidgetsSetting.ui
RESOURCES += GenshinImpactNaturalLaw.qrc \
    QtWidgetsMessageBox.qrc \
    QtWidgetsSetting.qrc
QMAKE_CXXFLAGS += -charset:utf-8