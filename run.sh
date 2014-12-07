#!/bin/sh

clear && clear

FILE="Strong_vigenere_cipher-bin"

LEN=$1
TESTS=$2
METHOD=$3

BASEDIR="$PWD"

make -C "$BASEDIR" rebuild || { echo Failed to compile ; exit; }

cd ..
test -d "build" || mkdir "build"
cd "build"

mv ../src/$FILE ./
cp ../src/text.txt ./

test -d "py_tests" || mkdir "py_tests"
cd "py_tests"

mkdir "${TESTS}_${LEN}"
cd ..

echo "Начинаем генерацию тестов, при генерации нельзя запускать второй"

for ticks in `seq 1 $TESTS`;do
	PASSWORD=`cat /dev/urandom | tr -dc "a-zA-Z0-9@#*=[]" | dd  bs=$LEN count=1 2>/dev/null`
	echo "Количество тестов $TESTS. Длина пароля $LEN. Тест№ $ticks"
	./$FILE -p $PASSWORD -m $METHOD
	mv "plotdata_${PASSWORD}.py" "./py_tests/${TESTS}_${LEN}/"
done

cd "./py_tests/${TESTS}_${LEN}"

echo "Строим графики, можно запускать еще тесты"

FILES="*.py"

for files in ${FILES};do
	echo "Строим график $files"
	python3 $files
done

echo "Подготавливаем файлы для видео"
mkdir	tmp
x=1; for i in *png; do counter=$(printf %03d $x); ln "$i" ./tmp/img"$counter".png; x=$(($x+1)); done
cd ./tmp


echo "Делаем видео"
ffmpeg -f image2 -i img%03d.png ${TESTS}_${LEN}.mpg
mv ${TESTS}_${LEN}.mpg ../

echo "Готово"

