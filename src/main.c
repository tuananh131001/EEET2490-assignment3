// -----------------------------------main.c
// -------------------------------------

#include "framebf.h"
#include "game.h"
#include "mbox.h"
#include "uart.h"
void displayMenu();
void main() {
    // declaring variables
    int count = 0;
    char str[40];  // char array to store user inputs
    int x_coordinate = 50;
    int y_coordinate = 200;
    Game game;
    uart_init();
    fb_init();
    displayMenu();
    uart_puts("\n");

    // Taking input commands
    while (1) {
        // read each char
        uart_puts("\n\n");

        uart_puts("YplOS>>> ");

        char command = uart_getc();

        uart_sendc(command);
        uart_sendc('\n');

        if (command == '1') {
            drawRect(150, 150, 400, 400, 0x03, 0);
            drawRect(300, 300, 350, 350, 0x2e, 1);

            drawCircle(960, 540, 250, 0x0e, 0);
            drawCircle(960, 540, 50, 0x13, 1);
            drawString(100, 100, "Nguyen Tuan Anh - s3864077", 0x0f);
            drawString(100, 120, "Vo Quoc Huy - s3823236", 0x0f);

        } else if (command == '2') {
            displayTomImage(x_coordinate, y_coordinate);
        } else if (command == '3') {
            controlTomJerryImage(x_coordinate, y_coordinate);
        } else if (command == '4') {
            displayTomAndJerryVideo(x_coordinate, y_coordinate);
        } else if (command == '5') {
            // render(&game.world);
            // move_player();

            init_game(&game);

            // render(&game.world);
            move_player(&game.world);
        } else if (command == '0') {
            clear_emulator_screen(1920, 1080);
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
