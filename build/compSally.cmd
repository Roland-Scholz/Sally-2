@echo off
set TOOLS=C:\github\FABIAN
PATH=%PATH%;%TOOLS%\tools;%TOOLS%\tools\z88dk\bin;

set ASM=z80asm

rem ****************************************************
rem * compile CP/M 2.2
rem ****************************************************
call :compile SALLY-F000 F000
if not %ERRORLEVEL%==0 goto error

call :compile test 0000
if not %ERRORLEVEL%==0 goto error

move ..\src\*.o ..\release
move ..\src\*.hex ..\release
move ..\src\*.lis ..\release
move ..\src\*.com ..\release

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


