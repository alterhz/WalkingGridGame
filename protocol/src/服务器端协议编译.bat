REM @echo off

protobufc --cpp_out=../message/ gpid.proto
protobufc --cpp_out=../message/ gmsg.proto
pause
