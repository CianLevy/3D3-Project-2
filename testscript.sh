#!/bin/bash

characters=('A' 'B' 'C' 'D' 'E' 'F')
characters=($(shuf -e "${characters[@]}"))


for i in 0 1 2 3 4 5
do
	xterm -title "Router "${characters[i]}"" -e ./my-router "${characters[i]};bash" &
done