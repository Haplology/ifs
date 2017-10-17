#!/bin/bash

cd ~/ifs/movie

ITERATIONS=22

TIMESTAMP=$(date +%s)
mkdir "$TIMESTAMP"
cp $0 "$TIMESTAMP"/"$TIMESTAMP"".sh" # copy self to current directory

cd "$TIMESTAMP"
pwd

# 0-1499 : 1507983065.mp4
FRAMES=1500 
FIRSTFRAME=1500
LASTFRAME=$(($FIRSTFRAME+$FRAMES-1))

FORKS=10

STARTIFS=~/ifs/svg/ifs1507859732.svg
ENDIFS=~/ifs/svg/ifs1507854069.svg
STARTIFS="$ENDIFS"
ENDIFS=~/ifs/svg/ifs1507881212.svg

echo "frames $FRAMES firstframe $FIRSTFRAME lastframe $LASTFRAME forks $FORKS"
echo "starting ifs $STARTIFS ending ifs $ENDIFS"

### START ###
# transformation 1 at start, printed on line 5 of the SVG file, e.g.:
# rotate      1.727962        xscale  0.767030        yscale  0.235160        xshift  0.284899        yshift  0.822697        red     165.000000      green   205.500000      blue    87.500000
START_ROTATE1=$(sed -n '5p' "$STARTIFS" | cut -f2)
START_XSCALE1=$(sed -n '5p' "$STARTIFS" | cut -f4)
START_YSCALE1=$(sed -n '5p' "$STARTIFS" | cut -f6)
START_XSHIFT1=$(sed -n '5p' "$STARTIFS" | cut -f8)
START_YSHIFT1=$(sed -n '5p' "$STARTIFS" | cut -f10)
START_RED1=$(sed -n '5p' "$STARTIFS" | cut -f12)
START_GREEN1=$(sed -n '5p' "$STARTIFS" | cut -f14)
START_BLUE1=$(sed -n '5p' "$STARTIFS" | cut -f16)

START_ROTATE2=$(sed -n '6p' "$STARTIFS" | cut -f2)
START_XSCALE2=$(sed -n '6p' "$STARTIFS" | cut -f4)
START_YSCALE2=$(sed -n '6p' "$STARTIFS" | cut -f6)
START_XSHIFT2=$(sed -n '6p' "$STARTIFS" | cut -f8)
START_YSHIFT2=$(sed -n '6p' "$STARTIFS" | cut -f10)
START_RED2=$(sed -n '6p' "$STARTIFS" | cut -f12)
START_GREEN2=$(sed -n '6p' "$STARTIFS" | cut -f14)
START_BLUE2=$(sed -n '6p' "$STARTIFS" | cut -f16)

START_ROTATE3=$(sed -n '7p' "$STARTIFS" | cut -f2)
START_XSCALE3=$(sed -n '7p' "$STARTIFS" | cut -f4)
START_YSCALE3=$(sed -n '7p' "$STARTIFS" | cut -f6)
START_XSHIFT3=$(sed -n '7p' "$STARTIFS" | cut -f8)
START_YSHIFT3=$(sed -n '7p' "$STARTIFS" | cut -f10)
START_RED3=$(sed -n '7p' "$STARTIFS" | cut -f12)
START_GREEN3=$(sed -n '7p' "$STARTIFS" | cut -f14)
START_BLUE3=$(sed -n '7p' "$STARTIFS" | cut -f16)

#### END ###
# transformation at end, or the fractal which is the last frame
END_ROTATE1=$(sed -n '5p' "$ENDIFS" | cut -f2)
END_XSCALE1=$(sed -n '5p' "$ENDIFS" | cut -f4)
END_YSCALE1=$(sed -n '5p' "$ENDIFS" | cut -f6)
END_XSHIFT1=$(sed -n '5p' "$ENDIFS" | cut -f8)
END_YSHIFT1=$(sed -n '5p' "$ENDIFS" | cut -f10)
END_RED1=$(sed -n '5p' "$ENDIFS" | cut -f12)
END_GREEN1=$(sed -n '5p' "$ENDIFS" | cut -f14)
END_BLUE1=$(sed -n '5p' "$ENDIFS" | cut -f16)

