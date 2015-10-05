@echo off
;rem Run_Linpack.bat

;rem move into the directory
set BATCHDIR=%~dp0%
cd /d %BATCHDIR%

%IMPERAS_ISS% --verbose --program ../../../Applications/linpack/linpack.CS_UMIPS-O0-g.elf ^
    --processorvendor mips.ovpworld.org --processorname mips32 --variant M14K ^
     --numprocessors 1     --parameter endian=big --semihostname mips32Newlib --semihostvendor mips.ovpworld.org   %*     -argv 11

:end
pause