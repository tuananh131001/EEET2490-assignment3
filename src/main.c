// -----------------------------------main.c
// -------------------------------------
#include "display_image.h"
#include "display_video.h"
#include "framebf.h"
#include "game.h"
#include "mbox.h"
#include "uart.h"
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
int strlen(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }
    return i;
}
void get_command() {
    static int CHAR_LIMIT = 40;
    char temp_str[CHAR_LIMIT];  // temp string to store the entire command
    get_input(temp_str);
    clear_temp_str(temp_str);
    int cmd = -1;
    static int TOTAL_CMD = 1;
    static char cmds[][20] = {"testfont"};
    // compare input
    for (int i = 0; i < TOTAL_CMD; i++) {
        // set_color reuse only take the first argument
        if (str_check(temp_str, cmds[i])) {
            cmd = i;
            break;
        }
    }
    // check arguments counts
    switch (cmd) {
        case 0:
            drawString(100, 100, "Nguyen Tuan Anh - s3864077", 0x0f);
            drawString(100, 120, "Vo Quoc Huy - s3823236", 0x0f);
            break;
        default:
            // if cmd == -1 => not found
            uart_puts("Invalid command. Please enter again!\n");
            break;
    }
    // reset string arrays
    for (int k = 0; k < 40; k++) {
        // Reset string input
        temp_str[k] = '\0';
    }
}
void get_input(char *temp_str) {
    int total_char = 0;  // total number of char that was input
    static char cmds[][20] = {"testfont"};
    int index_cmd[10];
    int index_count = 0;
    int index_to_print = 0;

    uart_puts("\nYplOS> ");
    while (1) {
        char c = uart_getc();
        // arrows detection in input

        // if (c == 0 || c == 0xE0) c = uart_getc();
        // if user presses ENTER, break the loop and execute a command
        if (c == 10) {
            uart_puts("\n");
            break;

        } else if (c == 9) {
            /*
            1. Get the input from user and store it in temp_str
            2. Compare the input with the commands and store the index of the
            command in index_cmd
            3. Clear the temp_str
            4. Print the rest of the command
            5. Change to next command when user press tab */
            for (int i = 0; i <= 10; i++) {
                if (compare_str(cmds[i], temp_str)) {
                    index_cmd[index_count] = i;
                    index_count++;
                }
            }
            // Clear temp str
            while (total_char != 0) {
                uart_sendc(8);
                uart_sendc(32);
                uart_sendc(8);
                total_char--;
            }
            clear_temp_str(temp_str);
            // print the rest command
            for (int j = 0; j < strlen(cmds[index_cmd[index_to_print]]); j++) {
                uart_sendc(cmds[index_cmd[index_to_print]][j]);
                temp_str[j] = cmds[index_cmd[index_to_print]][j];
                total_char++;
            }

            // change to next command when user press tab
            if (index_to_print < 9) {
                index_to_print++;
            } else {
                index_to_print = 0;
            }

        }
        // add each character into the string
        else if (c != 8) {
            uart_sendc(c);
            temp_str[total_char] = c;
            total_char++;
        }

        // delete each character when user press BackSpace each time
        // Use Ctrl + H on MacOS
        else if ((c == 127 || c == 8 || c == 51) && total_char > 0) {
            uart_sendc(c);
            uart_sendc(32);
            uart_sendc(8);
            total_char--;
            temp_str[total_char] = 0;
        }
    }
    uart_puts("Execute command: ");
    uart_puts(temp_str);
    uart_puts("\n");
}
/*
 * return 1: identical
 * return 0: different
 */
int str_check(char *str1, char *str2) {
    // compare length
    if (strlen(str1) != strlen(str2)) return 0;

    char *p1 = str1;  // pointer pointing to first character in str1
    char *p2 = str2;  // pointer pointing to first character in str2

    while (*p1 != '\0' && *p2 != '\0') {
        if (*p1 != *p2) return 0;

        p1++;
        p2++;
    }
    return 1;
}
// function ulity
// reference: stackoverflow
void clear_temp_str(char *s) {
    while (*s != '\0') {
        *(s++) = '\0';
    }
}
// find substring in string in sequence then return rest of the string
int compare_str(char *str, char *substr) {
    int i = 0;
    int j = 0;
    while (substr[i] != '\0') {
        if (str[i] == substr[j]) {
            i++;
            j++;
        } else {
            return 0;
        }
    }

    return 1;
}