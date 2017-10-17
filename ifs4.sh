#!/bin/bash

while true
do
    date
    
    # transformation 1
    ROTATE1=$(echo "$(($RANDOM%1000))*2.0*3.14159/1000.0" | bc -l)
    XSCALE1=$(echo "$(($RANDOM%1000))/1000.0" | bc -l)
    YSCALE1=$(echo "$(($RANDOM%1000))/1000.0" | bc -l)

    XSHIFT1=$(echo "$(($RANDOM%1000))/1000.0" | bc -l)
    YSHIFT1=$(echo "$(($RANDOM%1000))/1000.0" | bc -l)

    RED1=$(($RANDOM%256))
    GREEN1=$(($RANDOM%256))
    BLUE1=$(($RANDOM%256))

    # transformation 2 
    ROTATE2=$(echo "$(($RANDOM%1000))*2.0*3.14159/1000.0" | bc -l)
    XSCALE2=$(echo "$(($RANDOM%1000))/1000.0" | bc -l)
    YSCALE2=$(echo "$(($RANDOM%1000))/1000.0" | bc -l)

    XSHIFT2=$(echo "$(($RANDOM%1000))/1000.0" | bc -l)
    YSHIFT2=$(echo "$(($RANDOM%1000))/1000.0" | bc -l)

    RED2=$(($RANDOM%256))
    GREEN2=$(($RANDOM%256))
    BLUE2=$(($RANDOM%256))

    # transformation 3
    ROTATE3=$(echo "$(($RANDOM%1000))*2.0*3.14159/1000.0" | bc -l)
    XSCALE3=$(echo "$(($RANDOM%1000))/1000.0" | bc -l)
    YSCALE3=$(echo "$(($RANDOM%1000))/1000.0" | bc -l)

    XSHIFT3=$(echo "$(($RANDOM%1000))/1000.0" | bc -l)
    YSHIFT3=$(echo "$(($RANDOM%1000))/1000.0" | bc -l)

    RED3=$(($RANDOM%256))
    GREEN3=$(($RANDOM%256))
    BLUE3=$(($RANDOM%256))

    # transformation 4
    ROTATE4=$(echo "$(($RANDOM%1000))*2.0*3.14159/1000.0" | bc -l)
    XSCALE4=$(echo "$(($RANDOM%1000))/1000.0" | bc -l)
    YSCALE4=$(echo "$(($RANDOM%1000))/1000.0" | bc -l)

    XSHIFT4=$(echo "$(($RANDOM%1000))/1000.0" | bc -l)
    YSHIFT4=$(echo "$(($RANDOM%1000))/1000.0" | bc -l)

    RED4=$(($RANDOM%256))
    GREEN4=$(($RANDOM%256))
    BLUE4=$(($RANDOM%256))

    # get a unique filename within ifs directory (minus extension)
    FILENAME="ifs4_""$(date +%s)"
    while [ 0 -ne $(find ~/ifs | grep -c "$FILENAME" ) ]
    do
	sleep $(expr "$RANDOM" % 10)
	FILENAME="ifs4_""$(date +%s)"
    done
    echo "using as filename: $FILENAME"


    echo "generate fractal and render to SVG"
    SVG="$FILENAME"".svg"
    
    ./ifs --rotate1 "$ROTATE1" --xscale1 "$XSCALE1" --yscale1 "$YSCALE1" --xshift1 "$XSHIFT1" --yshift1 "$YSHIFT1" --red1 "$RED1" --green1 "$GREEN1" --blue1 "$BLUE1"     --rotate2 "$ROTATE2" --xscale2 "$XSCALE2" --yscale2 "$YSCALE2" --xshift2 "$XSHIFT2" --yshift2 "$YSHIFT2" --red2 "$RED2" --green2 "$GREEN2" --blue2 "$BLUE2"    --rotate3 "$ROTATE3" --xscale3 "$XSCALE3" --yscale3 "$YSCALE3" --xshift3 "$XSHIFT3" --yshift3 "$YSHIFT3" --red3 "$RED3" --green3 "$GREEN3" --blue3 "$BLUE3"   --rotate4 "$ROTATE4" --xscale4 "$XSCALE4" --yscale4 "$YSCALE4" --xshift4 "$XSHIFT4" --yshift4 "$YSHIFT4" --red4 "$RED4" --green4 "$GREEN4" --blue4 "$BLUE4" --iterations 15 > "$SVG"
    # 4^0 + 4^1 + 4^2 + 4^3 + 4^4 + 4^5 + 4^6 + 4^7 + 4^8 + 4^9 + 4^10 + 4^11 + 4^12 + 4^13 + 4^14 + 4^15 = 1,431,655,765

    echo "convert SVG to PNG, resize, normalize, and flip"
    PNG="$FILENAME"".png"
    convert -format png -resize 1400x1400 -normalize -flip "$SVG" "$PNG"
    
    mv "$SVG" ~/ifs/svg/

    date
    
    echo "pause"
    sleep 10
done
