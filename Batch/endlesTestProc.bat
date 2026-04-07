@echo off
echo Starte Endlos-Testprozess...
setlocal enabledelayedexpansion

set /a counter=1

:loop
echo Zeile !counter!
set /a counter+=1

rem kurze Pause (ca. 1 Sekunde)
ping -n 2 127.0.0.1 >nul

goto loop