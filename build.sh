#!/bin/bash

mkdir build && cd build
cmake ..
make
cp ../face_recognition/recognize.py .
read -p "얼굴 사진을 촬영하시겠습니까? (y/n)" $yn
case $yn in
    [Yy]* ) raspistill -v -o known.jpg; break;;
    [Nn]* ) exit;;
    * ) exit;;