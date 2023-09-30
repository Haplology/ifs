# ifs
Render an image of a fractal generated with an iterated function system (IFS).

### Dependencies
* gcc 
* [ImageMagick](http://www.imagemagick.org/script/index.php)
* `bmp.h` from [skeeto's BMP library](https://github.com/skeeto/bmp)

If you want to make animations:
* [FFmpeg](https://ffmpeg.org/) 
* Python 

### Quickstart

1. Get a copy of [skeeto's BMP library](https://github.com/skeeto/bmp) and put `bmp.h` in the same directory as ifs.c
2. Compile:`gcc -Wall ./ifs.c -o ifs -lm`
2. Generate a fractal rendered to a bitmap:`./ifs --rotate1 5.811973916078167 --rotate2 0.38627961521895965 --rotate3 5.4352409020747405 --rotate4 3.3014547039291364 --xscale1 0.5803490627378161 --xscale2 0.704957468168029 --xscale3 0.4024125647405239 --xscale4 0.27579071231051877 --yscale1 0.27515513710758005 --yscale2 0.716205801443019 --yscale3 0.5644603992967754 --yscale4 0.5149167446377391 --xshift1 0.43401746776072675 --xshift2 0.7202840952488043 --xshift3 0.253048304131543 --xshift4 0.5036578348369566 --yshift1 0.6782295267680867 --yshift2 0.7457870341404085 --yshift3 0.645867686325276 --yshift4 0.5755495975375138 --red1 178.00000000000003 --red2 13.000000000000004 --red3 108.00000000000003 --red4 117.00000000000001 --green1 149.00000000000003 --green2 237.00000000000003 --green3 14.999999999999993 --green4 89.0 --blue1 249.00000000000006 --blue2 138.00000000000003 --blue3 208.00000000000003 --blue4 230.00000000000003 --iterations 13` 
3. The bitmap is enormous.  You probably want to convert it into something nicer: `convert -verbose -format png -resize 3840x2160 -normalize -flip ifs.bmp ifs.png`
4. You may want to delete the bitmap: `rm ifs.bmp`

Your new fractal should be ifs.png

### Parameters
Rotation is in radians, scale and shift are from 0 to 1, and colors are from 0 to 255.  This supports from three to six transformations.

### Animation
Fractals with nearly equal parameters will look nearly the same.  If the changes in parameters are short and regular, the fractals appear to morph when combined to create an animation.  Below are instructions for making such an animation.

Execute animation.py to save a shell script in the current directory at render.sh.  Revise the shebang as necessary for your environment.  You can then execute the script `render.sh` to generate animation frames and then combine those frames into an animation file. 

The script `animation.py` randomly generates parameters of several fractals, calculates splines which are interpolated from the sequence of each of those parameters (for example a spline for xscale1, another spline for rotate1, etc.), and generates a series of calls to `ifs` to generate images of fractals at points at regular, short distances along those splines, so that the fractals will appear to transform smoothly when an animation is generated from those images.  The script `render.sh` will make one call at a time to `ifs` to generate each frame one by one, and once all frames have been generated, it will run `ffmpeg` to combine all of the frames into one animation.  

Run the following commands:
1. `python3 ./animation.py`
2. `chmod +x ./render.sh`
3. `./render.sh`

It takes a long time.  You can stop midway and resume later where you left off, because `render.sh` will not generate a frame if it already exists in the local directory.  
