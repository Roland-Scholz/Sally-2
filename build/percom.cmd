@echo off

call var-def.cmd
set REL=%REL%\atari
set SRC=%SRC%\utils

call :compile_module percom 2000
if NOT %result%==0 goto ende
call :compile_module percom_run 2e0
if NOT %result%==0 goto ende


cd %REL%
copy ..\%RES%\mydos90k.atr .

copy /Y /B percom.com + percom_run.com percom.com

..\%TOOLS%\xfddos -i mydos90k.atr percom.com

mkdir obj 2> nul > nul
mkdir lst 2> nul > nul

move *.lst 2> nul > nul
move *.o obj 2> nul > nul
move *.a obj 2> nul > nul

rem copy boot.atr e:

C:\Users\ROLAND.SCHOLZ\altirra\altirra.exe mydos90k.atr
goto eof

:ende

pause
goto eof


:compile_module
%CC65%\ca65 -l  %REL%\%1.lst %SRC%\%1.a65 -I %INC% -I %COMMON%\inc -o %REL%\%1.o
set result=%ERRORLEVEL%

if %result%==0 (

	%CC65%\ld65 -t none %REL%\%1.o -o %REL%\%1.a
	%CC65%\bin2hex %REL%\%1.a %REL%\%1.hex -o %2
	C:\github\Sally-2\jdk17.0.17_10\bin\java -jar %TOOLS%\Obj2Com\jar\ObjUtil.jar Obj2Com %REL%\%1.a %2
)

:eof