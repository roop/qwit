#############################################################################
# Makefile for building: qwit
# Generated by qmake (2.01a) (Qt 4.3.3) on: Sat Dec 20 17:58:29 2008
# Project:  qwit.pro
# Template: app
# Command: /usr/bin/qmake -unix -o Makefile qwit.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -march=nocona -O2 -pipe -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -march=nocona -O2 -pipe -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtNetwork -I/usr/include/qt4/QtNetwork -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Isrc -Ivar -Ivar
LINK          = g++
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib64/qt4 -lQtXml -L/usr/lib64 -L/usr/lib64/qt4 -lQtGui -L/usr/lib -lpng -lSM -lICE -lXrender -lXrandr -lXfixes -lXcursor -lfreetype -lfontconfig -lXext -lX11 -lQtNetwork -lQtCore -lz -lm -lrt -ldl -lpthread
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -sf
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = var/

####### Files

SOURCES       = src/LogsDialog.cpp \
		src/MainWindow.cpp \
		src/OptionsDialog.cpp \
		src/qwit.cpp \
		src/StatusTextEdit.cpp \
		src/Twitter.cpp \
		src/TwitterWidget.cpp \
		src/UserpicsDownloader.cpp var/moc_LogsDialog.cpp \
		var/moc_MainWindow.cpp \
		var/moc_OptionsDialog.cpp \
		var/moc_StatusTextEdit.cpp \
		var/moc_Twitter.cpp \
		var/moc_TwitterWidget.cpp \
		var/moc_UserpicsDownloader.cpp \
		var/qrc_qwit.cpp
OBJECTS       = var/LogsDialog.o \
		var/MainWindow.o \
		var/OptionsDialog.o \
		var/qwit.o \
		var/StatusTextEdit.o \
		var/Twitter.o \
		var/TwitterWidget.o \
		var/UserpicsDownloader.o \
		var/moc_LogsDialog.o \
		var/moc_MainWindow.o \
		var/moc_OptionsDialog.o \
		var/moc_StatusTextEdit.o \
		var/moc_Twitter.o \
		var/moc_TwitterWidget.o \
		var/moc_UserpicsDownloader.o \
		var/qrc_qwit.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		qwit.pro
QMAKE_TARGET  = qwit
DESTDIR       = 
TARGET        = qwit

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET): var/ui_LogsDialog.h var/ui_MainWindow.h var/ui_OptionsDialog.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: qwit.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/lib64/qt4/libQtXml.prl \
		/usr/lib64/qt4/libQtCore.prl \
		/usr/lib64/qt4/libQtGui.prl \
		/usr/lib64/qt4/libQtNetwork.prl
	$(QMAKE) -unix -o Makefile qwit.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/lib64/qt4/libQtXml.prl:
/usr/lib64/qt4/libQtCore.prl:
/usr/lib64/qt4/libQtGui.prl:
/usr/lib64/qt4/libQtNetwork.prl:
qmake:  FORCE
	@$(QMAKE) -unix -o Makefile qwit.pro

dist: 
	@$(CHK_DIR_EXISTS) var/qwit1.0.0 || $(MKDIR) var/qwit1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) var/qwit1.0.0/ && $(COPY_FILE) --parents src/LogsDialog.h src/MainWindow.h src/OptionsDialog.h src/StatusTextEdit.h src/Twitter.h src/TwitterWidget.h src/UserpicsDownloader.h var/qwit1.0.0/ && $(COPY_FILE) --parents qwit.qrc var/qwit1.0.0/ && $(COPY_FILE) --parents src/LogsDialog.cpp src/MainWindow.cpp src/OptionsDialog.cpp src/qwit.cpp src/StatusTextEdit.cpp src/Twitter.cpp src/TwitterWidget.cpp src/UserpicsDownloader.cpp var/qwit1.0.0/ && $(COPY_FILE) --parents src/LogsDialog.ui src/MainWindow.ui src/OptionsDialog.ui var/qwit1.0.0/ && (cd `dirname var/qwit1.0.0` && $(TAR) qwit1.0.0.tar qwit1.0.0 && $(COMPRESS) qwit1.0.0.tar) && $(MOVE) `dirname var/qwit1.0.0`/qwit1.0.0.tar.gz . && $(DEL_FILE) -r var/qwit1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: var/moc_LogsDialog.cpp var/moc_MainWindow.cpp var/moc_OptionsDialog.cpp var/moc_StatusTextEdit.cpp var/moc_Twitter.cpp var/moc_TwitterWidget.cpp var/moc_UserpicsDownloader.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) var/moc_LogsDialog.cpp var/moc_MainWindow.cpp var/moc_OptionsDialog.cpp var/moc_StatusTextEdit.cpp var/moc_Twitter.cpp var/moc_TwitterWidget.cpp var/moc_UserpicsDownloader.cpp