END_ROTATE2=$(sed -n '6p' "$ENDIFS" | cut -f2)
END_XSCALE2=$(sed -n '6p' "$ENDIFS" | cut -f4)
END_YSCALE2=$(sed -n '6p' "$ENDIFS" | cut -f6)
END_XSHIFT2=$(sed -n '6p' "$ENDIFS" | cut -f8)
END_YSHIFT2=$(sed -n '6p' "$ENDIFS" | cut -f10)
END_RED2=$(sed -n '6p' "$ENDIFS" | cut -f12)
END_GREEN2=$(sed -n '6p' "$ENDIFS" | cut -f14)
END_BLUE2=$(sed -n '6p' "$ENDIFS" | cut -f16)

END_ROTATE3=$(sed -n '7p' "$ENDIFS" | cut -f2)
END_XSCALE3=$(sed -n '7p' "$ENDIFS" | cut -f4)
END_YSCALE3=$(sed -n '7p' "$ENDIFS" | cut -f6)
END_XSHIFT3=$(sed -n '7p' "$ENDIFS" | cut -f8)
END_YSHIFT3=$(sed -n '7p' "$ENDIFS" | cut -f10)
END_RED3=$(sed -n '7p' "$ENDIFS" | cut -f12)
END_GREEN3=$(sed -n '7p' "$ENDIFS" | cut -f14)
END_BLUE3=$(sed -n '7p' "$ENDIFS" | cut -f16)

