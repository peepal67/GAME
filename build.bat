@echo off
setlocal

REM ---- Pre-built raylib install ----
set "RAYLIB_PATH=C:\raylib\raylib"
set "TOOLCHAIN=C:\w64devkit\w64devkit\bin"
set "PATH=%TOOLCHAIN%;%PATH%"
set "COMPILER=%TOOLCHAIN%\g++.exe"

"%COMPILER%" -o game.exe src\*.cpp ^
    -I %RAYLIB_PATH%\include ^
    -L %RAYLIB_PATH%\lib ^
    -lraylib -lopengl32 -lgdi32 -lwinmm ^
    -std=c++17 ^
    -Wall

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ==== BUILD FAILED ====
    exit /b 1
)

echo.
echo ==== BUILD SUCCESS ====
game.exe
