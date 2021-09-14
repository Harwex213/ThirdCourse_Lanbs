@echo off
@echo params: %1 %2 %3
@echo param 1: %1
@echo param 2: %2
@echo param 3: %3

set p1=%1
set p2=%2
set p3=%3

set /a sum=p1+p2
set /a mult=p1*p2
set /a div=p3/p1
set /a dif=p3-p1
set /a expression = (p3-p1)*(p1-p2)

@echo %p1%+%p2% = %sum%
@echo %p1%*%p2% = %mult%
@echo %p3%/%p1% = %div%
@echo %p3%-%p1% = %dif%
@echo (%p3%-%p1%)*(%p1%-%p2%) = %expression% 

Pause