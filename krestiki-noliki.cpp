#include <iostream>

// Игровое поле 3x3, заполненное числами от 1 до 9
char gameBoard[9] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
char currentPlayer = 'X'; // Текущий игрок (X — пользователь, O — компьютер)

// Функция для отрисовки игрового поля
void drawGameBoard() {
    std::cout << "\n " << gameBoard[0] << " | " << gameBoard[1] << " | " << gameBoard[2] << " \n"
        << "-----------\n"
        << " " << gameBoard[3] << " | " << gameBoard[4] << " | " << gameBoard[5] << " \n"
        << "-----------\n"
        << " " << gameBoard[6] << " | " << gameBoard[7] << " | " << gameBoard[8] << " \n\n";
    }

// Функция для очистки консоли
void clearConsole() {
    std::cout << "\x1b[2J"; // Очистка экрана
    std::cout << "\x1b[H";  // Перемещение курсора в начало экрана
    }

// Функция для проверки победы
bool checkForWin(char playerSymbol) {
    // Проверка всех возможных комбинаций для победы
    return (gameBoard[0] == playerSymbol && gameBoard[1] == playerSymbol && gameBoard[2] == playerSymbol) || // Первая строка
        (gameBoard[3] == playerSymbol && gameBoard[4] == playerSymbol && gameBoard[5] == playerSymbol) || // Вторая строка
        (gameBoard[6] == playerSymbol && gameBoard[7] == playerSymbol && gameBoard[8] == playerSymbol) || // Третья строка
        (gameBoard[0] == playerSymbol && gameBoard[3] == playerSymbol && gameBoard[6] == playerSymbol) || // Первый столбец
        (gameBoard[1] == playerSymbol && gameBoard[4] == playerSymbol && gameBoard[7] == playerSymbol) || // Второй столбец
        (gameBoard[2] == playerSymbol && gameBoard[5] == playerSymbol && gameBoard[8] == playerSymbol) || // Третий столбец
        (gameBoard[0] == playerSymbol && gameBoard[4] == playerSymbol && gameBoard[8] == playerSymbol) || // Главная диагональ
        (gameBoard[2] == playerSymbol && gameBoard[4] == playerSymbol && gameBoard[6] == playerSymbol);   // Побочная диагональ
    }

// Функция для проверки ничьей
bool checkForDraw() {
    for (int i = 0; i < 9; ++i) {
        if (gameBoard[i] != 'X' && gameBoard[i] != 'O') {
            return false; // Если найдена свободная клетка, игра продолжается
            }
        }
    return true; // Если все клетки заняты, это ничья
    }

// Функция для хода компьютера
void makeComputerMove() {
    // 1. Попытка выиграть
    for (int i = 0; i < 9; ++i) {
        if (gameBoard[i] != 'X' && gameBoard[i] != 'O') {
            gameBoard[i] = 'O'; // Предполагаем ход
            if (checkForWin('O')) {
                return; // Если это выигрышный ход, завершаем функцию
                }
            gameBoard[i] = '1' + i; // Отменяем ход
            }
        }

    // 2. Блокировка игрока
    for (int i = 0; i < 9; ++i) {
        if (gameBoard[i] != 'X' && gameBoard[i] != 'O') {
            gameBoard[i] = 'X'; // Предполагаем ход игрока
            if (checkForWin('X')) {
                gameBoard[i] = 'O'; // Блокируем выигрышный ход игрока
                return;
                }
            gameBoard[i] = '1' + i; // Отменяем ход
            }
        }

    // 3. Стратегический выбор клетки
    int priorityCells[9] = { 4, 0, 2, 6, 8, 1, 3, 5, 7 }; // Центр, углы, затем стороны
    for (int i = 0; i < 9; ++i) {
        int cell = priorityCells[i];
        if (gameBoard[cell] != 'X' && gameBoard[cell] != 'O') {
            gameBoard[cell] = 'O'; // Выбираем клетку по приоритету
            return;
            }
        }
    }

int main() {
    int chosenCell;
    clearConsole();

    while (true) {
        drawGameBoard();

        if (currentPlayer == 'X') {
            // Ход пользователя
            std::cout << "Your turn (X). Enter cell number (1-9): ";
            std::cin >> chosenCell;

            // Проверка корректности ввода
            if (chosenCell < 1 || chosenCell > 9 || gameBoard[chosenCell - 1] == 'X' || gameBoard[chosenCell - 1] == 'O') {
                std::cout << "Invalid move! Try again.\n";
                continue;
                }

            gameBoard[chosenCell - 1] = 'X'; // Пользователь делает ход
            }
        else {
            // Ход компьютера
            std::cout << "Computer's turn (O)...\n";
            makeComputerMove();
            }

        // Проверка на победу
        if (checkForWin(currentPlayer)) {
            drawGameBoard();
            std::cout << (currentPlayer == 'X' ? "You win!" : "Computer wins!") << "\n";
            break;
            }

        // Проверка на ничью
        if (checkForDraw()) {
            drawGameBoard();
            std::cout << "It's a draw!\n";
            break;
            }

        // Смена игрока
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        clearConsole();
        }

    return 0;
    }