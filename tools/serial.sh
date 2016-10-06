#!/bin/bash
if [ -z "$1" ]; then 
	echo -e "Usage: serial.sh DEVICE [ttyUSB0, ...]"
else
	sudo screen /dev/$1 115200
fi
