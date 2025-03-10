::---------------------------------------------------------
:: Copyright 2018 Florian Muecke. All rights reserved.
:: Use of this source code is governed by a BSD-style license that can be
:: found in the LICENSE.txt file.
::---------------------------------------------------------
@echo off
rem check for compiler
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat" || exit /b %errorlevel%
cl > nul 2>&1
if errorlevel 1 (
	echo Can't find C++ compiler tools. Please run create_env_cfg-x86.cmd from within Visual Studio Developer Command Prompt 
		pause	
		exit /b 1
	)
)

call _create_env_cfg.cmd x86 || exit /b %errorlevel%
if not exist "%BOOST_DIR%\boost" (
	echo Can't find boost. Please set "BOOST" to boost path.
	pause
	exit /b 1
)

if not exist "%QTDIR%\bin\qmake.exe" (
	echo Can't find qmake.exe. Please specify "QTDIR".
	pause
	exit /b 1
)

setlocal

set BASE_DIR=%~dp0..\..
call _create_versioninfo.cmd %BASE_DIR%\base || exit /b %errorlevel%

if "%2"=="debug" (set BIN_DIR=%BASE_DIR%\_build\bin\Debug) else (set BIN_DIR=%BASE_DIR%\_build\bin\Release)

:menu
::cls
echo;
echo Current config:
echo;
echo   QMAKESPEC : %QMAKESPEC%
echo   QTDIR     : %QTDIR%
echo   BOOST_DIR : %BOOST_DIR%
echo;
echo;
echo Select build mode:
echo;
echo   (1) make makefiles
echo   (2) build
echo   (3) rebuild
echo   (4) clean
echo   (5) run
echo   (6) build doc
echo   (7) setup
::echo   (8) all
echo   (q) quit
choice /C 12345678q /N
:: value "0" is reserved!
if %errorlevel%==9 exit /b 0
if %errorlevel%==8 goto cmd_all
if %errorlevel%==7 goto cmd_setup
if %errorlevel%==6 goto cmd_build_doc
if %errorlevel%==5 goto cmd_run
if %errorlevel%==4 goto cmd_clean
if %errorlevel%==3 goto cmd_rebuild
if %errorlevel%==2 goto cmd_build
if %errorlevel%==1 goto cmd_make_makefiles
GOTO the_end

:cmd_make_makefiles
	%~dp0_stopwatch start build
	call :make_makefiles || goto the_error
goto the_end

:cmd_clean
	%~dp0_stopwatch start build
	call :make_clean || goto the_error
goto the_end

:cmd_build
	%~dp0_stopwatch start build
	call :make_build || goto the_error
goto the_end

:cmd_rebuild
	%~dp0_stopwatch start build
	call :make_clean || goto the_error
	call :make_build || goto the_error
goto the_end

:cmd_run
	pushd "%BIN_DIR%"
	ipponboard.exe
	if errorlevel 1 (
		popd
		goto the_error
	)
	popd
goto menu

:cmd_build_doc
	%~dp0_stopwatch start build
	call :make_doc || goto the_error
goto the_end

:cmd_setup
	%~dp0_stopwatch start build
	call :make_setup || goto the_error
goto the_end

:cmd_all
	::%~dp0_stopwatch start build
	::call :make_clean 		|| goto the_error
	::call :make_makefiles 	|| goto the_error
	::call :make_build 		|| goto the_error
	::call :make_doc   		|| goto the_error
	::call :make_setup 		|| goto the_error
goto the_end

::-------------------------------
:make_makefiles
	pushd "%BASE_DIR%"
	echo --[Creating makefiles]--
	qmake -recursive || exit /b %errorlevel%
	popd
exit /b 0

::-------------------------------
:make_clean
	echo;
	echo --[clean]--
	rd /Q /S "%BIN_DIR%" >nul 2>&1
	if exist "%BASE_DIR%\base\versioninfo.h" del "%BASE_DIR%\base\versioninfo.h" >nul || exit /b %errorlevel%

	call :make_makefiles
	::call :clean test || exit /b %errorlevel%
	call :clean base || exit /b %errorlevel%
	call :clean GamepadDemo || exit /b %errorlevel%
	
	call %~dp0_create_versioninfo.cmd %BASE_DIR%\base || exit /b %errorlevel%
exit /b 0

::-------------------------------
:clean
	echo;
	echo -- Cleaning %1
	pushd %BASE_DIR%\%1
	jom /S /L clean>nul 2>&1 || exit /b %errorlevel%
	popd
exit /b 0

::-------------------------------
:make_build
	echo;
	echo --[build]--
	::call :compile test debug || exit /b %errorlevel%
	call :compile base || exit /b %errorlevel%
	call :compile GamepadDemo || exit /b %errorlevel%
	::call ::make_doc || exit /b %errorlevel%
	::call %~dp0_copy_files.cmd -release || exit /b %errorlevel%
exit /b 0

::-------------------------------
:compile
	echo;
	echo -- Compiling %1
	pushd %BASE_DIR%\%1
	if "%2"=="debug" (
		jom /L /S /F Makefile.Debug /J 1 || exit /b %errorlevel%
	) else (
		jom /L /S /F Makefile.Release /J 1 || exit /b %errorlevel%
	)
	popd
exit /b 0

::-------------------------------
:make_doc
	echo;
	echo -- building doc
	call "%~dp0_build_doc.cmd" %BASE_DIR% || exit /b %errorlevel%
exit /b 0

::-------------------------------
:make_setup
	echo;
	echo --[setup]--
	if not exist "%INNO_DIR%\iscc.exe" (
		echo Error: iscc.exe not found or INNO_DIR not defined!
		exit /b 1
	)
	
	"%INNO_DIR%\iscc.exe" /Q /O"%BIN_DIR%" "%BASE_DIR%\setup\setup.iss" || exit /b %errorlevel%
	dir /OD "%BIN_DIR%"
exit /b 0

::-------------------------------
:the_error
echo.
echo FAILED (code=%errorlevel%)
pause
goto menu

::-------------------------------
:the_end
echo.
echo SUCCESS
%~dp0_stopwatch stop build "Time Elapsed: {1}"
pause
goto menu
