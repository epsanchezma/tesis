#!/bin/bash
for file in $1/*
do
	./test "$file" &> /dev/null
done