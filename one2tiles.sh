#!/bin/zsh

#ts=("1754208355" "1759028719" "1758799323" "1740786690" "1775989862" "1775991736" "1775994457")
#ts=("1729211689" "1729235731")
#ts=("1730944501" "1731067263" "1731587222") # done 
# 1731675397 needs more than 15 iterations but not worth it
#ts=("1732102535" "1732192346" "1732235518" "1732245598" "1733052373" "1733484327" "1752032807")

ts=("1776588215" "1733027985" "1730026526" "1734670075")

TILESROOT=8
#MAXTILE=$(($TILESROOT * $TILESROOT - 1))

for TIMESTAMP in "${ts[@]}"; do
    grep "$TIMESTAMP" ~/ifs/command_line_options.tsv | cut -f2 | sed "s@^@~/ifs/ifs --tile TILENUMBER --tilesroot $TILESROOT --iterations 15 @" | sed "s/$/ > out/" > "$TIMESTAMP""_tilex.sh"

    echo "#!/bin/zsh" > "$TIMESTAMP""_tile"".sh"
    echo "if [ ! -s ./blank.jpg ]; then magick -size 6144x6144 xc:black blank.jpg; fi" >> "$TIMESTAMP""_tile"".sh"
    echo "echo 'foo' > out" >> "$TIMESTAMP""_tile"".sh"

    for i in {0000..0063}; do
	echo "if [ \$(grep -c tile.""$i"" out) -ne 0 ]; then cp blank.jpg tile.$i.jpg; fi" >> "$TIMESTAMP""_tile"".sh"
	echo "if [ ! -s ./tile.""$i"".jpg ]; then " >> "$TIMESTAMP""_tile"".sh"

	cat "$TIMESTAMP""_tilex.sh" | sed "s/TILENUMBER/$i/g" >> "$TIMESTAMP""_tile"".sh" 
	echo "magick ifs.bmp -verbose -format jpg -flip -resize 6144x6144 tile.""$i"".jpg; fi " >> "$TIMESTAMP""_tile"".sh"
	echo "" >> "$TIMESTAMP""_tile"".sh"
    done

    ### step 1 ###
    echo "magick tile.0000.jpg tile.0001.jpg tile.0002.jpg tile.0003.jpg +append row0.jpg" >> "$TIMESTAMP""_tile"".sh"
    echo "magick tile.0008.jpg tile.0009.jpg tile.0010.jpg tile.0011.jpg +append row1.jpg" >> "$TIMESTAMP""_tile"".sh"
    echo "magick tile.0016.jpg tile.0017.jpg tile.0018.jpg tile.0019.jpg +append row2.jpg" >> "$TIMESTAMP""_tile"".sh"
    echo "magick tile.0024.jpg tile.0025.jpg tile.0026.jpg tile.0027.jpg +append row3.jpg" >> "$TIMESTAMP""_tile"".sh"

    echo "magick row3.jpg row2.jpg row1.jpg row0.jpg -append $TIMESTAMP""_step1.jpg" >> "$TIMESTAMP""_tile"".sh"


    ### step 2 ###
    echo "magick tile.0004.jpg tile.0005.jpg tile.0006.jpg tile.0007.jpg +append row0.jpg" >> "$TIMESTAMP""_tile"".sh"
    echo "magick tile.0012.jpg tile.0013.jpg tile.0014.jpg tile.0015.jpg +append row1.jpg" >> "$TIMESTAMP""_tile"".sh"
    echo "magick tile.0020.jpg tile.0021.jpg tile.0022.jpg tile.0023.jpg +append row2.jpg" >> "$TIMESTAMP""_tile"".sh"
    echo "magick tile.0028.jpg tile.0029.jpg tile.0030.jpg tile.0031.jpg +append row3.jpg" >> "$TIMESTAMP""_tile"".sh"

    echo "magick row3.jpg row2.jpg row1.jpg row0.jpg -append $TIMESTAMP""_step2.jpg" >> "$TIMESTAMP""_tile"".sh"


    ### step 3 ###
    echo "magick tile.0032.jpg tile.0033.jpg tile.0034.jpg tile.0035.jpg +append row0.jpg" >> "$TIMESTAMP""_tile"".sh"
    echo "magick tile.0040.jpg tile.0041.jpg tile.0042.jpg tile.0043.jpg +append row1.jpg" >> "$TIMESTAMP""_tile"".sh"
    echo "magick tile.0048.jpg tile.0049.jpg tile.0050.jpg tile.0051.jpg +append row2.jpg" >> "$TIMESTAMP""_tile"".sh"
    echo "magick tile.0056.jpg tile.0057.jpg tile.0058.jpg tile.0059.jpg +append row3.jpg" >> "$TIMESTAMP""_tile"".sh"

    echo "magick row3.jpg row2.jpg row1.jpg row0.jpg -append $TIMESTAMP""_step3.jpg" >> "$TIMESTAMP""_tile"".sh"


    ### step 4 ###
    echo "magick tile.0036.jpg tile.0037.jpg tile.0038.jpg tile.0039.jpg +append row0.jpg" >> "$TIMESTAMP""_tile"".sh"
    echo "magick tile.0044.jpg tile.0045.jpg tile.0046.jpg tile.0047.jpg +append row1.jpg" >> "$TIMESTAMP""_tile"".sh"
    echo "magick tile.0052.jpg tile.0053.jpg tile.0054.jpg tile.0055.jpg +append row2.jpg" >> "$TIMESTAMP""_tile"".sh"
    echo "magick tile.0060.jpg tile.0061.jpg tile.0062.jpg tile.0063.jpg +append row3.jpg" >> "$TIMESTAMP""_tile"".sh"

    echo "magick row3.jpg row2.jpg row1.jpg row0.jpg -append $TIMESTAMP""_step4.jpg" >> "$TIMESTAMP""_tile"".sh"

    ### step 5 ###
    ### +append to make a row, call it step5
    ### top left step1 + top right step2
    echo "magick $TIMESTAMP""_step1.jpg ""$TIMESTAMP""_step2.jpg +append $TIMESTAMP""_step5.jpg" >> "$TIMESTAMP""_tile"".sh"

    ### step 6 ###
    ### bottom left step3 + bottom right step4
    echo "magick $TIMESTAMP""_step3.jpg ""$TIMESTAMP""_step4.jpg +append $TIMESTAMP""_step6.jpg" >> "$TIMESTAMP""_tile"".sh"

    ### step 7 ###
    ### -append bottom row step6 bottom to top row step5 to make a square
    ### equalize
    ### convert jpg to jpeg as last step
    echo "magick $TIMESTAMP""_step6.jpg ""$TIMESTAMP""_step5.jpg -append -equalize -resize 6144x6144 $TIMESTAMP"".jpg" >> "$TIMESTAMP""_tile"".sh"

    echo "rm tile.*.jpg" >> "$TIMESTAMP""_tile"".sh"
    echo "rm row*.jpg" >> "$TIMESTAMP""_tile"".sh"
    echo "rm ifs.bmp" >> "$TIMESTAMP""_tile"".sh"
    echo "rm *step*.jpg" >> "$TIMESTAMP""_tile"".sh"

    #echo "rm *.jpg" >> "$TIMESTAMP""_tile"".sh"
    echo "rm $TIMESTAMP""_tilex.sh" >> "$TIMESTAMP""_tile"".sh"

    chmod +x "$TIMESTAMP""_tile"".sh"
    ls ./"$TIMESTAMP""_tile"".sh"
done
