#!/bin/bash

compare_float()
{

	rv=$(echo "$1<$2" | bc)

		if [ "$rv" -eq "1" ];then
		#	echo "less than"
			return 1
		else
#			echo "111"
			return 2
		fi

}



n=0
cnt=$(($1))

while [ $n -lt $cnt ]
do

rt_time=`date +%T`
rt=`./read_stp_dht11.exe`


#read a b RValue d e TValue  <<< $rt 
#echo "RValue is $RValue"
#echo "TValue is $TValue"


echo $rt_time $rt
let n++
done 
