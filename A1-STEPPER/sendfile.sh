#!/bin/bash
#Send file to device character by character. Optimized for Apple-1
DEVICE="/dev/ttyACM0"
stty -F $DEVICE 115200 cs8 -cstopb -parenb
INPUT=$1
while IFS= read -r -n1 char
	do
	if [ "$char" = "" ];
		then
		echo -n -e '\x8d' > $DEVICE
		sleep 0.5
		echo
	fi
	echo -n "$char" > $DEVICE
	echo -n "$char"
	sleep 0.06
done < "$INPUT"
exit 0
