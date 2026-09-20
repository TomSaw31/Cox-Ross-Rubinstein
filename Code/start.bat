@echo off
rem Compilation du projet
cmake --build build-windows

rem Verification si la compilation a reussi avant d'executer
if %ERRORLEVEL% EQU 0 (
echo.
echo --- Execution du programme ---
build-windows\BinomialModel.exe
) else (
echo.
echo Erreur lors de la compilation. Le programme ne sera pas execute.
exit /b %ERRORLEVEL%
)