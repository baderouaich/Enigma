#!/bin/bash

set -xe

export VERSION=3.2.0

if [ ! -f "./linuxdeploy-x86_64.AppImage" ]; then
  echo "Downloading linuxdeploy..."
  wget "https://github.com/linuxdeploy/linuxdeploy/releases/latest/download/linuxdeploy-x86_64.AppImage"
  chmod +x "./linuxdeploy-x86_64.AppImage"
fi

if [ ! -d "./cmake-build-release" ]; then
  mkdir ./cmake-build-release
  pushd cmake-build-release
  cmake ../../../ -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles"
  make -j$(nproc)
  popd
fi

cp ./cmake-build-release/Enigma ./Enigma
cp ../../res/branding/Logo.png ./Enigma.png
printf "[Desktop Entry]\n\
Type=Application\n\
Name=Enigma\n\
Comment=A Simple, Reliable and Efficient Encryption Tool\n\
Exec=Enigma\n\
Icon=Enigma\n\
Categories=Utility;Security;\n\
Terminal=false\n\
Version=1.0" > ./Enigma.desktop


./linuxdeploy-x86_64.AppImage --output=appimage \
			      --appdir=./AppDir \
			      --executable=./Enigma \
			      --desktop-file=./Enigma.desktop \
			      --icon-file=./Enigma.png


rm -rf ./cmake-build-release/
