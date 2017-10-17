#!/bin/bash

# compile if necessary
if [ ! -e ifs ]
then
    echo "compiling ifs"
    gcc -Wall ~/ifs/ifs9.c -o ifs -lm
fi

while true
do
    date
    
    # transformation 1
    ROTATE1=$(echo "$(($RANDOM%1000))*2.0*3.14159/1000.0" | bc -l)
    XSCALE1=$(echo "(2.0*$(($RANDOM%1000))/1000.0) - 1.0" | bc -l)
    YSCALE1=$(echo "(2.0*$(($RANDOM%1000))/1000.0) - 1.0" | bc -l)
    XSHIFT1=$(echo "(2.0*$(($RANDOM%1000))/1000.0) - 1.0" | bc -l)
    YSHIFT1=$(echo "(2.0*$(($RANDOM%1000))/1000.0) - 1.0" | bc -l)
    RED1=$(($RANDOM%256))
    GREEN1=$(($RANDOM%256))
    BLUE1=$(($RANDOM%256))

    # transformation 2 
    ROTATE2=$(echo "$(($RANDOM%1000))*2.0*3.14159/1000.0" | bc -l)
    XSCALE2=$(echo "(2.0*$(($RANDOM%1000))/1000.0) - 1.0" | bc -l)
    YSCALE2=$(echo "(2.0*$(($RANDOM%1000))/1000.0) - 1.0" | bc -l)
    XSHIFT2=$(echo "(2.0*$(($RANDOM%1000))/1000.0) - 1.0" | bc -l)
    YSHIFT2=$(echo "(2.0*$(($RANDOM%1000))/1000.0) - 1.0" | bc -l)
    RED2=$(($RANDOM%256))
    GREEN2=$(($RANDOM%256))
    BLUE2=$(($RANDOM%256))

    # transformation 3
    ROTATE3=$(echo "$(($RANDOM%1000))*2.0*3.14159/1000.0" | bc -l)
    XSCALE3=$(echo "(2.0*$(($RANDOM%1000))/1000.0) - 1.0" | bc -l)
    YSCALE3=$(echo "(2.0*$(($RANDOM%1000))/1000.0) - 1.0" | bc -l)
    XSHIFT3=$(echo "(2.0*$(($RANDOM%1000))/1000.0) - 1.0" | bc -l)
    YSHIFT3=$(echo "(2.0*$(($RANDOM%1000))/1000.0) - 1.0" | bc -l)
    RED3=$(($RANDOM%256))
    GREEN3=$(($RANDOM%256))
    BLUE3=$(($RANDOM%256))
    
    # a fourth or fifth transformation like the above can be added 

    # get a unique filename within ifs directory (minus extension)
    FILENAME="ifs3_""$(date +%s)"
    while [ 0 -ne $(find ~/ifs | grep -c "$FILENAME" ) ]
    do
	sleep $(expr "$RANDOM" % 10)
	FILENAME="ifs3_""$(date +%s)"
    done
    echo "using as filename: $FILENAME"


    echo "generate fractal and render to SVG"
    SVG="$FILENAME"".svg"
    
    ./ifs --rotate1 "$ROTATE1" --xscale1 "$XSCALE1" --yscale1 "$YSCALE1" --xshift1 "$XSHIFT1" --yshift1 "$YSHIFT1" --red1 "$RED1" --green1 "$GREEN1" --blue1 "$BLUE1"     --rotate2 "$ROTATE2" --xscale2 "$XSCALE2" --yscale2 "$YSCALE2" --xshift2 "$XSHIFT2" --yshift2 "$YSHIFT2" --red2 "$RED2" --green2 "$GREEN2" --blue2 "$BLUE2"    --rotate3 "$ROTATE3" --xscale3 "$XSCALE3" --yscale3 "$YSCALE3" --xshift3 "$XSHIFT3" --yshift3 "$YSHIFT3" --red3 "$RED3" --green3 "$GREEN3" --blue3 "$BLUE3"  --iterations 19  > "$SVG"

    echo "convert SVG to PNG, resize, normalize, and flip"
    PNG="$FILENAME"".png"
    convert -format png -resize 1400x1400 -normalize -flip "$SVG" "$PNG"
    
    date
    
    echo "pause"
    sleep 10
done
