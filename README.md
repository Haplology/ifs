# ifs
Render an image of a fractal generated with an iterated function system (IFS).

### Dependencies
* C compiler
* [ImageMagick](http://www.imagemagick.org/script/index.php)
* `bmp.h` from [skeeto's BMP library](https://github.com/skeeto/bmp)
* Python 3

### Quickstart

1. Get a copy of [skeeto's BMP library](https://github.com/skeeto/bmp) and put `bmp.h` in the same directory as ifs.c
2. Compile:`gcc -Wall ./ifs.c -o ifs -lm`
3. `./one2tiles.sh`
4. `./1730026526_tile.sh`
5. When it is finished, you should have your new fractal at `1730026526.jpg`.

### Parameters
Rotation is in radians, scale and shift are from 0 to 1, and colors are from 0 to 255.  

Three to six contraction mappings are supported.  

The fractal looks better if it is rendered at first as a very large image and then shrunk down.  Since this program produces a bitmap, it is not possible to produce an image satisfactorily large in one go.  Therefore, this program can produce a given part of the whole fractal which can then be converted to JPEG.  You can run this program multiple times to produce each of the parts or "tiles" of the whole fractal which can then be all be appended together to produce an enormous JPEG image which can then be shrunk.  

The script `one2tiles.sh` does all of that.  That script retrieves the command line options for the desired fractal from `command_line_options.tsv` which already has a nice selection of fractals.

If you want to render your own fractal, you can add its parameters as command line options in `command_line_options.tsv` and then edit `one2tiles.sh` to generate a script to render it.  
