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
    # 3^0 + 3^1 + 3^2 + 3^3 + 3^4 + 3^5 + 3^6 + 3^7 + 3^8 + 3^9 + 3^10 + 3^11 + 3^12 + 3^13 + 3^14 + 3^15 + 3^16 + 3^17 + 3^18 = 581,130,733
    # 3^0 + 3^1 + 3^2 + 3^3 + 3^4 + 3^5 + 3^6 + 3^7 + 3^8 + 3^9 + 3^10 + 3^11 + 3^12 + 3^13 + 3^14 + 3^15 + 3^16 + 3^17 + 3^18 + 3^19 = 1,743,392,200


    echo "convert SVG to PNG, resize, normalize, and flip"
    PNG="$FILENAME"".png"
    convert -format png -resize 1400x1400 -normalize -flip "$SVG" "$PNG"
    
    mv "$SVG" ~/ifs/svg/

    date
    
    echo "pause"
    sleep 10
done
