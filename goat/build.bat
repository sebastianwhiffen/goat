@echo off

echo Shell is: %COMSPEC%
echo "building"

mkdir ..\..\build
pushd ..\..\build
cl -Zi ..\goat\code\win32_goat.cpp user32.lib
popd