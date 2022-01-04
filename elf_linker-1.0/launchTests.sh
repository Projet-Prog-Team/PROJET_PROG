#!/bin/sh
for file in `ls ./Tests/*.sh`
do
    ./$file
done