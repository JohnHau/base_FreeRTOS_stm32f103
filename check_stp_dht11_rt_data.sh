#!/bin/bash

compare_lt_float()
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


compare_lt_float $1 $2 
if [ "$?" -eq "1" ];then
echo "$1 is less than $2" 
else
echo "$1 is greater than $2" 
fi
exit

while read a b c d e f g h 
do
#echo "a is " $a 
echo -n  $b 
echo -n  $c 
echo     $d 

#echo "e is " $e 
echo -n  $f 
echo -n  $g 
echo     $h 

RValue=$d
TValue=$h



done < $1 
