@echo off
setlocal
set MODULE=sally2
set ASSEMBLER_PATH=zmac
set ASSEMBLER_EXE="%ASSEMBLER_PATH%/zmac.exe"
set ASM_EXTENSION=asm
set SOURCE_DIR=..\src
set OUTPUT_DIR=..\release

rem ****************************************************
rem * Clean (if requested)
rem ****************************************************
if "%1"=="--clean" GOTO :CLEAN

IF NOT EXIST %OUTPUT_DIR% MD %OUTPUT_DIR%

IF EXIST %ASSEMBLER_EXE% GOTO :compile
PUSHD %ASSEMBLER_PATH%\src
CALL build.bat
POPD
if not %ERRORLEVEL%==0 goto :error

rem ****************************************************
rem * compile Sally-2
rem ****************************************************
:compile
IF EXIST %OUTPUT_DIR% del /Q %OUTPUT_DIR%\*.*
IF NOT EXIST %OUTPUT_DIR% MD %OUTPUT_DIR%
call :compile %MODULE%
if not %ERRORLEVEL%==0 goto :error
call :compile ddinit-0100
if not %ERRORLEVEL%==0 goto :error
rem *** Build ATR 8000 ROM ***
set ASM_EXTENSION=MAC
call :compile ROM
if not %ERRORLEVEL%==0 goto error
ECHO Success!!
goto ende


rem ****************************************************
rem * Compile subroutine
rem ****************************************************
:compile
ECHO *** compile %1
%ASSEMBLER_EXE% --oo lst,hex,cim --od %OUTPUT_DIR% %SOURCE_DIR%\%1.%ASM_EXTENSION%
if %ERRORLEVEL%==0 ren %OUTPUT_DIR%\%1.cim %1.com
EXIT /B


rem ****************************************************
rem * Clean up the build output
rem ****************************************************
:CLEAN
IF EXIST %OUTPUT_DIR% del /Q %OUTPUT_DIR%\*.*
IF EXIST %OUTPUT_DIR% RD %OUTPUT_DIR%
PUSHD %ASSEMBLER_PATH%\src
CALL build.bat %1
POPD
goto ende


:error
	ECHO.
	ECHO Error during build.
	pause
:ende
endlocal
