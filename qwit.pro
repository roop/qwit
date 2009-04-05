######################################################################
# Automatically generated by qmake (2.01a) Tue Dec 16 18:48:51 2008
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += src images
INCLUDEPATH += src
OBJECTS_DIR = var
MOC_DIR = var
UI_SOURCES_DIR = var
UI_HEADERS_DIR = var
RCC_DIR = var

# Input
HEADERS += src/LogsDialog.h \
           src/MainWindow.h \
           src/OptionsDialog.h \
           src/StatusTextEdit.h \
           src/Twitter.h \
           src/TwitterWidget.h \
           src/UserpicsDownloader.h
FORMS += src/LogsDialog.ui src/MainWindow.ui src/OptionsDialog.ui
SOURCES += src/LogsDialog.cpp \
           src/MainWindow.cpp \
           src/OptionsDialog.cpp \
           src/qwit.cpp \
           src/StatusTextEdit.cpp \
           src/Twitter.cpp \
           src/TwitterWidget.cpp \
           src/UserpicsDownloader.cpp
TRANSLATIONS += \
           translations/qwit_tr_TR.ts \
           translations/qwit_pt_BR.ts \
           translations/qwit_es_ES.ts \
           translations/qwit_ru_RU.ts \
           translations/qwit_en_US.ts
QT += network xml
RESOURCES = qwit.qrc
target.path = /usr/bin
INSTALLS = target
CONFIG += debug
