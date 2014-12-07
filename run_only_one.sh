#!/bin/sh

clear && clear

FILE="Strong_vigenere_cipher-bin"

LEN=$1
METHOD=$2

BASEDIR="$PWD"

make -C "$BASEDIR" rebuild || { echo Failed to compile ; exit; }

PASSWORD=`cat /dev/urandom | tr -dc "a-z" | dd  bs=$LEN count=1 2>/dev/null`
echo "Шифруем на пароле ${PASSWORD}"

./$FILE -p $PASSWORD -m $METHOD

echo "Готово"

