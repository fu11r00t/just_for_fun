#include <cmath>
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#endif
#include <stdlib.h>
#include <string>

using namespace std;

const string GRAY = " .\'`^\",:;Il!i><~+?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao#MW&8%B@$";
const int W = 15;
const float CONTRAST = 0.2;
const float BACKLIGHT = 0; // 1.00 - CONTRAST
float x = 0, y = 0;
float t = 0.0;

#ifdef _WIN32
int buffersize(int *cols, int *rows)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    *cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    *rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return 0;
}
#else
int buffersize(int *cols, int *rows)
{
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    *rows = size.ws_row;
    *cols = size.ws_col;
    return 0;
}
#endif

int main(void)
{
    int cols, rows;
    buffersize(&cols, &rows);
    const int COLS = cols;
    const int ROWS = rows;
    char frame[ROWS][COLS];
    float dx = COLS * 0.1 / 30;
    float dy = ROWS * 0.3 / 30;
    float F = COLS * 0.1 / 30; // assuming 30 FPS
    while (true)
    {
        t -= F;
        x += dx;
        y += dy;
        if (x < 0)
        {
            x = 0;
            dx = -dx;
        }
        else if (x >= COLS)
        {
            x = COLS - dx;
            dx = -dx;
        }
        if (y < 0)
        {
            y = 0;
            dy = -dy;
        }
        else if (y >= ROWS)
        {
            y = ROWS - dy;
            dy = -dy;
        }

        for (int row = 0; row < ROWS; row++)
        {
            for (int col = 0; col < COLS; col++)
            {
                float a = row - y;
                float b = col - x;
                float distance = sqrtf(a * a + (b / 2) * (b / 2));
                float angle = 2 * M_PI * (distance / (COLS - 1)) * W * 2;
                float scale = (cosf(angle + t) + 1) / 2;
                scale = scale * CONTRAST + BACKLIGHT;
                scale = max(scale, 0.0f);
                scale = min(scale, 1.0f);
                frame[row][col] = GRAY[(int)((GRAY.length() - 1) * scale)];
            }
        }
        for (int row = 0; row < ROWS; row++)
        {
            for (int col = 0; col < COLS; col++)
            {
                cout << frame[row][col];
            }
            cout << endl;
        }
#ifdef _WIN32
        Sleep(33);
#else
        usleep(33000);
#endif
    }
}