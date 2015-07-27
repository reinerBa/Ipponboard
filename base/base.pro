TARGET = Ipponboard
DESTDIR = $$shell_path($$_PRO_FILE_PWD_/../bin)
TEMPLATE = app
LANGUAGE = C++
QT += widgets printsupport multimedia xmlpatterns

# This app depends on:
#   - gamepad
#   - core
# These need to be build first.
#PRE_TARGETDEPS += ../gamepad ../core

CONFIG += windows precompile_header
DEFINES += _WIN32 WIN32 "WINVER=0x0501" "_WIN32_WINNT=0x0501"

# Use Precompiled headers (PCH)
# (inclusion of header in HEADERS section is not required!)
#PRECOMPILED_HEADER  = ../base/pch.h
#disabled for mingw!

INCLUDEPATH += $$shell_path($$(BOOST_DIR))

QMAKE_LIBDIR += $$quote($$(BOOST_DIR)/stage/lib) \
    ../lib


CONFIG(release, release|debug) {
    QMAKE_LIBS += -lgamepad -lcore -lshell32 -lWinmm

	# copy required DLLs to output dir
	Qt5CoreDll.target = $$DESTDIR/Qt5Core.dll
	Qt5GuiDll.target = $$DESTDIR/Qt5Gui.dll
	Qt5PrintSupportDll.target = $$DESTDIR/Qt5PrintSupport.dll
	Qt5WidgetsDll.target = $$DESTDIR/Qt5Widgets.dll
	Qt5MultimediaDll.target = $$DESTDIR/Qt5Multimedia.dll
	Qt5NetworkDll.target = $$DESTDIR/Qt5Network.dll
	Qt5XmlPatternsDll.target = $$DESTDIR/Qt5XmlPatterns.dll
	Qt5CoreDll.commands = copy /Y "$$shell_path($$[QT_INSTALL_BINS]/Qt5Core.dll)" $$DESTDIR
	Qt5GuiDll.commands = copy /Y "$$shell_path($$[QT_INSTALL_BINS]/Qt5Gui.dll)" $$DESTDIR
	Qt5PrintSupportDll.commands = copy /Y "$$shell_path($$[QT_INSTALL_BINS]/Qt5PrintSupport.dll)" $$DESTDIR
	Qt5WidgetsDll.commands = copy /Y "$$shell_path($$[QT_INSTALL_BINS]/Qt5Widgets.dll)" $$DESTDIR
	Qt5MultimediaDll.commands = copy /Y "$$shell_path($$[QT_INSTALL_BINS]/Qt5Multimedia.dll)" $$DESTDIR
	Qt5NetworkDll.commands = copy /Y "$$shell_path($$[QT_INSTALL_BINS]/Qt5Network.dll)" $$DESTDIR
	Qt5XmlPatternsDll.commands = copy /Y "$$shell_path($$[QT_INSTALL_BINS]/Qt5XmlPatterns.dll)" $$DESTDIR
	QMAKE_EXTRA_TARGETS += Qt5CoreDll Qt5GuiDll Qt5PrintSupportDll Qt5WidgetsDll Qt5MultimediaDll Qt5NetworkDll Qt5XmlPatternsDll
	POST_TARGETDEPS += \
		$$DESTDIR/Qt5Core.dll \
		$$DESTDIR/Qt5Gui.dll \
		$$DESTDIR/Qt5PrintSupport.dll \
		$$DESTDIR/Qt5Widgets.dll \
		$$DESTDIR/Qt5Multimedia.dll \
		$$DESTDIR/Qt5Network.dll \
		$$DESTDIR/Qt5XmlPatterns.dll
}

