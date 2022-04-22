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

T_H_ALARM=50.0
T_L_ALARM=10.0

R_H_ALARM=85.0
R_L_ALARM=30.0

n=0
cnt=$(($1))

while [ $n -lt $cnt ]
do

#rt_time=`date +%T`
#rt=`./read_stp_dht11.exe`


#read a b RValue d e TValue  <<< $rt 
#echo "RValue is $RValue"
#echo "TValue is $TValue"

TValue=50.1

compare_float  $TValue $T_H_ALARM
#echo "it is $?"
if [ "$?"  -eq "2" ];then
echo "T alarm high"
fi

exit

echo $rt_time $rt
let n++
done 
