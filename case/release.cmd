@echo off
set TARGETDIR=..\release\case

rem *** Sally 2 front plate
copy sally-2-front-text-logo.3mf %TARGETDIR%
copy sally-2-front-text-logo.stl %TARGETDIR%
copy sally-2-front.stl %TARGETDIR%

rem *** Sally 2 back plate
copy sally-2-back-text.3mf %TARGETDIR%
copy sally-2-back-text.stl %TARGETDIR%
copy sally-2-back.stl %TARGETDIR%

rem *** Sally 2 top part
copy sally-2-top.3mf %TARGETDIR%
copy sally-2-top.stl %TARGETDIR%

rem *** Sally 2 top part
copy sally-2-top.3mf %TARGETDIR%
copy sally-2-top.stl %TARGETDIR%

rem *** Sally 2 bottom part
copy sally-2-bottom.3mf %TARGETDIR%
copy sally-2-bottom.stl %TARGETDIR%

rem *** Sally 2 label
copy sally-2-top-1.stl %TARGETDIR%
copy sally-2-top-2.stl %TARGETDIR%
copy sally-2-top-2.3mf %TARGETDIR%

rem *** Sally 2 small parts
copy sally-parts.3mf %TARGETDIR%
copy washer.stl %TARGETDIR%
copy spacer.stl %TARGETDIR%
copy spacer-led.stl %TARGETDIR%
copy spacer-round-15.stl %TARGETDIR%

pause