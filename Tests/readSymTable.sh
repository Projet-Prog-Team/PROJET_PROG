#!/bin/sh
echo "Test de la lecture de la table des symboles :"
for file in `grep -rIL . ./Examples_loader`
do
    ./ARM_runner_example -b $file > mySymTable.txt
    arm-none-eabi-readelf -s $file | tail -n +4 > trueSymTable.txt
    if [ "$(diff -EZbwB mySymTable.txt trueSymTable.txt)" ]
    then
        echo "Erreur dans $file :"
        diff -EZbwB mySymTable.txt trueSymTable.txt
    else
        echo "$file : OK"
    fi
done
rm trueSymTable.txt mySymTable.txt