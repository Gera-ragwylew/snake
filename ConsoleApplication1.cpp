#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <ctime>
#include <windows.h>
#define board_length 40
#define board_width 20
#define HALF_B_W 10
#define HALF_B_L 20
#define length 50

int Move(int flag);
bool Menu(int menu);
void Board();
void Exit();

int main() {
	int speed = 300;
	int check = 0;
	bool apple = true;
	bool end = false;
	bool flag2 = true;
	int X = 0;
	int Y = 0;
	int flag = 0;
	int segm = 2;
	char arr_snake_X[length];
	char arr_snake_Y[length];
	arr_snake_X[0] = HALF_B_L;
	arr_snake_Y[0] = HALF_B_W;
	int X_apple;
	int Y_apple;
	int menu = 0;
	srand(time(0)); // запуск генератора случайных чисел
	rand(); // холостой ход генератора случаный чисел
	COORD c; // объект для хранения координат
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); // создание хендла потока вывода
	CONSOLE_CURSOR_INFO cci = { sizeof(cci), false }; // создание параметров на отображение курсора
	SetConsoleCursorInfo(h, &cci); //связывание параметров и хендла

	end = Menu(menu);
	system("cls");

	if (!end)
		Board(); //отрисовка поля
	while (!end) {
		if (segm % 5 == 0 && flag2) { // ускорение
			speed -= 50;
			flag2 = false;
		}
		Sleep(speed); // скорость игры (длительность паузы)

		for (int k = 0; k < segm; k++) { // движение сегментов друг за другом
			arr_snake_X[segm - k] = arr_snake_X[segm - k - 1];
			arr_snake_Y[segm - k] = arr_snake_Y[segm - k - 1];
		}
		arr_snake_X[0] = arr_snake_X[0] + X; // движение головы
		arr_snake_Y[0] = arr_snake_Y[0] + Y;

		for (int i = 1; i < segm - 1; i++) { // отрисовка сегментов
			c.X = arr_snake_X[segm - i]; // устанавливаем в объект координат позицию хвоста "змейки"
			c.Y = arr_snake_Y[segm - i];
			SetConsoleCursorPosition(h, c); // двигаем туда курсор
			putchar('*');
		}

		c.X = arr_snake_X[segm]; // устанавливаем в объект координат предыдущую позицию головы "змейки"
		c.Y = arr_snake_Y[segm];
		SetConsoleCursorPosition(h, c);  // двигаем туда курсор
		putchar(' '); // затирамем хвост
		c.X = arr_snake_X[0]; // устанавливаем в объект координат позицию головы "змейки"
		c.Y = arr_snake_Y[0];
		SetConsoleCursorPosition(h, c); // двигаем туда курсор
		putchar('@'); // новая голова
		c.X = arr_snake_X[1]; // устанавливаем в объект координат позицию 2 сегмента "змейки"
		c.Y = arr_snake_Y[1];
		SetConsoleCursorPosition(h, c); // двигаем туда курсор
		putchar('*'); // 2 сегмент (так нужно)

		if (_kbhit()) { // напраление и нажатие
			flag = Move(flag);
		} if (flag == 0) {
			X = -1;
			Y = 0;
		}
		else if (flag == 1) {
			Y = -1;
			X = 0;
		}
		else if (flag == 2) {
			Y = +1;
			X = 0;
		}
		else if (flag == 3) {
			X = +1;
			Y = 0;
		}

		if (apple == true) { // генерация яблока
			while (check != segm) {
				check = 0;
				X_apple = rand() % (board_length - 2) + 1;
				Y_apple = rand() % (board_width - 2) + 1;
				for (int i = 0; i < segm; i++) { //проверка что яблоко не на теле змейки
					if (X_apple == arr_snake_X[i] && Y_apple == arr_snake_Y[i]) {
						break;
					}
					else
						check++;
				}
			}
			c.X = X_apple; // связываем объект координат с позициями "яблока"
			c.Y = Y_apple;
			SetConsoleCursorPosition(h, c); // отправляем курсор на позицию "яблока"
			SetConsoleTextAttribute(h, 12); // устанавливаем красный цвет для отрисовки "яблока"
			putchar('O'); // отображаем символ "яблока"
			SetConsoleTextAttribute(h, 10); // обратная установка цвета в зеленый - для дальнейшего отображения "змейки"
			apple = false;
			check = 0;
		}

		if (arr_snake_X[0] + X == X_apple && arr_snake_Y[0] + Y == Y_apple) { // поедание яблока увеличение змейки
			apple = true;
			segm++;
			if (segm % 5 != 0) {
				flag2 = true;
			}
		}

		for (int i = 0; i < board_width; i++) { // проверка на стену
			for (int j = 0; j < board_length; j++) {
				if ((i == 0 || i == board_width - 1) && j > 0 && j < board_length - 1) {
					if (arr_snake_Y[0] == i && arr_snake_X[0] == j) {
						end = true;
						break;
					}
				}
				else if ((i == 0 || i > 0) && (j == 0 || j == board_length - 1)) {
					if (arr_snake_Y[0] == i && arr_snake_X[0] == j) {
						end = true;
						break;
					}
				}
			}
		}

		for (int i = 3; i < segm; i++) { // проверка на "съедание себя"
			if (arr_snake_X[0] == arr_snake_X[i] && arr_snake_Y[0] == arr_snake_Y[i]) {
				end = true;
			}
		}
	}
	c.X = board_length / 2 + 1; // 
	c.Y = board_width / 2 + 1;
	SetConsoleCursorPosition(h, c); // 
	SetConsoleTextAttribute(h, 12); // 
	Exit();
}

void Board() { // отрисовка поля
	for (int i = 0; i < board_width; i++) {
		for (int j = 0; j < board_length; j++) {
			if ((i == 0 || i == board_width - 1) && j > 0 && j < board_length - 1) {
				printf("-");
			}
			else if ((i == 0 || i > 0) && (j == 0 || j == board_length - 1)) {
				printf("|");
				if (j == board_length - 1) {
					printf("\n");
				}
			}
			else {
				printf(" ");
			}
		}
	}
}

int Move(int flag) {
	switch (_getch()) {
	case 'w':
		if (flag != 2) {
			flag = 1;
			break;
		}
		else
			break;
	case 's':
		if (flag != 1) {
			flag = 2;
			break;
		}
		else
			break;
	case 'a':
		if (flag != 3) {
			flag = 0;
			break;
		}
		else
			break;
	case 'd':
		if (flag != 0) {
			flag = 3;
			break;
		}
		else
			break;
	}
	return flag;
}

bool Menu(int menu) {
	printf("\n\n\n\n%15cSNAKE\n", ' ');
	printf("%10cpress 1 to start\n", ' ');
	printf("%10cpress 2 to exit\n", ' ');
	scanf("%d", &menu);
	if (menu == 1) {
		return false;
	}
	else {
		return true;
	}
}

void Exit() {
	char esc = 0;
	printf("\n\n\n\n%15cYOU LOSE\n", ' ');
	printf("%10cpress Esc to exit\n", ' ');
	while (esc != 27) {
		esc = _getch();
	}
}