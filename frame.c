#include "frame.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h>
#include <math.h>

int nanosleep(const struct timespec *req, struct timespec *rem);

struct winsize w;
struct timespec ts;

#define NUM_ROWS w.ws_row
#define NUM_COLS w.ws_col
#define TERMINAL_FILE 0

char **frame;

// Clears the Screen so you can't see anything
void refresh()
{
    printf("\033[H\033[J");
    // printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    fflush(stdout);
}

// Sets Frame values to '-' or ' '
void clearFrame()
{
    for (int i = 0; i < NUM_COLS; i++)
    {
        for (int j = 0; j < NUM_ROWS; j++)
        {
            frame[i][j] = ' '; // Debugging value '-' else ' ' is more clean
        }
    }
    refresh();
}

// Mallocs all the neccesarry things for frame
bool frameSetup()
{
    if (ioctl(TERMINAL_FILE, TIOCGWINSZ, &w) == -1)
    {
        printf("Couldn't find terminal size");
        return 1;
    };
    frame = (char **) malloc(NUM_COLS * sizeof(char *));
    if (frame == NULL)
    {
        printf("Failed mallocing 'malloc(NUM_ROWS * sizeof(char *)' ");
    }
    for (int i = 0; i < NUM_COLS; i++)
    {
        frame[i] = malloc(NUM_ROWS * sizeof(char));
        if (frame[i] == NULL)
        {
            printf("Failed mallocing");
            return false;
        }
    }
    clearFrame();
    return true;
}

// Prints the Frame out and waits
void printFrame()
{
    ts.tv_sec = 1; // Delay
    ts.tv_nsec = 1000000000L / 24; // Delay
    for (int j = NUM_ROWS - 1; j >= 0; j--)
    {
        for (int i = 0; i < NUM_COLS; i++)
        {
            putchar(frame[i][j]);
        }
        fflush(stdout);
    }
    nanosleep(&ts, NULL);
}

// Draws the Line
void plotLineHigh(int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;
    if (dx < 0)
    {
        xi = -1;
        dx = -dx;
    }
    int D = (2 * dx) - dy;
    int x = x0;

    for (int y = y0; y <= y1; y++)
    {
        frame[x][y] = '*';
        if (D > 0)
        {
            x = x + xi;
            D = D + (2 * (dx - dy));
        }
        else
        {
            D = D + 2 * dx;
        }
    }
}

// Draws the Line
void plotLineLow(int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int yi = 1;
    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
    }
    int D = (2 * dy) - dx;
    int y = y0;

    for (int x = x0; x <= x1; x++)
    {
        frame[x][y] = '*';
        if (D > 0)
        {
            y += + yi;
            D += + (2 * (dy - dx));
        }
        else
        {
            D = D + 2 * dy;
        }
    }
}

// Draws the Line
void DrawLine(int x0, int y0, int x1, int y1)
{
    if (abs(y1 - y0) < abs(x1 - x0))
    {
        if (x0 > x1)
        {
            plotLineLow(x1, y1, x0, y0);
        }
        else
        {
            plotLineLow(x0, y0, x1, y1);
        }
    }
    else
    {
        if (y0 > y1)
        {
            plotLineHigh(x1, y1, x0, y0);
        }
        else
        {
            plotLineHigh(x0, y0, x1, y1);
        }
    }
}

void freeFrame()
{
    for (int i = 0; i < NUM_COLS; i++)
    {
        free(frame[i]);
    }
    free(frame);
}
