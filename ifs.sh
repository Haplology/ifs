#!/bin/bash

# compile if necessary
if [ ! -e ifs ]
then
    echo "compiling ifs"
    gcc -Wall ~/ifs/ifs.c -o ifs -lm
fi

while true
do
    # get a unique filename within ifs directory (minus extension)
    FILENAME="ifs""$(date +%s)"
    while [ 0 -ne $(find ~/ifs | grep -c "$FILENAME" ) ]
    do
	sleep $(expr "$RANDOM" % 10)
	FILENAME="ifs""$(date +%s)"
    done
    echo "using as filename: $FILENAME"

    echo "generate fractal and convert to SVG"
    SVG="$FILENAME"".svg"
    ./ifs > "$SVG"

    echo "convert to PNG, resize, normalize, and flip"
    PNG="$FILENAME"".png"
    convert -format png -resize 1400x1400 -normalize -flip "$SVG" "$PNG"
    rm "$SVG"

    echo "pause"
    sleep 5
done
