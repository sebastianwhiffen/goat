@echo off

echo Shell is: %COMSPEC%
echo "building"

mkdir ..\build
pushd ..\build
cd
cl -Zi ..\goat\code\win32_goat.cpp user32.lib Gdi32.lib
popd