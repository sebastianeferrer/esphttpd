#!/bin/bash

if [ -z "$1" ]; then 
	echo -e "Usage: flash.sh DEVICE [ttyUSB0, ...]"
else
	esptool.py --baud 115200 --port /dev/$1 write_flash -fs 4m -fm qio 0x00000 ./firmware/0x00000.bin 0x40000 ./firmware/0x40000.bin
fi
