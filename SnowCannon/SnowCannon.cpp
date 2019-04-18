#include "pch.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>

using namespace std;

const short SPEC_KEY = 224;
const short UP_ARROW_KEY = 72;
const short DOWN_ARROW_KEY = 80;
const short LEFT_ARROW_KEY = 75;
const short RIGHT_ARROW_KEY = 77;
const short ENTER_KEY = 13;
const short ESC_KEY = 27;
const short SPACE_KEY = 32;
const short BACKSPACE_KEY = 8;
const short NUM_0_KEY = 48;
const short NUM_1_KEY = 49;
const short NUM_2_KEY = 50;
const short NUM_3_KEY = 51;
const short NUM_4_KEY = 52;
const short NUM_5_KEY = 53;
const short NUM_6_KEY = 54;
const short NUM_7_KEY = 55;
const short NUM_8_KEY = 56;
const short NUM_9_KEY = 57;

struct Configs
{
	bool fireIsSpecKey;
	short int fire;
	bool moveLeftIsSpecKey;
	short int moveLeft;
	bool moveRightIsSpecKey;
	short int moveRight;
	bool exitIsSpecKey;
	short int exit;
	short int speed;
	char snowSymbol;
};

struct Tree
{
	Tree *Left;
	Tree *Down;
	Tree *Right;
};

void setcur(int x, int y)//установка курсора на позицию  x y 
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
};

void hidecursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

short int optionsMenu(short int num1, short int num2, short int arrow, short int speed);
void optionsSaver(Configs opt);

FILE *pFieldFile;
FILE *pConfigFile;
FILE *pTreeFile;

void printScreen(char(&c)[120], char(&f)[29][120])
{
	setcur(0, 0);
	hidecursor();
	for (int i = 0; i < 120; i++)
		cout << c[i];
	for (int i = 0; i < 29; i++)
		for (int j = 0; j < 120; j++)
			cout << f[i][j];
}

short int cannonControl(short int num1, short int num2, short int pos, Configs controls, char(&gun)[120])
{
	if ((num1 == SPEC_KEY) && (controls.moveLeftIsSpecKey || controls.moveRightIsSpecKey || controls.fireIsSpecKey))
	{
		if ((num2 == controls.moveLeft) && (pos > 0))
		{
			pos--;
			gun[pos] = gun[pos + 1];
			gun[pos + 1] = 0;
		}
		if ((num2 == controls.moveRight) && (pos < 119))
		{
			pos++;
			gun[pos] = gun[pos - 1];
			gun[pos - 1] = 0;
		}
	} else {
		if ((num1 == controls.moveLeft) && (pos > 0))
		{
			pos--;
			gun[pos] = gun[pos + 1];
			gun[pos + 1] = 0;
		}
		if ((num1 == controls.moveRight) && (pos < 119))
		{
			pos++;
			gun[pos] = gun[pos - 1];
			gun[pos - 1] = 0;
		}
	}

	return pos;
}

void fall(char(&field)[29][120], Configs opt)
{
	for (int i = 29; i > 0; i--)
		for (int j = 119; j >= 0; j--)
		{
			if ((field[i - 1][j] == opt.snowSymbol) && (field[i][j] == ' '))
			{
				field[i][j] = field[i - 1][j];
				field[i - 1][j] = ' ';
			}
			if ((field[i][j] == opt.snowSymbol) && (field[i + 1][j] != ' ') && (i < 27))
				if ((field[i][j - 1] == ' ') && (field[i + 1][j - 1] == ' ') && (field[i + 2][j - 1] == ' ') && (j > 0))
				{
					field[i + 1][j - 1] = opt.snowSymbol;
					field[i][j] = ' ';
				}
				else if ((field[i][j + 1] == ' ') && (field[i + 1][j + 1] == ' ') && (field[i + 2][j + 1] == ' ') && (j < 119))
				{
					field[i + 1][j + 1] = opt.snowSymbol;
					field[i][j] = ' ';
				}
		}
}

