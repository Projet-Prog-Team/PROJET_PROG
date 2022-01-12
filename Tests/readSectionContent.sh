echo "Test de la lecture du contenu des sections :"
for file in `grep -rIL . ./Examples_loader`
do
    error=0
    nbSections=`arm-none-eabi-readelf -h $file | grep "Number of section headers" | cut -d" " -f 15`
    for i in `seq 0 $(($nbSections-1))`
    do
        arm-none-eabi-readelf -x $i $file | cut -d' ' -f 4-7 | grep -v section > trueSection.txt
        ./ARM_runner --source $file -x $i | grep -v Section > mySection.txt 
        if [ "$(diff -EZbwB trueSection.txt mySection.txt)" ]
        then
            error=1
            echo "Erreur dans $file à la section $i :"
            diff -EZbwB trueSection.txt mySection.txt
        fi
    done
    if [ $error==0 ]
    then
        echo "$file : OK"
    fi
done
rm trueSection.txt mySection.txt
