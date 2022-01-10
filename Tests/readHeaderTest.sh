#!/bin/sh
echo "Test de la lecture du header :"
for file in `grep -rIL . ./Examples_loader`
do
    ./ARM_runner $file temp.txt -h rze > myHeader.txt
    arm-none-eabi-readelf -h $file | grep -v Flags | grep -v Magic > trueHeader.txt
    if [ "$(diff -EZbwB myHeader.txt trueHeader.txt)" ]
    then
        echo "Erreur dans $file :"
        diff -EZbwB myHeader.txt trueHeader.txt
    else
        echo "$file : OK"
    fi
done
rm trueHeader.txt myHeader.txt temp.txt

