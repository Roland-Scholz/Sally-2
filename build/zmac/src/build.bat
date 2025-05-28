@ECHO OFF
setlocal

rem ****************************************************
rem ****************************************************
set MSYS2_SHELL=C:\msys64\msys2_shell.cmd

rem ****************************************************
rem * Look for cmd file. If not found, print out instructions
rem ****************************************************
IF NOT EXIST %MSYS2_SHELL% GOTO :MISSING

rem ****************************************************
rem * Add parameters to shell command
rem ****************************************************
set MSYS2_SHELL_CMD=%MSYS2_SHELL% -no-start -defterm -ucrt64

rem ****************************************************
rem * Create the path string from the current working directory for use in MSYS2
rem ****************************************************
set "original_dir=%cd%"
set "drive=%original_dir:~0,1%"
call :tolower drive
set "modified_dir=%original_dir:\=/%"
set "MSYS2_path=/%drive%/%modified_dir:~3%

if "%1"=="--clean" GOTO :CLEAN

rem ****************************************************
rem * Build the code
rem ****************************************************
set COMMAND=make
call :runMSYS2 %COMMAND%
GOTO :TheEnd

rem ****************************************************
rem * Clean up the build output
rem ****************************************************
:CLEAN
set COMMAND=make clean
call :runMSYS2 %COMMAND%
GOTO :TheEnd


rem ****************************************************
rem * Function to invoke MSYS2
rem ****************************************************
:runMSYS2
setlocal enabledelayedexpansion
set "_CMD=!%1!"

CALL %MSYS2_SHELL_CMD% -c "bash -c 'cd %MSYS2_path%;%COMMAND%;exit'"

EXIT /B

rem ****************************************************
rem * Function to convert variable to lowercase
rem ****************************************************
:tolower
setlocal enabledelayedexpansion
set "_STRING=!%1!"

set "_UCASE=ABCDEFGHIJKLMNOPQRSTUVWXYZ"
set "_LCASE=abcdefghijklmnopqrstuvwxyz"

for /l %%a in (0,1,25) do (
   call set "_FROM=%%_UCASE:~%%a,1%%
   call set "_TO=%%_LCASE:~%%a,1%%
   call set "_STRING=%%_STRING:!_FROM!=!_TO!%%
)

endlocal & set "%1=%_STRING%"

EXIT /B

rem ****************************************************
rem * MSYS Installation Instructions
rem ****************************************************
:MISSING
ECHO MSYS2 not found. If you already installed it, open this file (build.bat)
ECHO and check that the line "set MSYS2_SHELL=" points to your installation location.
ECHO.
:UserChoice
choice /c yn /t 25 /d n /m "If you haven't installed MSYS2, would you like to see 
instructions?"

if not errorlevel 1 GOTO :UserChoice
if errorlevel 2 GOTO :TheEnd

ECHO.
ECHO.
ECHO.
ECHO ****************************************************
ECHO * MSYS Installation Instructions
ECHO ****************************************************
ECHO.
ECHO Step 1 - Install GCC using MSYS.
ECHO        - Download and install MSYS2 from: https://www.msys2.org/
ECHO          or, for Windows 7:
ECHO https://github.com/msys2/msys2-installer/releases/download/2022-10-28/msys2-x86_64-
20221028.exe
ECHO.
ECHO        - After installation, do NOT choose to "Run MSYS2 now."If you did,
ECHO          just exit and move onto the next step.
ECHO.
ECHO Step 2 - Open the MSYS2 terminal from the Start menu: there are 4 options to
ECHO          run. Use Start...All Programs...MSYS2...MSYS2 UCRT64.
ECHO.
ECHO Step 3 - Update the package database. On Windows 7 MAKE SURE TO CANCEL THE
ECHO          INSTALL after it begins or else MSYS2  will no longer run. Use the
ECHO          following command (case sensitive):
ECHO.
ECHO          pacman -Syu
ECHO.
ECHO          On Windows 7, when asked the following question, choose n:
ECHO.
ECHO          Proceed with installation? [Y/n] n
ECHO.
ECHO Step 4 - Install the MinGW-w64 toolchain by running the following
ECHO          command (case sensitive):
ECHO.
ECHO          pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
ECHO.
ECHO        - Press the Enter key to accept the default installation of all packages
ECHO.
ECHO Step 5 - Confirm the installation was successful by running the GCC program
ECHO          in the MSYS2 terminal with the following command:
ECHO.
ECHO          gcc --version
ECHO.
ECHO        - If gcc works, it will print out version information. If gcc is not
ECHO          installed, you see "-bash: gcb: command not found"
ECHO.
ECHO Step 6 - Run this build script again after MSYS2 and gcc are installed.
EXIT /B 1

:TheEnd
endlocal
EXIT /B

