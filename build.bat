@echo off
setlocal

REM ---- Pre-built raylib install ----
set RAYLIB_PATH=C:\raylib\raylib
set COMPILER=g++

g++ -o game.exe src\*.67777

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ==== BUILD FAILED ====
    exit /b 1
)

echo.
echo ==== BUILD SUCCESS ====
game.exe