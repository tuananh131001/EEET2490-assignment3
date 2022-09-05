
void get_command() {
    uart_puts("\n\n");

    // declaring variables
    int count = 0;
    char str[40];  // char array to store user inputs

    // Taking input commands
    uart_puts("Ypl OS >>> ");
    while (1) {
        // read each char
        char c = uart_getc();

        // if user presses ENTER, break the loop and execute a command
        if (c == 10) {
            uart_puts("\n");
            break;
        }

        // add each character into the string
        else if (c != 8) {
            uart_sendc(c);
            str[count] = c;
            count++;
        }

        // delete each character when user press BackSpace each time
        else if (c == 8 && count > 0) {
            uart_sendc(c);
            uart_sendc(32);
            uart_sendc(8);
            count--;
            str[count] = 0;  // make the element at that index in the string to
                             // be whitespace
        }
    }

    uart_puts("Execute command: ");
    uart_puts(str);
    uart_puts("\n");

    // uart_puts(split);

    // ----- COMMANDS -----//
    if (str[0] == '\0') {
        uart_puts("");
    }

    // command 1: help <command>
    else if (compare_str(str, "printfont") == 0) {
        drawString(100, 100, "Nguyen Tuan Anh - s3864077", 0x0f);
        drawString(100, 120, "Vo Quoc Huy - s3823236", 0x0f);
    }

    // Invalid command
    else {
        uart_puts("'");
        uart_puts(str);
        uart_puts("'");
        uart_puts(" is an invalid command. Please check again. \n");
    }

    // reset string arrays
    for (int k = 0; k < 40; k++) {
        // Reset string input
        str[k] = '\0';
    }
}

/**
 * Comparing 2 strings.
 */
int compare_str(char *a, char *b) {
    int flag = 0;
    while (*a != '\0' && *b != '\0')  // while loop
    {
        for (int i = 0; i < sizeof(a); i++) {
            if (*a != *b) {
                flag = 1;
            }
            a++;
            b++;
        }
    }
    if (flag == 0) {
        return 0;
    } else {
        return 1;
    }
}