CONFIG(debug, release|debug) {
	TARGET = Ipponboard_d
	QMAKE_LIBS += -lgamepad_d -lcore_d -lshell32 -lWinmm	

	# copy required DLLs to output dir
	Qt5CoreDll.target = $$DESTDIR/Qt5Cored.dll
	Qt5GuiDll.target = $$DESTDIR/Qt5Guid.dll
	Qt5PrintSupportDll.target = $$DESTDIR/Qt5PrintSupportd.dll
	Qt5WidgetsDll.target = $$DESTDIR/Qt5Widgetsd.dll
	Qt5MultimediaDll.target = $$DESTDIR/Qt5Multimediad.dll
	Qt5NetworkDll.target = $$DESTDIR/Qt5Networkd.dll
	Qt5XmlPatternsDll.target = $$DESTDIR/Qt5XmlPatternsd.dll
	Qt5CoreDll.commands = copy /Y "$$shell_path($$[QT_INSTALL_BINS]/Qt5Cored.dll)" $$DESTDIR
	Qt5GuiDll.commands = copy /Y "$$shell_path($$[QT_INSTALL_BINS]/Qt5Guid.dll)" $$DESTDIR
	Qt5PrintSupportDll.commands = copy /Y "$$shell_path($$[QT_INSTALL_BINS]/Qt5PrintSupportd.dll)" $$DESTDIR
	Qt5WidgetsDll.commands = copy /Y "$$shell_path($$[QT_INSTALL_BINS]/Qt5Widgetsd.dll)" $$DESTDIR
	Qt5MultimediaDll.commands = copy /Y "$$shell_path($$[QT_INSTALL_BINS]/Qt5Multimediad.dll)" $$DESTDIR
	Qt5NetworkDll.commands = copy /Y "$$shell_path($$[QT_INSTALL_BINS]/Qt5Networkd.dll)" $$DESTDIR
	Qt5XmlPatternsDll.commands = copy /Y "$$shell_path($$[QT_INSTALL_BINS]/Qt5XmlPatternsd.dll)" $$DESTDIR
	QMAKE_EXTRA_TARGETS += Qt5CoreDll Qt5GuiDll Qt5PrintSupportDll Qt5WidgetsDll Qt5MultimediaDll Qt5NetworkDll Qt5XmlPatternsDll
	POST_TARGETDEPS += \
		$$DESTDIR/Qt5Cored.dll \
		$$DESTDIR/Qt5Guid.dll \
		$$DESTDIR/Qt5PrintSupportd.dll \
		$$DESTDIR/Qt5Widgetsd.dll \
		$$DESTDIR/Qt5Multimediad.dll \
		$$DESTDIR/Qt5Networkd.dll \
		$$DESTDIR/Qt5XmlPatternsd.dll
} 

QMAKE_LFLAGS += /SUBSYSTEM:WINDOWS,5.01

# Auto select compiler 
win32-g++: COMPILER = mingw
win32-msvc2013: COMPILER = msvc

contains(COMPILER, mingw) {
	#QMAKE_CXXFLAGS += -std=c++0x
	QMAKE_CXXFLAGS += -std=c++11
	# get rid of some nasty boost warnings
    QMAKE_CXXFLAGS += -Wno-unused-local-typedef

	# copy all needed files to destdir
	QMAKE_POST_LINK += copy_files.cmd
}

contains(COMPILER, msvc) {
    QMAKE_CXX += /FS /MP

    # remove unneccessary output files
    QMAKE_POST_LINK += del /Q ..\\bin\\$${TARGET}.exp ..\\bin\\$${TARGET}.lib

    # copy all needed files to destdir
    QMAKE_POST_LINK += & copy_files.cmd
}

HEADERS = pch.h \
    MainWindow.h \
	MainWindowTeam.h \
    AddFighterDlg.h \
    ClubManager.h \
    ClubManagerDlg.h \
    MainWindowBase.h \
	ModeManagerDlg.h \
    SettingsDlg.h \
	ScoreScreen.h \
    View.h \
    FightCategoryManagerDlg.h \
    FightCategoryManager.h \
    FighterManager.h \
    FighterManagerDlg.h \
    ../util/path_helpers.h \
    ../util/qstring_serialization.h \
    ../widgets/Countdown.h \
    SplashScreen.h \
    ../widgets/ScaledImage.h \
    ../widgets/ScaledText.h \
    ../util/SimpleCsvFile.hpp 

SOURCES = Main.cpp \
    Main.cpp \
    MainWindow.cpp \
    MainWindowTeam.cpp \
    ../util/jsoncpp/json.cpp \
	AddFighterDlg.cpp \
    ClubManager.cpp \
    ClubManagerDlg.cpp \
    SettingsDlg.cpp \
    View.cpp \
    FightCategoryManagerDlg.cpp \
    FightCategoryManager.cpp \
    MainWindowBase.cpp \
	ModeManagerDlg.cpp \
    FighterManager.cpp \
    FighterManagerDlg.cpp \
	ScoreScreen.cpp \
    ../widgets/ScaledImage.cpp \
    ../widgets/ScaledText.cpp \
    SplashScreen.cpp \
    ../widgets/Countdown.cpp


FORMS = MainWindow.ui \
    MainWindowTeam.ui \
    view_vertical_single.ui \
    AddFighterDlg.ui \
    ClubManagerDlg.ui \
	ScoreScreen.ui \
    SettingsDlg.ui \
	ModeManagerDlg.ui \
    FightCategoryManagerDlg.ui \
    FighterManagerDlg.ui \
    view_horizontal.ui \
    SplashScreen.ui \
    ../widgets/Countdown.ui

OTHER_FILES += \
    TournamentModes.ini

RESOURCES += ipponboard.qrc
TRANSLATIONS = ../i18n/de.ts \
    ../i18n/nl.ts

win32:RC_FILE = ipponboard.rc
