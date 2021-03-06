#!/bin/bash

# TODO: pass those as parameters!
appName="FinancialFolder"
appFolder="/c/Users/Victor/Projects/financialocean"
appProject="conceptV1.pvr"

pckFolder="packages/"$appName

pyPath="/c/usr/vcpkg/installed/x64-windows/share/python2/Lib"
redistPath="/c/Program Files (x86)/Windows Kits/10/Redist/ucrt/DLLs/x64"
redistPath2="/c/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Redist/MSVC/14.27.29016/x64/Microsoft.VC142.CRT/vcruntime140_1.dll"
vcpkgLibs="/c/usr/vcpkg/installed/x64-windows/lib"

if [ ! -e $pckFolder ]; then
	mkdir -p $pckFolder 
fi

rm -rf $pckFolder/*

echo " copy app data"
cp -r $appFolder/* $pckFolder/

echo " copy polyvr"
mkdir $pckFolder/engine
cp -r build/Release/* $pckFolder/engine/
cp -r ressources $pckFolder/engine/ressources
cp -r setup $pckFolder/engine/setup
cp -r shader $pckFolder/engine/shader
cp -r examples $pckFolder/engine/examples

echo " copy libs"
cp -r $pyPath $pckFolder/engine/pyLibs
mkdir -p $pckFolder/engine/libs
cp -r "$redistPath"/* $pckFolder/engine/libs/
cp -r "$redistPath2" $pckFolder/engine/libs/
cp -r /c/usr/lib/opensg/* $pckFolder/engine/libs/
cp -r /c/usr/lib/cef/* $pckFolder/engine/libs/
cp -r "$vcpkgLibs"/* $pckFolder/engine/libs/


cat <<EOT >> $pckFolder/startApp.bat
@echo off

set PATH=%PATH%;%~f0\..\engine\libs;
set PYTHONPATH=%PYTHONPATH%;%~f0\..\engine\pyLibs
cd engine
polyvr.exe
EOT

#polyvr.exe --application ../$appProject

echo " done"

# package the folder $pckFolder with inno setup compiler