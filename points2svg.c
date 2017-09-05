#include <stdio.h>

int main() {
  // get bounds and derive height, width, and greater of the two 
  double xmin;
  double ymin;
  double xmax;
  double ymax;
  scanf("%lf\t%lf\t%lf\t%lf", &xmin, &ymin, &xmax, &ymax);
  //printf("%lf\t%lf\t%lf\t%lf\n", xmin, ymin, xmax, ymax);
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

  
  double x;
  double y;
  double red;
  double green;
  double blue;

  double padding = 0.01;
  double resolution = 2000.0;

  static int pixelrgb[2000][2000][3] = {0};
  //printf("green at (1, 1): %d\n", pixelrgb[1][1][1]);

  int xpixel;
  int ypixel;
  while (scanf("%lf\t%lf\t%lf\t%lf\t%lf", &x, &y, &red, &green, &blue) == 5 && !feof(stdin)) {
    //printf("%lf\t%lf\t%lf\t%lf\t%lf\n", x, y, red, green, blue);
    xpixel = (int)((((x-xmin+xshift)/greater)*resolution)*(1.0-padding*2.0)+resolution*padding);
    ypixel = (int)((((y-ymin+yshift)/greater)*resolution)*(1.0-padding*2.0)+resolution*padding);
    pixelrgb[xpixel][ypixel][0] = (int)red;
    pixelrgb[xpixel][ypixel][1] = (int)green;
    pixelrgb[xpixel][ypixel][2] = (int)blue;
  }


  int row;
  int column;
  printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
  printf("\n");
  printf("<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"100%%\" height=\"100%%\" viewBox=\"0 0 2000 2000\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">");
  printf("\n");
  printf("    <rect x=\"0\" y=\"0\" width=\"2000\" height=\"2000\" fill=\"black\" />");
  printf("\n");
  
  for (row = 0; row < 2000; row = row + 1) {
    for (column = 0; column < 2000; column = column + 1) {
      if ((pixelrgb[column][row][0]+pixelrgb[column][row][1]+pixelrgb[column][row][2]) > 0) {
	printf("    <circle cx=\"%d\" cy=\"%d\" r=\"0.5\" fill=\"rgb(%d, %d, %d)\" fill-opacity=\"0.5\" />", column, row, pixelrgb[column][row][0], pixelrgb[column][row][1], pixelrgb[column][row][2]);
	printf("\n");
      }
    }
  }

  printf("\n</svg>\n");
  return 0;
}
