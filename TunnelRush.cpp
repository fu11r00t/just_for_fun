#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <array>

const int MIN_GAP_SIZE = 10; // Минимальный размер проема
const int MAX_GAP_SIZE = 20; // Максимальный размер проема
const int MIN_TUNNEL_HEIGHT = 5; // Минимальная высота туннеля
const int MAX_TUNNEL_HEIGHT = 15; // Максимальная высота туннеля

int WIDTH, HEIGHT; // Ширина и высота экрана

void generateTunnelSegment(std::array<bool, 80>& segment, int gapStart, int gapSize) {
    for (int i = 0; i < WIDTH; ++i) {
        segment[i] = true; // Заполняем сегмент стенами
    }
    
    int gapEnd = gapStart + gapSize; // Конец проема

    for (int i = gapStart; i < gapEnd; ++i) {
        segment[i] = false; // Создаем проем в сегменте
    }
}

void drawTunnel(const std::deque<std::array<bool, 80>>& tunnel, int shipX, int shipY) {
    clear(); // Очищаем экран
    for (int i = 0; i < tunnel.size(); ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (tunnel[i][j]) {
                mvaddch(i, j, '#'); // Рисуем стену туннеля
            }
        }
    }
    mvaddch(shipY, shipX, '^'); // Рисуем корабль
    refresh(); // Обновляем экран
}

bool checkCollision(const std::deque<std::array<bool, 80>>& tunnel, int shipX, int shipY) {
    if (shipY < 0 || shipY >= tunnel.size() || shipX < 0 || shipX >= WIDTH) {
        return true; // Проверяем, что корабль не вышел за пределы туннеля
    }
    return tunnel[shipY][shipX]; // Проверяем столкновение корабля с туннелем
}

bool isShipInsideTunnel(int shipY, const std::deque<std::array<bool, 80>>& tunnel) {
    // Проверяем, что корабль находится внутри видимой области туннеля
    return shipY < tunnel.size();
}

void pauseGame() {
    mvprintw(HEIGHT / 2, WIDTH / 2 - 5, "PAUSED"); // Выводим сообщение "PAUSED"
    refresh(); // Обновляем экран
    while (true) {
        int ch = getch();
        if (ch == 'p' || ch == 'P') { // Если пользователь нажал 'p' или 'P'
            break; // Выходим из цикла паузы
        }
    }
    clear(); // Очищаем экран после выхода из паузы
}

int main() {
    std::deque<std::array<bool, 80>> tunnel; // Туннель
    std::array<bool, 80> segment; // Сегмент туннеля
    srand(time(nullptr)); // Инициализация генератора случайных чисел
    initscr(); // Инициализация ncurses
    getmaxyx(stdscr, HEIGHT, WIDTH); // Получение размеров экрана
    cbreak(); // Отключение буферизации ввода
    noecho(); // Отключение отображения вводимых символов
    curs_set(0); // Скрытие курсора
    timeout(80); // Установка таймаута для ввода

    int gapStart = rand() % (WIDTH - MAX_GAP_SIZE); // Начало проема
    int gapSize = MIN_GAP_SIZE + rand() % (MAX_GAP_SIZE - MIN_GAP_SIZE); // Размер проема
    int shipX = WIDTH / 2; // Начальная позиция корабля по X
    int shipY = HEIGHT - 1; // Начальная позиция корабля по Y

    while (true) {
        generateTunnelSegment(segment, gapStart, gapSize); // Генерируем сегмент туннеля
        tunnel.push_front(segment); // Добавляем сегмент в начало туннеля
        if (tunnel.size() > HEIGHT) {
            tunnel.pop_back(); // Удаляем последний сегмент, если туннель слишком длинный
        }
        drawTunnel(tunnel, shipX, shipY); // Рисуем туннель и корабль
        napms(80); // Задержка в 80 мс

        // Вычисляем середину проема
        int gapMiddle = gapStart + gapSize / 2;

        // Обновляем середину и размер проема случайным образом
        int change = rand() % 7 - 3; // -1, 0, или 1
        gapMiddle = (gapMiddle + change + WIDTH) % WIDTH;
        gapSize = MIN_GAP_SIZE + rand() % (MAX_GAP_SIZE - MIN_GAP_SIZE);

        // Пересчитываем начало проема на основе новой середины и размера
        gapStart = gapMiddle - gapSize / 2;

        // Обеспечиваем, что начало и размер проема находятся в допустимых границах
        if (gapStart < 0) gapStart = 0;
        if (gapStart + gapSize > WIDTH) gapStart = WIDTH - gapSize;

        // Двигаем корабль туда-сюда
        int ch = getch();
        if (ch == 'w' || ch == 'W' && shipY > 0) shipY--;
        if (ch == 's' || ch == 'S' && shipY < HEIGHT - 1) shipY++;
        if (ch == 'a' || ch == 'A' && shipX > 0) shipX--;
        if (ch == 'd' || ch == 'D' && shipX < WIDTH - 1) shipX++;
        if (ch == 'p' || ch == 'P') pauseGame(); // Вызываем функцию паузы

        // Проверяем столкновение только если корабль находится внутри туннеля
        if (isShipInsideTunnel(shipY, tunnel) && checkCollision(tunnel, shipX, shipY)) {
            mvprintw(HEIGHT / 2, WIDTH / 2 - 5, "GAME OVER"); // Выводим сообщение "GAME OVER"
            refresh(); // Обновляем экран
            napms(2000); // Задержка в 2000 мс
            break; // Выходим из цикла
        }
    }

    endwin(); // Завершаем работу ncurses

    return 0;
}