#!/bin/bash

while true
do
    START=$(date +%s)

    echo "generate fractal and convert to SVG"
    ~/ifs/ifs | ~/ifs/points2svg > ./ifs.svg

    while [ 0 -ne $(ps -e | grep -c 'mogrify') ]
    do
	sleep 10
	echo "waiting for a mogrify job to finish..."
    done
    
    echo "convert image format"
    mogrify -format png ./ifs.svg

    echo "resize, normalize, and flip"
    FILENAME="ifs""$(date +%s)"".png"
    convert -resize 1400x1400 -normalize -flip ./ifs.png "$FILENAME"
    display "$FILENAME" &

    echo "housekeeping"
    rm ./ifs.svg
    rm ./ifs.png

    END=$(date +%s)
    DIFF=$(echo "$END - $START" | bc)
    echo "time elapsed:"
    echo $DIFF

    echo "wait a minute"
    sleep 60
done
