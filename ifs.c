#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include "bmp.h"

// gcc -Wall ~/ifs/ifs.c -o ifs -lm

#define STARTPOINT 1000000
// 3840 * 6
#define XRES 23040
// 2160 * 6
#define YRES 12960
#define FOR_REAL 1
#define DRY_RUN 0
// FC stands for full color in rgb
#define FC 255.0

#define WIDTH   23040L
#define HEIGHT  12960L

static double rotate[10]; 
static double xscale[10];
static double yscale[10]; 
static double xshift[10]; 
static double yshift[10]; 
static int palette_red[10]; 
static int palette_green[10]; 
static int palette_blue[10]; 
static short int red_layer[XRES*YRES] = {0};
static short int green_layer[XRES*YRES] = {0};
static short int blue_layer[XRES*YRES] = {0};
static int transformations = 3;
static int iterations = 12;
static int filename = 0;

static double width;
static double height;
static double bitmap_xshift;
static double bitmap_yshift;
static double stretch;
static double padding = YRES*0.01;

static double xmin = FLT_MAX;
static double ymin = FLT_MAX;
static double xmax = -1.0*FLT_MAX;
static double ymax = -1.0*FLT_MAX;


double xstar(double x, double y, double rotate, double xscale, double xshift);
double ystar(double x, double y, double rotate, double yscale, double yshift);
void iterateFS(int iterations_left, double x, double y, double red, double green, double blue, int for_real);
void adjust_bounds(double x, double y);
void set_bitmap_adjustments(); 
void set_pixels(char *bmp);
void init_tr(int argc, char *argv[]);

double xstar(double x, double y, double rotate, double xscale, double xshift) {
  return ((x*cos(rotate) - y*sin(rotate))*xscale + xshift);
}

double ystar(double x, double y, double rotate, double yscale, double yshift) {
  return ((y*cos(rotate) + x*sin(rotate))*yscale + yshift);
}

void iterateFS(int iterations_left, double x, double y, double red, double green, double blue, int for_real) {
  if (iterations_left == 0) {
    /* stop recursion and save in pixel space */
    if ( for_real == DRY_RUN ) {
      // not for real --> dry run, just adjust x and y bounds
      adjust_bounds(x, y);
    } else {
      // for real, set pixel
      //printf("x %lf xmin %lf stretch %lf bitmap_xshift %lf\n", x, xmin, stretch, bitmap_xshift);
      int xpixel = (int)((x-xmin)*stretch + bitmap_xshift);
      //printf("y %lf ymin %lf stretch %lf bitmap_yshift %lf\n", y, ymin, stretch, bitmap_yshift);
      int ypixel = (int)((y-ymin)*stretch + bitmap_yshift);
      if ( xpixel >= 0 && xpixel <= XRES && ypixel >= 0 && ypixel <= YRES ) {
	red_layer[XRES*ypixel + xpixel] = (int)red;
	green_layer[XRES*ypixel + xpixel] = (int)green;
	blue_layer[XRES*ypixel + xpixel] = (int)blue;
      } else {
	printf("pixel out of bounds\n");
      }
    }
  } else {
    /* iterate FS recursively */
    int i;
    for( i = 0; i < transformations; i = i + 1 ) {
      iterateFS(iterations_left-1,
		xstar(x, y, rotate[i], xscale[i], xshift[i]*STARTPOINT),
		ystar(x, y, rotate[i], yscale[i], yshift[i]*STARTPOINT),
		(red + palette_red[i])*0.5,
		(green + palette_green[i])*0.5,
		(blue + palette_blue[i])*0.5,
		for_real
		);
    }
  }
  return;
}

void adjust_bounds(double x, double y) {
    if (x < xmin) {
      xmin = x;
    }
    if (y < ymin) {
      ymin = y;
    }
    if (x > xmax) {
      xmax = x;
    }
    if (y > ymax) {
      ymax = y;
    }
    
    return;
}


void set_bitmap_adjustments() {
  width = xmax - xmin;
  height = ymax - ymin;

  // stretch up to width or height, whichever is the lower factor
  stretch = (XRES/width < YRES/height) ? (XRES/width)*0.98 : (YRES/height)*0.98;
  // if we stretched to width, no extra padding for x, if stretch to height, extra padding to center x
  bitmap_xshift = (XRES/width < YRES/height) ? padding : (((XRES - width*stretch)*0.5)+padding);
  bitmap_yshift = (XRES/width < YRES/height) ? (((YRES - height*stretch)*0.5)+padding) : padding;
    
  return;
}

void set_pixels(char *bmp) {
  int x;
  int y;
  short int red;
  short int green;
  short int blue;

  for ( y = 0; y < YRES; y++ ) {
    for ( x = 0; x < XRES; x++ ) {
      red = red_layer[XRES*y + x];
      green = green_layer[XRES*y + x];
      blue = blue_layer[XRES*y + x];
      bmp_set(bmp, x, y, bmp_encode(red, green, blue));
    }
  }
  
  return;
}

