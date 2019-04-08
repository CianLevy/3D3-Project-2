#!/bin/bash

chr() {
  [ "$1" -lt 256 ] || return 1
  printf "\\$(printf '%03o' "$1")"
}
MAX=6
RANDOM=$$
for i in 1 2 3 4 5
do
	R=$(($RANDOM%6+65))
	echo $R
done