var/moc_LogsDialog.cpp: var/ui_LogsDialog.h \
		src/LogsDialog.h
	/usr/bin/moc $(DEFINES) $(INCPATH) src/LogsDialog.h -o var/moc_LogsDialog.cpp

var/moc_MainWindow.cpp: src/UserpicsDownloader.h \
		src/StatusTextEdit.h \
		src/TwitterWidget.h \
		src/Twitter.h \
		src/OptionsDialog.h \
		var/ui_OptionsDialog.h \
		src/LogsDialog.h \
		var/ui_LogsDialog.h \
		var/ui_MainWindow.h \
		src/MainWindow.h
	/usr/bin/moc $(DEFINES) $(INCPATH) src/MainWindow.h -o var/moc_MainWindow.cpp

var/moc_OptionsDialog.cpp: var/ui_OptionsDialog.h \
		src/OptionsDialog.h
	/usr/bin/moc $(DEFINES) $(INCPATH) src/OptionsDialog.h -o var/moc_OptionsDialog.cpp

var/moc_StatusTextEdit.cpp: src/StatusTextEdit.h
	/usr/bin/moc $(DEFINES) $(INCPATH) src/StatusTextEdit.h -o var/moc_StatusTextEdit.cpp

var/moc_Twitter.cpp: src/Twitter.h
	/usr/bin/moc $(DEFINES) $(INCPATH) src/Twitter.h -o var/moc_Twitter.cpp

var/moc_TwitterWidget.cpp: src/TwitterWidget.h
	/usr/bin/moc $(DEFINES) $(INCPATH) src/TwitterWidget.h -o var/moc_TwitterWidget.cpp

var/moc_UserpicsDownloader.cpp: src/UserpicsDownloader.h
	/usr/bin/moc $(DEFINES) $(INCPATH) src/UserpicsDownloader.h -o var/moc_UserpicsDownloader.cpp

compiler_rcc_make_all: var/qrc_qwit.cpp
compiler_rcc_clean:
	-$(DEL_FILE) var/qrc_qwit.cpp
var/qrc_qwit.cpp: qwit.qrc \
		images/exit.png \
		images/reload.png \
		images/logs.png \
		images/reply.png \
		images/qwit.png \
		images/options.png
	/usr/bin/rcc -name qwit qwit.qrc -o var/qrc_qwit.cpp

compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: var/ui_LogsDialog.h var/ui_MainWindow.h var/ui_OptionsDialog.h
compiler_uic_clean:
	-$(DEL_FILE) var/ui_LogsDialog.h var/ui_MainWindow.h var/ui_OptionsDialog.h
var/ui_LogsDialog.h: src/LogsDialog.ui
	/usr/bin/uic src/LogsDialog.ui -o var/ui_LogsDialog.h

var/ui_MainWindow.h: src/MainWindow.ui
	/usr/bin/uic src/MainWindow.ui -o var/ui_MainWindow.h

var/ui_OptionsDialog.h: src/OptionsDialog.ui
	/usr/bin/uic src/OptionsDialog.ui -o var/ui_OptionsDialog.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_rcc_clean compiler_uic_clean 

