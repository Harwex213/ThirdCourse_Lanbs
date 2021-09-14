@echo off
@echo params: %1 %2 %3
@echo param 1: %1
@echo param 2: %2
@echo param 3: %3

set p1=%1
set p2=%2
set p3=%3

set /a result=p1%p3%p2

@echo result = %result%

Pause