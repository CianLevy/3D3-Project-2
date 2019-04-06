#!/bin/bash

declare -a characters=(A B C D E)
characters = $(shuf -e "${characters[@]}")
echo "$characters"


#for i in 1 2 3 4 5 
#do
#	cat /dev/urandom| LC_CTYPE=C tr -dc 'A-F'|head -c 5
#	echo
#done