@ECHO OFF
cpplint --recursive --extensions=c,h,inc %cd%
@ECHO ON