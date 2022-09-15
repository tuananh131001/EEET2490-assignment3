#include "tom_image.h"
#include "jerry_image.h"
#include "framebf.h"
#include "helper.h"
#include "uart.h"
#include "game_over.h"
#include "game_win.h"
#include "authors.h"
#include "spaceship.h"
#include "life.h"
#include "game_image.h"
// Function to display tom image
void displayTomImage(int x, int y){
  for(int h = 0; h < tom_image_height ; h++){
    for (int w = 0; w < tom_image_width; w++){
      drawPixelARGB32(x + w, y + h, tom_image[ h * tom_image_width + w]);
    }
  }
}

// Function to display game over image
void displayGameOverImage(int x, int y){
  for(int h = 0; h < game_over_height_image ; h++){
    for (int w = 0; w < game_over_width_image; w++){
      drawPixelARGB32(x + w, y + h, game_over_image[ h * game_over_width_image + w]);
    }
  }
}

// Function to display space ship image
void displaySpaceShipImage(int x, int y){
  for(int h = 0; h < space_ship_height ; h++){
    for (int w = 0; w < space_ship_width; w++){
      drawPixelARGB32(x + w, y + h, space_ship[ h * space_ship_width + w]);
    }
  }
}

// Function to display space ship level 2 image
void displaySpaceShipImageLevel2(int x, int y){
  for(int h = 0; h < space_ship_height2 ; h++){
    for (int w = 0; w < space_ship_width2; w++){
      drawPixelARGB32(x + w, y + h, space_ship2[ h * space_ship_width2 + w]);
    }
  }
}

// Function to display space ship level 3 image
void displaySpaceShipImageLevel3(int x, int y){
  for(int h = 0; h < space_ship_height3 ; h++){
    for (int w = 0; w < space_ship_width3; w++){
      drawPixelARGB32(x + w, y + h, space_ship3[ h * space_ship_width3 + w]);
    }
  }
}
// Function to display space ship level 4 image
void displaySpaceShipImageLevel4(int x, int y){
  for(int h = 0; h < space_ship_height4 ; h++){
    for (int w = 0; w < space_ship_width4; w++){
      drawPixelARGB32(x + w, y + h, space_ship4[ h * space_ship_width4 + w]);
    }
  }
}

// Function to display game win image
void displayGameWinImage(int x, int y){
  for(int h = 0; h < game_win_height_image ; h++){
    for (int w = 0; w < game_win_width_image; w++){
      drawPixelARGB32(x + w, y + h, game_win_image[ h * game_win_width_image + w]);
    }
  }
}

// Function to display tom jerry image (oversize image)
void displayTomJerryImage (int x, int y){
  for(int h = 0; h < jerry_height_image ; h++){
    for (int w = 0; w < jerry_width_image; w++){
      drawPixelARGB32(x + w, y + h, jerry_image[ h * jerry_width_image + w]);
    }
  }
}

void displayAuthorsImage (int x, int y){
  for(int h = 0; h < authors_height_image ; h++){
    for (int w = 0; w < authors_width_image; w++){
      drawPixelARGB32(x + w, y + h, authors_image[ h * authors_width_image + w]);
    }
  }
}

void displayWordPlayerLife(int x,int y){
  for(int h = 0; h < word_lives_height ; h++){
    for (int w = 0; w < word_lives_width; w++){
      drawPixelARGB32(x + w, y + h, word_lives_image[ h * word_lives_width + w]);
    }
  }
}

void displayPlayerLife(int x,int y){
  for(int h = 0; h < life_height ; h++){
    for (int w = 0; w < life_width; w++){
      drawPixelARGB32(x + w, y + h, life_image[ h * life_width + w]);
    }
  }
}

void clearPlayerLife(int x,int y){
  for(int h = 0; h < life_height ; h++){
    for (int w = 0; w < life_width; w++){
      drawPixelARGB32(x + w, y + h, 0);
    }
  }
}

// Function display score
void displayScore(int x, int y){
  for(int h = 0; h < word_score_height ; h++){
    for (int w = 0; w < word_score_width; w++){
      drawPixelARGB32(x + w, y + h, word_score_image[ h * word_score_width + w]);
    }
  }
}

