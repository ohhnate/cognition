@echo off
REM Build and Run Script for Cognition Engine

REM Clean and recreate build directory
if exist build rmdir /s /q build
mkdir build
cd build

REM Configure CMake
cmake ..

REM Build the project
cmake --build . --config Release

REM Run the executable
Release\cognition.exe

REM Return to the original directory
cd ..

pause