@echo off
set MODULE=sally2
set TOOLS=C:\github\FABIAN
PATH=%PATH%;%TOOLS%\tools;%TOOLS%\tools\z88dk\bin;

set ASM=z80asm

rem ****************************************************
rem * compile Rolli-1
rem ****************************************************
call :compile %MODULE%-f000 F000
if not %ERRORLEVEL%==0 goto error
call :compile %MODULE%-0000 0000
if not %ERRORLEVEL%==0 goto error
call :compile ddinit-0100 0100
if not %ERRORLEVEL%==0 goto error

del /Q	..\release\*.*

move ..\src\*.o ..\release	>nul 2>&1	
move ..\src\*.hex ..\release	>nul 2>&1
move ..\src\*.lis ..\release	>nul 2>&1
move ..\src\*.com ..\release	>nul 2>&1

pushd	..\release
copy	%MODULE%-0000.com /B + %MODULE%-f000.com /B %MODULE%.com /B
bin2hex %MODULE%.com %MODULE%.hex -o 0000
popd

pause
goto ende



pause
goto ende

rem ----------------------------------------------
:compile
	pushd ..\src
	echo *** compile %1
	%ASM% -mz80 %3 -b -l -o%1.com %1 
	if not %ERRORLEVEL%==0 goto ende

	bin2hex %1.com %1.hex -o %2
	popd
	goto ende
rem ----------------------------------------------

:error
	pause
:ende