void playingGame(char(&cannon)[120], char(&field)[29][120], Configs controls)
{
	short int choose = 0, tempChoose = 0, arrowControl = NUM_1_KEY;
	short int position = 0;
	bool placeIsOver = true;
	system("cls");
	
	printScreen(cannon, field);
	int timer = 0;


	while (choose != controls.exit)
	{
		placeIsOver = true;
		timer += controls.speed;
		if (_kbhit())
		{
			choose = _getch();
			if (choose == SPEC_KEY)
				tempChoose = _getch();
			if ((controls.exitIsSpecKey) && (tempChoose == controls.exit))
				break;
			position = cannonControl(choose, tempChoose, position, controls, cannon);
			if (choose == controls.fire)
				field[0][position] = controls.snowSymbol;
			else if ((controls.fireIsSpecKey) && (tempChoose == controls.fire))
				field[0][position] = controls.snowSymbol;
		}
		for (int i = 0; i < 120; i++)
			if (field[0][i] != controls.snowSymbol)
				placeIsOver = false;
		if (placeIsOver)
		{
			system("cls");
			cout << "\n\n\t\t\t\t\t\tPLACE IS OVER!";
			cout << "\n\n\t\t\t\t\t\tPress any key";
			for (int i = 0; i < 120; i++)
				field[0][i] = ' ';
			cannon[0] = cannon[position];
			if (position != 0)
				cannon[position] = 0;
			_getch();
			return;
		}

		if (timer >= 1000)
		{
			timer = 0;
			fall(field, controls);
		}

		
		
		printScreen(cannon, field);
	}
	cannon[0] = cannon[position];
	if (position != 0)
		cannon[position] = 0;
}

Configs optionsLoader(Configs opt)
{
	bool *b = new bool;
	short int *i = new short int;
	char *c = new char;
	
	pConfigFile = fopen("D:\\Temp\\SnowCannon\\Config.txt", "r+b");
	fread(b, sizeof(bool), 1, pConfigFile);
	opt.fireIsSpecKey = *b;
	fread(i, sizeof(short int), 1, pConfigFile);
	opt.fire = *i;
	fread(b, sizeof(bool), 1, pConfigFile);
	opt.moveLeftIsSpecKey = *b;
	fread(i, sizeof(short int), 1, pConfigFile);
	opt.moveLeft = *i;
	fread(b, sizeof(bool), 1, pConfigFile);
	opt.moveRightIsSpecKey = *b;
	fread(i, sizeof(short int), 1, pConfigFile);
	opt.moveRight = *i;
	fread(b, sizeof(bool), 1, pConfigFile);
	opt.exitIsSpecKey = *b;
	fread(i, sizeof(short int), 1, pConfigFile);
	opt.exit = *i;
	fread(i, sizeof(short int), 1, pConfigFile);
	opt.speed = *i;
	fread(c, sizeof(char), 1, pConfigFile);
	opt.snowSymbol = *c;

	return opt;
}

