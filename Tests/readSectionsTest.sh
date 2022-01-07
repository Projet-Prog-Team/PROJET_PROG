#!/bin/sh
echo "Test de la lecture des sections :"
for file in `grep -rIL . ./Examples_loader`
do
    ./ARM_runner -S $file > mySections.txt
    arm-none-eabi-readelf -S $file | head -n -5 | tail -n +5 > trueSections.txt
    if [ "$(diff -EZbwB mySections.txt trueSections.txt)" ]
    then
        echo "Erreur dans $file :"
        diff -EZbwB mySections.txt trueSections.txt
    else
        echo "$file : OK"
    fi
done
rm trueSections.txt mySections.txt