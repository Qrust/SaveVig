#!/bin/sh

FILE="Strong_vigenere_cipher-bin"

BASEDIR="$PWD"

make -C "$BASEDIR" rebuild

echo ""
./$FILE