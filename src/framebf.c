#include "game.h"
#include "mbox.h"
#include "object.h"
#include "terminal.h"

unsigned int width, height, pitch, isrgb;
unsigned char *fb;

void fb_init()
{
    mBuf[0] = 35 * 4; // Length of message in bytes
    mBuf[1] = MBOX_REQUEST;

    mBuf[2] = MBOX_TAG_SETPHYWH; // Tag identifier
    mBuf[3] = 8;                 // Value size in bytes
    mBuf[4] = 0;
    mBuf[5] = 1024; // Value(width)
    mBuf[6] = 768;  // Value(height)

    mBuf[7] = MBOX_TAG_SETVIRTWH;
    mBuf[8] = 8;
    mBuf[9] = 8;
    mBuf[10] = 1024;
    mBuf[11] = 768;

    mBuf[12] = MBOX_TAG_SETVIRTOFF;
    mBuf[13] = 8;
    mBuf[14] = 8;
    mBuf[15] = 0; // Value(x)
    mBuf[16] = 0; // Value(y)

    mBuf[17] = MBOX_TAG_SETDEPTH;
    mBuf[18] = 4;
    mBuf[19] = 4;
    mBuf[20] = 32; // Bits per pixel

    mBuf[21] = MBOX_TAG_SETPXLORDR;
    mBuf[22] = 4;
    mBuf[23] = 4;
    mBuf[24] = 1; // RGB

    mBuf[25] = MBOX_TAG_GETFB;
    mBuf[26] = 8;
    mBuf[27] = 8;
    mBuf[28] = 4096; // FrameBufferInfo.pointer
    mBuf[29] = 0;    // FrameBufferInfo.size

    mBuf[30] = MBOX_TAG_GETPITCH;
    mBuf[31] = 4;
    mBuf[32] = 4;
    mBuf[33] = 0; // Bytes per line

    mBuf[34] = MBOX_TAG_LAST;

    // Check call is successful and we have a pointer with depth 32
    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP) && mBuf[20] == 32 &&
        mBuf[28] != 0)
    {
        mBuf[28] &= 0x3FFFFFFF; // Convert GPU address to ARM address
        width = mBuf[10];       // Actual physical width
        height = mBuf[11];      // Actual physical height
        pitch = mBuf[33];       // Number of bytes per line
        isrgb = mBuf[24];       // Pixel order
        fb = (unsigned char *)((long)mBuf[28]);
    }
}

void drawPixel(int x, int y, unsigned char attr)
{
    int offs = (y * pitch) + (x * 4);
    *((unsigned int *)(fb + offs)) = vgapal[attr & 0x0f];
}

void drawRect(int x1, int y1, int x2, int y2, unsigned char attr, int fill)
{
    int y = y1;

    while (y <= y2)
    {
        int x = x1;
        while (x <= x2)
        {
            if ((x == x1 || x == x2) || (y == y1 || y == y2))
                drawPixel(x, y, attr);
            else if (fill)
                drawPixel(x, y, (attr & 0xf0) >> 4);
            x++;
        }
        y++;
    }
}
// Draw an object like ship , alien with pixel data in object.h
void drawEntity(Entity entity)
{
    int *colorptr;
    int width = entity.dimension.width;
    int height = entity.dimension.height;

    int x = entity.position.x;
    int oldX = x;
    int y = entity.position.y;
    if (entity.type == PAWN)
        colorptr = (int *)pawn_sprite.image_pixels;
    // else if (entity.type == KNIGHT)
    //     colorptr = (int *)knight_sprite.image_pixels;
    else if (entity.type == QUEEN)
        colorptr = (int *)queen_sprite.image_pixels;
    else if (entity.type == PLAYER)
        colorptr = (int *)blue_ship_sprite.image_pixels;
    else if (entity.type == BUNKER)
    {
        if (entity.health.current_health <= BUNKER_HEALTH / 3)
        {
            colorptr = (int *)bunker_3.image_pixels;
            width = bunker_3.width;
            height = bunker_3.height;
        }
        else if (entity.health.current_health <= BUNKER_HEALTH / 2)
        {
            colorptr = (int *)bunker_2.image_pixels;
            width = bunker_2.width;
            height = bunker_2.height;
        }
        else
        {
            colorptr = (int *)bunker_1.image_pixels;
            width = bunker_1.width;
            height = bunker_1.height;
        }
    }

    // draw in 2D
    for (int i = 0; i < (width * height); i++)
    {
        x++;
        if (i % width == 0)
        {
            y++;
            x = oldX;
        }
        drawPixel(x, y, colorptr[i]);
    }
}
void clear_projectile(Position position, Dimension dimension)
{
    int width = dimension.width;
    int height = dimension.height;

    int x = position.x;
    int oldX = x;
    int y = position.y;

    for (int i = 0; i < (width * height); i++)
    {
        x++;
        if (i % width == 0)
        {
            y++;
            x = oldX;
        }
        drawPixel(x, y, 0);
    }
}
void draw_projectile(Type type, Position position, Dimension dimension)
{
    int *colorptr;
    int width = dimension.width;
    int height = dimension.height;

    // if (type != PLAYER)
    colorptr = (int *)green_laser.image_pixels;
    // else
    //     colorptr = (int *)red_laser.image_pixels;

    int x = position.x;
    int oldX = x;
    int y = position.y;
    for (int i = 0; i < (width * height); i++)
    {
        x++;
        if (i % width == 0)
        {
            y++;
            x = oldX;
        }
        drawPixel(x, y, colorptr[i]);
    }
}

