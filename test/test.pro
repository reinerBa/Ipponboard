TEMPLATE = app
CONFIG += console
TARGET = IpponboardTest
DESTDIR = $$shell_path($$_PRO_FILE_PWD_/../bin)

HEADERS = \
	TestJson.hpp \
	TestScore.hpp \
	TestFight.hpp \
    TestTournamentMode.hpp

SOURCES = \
    IpponboardTest.cpp \
    ../util/jsoncpp/json.cpp

OTHER_FILES += \
    TestData/TournamentModes-test.ini \
    TestData/utf8.json \
    TestData/utf8_with_bom.json

QMAKE_CXXFLAGS += -EHsc

# Copy required DLLs to output directory
CONFIG(debug, debug|release) {
    Qt5Core.commands = copy /Y "$$shell_path($$[QT_INSTALL_BINS]/Qt5Cored.dll)" $$DESTDIR
    Qt5Cored.target = $$DESTDIR/QtCored5.dll
    QMAKE_EXTRA_TARGETS += Qt5Cored
    POST_TARGETDEPS += $$DESTDIR/Qt5Cored.dll
} else:CONFIG(release, debug|release) {
    Qt5Core.commands = copy /Y "$$shell_path($$[QT_INSTALL_BINS]/Qt5Core.dll)" $$DESTDIR
    Qt5Core.target = $$DESTDIR/Qt5Core.dll
    QMAKE_EXTRA_TARGETS += Qt5Core
    POST_TARGETDEPS += $$DESTDIR/Qt5Core.dll
} else {
    error(Unknown set of dependencies.)
}

QMAKE_POST_LINK += "xcopy /Y /Q /I TestData bin\\TestData"
