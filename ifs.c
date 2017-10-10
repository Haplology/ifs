#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>

#define ITERATIONS 11
#define STARTPOINT 100000
#define RESOLUTION 3000
#define MAX_TRANSFORMATIONS 6
#define PADDING 0.01
#define MINSCALE 0.2
#define MAXSCALE 0.7


static double rotate[MAX_TRANSFORMATIONS]; 
static double xscale[MAX_TRANSFORMATIONS];
static double yscale[MAX_TRANSFORMATIONS]; 
static double xshift[MAX_TRANSFORMATIONS]; 
static double yshift[MAX_TRANSFORMATIONS]; 
static double palette_red[MAX_TRANSFORMATIONS]; 
static double palette_green[MAX_TRANSFORMATIONS]; 
static double palette_blue[MAX_TRANSFORMATIONS]; 
static short int pixelrgb[RESOLUTION*RESOLUTION*3] = {0};



double xstar(double x, double y, double rotate, double xscale, double xshift);
double ystar(double x, double y, double rotate, double yscale, double yshift);
void iterateFS(int iterations_left, double x, double y, double red, double green, double blue, int transformations, double xmin, double ymin, double xmax, double ymax, short int pixelrgb[]);
void find_bounds(int iterations_left, double x, double y, double *xminp, double *yminp, double *xmaxp, double *ymaxp, int transformations);
double random_angle();
double normalized_random();
double random_scale();
double random_shift();
int random_color();
int random_number_of_transformations();




double xstar(double x, double y, double rotate, double xscale, double xshift) {
  return ((x*cos(rotate) - y*sin(rotate))*xscale + xshift);
}

double ystar(double x, double y, double rotate, double yscale, double yshift) {
  return ((y*cos(rotate) + x*sin(rotate))*yscale + yshift);
}

void iterateFS(int iterations_left, double x, double y, double red, double green, double blue, int transformations, double xmin, double ymin, double xmax, double ymax, short int pixelrgb[]) {
  if (iterations_left == 0) {
    /* stop recursion and save in pixel space */
    double width = xmax - xmin;
    double height = ymax - ymin;
    double greater;
    double lesser;
    double xshift = 0.0;
    double yshift = 0.0;
    if (width > height) {
      greater = width;
      lesser = height;
      yshift = (greater - lesser)*0.5;
    } else {
      greater = height;
      lesser = width;
      xshift = (greater - lesser)*0.5;
    }

    int xpixel = (int)((((x-xmin+xshift)/greater)*RESOLUTION)*(1.0-PADDING*2.0)+RESOLUTION*PADDING);
    int ypixel = (int)((((y-ymin+yshift)/greater)*RESOLUTION)*(1.0-PADDING*2.0)+RESOLUTION*PADDING);
    pixelrgb[RESOLUTION*3*xpixel + 3*ypixel + 0] = (int)red;
    pixelrgb[RESOLUTION*3*xpixel + 3*ypixel + 1] = (int)green;
    pixelrgb[RESOLUTION*3*xpixel + 3*ypixel + 2] = (int)blue;
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
		transformations, 
		xmin, ymin, xmax, ymax,
		pixelrgb
		);
    }
  }
  return;
}

void find_bounds(int iterations_left, double x, double y, double *xminp, double *yminp, double *xmaxp, double *ymaxp, int transformations) {
  if (iterations_left == 0) {
    /* stop recursion  */
    if (x < *xminp) {
      *xminp = x;
    }
    if (y < *yminp) {
      *yminp = y;
    }
    if (x > *xmaxp) {
      *xmaxp = x;
    }
    if (y > *ymaxp) {
      *ymaxp = y;
    }
  } else {
    /* iterate FS recursively */
    int i;
    for( i = 0; i < transformations; i = i + 1 ) {
      find_bounds(iterations_left-1,
		  xstar(x, y, rotate[i], xscale[i], xshift[i]*STARTPOINT),
		  ystar(x, y, rotate[i], yscale[i], yshift[i]*STARTPOINT),
		  xminp, yminp, xmaxp, ymaxp,
		  transformations);
    }
  }
  return;
}

double normalized_random() {
  double denom = (double)RAND_MAX;
  int r = rand();
  double numer = (double)r;
  return numer/denom;
}

double random_angle() {
  double r = normalized_random();
  return r*2.0*3.14159;
}

double random_scale() {
  return (MINSCALE + (MAXSCALE - MINSCALE)*normalized_random()); 
}

double random_shift() {
  double r = normalized_random();
  return r;
}

int random_color() {
  return rand() % 256;
}

int random_number_of_transformations() {
  int min_transformations = 3;
  return (min_transformations + (rand() % (MAX_TRANSFORMATIONS-min_transformations+1)));
}










int main() {
  srand(time(NULL)); 

  int i;
  for (i = 0; i < MAX_TRANSFORMATIONS; i++) {
    rotate[i] = random_angle();
    xscale[i] = random_scale();
    yscale[i] = random_scale();
    xshift[i] = random_shift();
    yshift[i] = random_shift();
    palette_red[i] = random_color();
    palette_green[i] = random_color();
    palette_blue[i] = random_color();
  }

  int transformations = random_number_of_transformations();  
  
  double xmin = FLT_MAX;
  double ymin = FLT_MAX;
  double xmax = -1.0*FLT_MAX;
  double ymax = -1.0*FLT_MAX;
  find_bounds(ITERATIONS, STARTPOINT, STARTPOINT, &xmin, &ymin, &xmax, &ymax, transformations);

  iterateFS(ITERATIONS, STARTPOINT, STARTPOINT, 255.0, 255.0, 255.0, transformations, xmin, ymin, xmax, ymax, pixelrgb);

  printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  printf("<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"100%%\" height=\"100%%\" viewBox=\"0 0 %d %d\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n", RESOLUTION, RESOLUTION);
  printf("    <rect x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" fill=\"black\" />\n", RESOLUTION, RESOLUTION);
  
  int x;
  int y;
  short int red;
  short int green;
  short int blue;
  for ( x = 0; x < RESOLUTION; x++ ) {
    for ( y = 0; y < RESOLUTION; y++ ) {
      red = pixelrgb[RESOLUTION*3*x + 3*y + 0];
      green = pixelrgb[RESOLUTION*3*x + 3*y + 1];
      blue = pixelrgb[RESOLUTION*3*x + 3*y + 2];
      if ((red != 0) || (green != 0) || (blue != 0)) {
	printf("    <circle cx=\"%d\" cy=\"%d\" r=\"0.4\" fill=\"rgb(%d, %d, %d)\" />\n",
	       x, y,
	       red, green, blue);
      }
    }
  }
  printf("\n</svg>\n");


  return(0);
}
