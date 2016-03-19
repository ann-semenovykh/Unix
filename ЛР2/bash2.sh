#!/bin/bash
echo "Начальное количество палочек ${1}"
start=${1}
while [ "$start" -gt 0 ]
do
	user=5
	while [ "$user" -gt 4 -o "$user" -lt 1 -o "$user" -gt $start ]
	do
		echo -n "Ваш ход. Введите количество палочек (1-4):"
		read user
	done
	let start=$start-$user
	echo "Осталось $start палочек"	
	if [ "$start" -eq 1 ]
	then
		echo "Вы выиграли"
 		break		
	elif [ "$start" -le 5 -a "$start" -ge 0 ]
	then
		if [ "$start" -gt 0 ]
		then
			echo "Ход компьютера - " $(($start-1)) " палочек"
		fi
		echo "Вы проиграли"
		break
	else 
		comp=$(($RANDOM%4+1))
		let start=$start-$comp
		echo "Ход компьютера - $comp палочек"
		echo "Осталось $start палочек"	
		if [ "$start" = 1 ] ;	then
			echo "Вы проиграли"
			break
		fi
	fi
	
done