// Function display score
void displayScore1(int x, int y){
  for(int h = 0; h < score_1_height ; h++){
    for (int w = 0; w < score_1_width; w++){
      drawPixelARGB32(x + w, y + h, score_1_image[ h * score_1_width + w]);
    }
  }
}
// Function display score
void displayScore2(int x, int y){
  for(int h = 0; h < score_2_height ; h++){
    for (int w = 0; w < score_2_width; w++){
      drawPixelARGB32(x + w, y + h, score_2_image[ h * score_2_width + w]);
    }
  }
}
// Function display score
void displayScore3(int x, int y){
  for(int h = 0; h < score_3_height ; h++){
    for (int w = 0; w < score_3_width; w++){
      drawPixelARGB32(x + w, y + h, score_3_image[ h * score_3_width + w]);
    }
  }
}
// Function display score
void displayScore4(int x, int y){
  for(int h = 0; h < score_4_height ; h++){
    for (int w = 0; w < score_4_width; w++){
      drawPixelARGB32(x + w, y + h, score_4_image[ h * score_4_width + w]);
    }
  }
}
// Function display score
void displayScore5(int x, int y){
  for(int h = 0; h < score_5_height ; h++){
    for (int w = 0; w < score_5_width; w++){
      drawPixelARGB32(x + w, y + h, score_5_image[ h * score_5_width + w]);
    }
  }
}
// Function display score
void displayScore6(int x, int y){
  for(int h = 0; h < score_6_height ; h++){
    for (int w = 0; w < score_6_width; w++){
      drawPixelARGB32(x + w, y + h, score_6_image[ h * score_6_width + w]);
    }
  }
}
// Function display score
void displayScore7(int x, int y){
  for(int h = 0; h < score_7_height ; h++){
    for (int w = 0; w < score_7_width; w++){
      drawPixelARGB32(x + w, y + h, score_7_image[ h * score_7_width + w]);
    }
  }
}
// Function display score
void displayScore8(int x, int y){
  for(int h = 0; h < score_8_height ; h++){
    for (int w = 0; w < score_8_width; w++){
      drawPixelARGB32(x + w, y + h, score_8_image[ h * score_8_width + w]);
    }
  }
}
// Function display score
void displayScore9(int x, int y){
  for(int h = 0; h < score_9_height ; h++){
    for (int w = 0; w < score_9_width; w++){
      drawPixelARGB32(x + w, y + h, score_9_image[ h * score_9_width + w]);
    }
  }
}

void displayScore0(int x, int y){
  for(int h = 0; h < score_0_height ; h++){
    for (int w = 0; w < score_0_width; w++){
      drawPixelARGB32(x + w, y + h, score_0_image[ h * score_0_width + w]);
    }
  }
}

void displayExplosion(int x, int y){
  for(int h = 0; h < explosion_height ; h++){
    for (int w = 0; w < explosion_width; w++){
      drawPixelARGB32(x + w, y + h, explosion_image[ h * explosion_width + w]);
      // wait_msec(100000);
      // drawPixelARGB32(x + w, y + h, 0);
    }
  }
}

void clearScore(int num, int x, int y){
  for(int h = 0; h < score_0_height ; h++){
    for (int w = 0; w < score_0_height; w++){
      drawPixelARGB32(x + w, y + h, 0);
    }
  }
}

// Function to move vertical
void moveVerticalScreen(int width, int height , int x , int y) {
  for (int h = 0; h < height; h++){
    for (int w = 0; w < width; w++){
      drawPixel(x + w, y + h, 0);
    }
  }
}

// Function to control tom jerry image (oversize image)
void controlTomJerryImage(int x, int y){
  // Prompt the user how to use
  uart_puts("Press w to scroll up: \n");
  uart_puts("Press s to scroll down: \n");
  uart_puts("Press o to out: \n");
  // Assign value for screen width, height and value for each time scroll up or down
  int screen_width = 1200;
  int screen_height = 800;
  int scroll_vertical_value = 20;
  displayTomJerryImage(x,y);
  while (1) {
    // Declare character variable to get user input
    char character = uart_getc();
    // if character = w, scroll up -> screen up
    if (character == 'w') {
      // Move the screen
      moveVerticalScreen(screen_width,scroll_vertical_value,x,y+screen_height-scroll_vertical_value);
      // Calculate new y coordinate value
      y = y - scroll_vertical_value;
      // Display image
      displayTomJerryImage(x,y);

    } 
    // if character = s, scroll down -> screen down
    else if (character == 's') {
      // Move the screen
      moveVerticalScreen(screen_width,scroll_vertical_value,x,y);
      // Calculate new y coordinate value
      y = y + scroll_vertical_value;
      // Display image
      displayTomJerryImage(x,y);

    }
    // if character = o, out -> stop move screen
    else if (character == 'o') {
      moveVerticalScreen(screen_width,scroll_vertical_value,x,y+screen_height - scroll_vertical_value);
      // Prompt message for user
      uart_puts("\n\nSuccessfully out!\n");
      return;
    }
  }
}