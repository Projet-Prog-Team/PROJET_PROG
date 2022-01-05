#!/bin/sh
echo "Test de la lecture des tables de réimplantations :"
for file in `grep -rIL . ./Examples_loader`
do
    ./ARM_runner_example -a $file > myHeader.txt
    arm-none-eabi-readelf -r $file | grep -v Relocation | grep -v Offset > trueHeader.txt
    if [ "$(diff -EZbwB myHeader.txt trueHeader.txt)" ]
    then
        echo "Erreur dans $file :"
        diff -EZbwB myHeader.txt trueHeader.txt
    else
        echo "$file : OK"
    fi
done
rm trueHeader.txt myHeader.txt

