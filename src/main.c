// -----------------------------------main.c
// -------------------------------------

#include "display_image.h"
#include "display_video.h"
#include "framebf.h"
#include "game.h"
#include "mbox.h"
#include "uart.h"
#include "select.h"
void main() {
    uart_init();
    fb_init();
    int x_coordinate = 50;
    int y_coordinate = 200;
    // drawRect(150, 150, 400, 400, 0x03, 0);
    // drawRect(300, 300, 350, 350, 0x2e, 1);

    // drawCircle(960, 540, 250, 0x0e, 0);
    // drawCircle(960, 540, 50, 0x13, 1);

    // drawPixel(250, 250, 0x0e);

    // controlTomJerryImage(x_coordinate,y_coordinate);

    // echo everything back
    while (1) {
        // read each char
        get_command();
    }
    Game game;

    init_game(&game);

    // render(&game.world);
    move_player(&game.world);

    // displayTomAndJerryVideo(x_coordinate,y_coordinate);

    // while (1){
    // }
}

// #include "uart.h"
// #include "mbox.h"
// #include "framebf.h"

// void main()
// {
//     // set up serial console
//  uart_init();

//  // say hello
//  uart_puts("\n\nHello World!\n");

//  // Initialize frame buffer
//  framebf_init();

//  // Draw something on the screen
// //  drawRectARGB32(100,100,400,400,0x00AABB00,1); //RED
// //  drawRectARGB32(150,150,400,400,0x0000BB00,1); //GREEN
// //  drawRectARGB32(200,200,400,400,0x000000CC,1); //BLUE
// //  drawRectARGB32(250,250,400,400,0x00FFFF00,1); //YELLOW
// //  drawPixelARGB32(300, 300, 0x00FF0000); //RED
//  drawLineARGB32(0, 300, 0x00FF0000, 1); //RED
//  // draw circle yellow
//     drawCircleARGB32(300, 300, 100, 0x00FFFF00, 0); //YELLOW
// //  drawCircleARGB32(300, 300, 100, 0x00AABB00,0);

//  // echo everything back
//  while(1) {
//   //read each char
//   char c = uart_getc();

//   //send back twice
//   uart_sendc(c);
//   uart_sendc(c);
//  }
// }
