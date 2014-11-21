#!/bin/sh

clear && clear

FILE="Strong_vigenere_cipher-bin"
PASSWORD="k1z2u3"


BASEDIR="$PWD"

make -C "$BASEDIR" rebuild

echo ""
./$FILE -p $PASSWORD