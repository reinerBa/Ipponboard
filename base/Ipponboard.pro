#------------------------------------------------------------------------------
# This app depends on:
#   - gamepad
#   - core
# These need to be build first.
#PRE_TARGETDEPS += ../gamepad ../core

TEMPLATE = app
LANGUAGE = C++
CONFIG += windows precompile_header
DEFINES += _WIN32

# Use Precompiled headers (PCH)
# (inclusion of header in HEADERS section is not required!)
#PRECOMPILED_HEADER  = ../base/pch.h
#disabled for mingw!

INCLUDEPATH += $$quote($$(BOOST_DIR))

QMAKE_LIBDIR += $$quote($$(BOOST_DIR)/stage/lib) \
    ../lib

DESTDIR = ../bin

CONFIG(release, release|debug) {
    TARGET = Ipponboard
    QMAKE_LIBS += -lgamepad -lcore -lshell32 -lWinmm
}

CONFIG(debug, release|debug) {
    TARGET = Ipponboard_d
    QMAKE_LIBS += -lgamepad_d -lcore_d -lshell32 -lWinmm
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
    DEFINES += "WINVER=0x0501"
    DEFINES += WIN32 _WIN32_WINNT=0x0501

    # remove unneccessary output files
    QMAKE_POST_LINK += del /Q ..\\bin\\$${TARGET}.exp ..\\bin\\$${TARGET}.lib

    # copy all needed files to destdir
    QMAKE_POST_LINK += & copy_files.cmd
}

HEADERS = pch.h \
    ../SingleTournament/MainWindow.h \
	../TeamTournament/MainWindowTeam.h \
    AddFighterDlg.h \
    ClubManager.h \
    ClubManagerDlg.h \
    MainWindowBase.h \
	../TeamTournament/ModeManagerDlg.h \
    SettingsDlg.h \
	../TeamTournament/ScoreScreen.h \
    View.h \
    FightCategoryManagerDlg.h \
    FightCategoryManager.h \
    FighterManager.h \
    FighterManagerDlg.h \
    ../util/path_helpers.h \
    ../util/qstring_serialization.h \
    ../widgets/Countdown.h \
    ../widgets/SplashScreen.h \
    ../widgets/ScaledImage.h \
    ../widgets/ScaledText.h \
    ../util/SimpleCsvFile.hpp 

SOURCES = Main.cpp \
    Main.cpp \
    ../SingleTournament/MainWindow.cpp \
    ../TeamTournament/MainWindowTeam.cpp \
    ../util/jsoncpp/json.cpp \
	AddFighterDlg.cpp \
    ClubManager.cpp \
    ClubManagerDlg.cpp \
    SettingsDlg.cpp \
    View.cpp \
    FightCategoryManagerDlg.cpp \
    FightCategoryManager.cpp \
    MainWindowBase.cpp \
	../TeamTournament/ModeManagerDlg.cpp \
    FighterManager.cpp \
    FighterManagerDlg.cpp \
	../TeamTournament/ScoreScreen.cpp \
    ../widgets/ScaledImage.cpp \
    ../widgets/ScaledText.cpp \
    ../widgets/SplashScreen.cpp \
    ../widgets/Countdown.cpp


FORMS = ../SingleTournament/MainWindow.ui \
    ../TeamTournament/MainWindowTeam.ui \
    ../SingleTournament/view_vertical_single.ui \
    AddFighterDlg.ui \
    ClubManagerDlg.ui \
	../TeamTournament/ScoreScreen.ui \
    SettingsDlg.ui \
	../TeamTournament/ModeManagerDlg.ui \
    FightCategoryManagerDlg.ui \
    FighterManagerDlg.ui \
    view_horizontal.ui \
    ../widgets/SplashScreen.ui \
    ../widgets/Countdown.ui

OTHER_FILES += \
    TournamentModes.ini

RESOURCES += ipponboard.qrc
TRANSLATIONS = ../i18n/Ipponboard_de.ts \
    ../i18n/Ipponboard_nl.ts

win32:RC_FILE = ipponboard.rc
