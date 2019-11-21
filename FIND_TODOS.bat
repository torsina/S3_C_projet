@ECHO OFF
findstr /S /R /I /N /O /C:"//.TODO.(*.*)*" *.c *.h *.inc
