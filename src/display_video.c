#include "tom_jerry_video.h"
#include "framebf.h"

void video_wait_ms(unsigned int n)
{
    register unsigned long f, t, r;

    // Get the current counter frequency
    asm volatile("mrs %0, cntfrq_el0"
                 : "=r"(f));
    // Read the current counter
    asm volatile("mrs %0, cntpct_el0"
                 : "=r"(t));
    // Calculate expire value for counter
    t += ((f / 1000) * n) / 1000;
    do
    {
        asm volatile("mrs %0, cntpct_el0"
                     : "=r"(r));
    } while (r < t);
}

// Function to display tom and jerry video
void displayTomAndJerryVideo(int x, int y)
{
    // Add message to prompt the user how to use
    uart_puts("Video is playing ...\n");
    uart_puts("Press s to stop ");
    char character = uart_get_char();
    // Keep displaying the video until the user press s
    while (character != 's')
    {
        // loop through all the frame image video
        for (int i = 0; i < 15; i ++) {
            // display each frame image
            display_frame_image(tom_jerry_video[i], x, y, tom_jerry_video_width, tom_jerry_video_height+y);
            video_wait_ms(100000);
            }
     
        character = uart_get_char();
    }
    // Add message to announce the user
    uart_puts("\nStopping video ...");
}