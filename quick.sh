#!/bin/zsh

TIMESTAMP=$(date +%s)

echo "#!/bin/zsh" > "$TIMESTAMP"".sh"
awk 'BEGIN { srand(); printf "~/ifs/ifs --tile 0 --tilesroot 1 --rotate1 %f --rotate2 %f --rotate3 %f --rotate4 %f --xscale1 %f --xscale2 %f --xscale3 %f --xscale4 %f --yscale1 %f --yscale2 %f --yscale3 %f --yscale4 %f --xshift1 %f --xshift2 %f --xshift3 %f --xshift4 %f --yshift1 %f --yshift2 %f --yshift3 %f --yshift4 %f --red1 %f --red2 %f --red3 %f --red4 %f --green1 %f --green2 %f --green3 %f --green4 %f --blue1 %f --blue2 %f --blue3 %f --blue4 %f --iterations 13 \n", rand()*6.283, rand()*6.283, rand()*6.283, rand()*6.283, rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand(),  rand()*255, rand()*255, rand()*255, rand()*255, rand()*255, rand()*255, rand()*255, rand()*255, rand()*255, rand()*255, rand()*255, rand()*255 }' >> "$TIMESTAMP"".sh"

# normalize -> equalize
echo "magick ifs.bmp -verbose -format jpg -equalize -flip ""$TIMESTAMP"".jpg" >> "$TIMESTAMP"".sh"

echo "mv $TIMESTAMP"".jpg product" >> "$TIMESTAMP"".sh"

echo "rm ifs.bmp" >> "$TIMESTAMP"".sh"

chmod +x "$TIMESTAMP"".sh"
./"$TIMESTAMP"".sh"
