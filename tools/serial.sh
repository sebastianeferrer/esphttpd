#!/bin/bash
if [ -z "$1" ]; then 
	sudo screen /dev/ttyUSB0 115200
else
	sudo screen /dev/$1 115200
fi
