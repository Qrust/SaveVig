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
mkdir $PASSWORD
mv *.py $PASSWORD
cd ./$PASSWORD

FILES="./*.py"

for files in $FILES
do
	echo "Строим график $files"
	python3 $files
done

mkdir	tmp

x=1; for i in *png; do counter=$(printf %03d $x); ln "$i" ./tmp/img"$counter".png; x=$(($x+1)); done

cd ./tmp

ffmpeg -f image2 -i img%03d.png ${LEN}.mpg

mv ${LEN}.mpg $BASEDIR/py_tests/
