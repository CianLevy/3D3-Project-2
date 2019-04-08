#!/bin/bash
if (( $# == 2 )); then
	./my-router g $1 $2
else
	echo "Invalid arguments"	
fi