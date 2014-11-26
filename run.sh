#!/bin/sh

clear && clear

FILE="Strong_vigenere_cipher-bin"

LEN=$1
TESTS=$2

BASEDIR="$PWD"

make -C "$BASEDIR" rebuild

echo "Начинаем генерацию тестов"

for ticks in `seq 1 $TESTS`
do
	PASSWORD=`cat /dev/urandom | tr -dc "a-zA-Z0-9@#*=[]" | dd  bs=$LEN count=1 2>/dev/null`
	echo "Шифруем на $PASSWORD"
	./$FILE -p $PASSWORD
done

cd ./py_tests

FILES="./*.py"

for files in $FILES
do
	echo "Строим график $files"
	python3 $files
done