Configs options(Configs opt)
{
	short int choose = 0, tempChoose = 0, arrowControl = NUM_1_KEY, mainChoose = 0, speedChoose = 0, tempSpeedChoose = 0, temp = 0, tempIs = 0;

	cout << '\n' << "Move left   <--";
	cout << '\n' << "Move right";
	cout << '\n' << "Fire\n";
	printf("Speed: %4d", opt.speed);
	cout << "\n\n" << "Press Esc to exit";

	while (choose != ESC_KEY)
	{
		do
		{
			choose = _getch();
			if (choose == SPEC_KEY)
				tempChoose = _getch();
		} while ((choose != ESC_KEY) && (choose != NUM_1_KEY) && (choose != NUM_2_KEY)
			  && (choose != NUM_3_KEY) && (choose != NUM_4_KEY) && (choose != SPEC_KEY) && (choose != ENTER_KEY));
		if (choose == ENTER_KEY)
		{
			system("cls");
			switch (arrowControl)
			{
			case NUM_1_KEY:
				optionsMenu(choose, tempChoose, arrowControl, opt.speed);
				temp = opt.moveLeft;
				tempIs = opt.moveLeftIsSpecKey;
				mainChoose = _getch();
				if (mainChoose == SPEC_KEY)
				{
					opt.moveLeftIsSpecKey = 1;
					opt.moveLeft = _getch();
				} else {
					opt.moveLeftIsSpecKey = 0;
					opt.moveLeft = mainChoose;
				}
				if ((opt.moveLeft == opt.moveRight) || (opt.moveLeft == opt.fire))
				{
					opt.moveLeft = temp;
					opt.moveLeftIsSpecKey = tempIs;
				}
				break;
			case NUM_2_KEY:
				optionsMenu(choose, tempChoose, arrowControl, opt.speed);
				temp = opt.moveRight;
				tempIs = opt.moveRightIsSpecKey;
				mainChoose = _getch();
				if (mainChoose == SPEC_KEY)
				{
					opt.moveRightIsSpecKey = 1;
					opt.moveRight = _getch();
				}
				else {
					opt.moveRightIsSpecKey = 0;
					opt.moveRight = mainChoose;
				}
				if ((opt.moveRight == opt.moveLeft) || (opt.moveRight == opt.fire))
				{
					opt.moveRight = temp;
					opt.moveRightIsSpecKey = tempIs;
				}
				break;
			case NUM_3_KEY:
				optionsMenu(choose, tempChoose, arrowControl, opt.speed);
				temp = opt.fire;
				tempIs = opt.fireIsSpecKey;
				mainChoose = _getch();
				if (mainChoose == SPEC_KEY)
				{
					opt.fireIsSpecKey = 1;
					opt.fire = _getch();
				}
				else {
					opt.fireIsSpecKey = 0;
					opt.fire = mainChoose;
				}
				if ((opt.fire == opt.moveLeft) || (opt.fire == opt.moveRight))
				{
					opt.fire = temp;
					opt.fireIsSpecKey = tempIs;
				}
				break;
			case NUM_4_KEY:
				speedChoose = 0;
				while (speedChoose != ENTER_KEY)
				{
					system("cls");
					optionsMenu(choose, tempChoose, arrowControl, opt.speed);
					do
					{
						speedChoose = _getch();
						if (speedChoose == SPEC_KEY)
							tempSpeedChoose = _getch();
					} while ((speedChoose != SPEC_KEY) && (speedChoose != ENTER_KEY));
					if ((speedChoose == SPEC_KEY) && (tempSpeedChoose == LEFT_ARROW_KEY) && (opt.speed > 1))
						opt.speed /= 10;
					else if ((speedChoose == SPEC_KEY) && (tempSpeedChoose == RIGHT_ARROW_KEY) && (opt.speed < 1000))
						opt.speed *=10;
				}
			}
			choose = arrowControl;
		}
		system("cls");

		arrowControl = optionsMenu(choose, tempChoose, arrowControl, opt.speed);
	}
	optionsSaver(opt);
	return opt;
}

void optionsSaver(Configs opt)
{
	pConfigFile = fopen("D:\\Temp\\SnowCannon\\Config.txt", "w+b");
	fwrite(&(opt.fireIsSpecKey), sizeof(bool), 1, pConfigFile);
	fwrite(&(opt.fire), sizeof(short int), 1, pConfigFile);
	fwrite(&(opt.moveLeftIsSpecKey), sizeof(bool), 1, pConfigFile);
	fwrite(&(opt.moveLeft), sizeof(short int), 1, pConfigFile);
	fwrite(&(opt.moveRightIsSpecKey), sizeof(bool), 1, pConfigFile);
	fwrite(&(opt.moveRight), sizeof(short int), 1, pConfigFile);
	fwrite(&(opt.exitIsSpecKey), sizeof(bool), 1, pConfigFile);
	fwrite(&(opt.exit), sizeof(short int), 1, pConfigFile);
	fwrite(&(opt.speed), sizeof(short int), 1, pConfigFile);
	fwrite(&(opt.snowSymbol), sizeof(char), 1, pConfigFile);
	fclose(pConfigFile);
}

void fieldLoader(char(&f)[29][120])
{
	char c;
	pFieldFile = fopen("D:\\Temp\\SnowCannon\\Field.txt", "r+b");  //изменены настройки препроцессора
	for (int i = 0; i < 29; i++)
	{
		for (int j = 0; j < 120; j++)
			fread(&f[i][j], sizeof(char), 1, pFieldFile);
		fread(&c, sizeof(char), 1, pFieldFile);
		if (c != 10)
		{
			while (c != 10)
				fread(&c, sizeof(char), 1, pFieldFile);
		}
	}
	fclose(pFieldFile);

	for (int i = 0; i < 29; i++)
		for (int j = 0; j < 120; j++)
			if ((f[i][j] == 9) || (f[i][j] == 10))
				f[i][j] = ' ';
}

void fieldSaver(char(&f)[29][120])
{
	char c = 10;
	pFieldFile = fopen("D:\\Temp\\SnowCannon\\Field.txt", "w");
	for (int i = 0; i < 29; i++)
	{
		fwrite(&(f[i][0]), sizeof(char), 120, pFieldFile);
		fwrite(&c, sizeof(char), 1, pFieldFile);
	}
	fclose(pFieldFile);
}

