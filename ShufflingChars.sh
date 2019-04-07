#!/bin/bash

# The previous one did not save the shuffled characters into the array. This will save the randomly sorted array into
# the characters array

# Declaring an array
declare -a characters

# tr means transform.
# AWK is the language
# \t is tab
# $0 is used to keep them as characters when passing
# sort -n means sort while array is not NULL
# cut -f2- prevents the random array being created into a file
characters=($(echo A,B,C,D,E,F | tr ',' '\n' | awk 'BEGIN { srand() } { print rand() "\t" $0 }' | sort -n | cut -f2- ))

echo "Order of routers being started"

# Printing the entire array
# Replace the @ with a number to print the character in that position
echo ${characters[@]}
