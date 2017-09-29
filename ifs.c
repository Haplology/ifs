#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define ITERATIONS 12
#define SIZE 2000
#define MAX_TRANSFORMATIONS 4
#define COLOR_DECAY 1.0

static double rotate[MAX_TRANSFORMATIONS]; 
static double xscale[MAX_TRANSFORMATIONS];
static double yscale[MAX_TRANSFORMATIONS]; 
static double xshift[MAX_TRANSFORMATIONS]; 
static double yshift[MAX_TRANSFORMATIONS]; 
static double palette_red[MAX_TRANSFORMATIONS]; 
static double palette_green[MAX_TRANSFORMATIONS]; 
static double palette_blue[MAX_TRANSFORMATIONS]; 



double xstar(double x, double y, double rotate, double xscale, double xshift);
double ystar(double x, double y, double rotate, double yscale, double yshift);
void iterateFS(int iterations_left, double x, double y, double red, double green, double blue, int transformations);
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

void iterateFS(int iterations_left, double x, double y, double red, double green, double blue, int transformations) {
  if (iterations_left == 0) {
    /* stop recursion and print out final values of x and y */
    printf("%f\t%f\t%f\t%f\t%f\n", x, y, red, green, blue);
  } else {
    /* iterate FS recursively */
    int i;
    for( i = 0; i < transformations; i = i + 1 ) {
      iterateFS(iterations_left-1,
		xstar(x, y, rotate[i], xscale[i], xshift[i]*SIZE),
		ystar(x, y, rotate[i], yscale[i], yshift[i]*SIZE),
		(red*COLOR_DECAY + palette_red[i])*0.5,
		(green*COLOR_DECAY + palette_green[i])*0.5,
		(blue*COLOR_DECAY + palette_blue[i])*0.5,
		transformations
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
		  xstar(x, y, rotate[i], xscale[i], xshift[i]*SIZE),
		  ystar(x, y, rotate[i], yscale[i], yshift[i]*SIZE),
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
  double r = normalized_random();
  double min_scale = 0.4;
  double max_scale = 0.6;
  double scale_range = max_scale - min_scale;
  return (min_scale + scale_range*r);
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
  
  double xmin = 100000.0;
  double ymin = 100000.0;
  double xmax = -100000.0;
  double ymax = -100000.0;
  find_bounds(ITERATIONS, SIZE, SIZE, &xmin, &ymin, &xmax, &ymax, transformations);
  printf("%f\t%f\t%f\t%f\n", xmin, ymin, xmax, ymax);

  iterateFS(ITERATIONS, SIZE, SIZE, 255.0, 255.0, 255.0, transformations);
  return(0);
}
