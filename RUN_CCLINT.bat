@ECHO OFF
cpplint --recursive --extensions=c,h,inc --exclude=make-build-debug %cd%
@ECHO ON