void tree(char(&field)[29][120])
{
	short int choose = 0, tempChoose = 0;
	short int coordX = 0, coodrY = 0;
	char temp = 0;
	char empty[120] = {0};
	system("cls");
	cout << '\n';
	printScreen(empty, field);
	while (choose != ESC_KEY)
	{
		do
		{
			choose = _getch();
			if (choose == SPEC_KEY)
				tempChoose = _getch();


		} while ((choose != ESC_KEY) && (choose != ENTER_KEY));

	}
}

short int menu(short int num1, short int num2, short int arrow)
{
	if (num1 == SPEC_KEY)
	{
		if (num2 == UP_ARROW_KEY)
			if (arrow > NUM_1_KEY)
				num1 = --arrow;
			else
				num1 = NUM_3_KEY;
		else if (num2 == DOWN_ARROW_KEY)
			if (arrow < NUM_3_KEY)
				num1 = ++arrow;
			else
				num1 = NUM_1_KEY;
		else
			num1 = arrow;
	}

	cout << '\n' << "Start   ";
	if (num1 == NUM_1_KEY)
		cout << "<--";
	cout << '\n' << "Options ";
	if (num1 == NUM_2_KEY)
		cout << "<--";
	cout << '\n' << "Save    ";
	if (num1 == NUM_3_KEY)
		cout << "<--";
	cout << "\n\n" << "Press Esc to exit";
	return num1;
}

short int optionsMenu(short int num1, short int num2, short int arrow, short int speed)
{
	if (num1 == SPEC_KEY)
	{
		if (num2 == UP_ARROW_KEY)
			if (arrow > NUM_1_KEY)
				num1 = --arrow;
			else
				num1 = NUM_4_KEY;
		else if (num2 == DOWN_ARROW_KEY)
			if (arrow < NUM_4_KEY)
				num1 = ++arrow;
			else
				num1 = NUM_1_KEY;
		else
			num1 = arrow;
	}

	cout << '\n' << "Move left   ";
	if (num1 == NUM_1_KEY)
		cout << "<--";
	cout << '\n' << "Move right  ";
	if (num1 == NUM_2_KEY)
		cout << "<--";
	cout << '\n' << "Fire        ";
	if (num1 == NUM_3_KEY)
		cout << "<--";
	cout << '\n';
	printf("Speed: %4d", speed);
	if (num1 == NUM_4_KEY)
		cout << " <--";
	cout << "\n\n" << "Press Esc to exit";
	return num1;
}

//размер консоли 120 на 30 символов
int main()
{
	short int choose = 0, tempChoose = 0, arrowControl = NUM_1_KEY;
	char cannon[120] = { 0 };
	char field[29][120] = { 0 };
	int i = 0, j = 0;
	Configs controls;
	{
		controls.fireIsSpecKey = false;
		controls.fire = ENTER_KEY;
		controls.moveLeftIsSpecKey = true;
		controls.moveLeft = LEFT_ARROW_KEY;
		controls.moveRightIsSpecKey = true;
		controls.moveRight = RIGHT_ARROW_KEY;
		controls.exitIsSpecKey = false;
		controls.exit = ESC_KEY;
		controls.speed = 100;
	}

	controls = optionsLoader(controls);
	//controls.snowSymbol = '*';
	cannon[0] = '\xD0';
	memset(field, '\x20', 29*120);
	fieldLoader(field);

	{
		cout << '\n' << "Start   <--";
		cout << '\n' << "Options ";
		cout << '\n' << "Save    ";
		cout << "\n\n" << "Press Esc to exit";
	}
	
	while (choose != ESC_KEY)
	{
		do
		{
			choose = _getch();
			if (choose == SPEC_KEY)
				tempChoose = _getch();
		} while ((choose != ESC_KEY) && (choose != NUM_1_KEY) && (choose != NUM_2_KEY) 
			  && (choose != NUM_3_KEY) && (choose != SPEC_KEY) && (choose != ENTER_KEY));
		if (choose == ENTER_KEY)
		{
			system("cls");
			switch (arrowControl)
			{
			case NUM_1_KEY:
				playingGame(cannon, field, controls);
				break;
			case NUM_2_KEY:
				controls = options(controls);
				break;
			case NUM_3_KEY:
			{
				fieldSaver(field);
				cout << "Saving...\n";
				Sleep(1000);
				cout << "Complete!";
				Sleep(800);
			}
			}
			choose = arrowControl;
		}
		system("cls");

		arrowControl = menu(choose, tempChoose, arrowControl);
	}

	return 0;
}