void init_tr(int argc, char *argv[]) {
  int c;

  while (1)
    {
      static struct option long_options[] =
        {
          /* These options don’t set a flag.
             We distinguish them by their indices. */
          {"rotate1",  required_argument, 0, 'a'},
          {"xscale1",  required_argument, 0, 'b'},
          {"yscale1",  required_argument, 0, 'c'},
          {"xshift1",  required_argument, 0, 'd'},
          {"yshift1",  required_argument, 0, 'e'},
          {"red1",  required_argument, 0, 'f'},
          {"green1",  required_argument, 0, 'g'},
          {"blue1",  required_argument, 0, 'h'},
	  
          {"rotate2",  required_argument, 0, 'i'},
          {"xscale2",  required_argument, 0, 'j'},
          {"yscale2",  required_argument, 0, 'k'},
          {"xshift2",  required_argument, 0, 'l'},
          {"yshift2",  required_argument, 0, 'm'},
          {"red2",  required_argument, 0, 'n'},
          {"green2",  required_argument, 0, 'o'},
          {"blue2",  required_argument, 0, 'p'},
	  
          {"rotate3",  required_argument, 0, 'q'},
          {"xscale3",  required_argument, 0, 'r'},
          {"yscale3",  required_argument, 0, 's'},
          {"xshift3",  required_argument, 0, 't'},
          {"yshift3",  required_argument, 0, 'u'},
          {"red3",  required_argument, 0, 'v'},
          {"green3",  required_argument, 0, 'w'},
          {"blue3",  required_argument, 0, 'x'},
	  
          {"rotate4",  required_argument, 0, 'y'},
          {"xscale4",  required_argument, 0, 'z'},
          {"yscale4",  required_argument, 0, 'A'},
          {"xshift4",  required_argument, 0, 'B'},
          {"yshift4",  required_argument, 0, 'C'},
          {"red4",  required_argument, 0, 'D'},
          {"green4",  required_argument, 0, 'E'},
          {"blue4",  required_argument, 0, 'F'},

          {"rotate5",  required_argument, 0, 'G'},
          {"xscale5",  required_argument, 0, 'H'},
          {"yscale5",  required_argument, 0, 'I'},
          {"xshift5",  required_argument, 0, 'J'},
          {"yshift5",  required_argument, 0, 'K'},
          {"red5",  required_argument, 0, 'L'},
          {"green5",  required_argument, 0, 'M'},
          {"blue5",  required_argument, 0, 'N'},

	  {"iterations", required_argument, 0, 'O'},
	  
          {"rotate6",  required_argument, 0, 'P'},
          {"xscale6",  required_argument, 0, 'Q'},
          {"yscale6",  required_argument, 0, 'R'},
          {"xshift6",  required_argument, 0, 'S'},
          {"yshift6",  required_argument, 0, 'T'},
          {"red6",  required_argument, 0, 'U'},
          {"green6",  required_argument, 0, 'V'},
          {"blue6",  required_argument, 0, 'W'},

	  {"filename", required_argument, 0, 'X'}, 
	  
          {0, 0, 0, 0}
        };
      /* getopt_long stores the option index here. */
      int option_index = 0;

      c = getopt_long (argc, argv, "a:b:c:d:e:f:g:h:i:j:k:l:m:n:o:p:q:r:s:t:u:v:w:x:y:z:A:B:C:D:E:F:G:H:I:J:K:L:M:N:O:P:Q:R:S:T:U:V:W:X",
                       long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;

      switch (c) {
      case 'a':
	//printf("%s\n", optarg);
	rotate[0] = atof(optarg);
	//printf("rotation #1 = %lf\n", rotate[0]);
	break;
      case 'b':
	xscale[0] = atof(optarg);
	break;
      case 'c':
	yscale[0] = atof(optarg);
	break; 
      case 'd':
	xshift[0] = atof(optarg);
	break; 
      case 'e':
	yshift[0] = atof(optarg);
	break; 
      case 'f':
	palette_red[0] = atoi(optarg);
	break;
      case 'g':
	palette_green[0] = atoi(optarg);
	break;
      case 'h':
	palette_blue[0] = atoi(optarg);
	break;
      
      case 'i':
	rotate[1] = atof(optarg);
	break;
      case 'j':
	xscale[1] = atof(optarg);
	break;
      case 'k':
	yscale[1] = atof(optarg);
	break; 
      case 'l':
	xshift[1] = atof(optarg);
	break; 
      case 'm':
	yshift[1] = atof(optarg);
	break; 
      case 'n':
	palette_red[1] = atoi(optarg);
	break;
      case 'o':
	palette_green[1] = atoi(optarg);
	break;
      case 'p':
	palette_blue[1] = atoi(optarg);
	break; 

      case 'q':
	rotate[2] = atof(optarg);
	break;
      case 'r':
	xscale[2] = atof(optarg);
	break;
      case 's':
	yscale[2] = atof(optarg);
	break; 
      case 't':
	xshift[2] = atof(optarg);
	break; 
      case 'u':
	yshift[2] = atof(optarg);
	break; 
      case 'v':
	palette_red[2] = atoi(optarg);
	break;
      case 'w':
	palette_green[2] = atoi(optarg);
	break;
      case 'x':
	palette_blue[2] = atoi(optarg);
	break;
	
      case 'y':
	rotate[3] = atof(optarg);
	transformations = (transformations < 4) ? 4 : transformations;
	break;
      case 'z':
	xscale[3] = atof(optarg);
	transformations = (transformations < 4) ? 4 : transformations;
	break;
      case 'A':
	yscale[3] = atof(optarg);
	transformations = (transformations < 4) ? 4 : transformations;
	break; 
      case 'B':
	xshift[3] = atof(optarg);
	transformations = (transformations < 4) ? 4 : transformations;
	break; 
      case 'C':
	yshift[3] = atof(optarg);
	transformations = (transformations < 4) ? 4 : transformations;
	break; 
      case 'D':
	palette_red[3] = atoi(optarg);
	transformations = (transformations < 4) ? 4 : transformations;
	break;
      case 'E':
	palette_green[3] = atoi(optarg);
	transformations = (transformations < 4) ? 4 : transformations;
	break;
      case 'F':
	palette_blue[3] = atoi(optarg);
	transformations = (transformations < 4) ? 4 : transformations;
	break; 

      case 'G':
	rotate[4] = atof(optarg);
	transformations = (transformations < 5) ? 5 : transformations;
	break;
      case 'H':
	xscale[4] = atof(optarg);
	transformations = (transformations < 5) ? 5 : transformations;
	break;
      case 'I':
	yscale[4] = atof(optarg);
	transformations = (transformations < 5) ? 5 : transformations;
	break; 
      case 'J':
	xshift[4] = atof(optarg);
	transformations = (transformations < 5) ? 5 : transformations;
	break; 
      case 'K':
	yshift[4] = atof(optarg);
	transformations = (transformations < 5) ? 5 : transformations;
	break; 
      case 'L':
	palette_red[4] = atoi(optarg);
	transformations = (transformations < 5) ? 5 : transformations;
	break;
      case 'M':
	palette_green[4] = atoi(optarg);
	transformations = (transformations < 5) ? 5 : transformations;
	break;
      case 'N':
	palette_blue[4] = atoi(optarg);
	transformations = (transformations < 5) ? 5 : transformations;
	break;
       
      case 'O':
	iterations = atoi(optarg);
	break;
	
      case 'P':
	rotate[5] = atof(optarg);
	transformations = (transformations < 6) ? 6 : transformations;
	break;
      case 'Q':
	xscale[5] = atof(optarg);
	transformations = (transformations < 6) ? 6 : transformations;
	break;
      case 'R':
	yscale[5] = atof(optarg);
	transformations = (transformations < 6) ? 6 : transformations;
	break; 
      case 'S':
	xshift[5] = atof(optarg);
	transformations = (transformations < 6) ? 6 : transformations;
	break; 
      case 'T':
	yshift[5] = atof(optarg);
	transformations = (transformations < 6) ? 6 : transformations;
	break; 
      case 'U':
	palette_red[5] = atoi(optarg);
	transformations = (transformations < 6) ? 6 : transformations;
	break;
      case 'V':
	palette_green[5] = atoi(optarg);
	transformations = (transformations < 6) ? 6 : transformations;
	break;
      case 'W':
	palette_blue[5] = atoi(optarg);
	transformations = (transformations < 6) ? 6 : transformations;
	break;
      case 'X':
	filename = atoi(optarg);
	break;
	
      default:
	abort ();
      }
    }

  return;
}

int main(int argc, char *argv[])
{
  init_tr(argc, argv);


  FILE *f;
  
  static char bmp[BMP_SIZE(WIDTH, HEIGHT)];
  bmp_init(bmp, WIDTH, HEIGHT);

  // dry run to find upper and lower bounds on x and y
  iterateFS(iterations, STARTPOINT, STARTPOINT, FC, FC, FC, DRY_RUN);

  // based on xmin, ymin, xmax, ymax found in dry run, set other values for conv to bitmap
  set_bitmap_adjustments();

  // calculate IFS for real
  iterateFS(iterations, STARTPOINT, STARTPOINT, FC, FC, FC, FOR_REAL);

  set_pixels(bmp);

  switch (filename) {
  case 0:
    f = fopen("ifs0.bmp", "wb");
    break;
  case 1:
    f = fopen("ifs1.bmp", "wb");
    break;
  case 2:
    f = fopen("ifs2.bmp", "wb");
    break;
  case 3:
    f = fopen("ifs3.bmp", "wb");
    break;
  case 4:
    f = fopen("ifs4.bmp", "wb");
    break;
  case 5:
    f = fopen("ifs5.bmp", "wb");
    break;
  case 6:
    f = fopen("ifs6.bmp", "wb");
    break;
  case 7:
    f = fopen("ifs7.bmp", "wb");
    break;
  case 8:
    f = fopen("ifs8.bmp", "wb");
    break;
  case 9:
    f = fopen("ifs9.bmp", "wb");
    break;
  default:
    f = fopen("test.bmp", "wb");
  }
  
  fwrite(bmp, sizeof(bmp), 1, f);
  fclose(f);

  return(0);
}