for i in $(seq -f "%05g" "$FIRSTFRAME" "$LASTFRAME")
do
    date
    # get a unique filename within ifs directory (minus extension)
    FILENAME="ifs""$i"
    echo "using as filename: $FILENAME"

    ROTATE1=$(echo "($START_ROTATE1*($LASTFRAME-$i) + $END_ROTATE1*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)" | bc -l)
    XSCALE1=$(echo "($START_XSCALE1*($LASTFRAME-$i) + $END_XSCALE1*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)" | bc -l)
    YSCALE1=$(echo "($START_YSCALE1*($LASTFRAME-$i) + $END_YSCALE1*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)" | bc -l)
    XSHIFT1=$(echo "($START_XSHIFT1*($LASTFRAME-$i) + $END_XSHIFT1*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)" | bc -l)
    YSHIFT1=$(echo "($START_YSHIFT1*($LASTFRAME-$i) + $END_YSHIFT1*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)" | bc -l)
    RED1=$(echo    "($START_RED1*($LASTFRAME-$i)    + $END_RED1*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)"    | bc -l)
    GREEN1=$(echo  "($START_GREEN1*($LASTFRAME-$i)  + $END_GREEN1*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)"  | bc -l)
    BLUE1=$(echo   "($START_BLUE1*($LASTFRAME-$i)   + $END_BLUE1*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)"   | bc -l)
    
    ROTATE2=$(echo "($START_ROTATE2*($LASTFRAME-$i) + $END_ROTATE2*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)" | bc -l)
    XSCALE2=$(echo "($START_XSCALE2*($LASTFRAME-$i) + $END_XSCALE2*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)" | bc -l)
    YSCALE2=$(echo "($START_YSCALE2*($LASTFRAME-$i) + $END_YSCALE2*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)" | bc -l)
    XSHIFT2=$(echo "($START_XSHIFT2*($LASTFRAME-$i) + $END_XSHIFT2*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)" | bc -l)
    YSHIFT2=$(echo "($START_YSHIFT2*($LASTFRAME-$i) + $END_YSHIFT2*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)" | bc -l)
    RED2=$(echo    "($START_RED2*($LASTFRAME-$i)    + $END_RED2*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)"    | bc -l)
    GREEN2=$(echo  "($START_GREEN2*($LASTFRAME-$i)  + $END_GREEN2*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)"  | bc -l)
    BLUE2=$(echo   "($START_BLUE2*($LASTFRAME-$i)   + $END_BLUE2*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)"   | bc -l)
    
    ROTATE3=$(echo "($START_ROTATE3*($LASTFRAME-$i) + $END_ROTATE3*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)" | bc -l)
    XSCALE3=$(echo "($START_XSCALE3*($LASTFRAME-$i) + $END_XSCALE3*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)" | bc -l)
    YSCALE3=$(echo "($START_YSCALE3*($LASTFRAME-$i) + $END_YSCALE3*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)" | bc -l)
    XSHIFT3=$(echo "($START_XSHIFT3*($LASTFRAME-$i) + $END_XSHIFT3*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)" | bc -l)
    YSHIFT3=$(echo "($START_YSHIFT3*($LASTFRAME-$i) + $END_YSHIFT3*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)" | bc -l)
    RED3=$(echo    "($START_RED3*($LASTFRAME-$i)    + $END_RED3*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)"    | bc -l)
    GREEN3=$(echo  "($START_GREEN3*($LASTFRAME-$i)  + $END_GREEN3*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)"  | bc -l)
    BLUE3=$(echo   "($START_BLUE3*($LASTFRAME-$i)   + $END_BLUE3*($i-$FIRSTFRAME))/($LASTFRAME-$FIRSTFRAME)"   | bc -l)

    
    echo "generate fractal and convert to SVG"
    SVG="$FILENAME"".svg"

    if [ $(echo "$i%$FORKS" | bc) -eq 0 ]
    then
	./ifs --rotate1 "$ROTATE1" --xscale1 "$XSCALE1" --yscale1 "$YSCALE1" --xshift1 "$XSHIFT1" --yshift1 "$YSHIFT1" --red1 "$RED1" --green1 "$GREEN1" --blue1 "$BLUE1" --rotate2 "$ROTATE2" --xscale2 "$XSCALE2" --yscale2 "$YSCALE2" --xshift2 "$XSHIFT2" --yshift2 "$YSHIFT2" --red2 "$RED2" --green2 "$GREEN2" --blue2 "$BLUE2" --rotate3 "$ROTATE3" --xscale3 "$XSCALE3" --yscale3 "$YSCALE3" --xshift3 "$XSHIFT3" --yshift3 "$YSHIFT3" --red3 "$RED3" --green3 "$GREEN3" --blue3 "$BLUE3" --iterations "$ITERATIONS" > "$SVG"
    else
	./ifs --rotate1 "$ROTATE1" --xscale1 "$XSCALE1" --yscale1 "$YSCALE1" --xshift1 "$XSHIFT1" --yshift1 "$YSHIFT1" --red1 "$RED1" --green1 "$GREEN1" --blue1 "$BLUE1" --rotate2 "$ROTATE2" --xscale2 "$XSCALE2" --yscale2 "$YSCALE2" --xshift2 "$XSHIFT2" --yshift2 "$YSHIFT2" --red2 "$RED2" --green2 "$GREEN2" --blue2 "$BLUE2" --rotate3 "$ROTATE3" --xscale3 "$XSCALE3" --yscale3 "$YSCALE3" --xshift3 "$XSHIFT3" --yshift3 "$YSHIFT3" --red3 "$RED3" --green3 "$GREEN3" --blue3 "$BLUE3" --iterations "$ITERATIONS" > "$SVG" &
    fi
    sleep 1
done

for i in $(seq -f "%05g" "$FIRSTFRAME" "$LASTFRAME")
do
    FILENAME="ifs""$i"
    echo "convert $FILENAME to PNG, resize, normalize, and flip"
    SVG="$FILENAME"".svg"
    PNG="$FILENAME"".png"
    convert -format png -resize 1400x1400 -normalize -flip "$SVG" "$PNG"
done

ffmpeg -r 30 -i ifs%05d.png "$TIMESTAMP"".mp4"

echo "$TIMESTAMP"".mp4"
