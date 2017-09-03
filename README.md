# ifs
Render an image of a fractal generated with an iterated function system (IFS).

### Requirements
Any normal C compiler will do.  The script assumes a regular Linux environment with bash.

### Quickstart
First, compile the C code.  Then, you can run the shell script to run those newly compiled binaries and convert the SVG file to a more manageable PNG file with a few tricks to make it look nicer.
1. `gcc -Wall ifs.c -o ifs -lm`
2. `gcc -Wall points2svg.c -o points2svg -lm`
3. `bash ./ifs.sh`

### Generating Fractals
Minimally: `./ifs | ./points2svg > fractal.svg`.  From the second line of its output to the last, `ifs` prints the points and RGB values of a fractal that it generates based on random parameters.  On the first line of its output, `ifs` prints the minimum and maximum x and y values of those subsequently printed points.  `points2svg` accepts the output of `ifs` and prints an SVG file of the fractal to stdout.

These SVG files are very large and demanding to display, so I would recommend then converting the SVG file to a more compact format.  On a regular Linux environment, you can just run `ifs.sh` on the shell and it will take care of everything (after you compile ifs.c and points2svg.c).
