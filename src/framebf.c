#include "display_image.h"
#include "mbox.h"
#include "object.h"
#include "terminal.h"

unsigned int width, height, pitch, isrgb;
unsigned char *fb;
// Use RGBA32 (32 bits for each pixel)
#define COLOR_DEPTH 32

// Pixel Order: BGR in memory order (little endian --> RGB in byte order)
#define PIXEL_ORDER 0

void fb_init() {
    mBuf[0] = 35 * 4;  // Length of message in bytes
    mBuf[1] = MBOX_REQUEST;

    mBuf[2] = MBOX_TAG_SETPHYWH;  // Tag identifier
    mBuf[3] = 8;                  // Value size in bytes
    mBuf[4] = 0;
    mBuf[5] = 1024;  // Value(width)
    mBuf[6] = 768;   // Value(height)

    mBuf[7] = MBOX_TAG_SETVIRTWH;
    mBuf[8] = 8;
    mBuf[9] = 8;
    mBuf[10] = 1024;
    mBuf[11] = 768;

    mBuf[12] = MBOX_TAG_SETVIRTOFF;
    mBuf[13] = 8;
    mBuf[14] = 8;
    mBuf[15] = 0;  // Value(x)
    mBuf[16] = 0;  // Value(y)

    mBuf[17] = MBOX_TAG_SETDEPTH;
    mBuf[18] = 4;
    mBuf[19] = 4;
    mBuf[20] = COLOR_DEPTH;  // Bits per pixel

    mBuf[21] = MBOX_TAG_SETPXLORDR;
    mBuf[22] = 4;
    mBuf[23] = 4;
    mBuf[24] = PIXEL_ORDER;  // RGB

    mBuf[25] = MBOX_TAG_GETFB;
    mBuf[26] = 8;
    mBuf[27] = 8;
    mBuf[28] = 4096;  // FrameBufferInfo.pointer
    mBuf[29] = 0;     // FrameBufferInfo.size

    mBuf[30] = MBOX_TAG_GETPITCH;
    mBuf[31] = 4;
    mBuf[32] = 4;
    mBuf[33] = 0;  // Bytes per line

    mBuf[34] = MBOX_TAG_LAST;

    // Check call is successful and we have a pointer with depth 32
    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP) && mBuf[20] == 32 &&
        mBuf[28] != 0) {
        mBuf[28] &= 0x3FFFFFFF;  // Convert GPU address to ARM address
        width = mBuf[10];        // Actual physical width
        height = mBuf[11];       // Actual physical height
        pitch = mBuf[33];        // Number of bytes per line
        isrgb = mBuf[24];        // Pixel order
        fb = (unsigned char *)((long)mBuf[28]);
    }
}

void drawPixel(int x, int y, unsigned char attr) {
    int offs = (y * pitch) + (x * 4);
    *((unsigned int *)(fb + offs)) = vgapal[attr & 0x0f];
}

void drawPixelARGB32(int x, int y, unsigned int attr) {
    int offs = (y * pitch) + (COLOR_DEPTH / 8 * x);

    /* //Access and assign each byte
     *(fb + offs    ) = (attr >> 0 ) & 0xFF; //BLUE
     *(fb + offs + 1) = (attr >> 8 ) & 0xFF; //GREEN
     *(fb + offs + 2) = (attr >> 16) & 0xFF; //RED
     *(fb + offs + 3) = (attr >> 24) & 0xFF; //ALPHA
     */

    // Access 32-bit together
    *((unsigned int *)(fb + offs)) = attr;
}

void drawRect(int x1, int y1, int x2, int y2, unsigned char attr, int fill) {
    int y = y1;

    while (y <= y2) {
        int x = x1;
        while (x <= x2) {
            if ((x == x1 || x == x2) || (y == y1 || y == y2))
                drawPixel(x, y, attr);
            else if (fill)
                drawPixel(x, y, (attr & 0xf0) >> 4);
            x++;
        }
        y++;
    }
}
void drawChar(unsigned char ch, int x, int y, unsigned char attr) {
    // add the index of a-z or number
    int index = (ch >= 'A' && ch <= 'Z') ? ch - 55 : ch - 48;
    // add index of the space
    index = (ch == ' ') ? 37 : index;
    for (int i = 0; i < FONT_HEIGHT; i++) {
        for (int j = 0; j < FONT_WIDTH; j++) {
            // get the pixel of the character
            unsigned char *glyph =
                (unsigned char *)&epd_bitmap_allArray[index]
                                                     [i * FONT_WIDTH + j];
            // if the pixel is white then draw the pixel with the color 
            // otherwise skip it
            unsigned long mask = 0xffff;
            unsigned char col = (*glyph & mask) ? (attr & 0x0f) : 1;
            if (col == 1) {
                continue;
            }
            drawPixel(x + j, y + i, col);
        }
    }
}

void drawString(int x, int y, char *s, unsigned char attr) {
    while (*s) {
        if (*s == '\r') {
            x = 0;
        } else if (*s == '\n') {
            x = 0;
            y += FONT_HEIGHT;
        } else {
            drawChar(*s, x, y, attr);
            x += FONT_WIDTH;
        }
        s++;
    }
}
// Draw list of frame images in video
void display_frame_image(unsigned int frame_image[], int x, int y, int width,
                         int height) {
    int num = 0;

    while (y < height) {
        for (x = 0; x < width; x++) {
            int offs = (y * pitch) + (x * 4);
            *((unsigned int *)(fb + offs)) = frame_image[num];
            num++;
        }
        y++;
        x = 0;
    }
}