@echo off

call var-def.cmd
set REL=%REL%\atari
set SRC=%SRC%\utils

%TOOLS%\cc65\bin\cl65 -t atari -m %REL%\dissz80.map %SRC%\DISSZ80.C -o %REL%\dissz80.com

cd %REL%
copy ..\%RES%\mydos90k.atr .

copy /Y /B dissz80.com autorun.ar0

..\%TOOLS%\xfddos -i mydos90k.atr autorun.ar0

rem copy boot.atr e:
"C:\Program Files (x86)\Altirra\Altirra64.exe" mydos90k.atr
rem "C:\Program Files\aspeqt\AspeQt.exe"
rem pause
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
	java -jar %TOOLS%\Obj2Com\jar\ObjUtil.jar Obj2Com %REL%\%1.a %2
)

:eof