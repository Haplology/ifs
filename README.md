# ifs
Render an image of a fractal generated with an iterated function system (IFS).

### System Requirements
* bash
* gcc 
* [ImageMagick](http://www.imagemagick.org/script/index.php) (for conversion from SVG)
* [FFmpeg](https://ffmpeg.org/) (for generating animations)

### Quickstart
Let's make a Sierpinski triangle.

1. `gcc -Wall ~/ifs/ifs.c -o ifs -lm`
2. `./ifs --rotate1 0 --xscale1 0.5 --yscale1 0.5 --xshift1 0 --yshift1 0 --red1 255 --green1 0 --blue1 0 --rotate2 0 --xscale2 0.5 --yscale2 0.5 --xshift2 0.5 --yshift2 0 --red2 0 --green2 255 --blue2 0 --rotate3 0 --xscale3 0.5 --yscale3 0.5 --xshift3 0.25 --yshift3 0.5 --red3 0 --green3 0 --blue3 255 sierpinski.svg` 
3. `convert -format png -resize 1400x1400 -normalize -flip sierpinski.svg sierpinski.png`

### Scripts 
ifs3.sh generates fractals from three functions, ifs4.sh from four, and ifs5.sh from five. movie.sh generates an animation made by transitioning from one fractal to another, but before you use that, you need to specify the SVG files that it will use to get the parameters for those two fractals.

### Parameters
Rotation is in radians, scale and shift are from 0 to 1, and colors are from 0 to 255.

#### Three functions 
`ifs --rotate1 --xscale1 --yscale1 --xshift1 --yshift1 --red1 --green1 --blue1 --rotate2 --xscale2 --yscale2 --xshift2 --yshift2 --red2 --green2 --blue2 --rotate3 --xscale3 --yscale3 --xshift3 --yshift3 --red3 --green3 --blue3 `

#### Four functions 
`ifs --rotate1 --xscale1 --yscale1 --xshift1 --yshift1 --red1 --green1 --blue1 --rotate2 --xscale2 --yscale2 --xshift2 --yshift2 --red2 --green2 --blue2 --rotate3 --xscale3 --yscale3 --xshift3 --yshift3 --red3 --green3 --blue3 --rotate4 --xscale4 --yscale4 --xshift4 --yshift4 --red4 --green4 --blue4 `

#### Five functions 
`ifs --rotate1 --xscale1 --yscale1 --xshift1 --yshift1 --red1 --green1 --blue1 --rotate2 --xscale2 --yscale2 --xshift2 --yshift2 --red2 --green2 --blue2 --rotate3 --xscale3 --yscale3 --xshift3 --yshift3 --red3 --green3 --blue3 --rotate4 --xscale4 --yscale4 --xshift4 --yshift4 --red4 --green4 --blue4 --rotate5 --xscale5 --yscale5 --xshift5 --yshift5 --red5 --green5 --blue5 `
