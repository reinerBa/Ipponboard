@echo off
setlocal
REM -- language file
call ..\env_cfg.bat

SET DEST=..\bin
SET THIRDPARTY=..\..\3rdParty

rem (mkdir is recursive) if not exist "%DEST%" mkdir "%DEST%">nul
if not exist "%DEST%\lang" mkdir "%DEST%\lang">nul
"%QTDIR%\lrelease" -compress -silent ..\i18n\core_de.ts -qm ..\i18n\core_de.qm
if errorlevel 1 exit /b 1
"%QTDIR%\lrelease" -compress -silent ..\i18n\core_nl.ts -qm ..\i18n\core_nl.qm
if errorlevel 1 exit /b 1
"%QTDIR%\lrelease" -compress -silent ..\i18n\de.ts -qm ..\i18n\de.qm
if errorlevel 1 exit /b 1
"%QTDIR%\lrelease" -compress -silent ..\i18n\nl.ts -qm ..\i18n\nl.qm
if errorlevel 1 exit /b 1
copy /Y "..\i18n\core_??.qm" "%DEST%\lang">nul
if errorlevel 1 exit /b 1
copy /Y "..\i18n\??.qm" "%DEST%\lang">nul
if errorlevel 1 exit /b 1

REM -- sounds
if not exist "%DEST%\sounds" mkdir "%DEST%\sounds">nul
copy /Y "..\base\sounds\buzzer.wav" "%DEST%\sounds">nul
if errorlevel 1 exit /b 1
copy /Y "%THIRDPARTY%\sounds\*.wav" "%DEST%\sounds">nul
if errorlevel 1 exit /b 1

REM -- binaries
rem copy /Y "%THIRDPARTY%\msvc120\*.dll" "%DEST%">nul
rem if errorlevel 1 exit /b 1

REM -- doc
rem copy /Y "%DEST%\doc\Anleitung.pdf" "%DEST%">nul
rem copy /Y "%DEST%\doc\manual.pdf" "%DEST%">nul

REM -- categories
copy /Y "..\base\categories.json" "%DEST%">nul
if errorlevel 1 exit /b 1
REM -- clubs
if not exist "%DEST%\clubs" mkdir "%DEST%\clubs">nul
copy /Y "clubs.json" "%DEST%">nul
if errorlevel 1 exit /b 1
copy /Y "clubs\*.png" "%DEST%\clubs">nul
if errorlevel 1 exit /b 1

REM -- templates
if not exist "%DEST%\templates" mkdir "%DEST%\templates">nul
copy /Y "templates\*.*" "%DEST%\templates">nul
if errorlevel 1 exit /b 1

REM -- modes
copy /Y TournamentModes.ini "%DEST%">nul
if errorlevel 1 exit /b 1

REM -- programme
REM if not exist "%DEST%\Ipponboard.exe" (
	REM echo ERROR: Ipponboard.exe not found
	REM pause
	REM exit 1
REM )

copy /Y "..\CHANGELOG.txt" "%DEST%">nul
if errorlevel 1 exit /b 1