####### Compile

var/LogsDialog.o: src/LogsDialog.cpp src/LogsDialog.h \
		var/ui_LogsDialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o var/LogsDialog.o src/LogsDialog.cpp

var/MainWindow.o: src/MainWindow.cpp src/MainWindow.h \
		src/UserpicsDownloader.h \
		src/StatusTextEdit.h \
		src/TwitterWidget.h \
		src/Twitter.h \
		src/OptionsDialog.h \
		var/ui_OptionsDialog.h \
		src/LogsDialog.h \
		var/ui_LogsDialog.h \
		var/ui_MainWindow.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o var/MainWindow.o src/MainWindow.cpp

var/OptionsDialog.o: src/OptionsDialog.cpp src/MainWindow.h \
		src/UserpicsDownloader.h \
		src/StatusTextEdit.h \
		src/TwitterWidget.h \
		src/Twitter.h \
		src/OptionsDialog.h \
		var/ui_OptionsDialog.h \
		src/LogsDialog.h \
		var/ui_LogsDialog.h \
		var/ui_MainWindow.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o var/OptionsDialog.o src/OptionsDialog.cpp

var/qwit.o: src/qwit.cpp src/MainWindow.h \
		src/UserpicsDownloader.h \
		src/StatusTextEdit.h \
		src/TwitterWidget.h \
		src/Twitter.h \
		src/OptionsDialog.h \
		var/ui_OptionsDialog.h \
		src/LogsDialog.h \
		var/ui_LogsDialog.h \
		var/ui_MainWindow.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o var/qwit.o src/qwit.cpp

var/StatusTextEdit.o: src/StatusTextEdit.cpp src/StatusTextEdit.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o var/StatusTextEdit.o src/StatusTextEdit.cpp

var/Twitter.o: src/Twitter.cpp src/Twitter.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o var/Twitter.o src/Twitter.cpp

var/TwitterWidget.o: src/TwitterWidget.cpp src/TwitterWidget.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o var/TwitterWidget.o src/TwitterWidget.cpp

var/UserpicsDownloader.o: src/UserpicsDownloader.cpp src/UserpicsDownloader.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o var/UserpicsDownloader.o src/UserpicsDownloader.cpp

var/moc_LogsDialog.o: var/moc_LogsDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o var/moc_LogsDialog.o var/moc_LogsDialog.cpp

var/moc_MainWindow.o: var/moc_MainWindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o var/moc_MainWindow.o var/moc_MainWindow.cpp

var/moc_OptionsDialog.o: var/moc_OptionsDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o var/moc_OptionsDialog.o var/moc_OptionsDialog.cpp

var/moc_StatusTextEdit.o: var/moc_StatusTextEdit.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o var/moc_StatusTextEdit.o var/moc_StatusTextEdit.cpp

var/moc_Twitter.o: var/moc_Twitter.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o var/moc_Twitter.o var/moc_Twitter.cpp

var/moc_TwitterWidget.o: var/moc_TwitterWidget.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o var/moc_TwitterWidget.o var/moc_TwitterWidget.cpp

var/moc_UserpicsDownloader.o: var/moc_UserpicsDownloader.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o var/moc_UserpicsDownloader.o var/moc_UserpicsDownloader.cpp

var/qrc_qwit.o: var/qrc_qwit.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o var/qrc_qwit.o var/qrc_qwit.cpp

####### Install

install_target: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/bin/ || $(MKDIR) $(INSTALL_ROOT)/usr/bin/ 
	-$(INSTALL_PROGRAM) "$(QMAKE_TARGET)" "$(INSTALL_ROOT)/usr/bin/$(QMAKE_TARGET)"

uninstall_target:  FORCE
	-$(DEL_FILE) "$(INSTALL_ROOT)/usr/bin/$(QMAKE_TARGET)"
	-$(DEL_DIR) $(INSTALL_ROOT)/usr/bin/ 


install:  install_target  FORCE

uninstall: uninstall_target   FORCE

FORCE:

