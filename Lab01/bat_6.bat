@chcp 65001
@echo off
if "%*" == "" (goto null) else (goto notnull)

:null
echo — строка параметров:
echo — режим: %1
echo — имя файла: %2
echo —-+ %~n0 режим файл
echo —-++ режим = {создать, удалить}
echo —-++ файл = имя файла
goto exit

:notnull
if not "%2" == "" goto file else goto nofile

:nofile
echo — строка параметров: %*
echo — режим: %1
echo — имя файла: %2
echo —-+ не задано имя файла
goto exit

:file
if "%1" == "создать" goto create
if "%1" == "удалить" goto delete else goto failmode

:failmode
echo — строка параметров: %*
echo — режим: %1
echo — имя файла: %2
echo —-+ режим задан некорректно
goto exit

:create
if exist %2 goto foundfile else goto createfile
goto createfile

:delete
if exist %2 goto deletefile else goto notfoundfile

:notfoundfile
echo — строка параметров: %*
echo — режим: %1
echo — имя файла: %2
echo —-+ файл %2 не найден
goto exit

:foundfile
echo — строка параметров: %*
echo — режим: %1
echo — имя файла: %2
echo —-+ файл %2 уже есть
goto exit

:createfile
copy /b NUL %2
echo — строка параметров: %*
echo — режим: %1
echo — имя файла: %2
echo —-+ файл %2 создан
goto exit

:deletefile
del %2
echo — строка параметров: %*
echo — режим: %1
echo — имя файла: %2
echo —-+ файл %2 удален
goto exit

:exit
