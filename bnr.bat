REM Attempting to build project

set core_count=%NUMBER_OF_PROCESSORS%
make -j %core_count%
JotPK.elf