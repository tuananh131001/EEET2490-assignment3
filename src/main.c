// -----------------------------------main.c
// -------------------------------------

#include "display_image.h"
#include "display_video.h"
#include "framebf.h"
#include "game.h"
#include "helper.h"
#include "mbox.h"
#include "uart.h"
void displayMenu();
void main() {
    // declaring variables
    int count = 0;
    char str[40];  // char array to store user inputs
    int x_coordinate = 400;
    int y_coordinate = 300;
    Game game;
    uart_init();
    fb_init();

    // Taking input commands
    while (1) {
        displayMenu();
        uart_puts("\n");
        // read each char
        uart_puts("\n\n");

        uart_puts("YplOS>>> ");

        char command = uart_getc();

        uart_sendc(command);
        uart_sendc('\n');

        if (command == '1') {
            clear_emulator_screen(1920, 1080);
            // drawRect(150, 150, 400, 400, 0x03, 0);
            // drawRect(300, 300, 350, 350, 0x2e, 1);

            // drawCircle(960, 540, 250, 0x0e, 0);
            // drawCircle(960, 540, 50, 0x13, 1);
            displayBackgroundText(0, 0);
            drawString(150, 200, "NGUYEN TUAN ANH", 0xa);
            drawString(290, 250, "S3864077", 0x3);

            drawString(230, 350, "VO QUOC HUY", 0x2);
            drawString(290, 400, "S3823236", 0xF);

            
            // drawChar('A', 1920 / 2 - 200, 100, 0x0e);
            // drawChar('B', 1920 / 2 - 200, 200, 0x0e);

        } else if (command == '2') {
            clear_emulator_screen(1024, 768);
            displayTomImage(x_coordinate, y_coordinate);
        } else if (command == '3') {
            clear_emulator_screen(1024, 768);
            controlTomJerryImage(20, y_coordinate);
        } else if (command == '4') {
            clear_emulator_screen(1024, 768);
            displayTomAndJerryVideo(x_coordinate, y_coordinate);
        } else if (command == '5') {
            // render(&game.world);
            // move_player();
            clear_emulator_screen(1024, 768);
            init_game(&game);
            show_main_menu(&game);
            if (game.game_start) {
                while (!quitGame) {
                    restart_game(&game);
                    move_player(&game.world);
                }
            }
            // render(&game.world);

        } else if (command == '0') {
            clear_emulator_screen(1024, 768);
        } else {
            uart_puts("Invalid command. Please try again");
        }
    }
}

void displayMenu() {
    uart_puts(
        "\n\n\tEnter a number to choose command:\n"
        "\t1.\tDisplay text on screen\n"
        "\t2.\tDisplay a small image\n"
        "\t3.\tDisplay a scrollable large image\n"
        "\t4.\tDisplay a video\n"
        "\t5.\tPlay game\n"
        "\t0.\tClear the screen\n");
}
