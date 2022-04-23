#!/bin/bash

cnt=0
while read a b c
do 
echo $a
if [[ -z $b ]];then
   $((cnt++))
fi
done < rt.log


echo "cnt is $cnt"
