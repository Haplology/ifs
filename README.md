# ifs
Render an image of a fractal generated with an iterated function system (IFS).

### System Requirements
* bash
* gcc 
* [ImageMagick](http://www.imagemagick.org/script/index.php) 

### Quickstart
From within the ifs directory, run this script to compile the binary and generate IFS fractals in an endless loop: 

1. `chmod +x ./ifs.sh`
2. `./ifs.sh`

### Generating Fractals
This generates fractals based on parameters chosen at random.  You can fiddle with the ranges of random parameters by making changes at the head of ifs.c and recompiling.  
