REM This script builds the butano project depending on the number of cpu cores your computers cpu uses
REM Copy and paste this script into the butano project folder and run it to build the project
@ECHO OFF
set core_count=%NUMBER_OF_PROCESSORS%
make -j %core_count%
