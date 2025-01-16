#include <windows.h>
#include <deque>
#include <cstdlib>
#include <ctime>

const int WIDTH = 40;
const int HEIGHT = 30;
const int CELL_SIZE = 20;

struct Point {
    int x, y;
    };

std::deque<Point> snake;
Point apple;
int direction; // 0: вверх, 1: вправо, 2: вниз, 3: влево
bool gameOver;
HBRUSH snakeBrush, appleBrush, borderBrush;

void Setup() {
    gameOver = false;
    direction = rand() % 4; // Начальное направление рандомно
    snake.push_back({ WIDTH / 2, HEIGHT / 2 }); // Изначальная позиция змейки в центре
    apple = { rand() % WIDTH, rand() % HEIGHT }; // Случайная позиция яблока
    }

void Draw(HDC hdc) {
    // Очистка экрана
    RECT rect = { 0, 0, WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE };
    FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
    // Отрисовка границ
    RECT borderRect = { 0, 0, WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE };
    FrameRect(hdc, &borderRect, borderBrush);
    // Отрисовка змеи
    for (const auto& segment : snake) {
        RECT cell = { segment.x * CELL_SIZE, segment.y * CELL_SIZE,
                     (segment.x + 1) * CELL_SIZE, (segment.y + 1) * CELL_SIZE };
        FillRect(hdc, &cell, snakeBrush);
        FrameRect(hdc, &cell, (HBRUSH)GetStockObject(BLACK_BRUSH)); // Тень для змейки
        }
    // Отрисовка яблока
    RECT appleRect = { apple.x * CELL_SIZE, apple.y * CELL_SIZE,
                     (apple.x + 1) * CELL_SIZE, (apple.y + 1) * CELL_SIZE };
    FillRect(hdc, &appleRect, appleBrush);
    FrameRect(hdc, &appleRect, (HBRUSH)GetStockObject(BLACK_BRUSH)); // Тень для еды
    }

void Logic() {
    Point newHead = snake.front();
    switch (direction) {
            case 0: newHead.y--; break; // Вверх
            case 1: newHead.x++; break; // Вправо
            case 2: newHead.y++; break; // Вниз
            case 3: newHead.x--; break; // Влево
        }
    // Проверка ДТП
    if (newHead.x < 0 || newHead.x >= WIDTH || newHead.y < 0 || newHead.y >= HEIGHT) {
        gameOver = true;
        return;
        }
    for (const auto& segment : snake) {
        if (segment.x == newHead.x && segment.y == newHead.y) {
            gameOver = true;
            return;
            }
        }
    // Добавление сегмента с головы
    snake.push_front(newHead);
    // Проверим, съела ли змея яблоко
    if (newHead.x == apple.x && newHead.y == apple.y) {
        apple = { rand() % WIDTH, rand() % HEIGHT };
        }
    else {
        snake.pop_back(); // Удаление хвоста, если яблоко не съедено
        }
    }

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
            case WM_KEYDOWN:
                switch (wParam) {
                        case VK_UP: if (direction != 2) direction = 0; break; // Вверх
                        case VK_RIGHT: if (direction != 3) direction = 1; break; // Вправо
                        case VK_DOWN: if (direction != 0) direction = 2; break; // Вниз
                        case VK_LEFT: if (direction != 1) direction = 3; break; // Влево
                        case 'X': gameOver = true; break; // Выход
                    }
                break;

            case WM_PAINT: {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                Draw(hdc);
                EndPaint(hwnd, &ps);
                break;
                }

            case WM_DESTROY:
                PostQuitMessage(0);
                break;

            default:
                return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    return 0;
    }

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    srand(static_cast<unsigned int>(time(0))); // Инициализация генератора случайных чисел
    Setup();
    snakeBrush = CreateSolidBrush(RGB(0, 255, 0)); // Зеленый цвет для змеи
    appleBrush = CreateSolidBrush(RGB(255, 0, 0)); // Красный цвет для яблока
    borderBrush = CreateSolidBrush(RGB(255, 255, 255)); // Белый цвет для границ фрейма
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "Snake";
    RegisterClass(&wc);
    // Рассчет размеров окна с учетом рамки и заголовка
    RECT rect = { 0, 0, WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE); // Учитываем рамку и заголовок
    // Создание окна с увеличенными размерами
    HWND hwnd = CreateWindow("Snake", "Snake", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    MSG msg = { 0 };
    while (!gameOver) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            }
        else {
            Logic();
            InvalidateRect(hwnd, NULL, TRUE); // Перерисовка окна
            Sleep(150); // Задержка для контроля скорости
            }
        }
    MessageBox(hwnd, "Game Over!", "Snake", MB_OK);
    DestroyWindow(hwnd);
    DeleteObject(snakeBrush);
    DeleteObject(appleBrush);
    DeleteObject(borderBrush);
    return 0;
    }