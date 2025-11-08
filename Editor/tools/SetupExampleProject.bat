@echo off

cd ../
".\tools\premake5.exe" vs2022 --file="ExampleProject\assets\Scripts\premake5.lua"

PAUSE