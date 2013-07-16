@echo off

:: re-create msvc directory.
if exist msvc rd /S /Q msvc
mkdir msvc
cd msvc

cmake -G "Visual Studio 11" ../
pause