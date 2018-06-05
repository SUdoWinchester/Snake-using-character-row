echo off

set aplicativo=sneake.exe
set copilar=gcc -o sneake main.c

open main.c
echo O que vai fazer agora?
echo Copilar de volta (C);
echo Executar(E);
echo Sair(S);
echo.
@set /p txt= "#>"
if /i %txt% equ c goto cop
if /i %txt% equ e goto exe
if /i %txt% equ s goto sair

:main
cls
echo.
echo O que vai fazer agora?
echo Copilar de volta (C);
echo Executar(E);
echo Apagar(A);
echo Sair(S);
echo.
echo %txt%
@set /p txt= "#>"
if /i %txt% equ c goto cop
if /i %txt% equ e goto exe
if /i %txt% equ a goto delete
if /i %txt% equ s goto sair
goto main

:cop
cls
echo COPILANDO:
%copilar%
pause
goto main

:exe
cls
%aplicativo%
pause
goto main

:delete
del %aplicativo%
pause
goto main

:sair