void drawBar(int health, int x, int y)
{
    int *colorptr;
    int width = 40;
    int height = 40;

    int oldX = x;
    colorptr = (int *)blue_ship_sprite.image_pixels;
    for (int i = 0; i < (width * height); i++)
    {
        x++;
        if (i % width == 0)
        {
            y++;
            x = oldX;
        }
        drawPixel(x, y, colorptr[i]);
    }
}

void drawScore(int num, int x, int y)
{
    printf("num is %d \n", num);

    int *colorptr;
    int width = 32;
    int height = 51;
    if (num == 1)
        colorptr = (int *)ONE_IMAGE.image_pixels;
    else if (num == 2)
        colorptr = (int *)TWO_IMAGE.image_pixels;
    else if (num == 3)
        colorptr = (int *)THREE_IMAGE.image_pixels;
    else if (num == 4)
        colorptr = (int *)FOUR_IMAGE.image_pixels;
    else if (num == 5)
        colorptr = (int *)FIVE_IMAGE.image_pixels;
    else if (num == 6)
        colorptr = (int *)SIX_IMAGE.image_pixels;
    else if (num == 7)
        colorptr = (int *)SEVEN_IMAGE.image_pixels;
    else if (num == 8)
        colorptr = (int *)EIGHT_IMAGE.image_pixels;
    else if (num == 9)
    {
        colorptr = (int *)NINE_IMAGE.image_pixels;
    }

    else if (num == 0)
    {
        colorptr = (int *)ZERO_IMAGE.image_pixels;
    }

    int oldX = x;

    for (int i = 0; i < (width * height); i++)
    {
        x++;
        if (i % width == 0)
        {
            y++;
            x = oldX;
        }
        drawPixel(x, y, colorptr[i]);
    }
}

void drawLine(int x1, int y1, int x2, int y2, unsigned char attr)
{
    int dx, dy, p, x, y;

    dx = x2 - x1;
    dy = y2 - y1;
    x = x1;
    y = y1;
    p = 2 * dy - dx;

    while (x < x2)
    {
        if (p >= 0)
        {
            drawPixel(x, y, attr);
            y++;
            p = p + 2 * dy - 2 * dx;
        }
        else
        {
            drawPixel(x, y, attr);
            p = p + 2 * dy;
        }
        x++;
    }
}

void drawCircle(int x0, int y0, int radius, unsigned char attr, int fill)
{
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y)
    {
        if (fill)
        {
            drawLine(x0 - y, y0 + x, x0 + y, y0 + x, (attr & 0xf0) >> 4);
            drawLine(x0 - x, y0 + y, x0 + x, y0 + y, (attr & 0xf0) >> 4);
            drawLine(x0 - x, y0 - y, x0 + x, y0 - y, (attr & 0xf0) >> 4);
            drawLine(x0 - y, y0 - x, x0 + y, y0 - x, (attr & 0xf0) >> 4);
        }
        drawPixel(x0 - y, y0 + x, attr);
        drawPixel(x0 + y, y0 + x, attr);
        drawPixel(x0 - x, y0 + y, attr);
        drawPixel(x0 + x, y0 + y, attr);
        drawPixel(x0 - x, y0 - y, attr);
        drawPixel(x0 + x, y0 - y, attr);
        drawPixel(x0 - y, y0 - x, attr);
        drawPixel(x0 + y, y0 - x, attr);

        if (err <= 0)
        {
            y += 1;
            err += 2 * y + 1;
        }

        if (err > 0)
        {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}

void drawChar(unsigned char ch, int x, int y, unsigned char attr)
{
    unsigned char *glyph =
        (unsigned char *)&font + (ch < FONT_NUMGLYPHS ? ch : 0) * FONT_BPG;

    for (int i = 0; i < FONT_HEIGHT; i++)
    {
        for (int j = 0; j < FONT_WIDTH; j++)
        {
            unsigned char mask = 1 << j;
            unsigned char col =
                (*glyph & mask) ? attr & 0x0f : (attr & 0xf0) >> 4;

            drawPixel(x + j, y + i, col);
        }
        glyph += FONT_BPL;
    }
}

void drawString(int x, int y, char *s, unsigned char attr)
{
    while (*s)
    {
        if (*s == '\r')
        {
            x = 0;
        }
        else if (*s == '\n')
        {
            x = 0;
            y += FONT_HEIGHT;
        }
        else
        {
            drawChar(*s, x, y, attr);
            x += FONT_WIDTH;
        }
        s++;
    }
}

// Draw list of frame images in video
void display_frame_image(unsigned int frame_image[], int x, int y, int width,
                         int height)
{
    int num = 0;

    while (y < height)
    {
        for (x = 0; x < width; x++)
        {
            int offs = (y * pitch) + (x * 4);
            *((unsigned int *)(fb + offs)) = frame_image[num];
            num++;
        }
        y++;
        x = 0;
    }
}
