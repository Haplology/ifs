#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

#define STARTPOINT 1000000
#define RESOLUTION 3100
#define PADDING 0.01
#define CUTOFF_INIT 1.0
#define FOR_REAL 1
#define DRY_RUN 0
// FC stands for full color in rgb
#define FC 255.0 

static double rotate[10]; 
static double xscale[10];
static double yscale[10]; 
static double xshift[10]; 
static double yshift[10]; 
static int palette_red[10]; 
static int palette_green[10]; 
static int palette_blue[10]; 
static short int pixelrgb[RESOLUTION*RESOLUTION*3] = {0};
static int transformations = 3;
static int iterations = 12;
// 5^0 + 5^1 + 5^2 + 5^3 + 5^4 + 5^5 + 5^6 + 5^7 + 5^8 + 5^9 + 5^10 + 5^11 + 5^12 + 5^13 = 1525878906

static double width;
static double height;
static double greater;
static double lesser;
static double bitmap_xshift;
static double bitmap_yshift;

static double xmin = FLT_MAX;
static double ymin = FLT_MAX;
static double xmax = -1.0*FLT_MAX;
static double ymax = -1.0*FLT_MAX;

double xstar(double x, double y, double rotate, double xscale, double xshift);
double ystar(double x, double y, double rotate, double yscale, double yshift);
void iterateFS(int iterations_left, double x, double y, double red, double green, double blue, double xcutoff, double ycutoff, int for_real);
void adjust_bounds(double x, double y);
void set_bitmap_adjustments(); 
void print_svg();
void init_tr(int argc, char *argv[]);

double xstar(double x, double y, double rotate, double xscale, double xshift) {
  return ((x*cos(rotate) - y*sin(rotate))*xscale + xshift);
}

double ystar(double x, double y, double rotate, double yscale, double yshift) {
  return ((y*cos(rotate) + x*sin(rotate))*yscale + yshift);
}

void iterateFS(int iterations_left, double x, double y, double red, double green, double blue, double xcutoff, double ycutoff, int for_real) {
  if (iterations_left == 0 || (xcutoff < (0.71 / RESOLUTION) && ycutoff < (0.71 / RESOLUTION)) ) {
    /* stop recursion and save in pixel space */
    if ( for_real == DRY_RUN ) {
      // not for real --> dry run, just adjust x and y bounds
      adjust_bounds(x, y);
    } else {
      // for real, set pixel 
      int xpixel = (int)((((x-xmin+bitmap_xshift)/greater)*RESOLUTION)*(1.0-PADDING*2.0)+RESOLUTION*PADDING);
      int ypixel = (int)((((y-ymin+bitmap_yshift)/greater)*RESOLUTION)*(1.0-PADDING*2.0)+RESOLUTION*PADDING);
      if ( xpixel >= 0 && xpixel <= RESOLUTION && ypixel >= 0 && ypixel <= RESOLUTION ) {
	pixelrgb[RESOLUTION*3*xpixel + 3*ypixel + 0] = (int)red;
	pixelrgb[RESOLUTION*3*xpixel + 3*ypixel + 1] = (int)green;
	pixelrgb[RESOLUTION*3*xpixel + 3*ypixel + 2] = (int)blue;
	//printf("%d %d\n", xpixel, ypixel);
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
		xcutoff*fabs(xscale[i]), ycutoff*fabs(yscale[i]), 
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
  
  if (width > height) {
    greater = width;
    lesser = height;
    bitmap_xshift = 0.0;
    bitmap_yshift = (greater - lesser)*0.5;
  } else {
    greater = height;
    lesser = width;
    bitmap_xshift = (greater - lesser)*0.5;
    bitmap_yshift = 0.0;
  }
  
  return;
}

void print_svg() {
  int x;
  int y;
  short int red;
  short int green;
  short int blue;
  int i;
  
  printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  printf("<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"100%%\" height=\"100%%\" viewBox=\"0 0 %d %d\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n", RESOLUTION, RESOLUTION);
  printf("    <rect x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" fill=\"black\" />\n", RESOLUTION, RESOLUTION);
  printf("    <desc>\n");
  for ( i = 0; i < transformations; i++ ) {
    printf("    rotate\t%lf\txscale\t%lf\tyscale\t%lf\txshift\t%lf\tyshift\t%lf\tred\t%d\tgreen\t%d\tblue\t%d\n", rotate[i], xscale[i], yscale[i], xshift[i], yshift[i], palette_red[i], palette_green[i], palette_blue[i]);
  }
  printf("     ./ifs ");
  for ( i = 0; i < transformations; i++ ) {
    printf("--rotate%d %lf --xscale%d %lf --yscale%d %lf --xshift%d %lf --yshift%d %lf --red%d %d --green%d %d --blue%d %d ", i+1, rotate[i], i+1, xscale[i], i+1, yscale[i], i+1, xshift[i], i+1, yshift[i], i+1, palette_red[i], i+1, palette_green[i], i+1, palette_blue[i]);
  }
  printf("--iterations %d\n", iterations);
  printf("    </desc>\n");
  
  for ( x = 0; x < RESOLUTION; x++ ) {
    for ( y = 0; y < RESOLUTION; y++ ) {
      red = pixelrgb[RESOLUTION*3*x + 3*y + 0];
      green = pixelrgb[RESOLUTION*3*x + 3*y + 1];
      blue = pixelrgb[RESOLUTION*3*x + 3*y + 2];
      if (red+green+blue > 0) {
	printf("    <circle cx=\"%d\" cy=\"%d\" r=\"0.4\" fill=\"rgb(%d, %d, %d)\" />\n",
	       x, y,
	       red, green, blue);
      }
    }
  }
  printf("\n</svg>\n");

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
	  
          {0, 0, 0, 0}
        };
      /* getopt_long stores the option index here. */
      int option_index = 0;

      c = getopt_long (argc, argv, "a:b:c:d:e:f:g:h:i:j:k:l:m:n:o:p:q:r:s:t:u:v:w:x:y:z:A:B:C:D:E:F:G:H:I:J:K:L:M:N:O",
                       long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;

      switch (c) {
      case 'a':
	//printf("%s\n", optarg);
	rotate[0] = atof(optarg);
	//printf("rototion #1 = %lf\n", rotate[0]);
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
	
      default:
	abort ();
      }
    }

  return;
}

int main(int argc, char *argv[])
{
  init_tr(argc, argv);
  
  // dry run to find upper and lower bounds on x and y
  iterateFS(iterations, STARTPOINT, STARTPOINT, FC, FC, FC, CUTOFF_INIT, CUTOFF_INIT, DRY_RUN);

  // based on xmin, ymin, xmax, ymax found in dry run, set other values for conv to bitmap
  set_bitmap_adjustments();

  // calculate IFS for real
  iterateFS(iterations, STARTPOINT, STARTPOINT, FC, FC, FC, CUTOFF_INIT, CUTOFF_INIT, FOR_REAL);

  // based on bitmap in pixelrgb, print SVG to stdout
  print_svg();
  

  return(0);
}
