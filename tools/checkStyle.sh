#!/bin/bash

# Script to populate cpplint for each c++ code file in src folder
#
# This script is ment to be executed in SRC FOLDER so all paths 
# are relative to that position.

LINT="./cpplint.py"
FOLDER="../src"


function pause() {
	read -p "$*"
}


echo ''
echo '-------------------------------------------------------------------------------'
echo ''
echo '****************************************************'
echo '************** STYLE ISUES FOUND *******************'
echo '**** '`date`
echo '****************************************************'



for SRC in $(find $FOLDER -type f \( -name '*.hpp' -o -name '*.cpp' -o -name '*.c' -o -name '*.h' \)); do
	echo ''
	echo '-----------------------------------------'
	echo 'Checking file '$SRC
	echo '-----------------------------------------'
	python -u $LINT $SRC
	echo ''
done
