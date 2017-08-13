#!/bin/bash

path=$1
echo $path
	file=`ls $path *|grep pla`
	echo $file
	for f in $file
	do
		echo $path$f
		./cr $path$f
	done
