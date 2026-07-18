REM Attempting to build project
make clean
(type nul >> "C:./JotPK.gba") 2>nul && (echo File is FREE) || (echo File is OPEN/LOCKED exit /b) 
@ECHO OFF
set core_count=%NUMBER_OF_PROCESSORS%
make -j %core_count%
