#include "tom_image.h"
#include "framebf.h"

void displayTomImage(int x, int y){
  for(int h = 0; h < tom_image_height ; h++){
    for (int w = 0; w < tom_image_width; w++){
      drawPixel(x + w, y + h, tom_image[ h * tom_image_width + w]);
    }
  }
}
