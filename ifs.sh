#!/bin/bash

# compile if necessary
if [ ! -e ifs ]
then
    echo "compiling ifs"
    gcc -Wall ifs.c -o ifs -lm
fi

if [ ! -e points2svg ]
then
    echo "compiling points2svg"
    gcc -Wall points2svg.c -o points2svg -lm
fi

while true
do
    START=$(date +%s)

    echo "generate fractal and convert to SVG"
    ./ifs | ./points2svg > ifs.svg

    echo "convert to PNG, resize, normalize, and flip"
    FILENAME="ifs""$(date +%s)"".png"
    convert -format png -resize 1400x1400 -normalize -flip ifs.svg "$FILENAME"
    # display "$FILENAME" &

    END=$(date +%s)
    DIFF=$(echo "$END - $START" | bc)
    echo "time elapsed:"
    echo $DIFF

    echo "wait a minute"
    sleep 60
done
