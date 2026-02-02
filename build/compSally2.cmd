@echo off
PATH=%PATH%;C:\msys64\usr\bin
setlocal
set ASSEMBLER_PATH=zmac
set ASSEMBLER_EXE="%ASSEMBLER_PATH%/zmac.exe"
set ASM_EXTENSION=asm
set SOURCE_DIR=..\src
set OUTPUT_DIR=..\release\firmware

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
rem * compile all modules
rem ****************************************************
:compile
IF EXIST %OUTPUT_DIR% del /Q %OUTPUT_DIR%\*.*
IF NOT EXIST %OUTPUT_DIR% MD %OUTPUT_DIR%
@echo off

rem ****************************************************
rem * compile sally2 firmware
rem ****************************************************
for /f "tokens=* usebackq" %%a in (`git describe --tags --always`) do set SV=%%a              
echo SVERSION DB "%SV:~0,16%" > %SOURCE_DIR%\version.asm

set MODULE=sally2
call :compile %MODULE% DUMMMYSYMBOL
if not %ERRORLEVEL%==0 goto :error
move %OUTPUT_DIR%\%MODULE%*.* %OUTPUT_DIR%\Sally2

rem ****************************************************
rem * compile sally2 firmware
rem ****************************************************
for /f "tokens=* usebackq" %%a in (`git describe --tags --always`) do set SV=%%a              
echo SVERSION DB "%SV:~0,16%" > %SOURCE_DIR%\version.asm

set MODULE=sally2rs
call :compile %MODULE% DUMMMYSYMBOL
if not %ERRORLEVEL%==0 goto :error
move %OUTPUT_DIR%\%MODULE%*.* %OUTPUT_DIR%\Sally2


rem ****************************************************
rem * compile DDINIT for Sally 2
rem ****************************************************
set MODULE=ddinit
call :compile %MODULE% SALLYBUILD
if not %ERRORLEVEL%==0 goto :error
move %OUTPUT_DIR%\%MODULE%.com %OUTPUT_DIR%\ddinits2.com
move %OUTPUT_DIR%\%MODULE%*.* %OUTPUT_DIR%\CPM

rem ****************************************************
rem * compile DDINIT for ATR8000
rem ****************************************************
rem call :compile ddinit DUMMYSYMBOL
rem if not %ERRORLEVEL%==0 goto :error

rem ****************************************************
rem * compile ATR8000 (4k) ROM
rem ****************************************************
set ASM_EXTENSION=MAC
set MODULE=ROM
call :compile %MODULE% DUMMYSYMBOL
if not %ERRORLEVEL%==0 goto error
move %OUTPUT_DIR%\%MODULE%*.* %OUTPUT_DIR%\ATR8000

rem ****************************************************
rem * compile SALLYMON
rem ****************************************************
set ASM_EXTENSION=MAC
set MODULE=SALLYMON
call :compile %MODULE% DUMMYSYMBOL
if not %ERRORLEVEL%==0 goto error
move %OUTPUT_DIR%\%MODULE%*.* %OUTPUT_DIR%\misc

rem ****************************************************
rem * compile ZEXDOC
rem ****************************************************
set ASM_EXTENSION=MAC
set MODULE=zexdoc
call :compile %MODULE% DUMMYSYMBOL
if not %ERRORLEVEL%==0 goto error
move %OUTPUT_DIR%\%MODULE%*.* %OUTPUT_DIR%\CPM

rem ****************************************************
rem * compile ZEXALL
rem ****************************************************
set ASM_EXTENSION=MAC
set MODULE=zexall
call :compile %MODULE% DUMMYSYMBOL
if not %ERRORLEVEL%==0 goto error
move %OUTPUT_DIR%\%MODULE%*.* %OUTPUT_DIR%\CPM

ECHO Success!!
goto ende


rem ****************************************************
rem * Compile subroutine
rem ****************************************************
:compile
ECHO ****************************************************
ECHO *** compile %1 -D%2
ECHO ****************************************************

%ASSEMBLER_EXE% -D%2 --oo lst,hex,cim --od %OUTPUT_DIR% %SOURCE_DIR%\%1.%ASM_EXTENSION%
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
pause
endlocal
