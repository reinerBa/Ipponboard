TEMPLATE = lib
LANGUAGE = C++
CONFIG += precompile_header static exceptions
DEFINES += _WIN32 "WINVER=0x0501" WIN32 "_WIN32_WINNT=0x0501"

# Use Precompiled headers (PCH)
# (inclusion of header in HEADERS section is not required!)
#PRECOMPILED_HEADER = ../base/pch.h
#disabled due to mingw reasons

INCLUDEPATH += $$shell_path($$(BOOST_DIR))

QMAKE_LIBDIR += $$shell_path($$(BOOST_DIR)/stage/lib)

DESTDIR = $$shell_path(../lib)

#----------------------------------------------------------
# Create our custom prebuild target for the release build
#----------------------------------------------------------
prebuild.commands = $$shell_path(../base/create_versioninfo.cmd)
prebuildhook.depends = prebuild
QMAKE_EXTRA_TARGETS += prebuild
CONFIG(release, debug|release):prebuildhook.target = Makefile.Release
CONFIG(debug, debug|release):prebuildhook.target = Makefile.Debug
QMAKE_EXTRA_TARGETS += prebuildhook

# Auto select compiler
win32-g++: COMPILER = mingw
win32-msvc2013: COMPILER = msvc

CONFIG(release, release|debug) {
	TARGET = core
}

CONFIG(debug, release|debug) {
	TARGET = core_d

	contains(COMPILER, msvc) {
		QMAKE_CXX += /Od
	}
}

contains(COMPILER, mingw) {
	QMAKE_CXXFLAGS += -std=c++11
	# get rid of some nasty boost warnings
	QMAKE_CXXFLAGS += -Wno-unused-local-typedefs
}
contains(COMPILER, msvc) {
	QMAKE_CXX += /FS
}

HEADERS = \
	../base/pch.h \
	../util/path_helpers.h \
	../util/qstring_serialization.h \
	Controller.h \
	Enums.h \
	EnumStrings.h \
	Fight.h \
	FightCategory.h \
	iController.h \
	iControllerCore.h \
	iView.h \
	Score.h \
	StateMachine.h \
	Tournament.h \
	Fighter.h \
	TournamentMode.h \
	TournamentModel.h \
	ControllerConfig.h

SOURCES = \
	Controller.cpp \
	FightCategory.cpp \
	Fighter.cpp \
	Fight.cpp \
	Score.cpp \
	StateMachine.cpp \
	TournamentMode.cpp \
	TournamentModel.cpp
	
#OTHER_FILES +=

TRANSLATIONS = \
	../i18n/core_de.ts \
	../i18n/core_nl.ts
