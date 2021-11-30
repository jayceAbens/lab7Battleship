/*
	Jayce Abens
	C++	Fall 2021
	Due:
	Lab 7: Battleship

	For this lab you will make a simple battleship game. You will need four
	10 x 10 grids(two for the user and two for the computer).The program will begin with
	setup.The program will need to randomly place the computers 5 ships, making sure the
	ships do not overlap or go off the board.
*/

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <cmath>

#define pFleet 0
#define pShots 1
#define cFleet 2
#define cShots 3

using namespace std;

bool debugging = false;

void resetGame(char gameBoard[4][10][10]); // 1: player fleet, 2: player shots, 3:computer fleet, 4: computer shots 
void printBoard(char board[10][10]);
void printIntro();
void printTransition();
void playerSetup(char gameBoard[10][10], struct shipType ship[5]);
void computerSetup(char gameBoard[10][10], struct shipType ship[5]);
bool placeShip(int x, int y, int length, char direction, char shipChar, char gameBoard[10][10]);
void playerFire(char playerShots[10][10], char computerFleet[10][10]);
bool computerFire(char computerShots[10][10], char playerFleet[10][10]);
bool checkWin(char gameBoard[10][10]);
bool playAgain(bool win, int wins, int losses);

struct shipType {
	string name;
	int length;
};

int main()
{
	char gameBoard[4][10][10];

	shipType ship[5];

	ship[0].name = "CARRIER";
	ship[0].length = 5;

	ship[1].name = "BATTLESHIP";
	ship[1].length = 4;

	ship[2].name = "CRUISER";
	ship[2].length = 3;

	ship[3].name = "SUBMARINE";
	ship[3].length = 3;

	ship[4].name = "DESTROYER";
	ship[4].length = 2;

	bool pWin;
	bool cWin;
	int wins = 0;
	int losses = 0;

	do {
		pWin = false;
		cWin = false;

		printIntro();
		resetGame(gameBoard);
		playerSetup(gameBoard[pFleet], ship);
		computerSetup(gameBoard[cFleet], ship);
		printTransition();
		do
		{
			playerFire(gameBoard[pShots], gameBoard[cFleet]);
			if (checkWin(gameBoard[cFleet])) 
			{
				pWin = true;
				wins++;
			}
			else
			{
				if(computerFire(gameBoard[cShots], gameBoard[pFleet]) || checkWin(gameBoard[pFleet]))
				{
					cWin = true;
					losses++;
				}
			}
		} while (!pWin && !cWin);
	} while (playAgain(pWin, wins, losses));

	return 0;
}

void printIntro()
{
	cout << "\t /$$$$$$$   /$$$$$$  /$$$$$$$$ /$$$$$$$$ /$$       /$$$$$$$$  /$$$$$$  /$$   /$$ /$$$$$$ /$$$$$$$ \n";
	cout << "\t| $$__  $$ /$$__  $$|__  $$__/|__  $$__/| $$      | $$_____/ /$$__  $$| $$  | $$|_  $$_/| $$__  $$\n";
	cout << "\t| $$  \\ $$| $$  \\ $$   | $$      | $$   | $$      | $$      | $$  \\__/| $$  | $$  | $$  | $$  \\ $$\n";
	cout << "\t| $$$$$$$ | $$$$$$$$   | $$      | $$   | $$      | $$$$$   |  $$$$$$ | $$$$$$$$  | $$  | $$$$$$$/\n";
	cout << "\t| $$__  $$| $$__  $$   | $$      | $$   | $$      | $$__/    \\____  $$| $$__  $$  | $$  | $$____/ \n";
	cout << "\t| $$  \\ $$| $$  | $$   | $$      | $$   | $$      | $$       /$$  \\ $$| $$  | $$  | $$  | $$      \n";
	cout << "\t| $$$$$$$/| $$  | $$   | $$      | $$   | $$$$$$$$| $$$$$$$$|  $$$$$$/| $$  | $$ /$$$$$$| $$      \n";
	cout << "\t|_______/ |__/  |__/   |__/      |__/   |________/|________/ \\______/ |__/  |__/|______/|__/      \n\n";
	cout << "------------------------------------------------------------------------------------------------------------------\n\n";
	cout << "\t- This is a strategy type guessing game where you will test your skills against the computer.\n";
	cout << "\t- First you will each place a fleet of 5 ships on a game board concealed from the other.\n";
	cout << "\t- You will then alternate turns firing shots at the other player's ships.\n";
	cout << "\t- The objective of the game: destroy the opposing player's fleet before yours is sunk.\n\n\n\n";
	
	cout << flush;
	system("PAUSE");
	system("CLS");

	cout << "\t ____ _____  _    ____ _____   _       ____  _                  _____ _           _   \n";
	cout << "\t/ ___|_   _|/ \\  / ___| ____| / |  _  |  _ \\| | __ _  ___ ___  |  ___| | ___  ___| |_ \n";
	cout << "\t\\___ \\ | | / _ \\| |  _|  _|   | | (_) | |_) | |/ _` |/ __/ _ \\ | |_  | |/ _ \\/ _ \\ __|\n";
	cout << "\t ___) || |/ ___ \\ |_| | |___  | |  _  |  __/| | (_| | (_|  __/ |  _| | |  __/  __/ |_ \n";
	cout << "\t|____/ |_/_/   \\_\\____|_____| |_| (_) |_|   |_|\\__,_|\\___\\___| |_|   |_|\\___|\\___|\\__|\n\n";  
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << "\t\t1. Enter the coordinate points of each ship placement. (1-10)\n\n";
	cout << "\t\t2. Enter the direction to place the ship. (U-D-L-R)\n\n\n\n";

	cout << flush;
	system("PAUSE");
	system("CLS");
}

void printTransition()
{
	cout << "\t ____ _____  _    ____ _____   ____           _   _   _             _    _ \n";
	cout << "\t/ ___|_   _|/ \\  / ___| ____| |___ \\   _     / \\ | |_| |_ __ _  ___| | _| |\n";
	cout << "\t\\___ \\ | | / _ \\| |  _|  _|     __) | (_)   / _ \\| __| __/ _` |/ __| |/ / |\n";
	cout << "\t ___) || |/ ___ \\ |_| | |___   / __/   _   / ___ \\ |_| || (_| | (__|   <|_|\n";
	cout << "\t|____/ |_/_/   \\_\\____|_____| |_____| (_) /_/   \\_\\__|\\__\\__,_|\\___|_|\\_(_)\n\n";
	cout << "------------------------------------------------------------------------------------------\n\n";
	cout << "\t\t1. Enter the target coordinate to fire upon. (1-10)\n\n";
	cout << "\t\t2. View the computers shot upon your fleet.\n\n\n\n";

	cout << flush;
	system("PAUSE");
	system("CLS");
}

void resetGame(char gameBoard[4][10][10])
{
	for (int z = 0; z < 4; z++)
	{
		for (int y = 0; y < 10; y++)
		{
			for (int x = 0; x < 10; x++)
			{
				gameBoard[z][x][y] = '~';
			}
		}
	}
}

void printBoard(char gameBoard[10][10])
{
	cout << "   1 2 3 4 5 6 7 8 9 10\n";
	for (int y = 0; y < 10; y++)
	{
		cout << setw(2) << y + 1;
		for (int x = 0; x < 10; x++)
		{
			cout << " " << gameBoard[x][y];
		}
		cout << endl;
	}
}

bool placeShip(int x, int y, int length, char direction, char shipChar, char gameBoard[10][10])
{
	switch (direction)
	{
	case 'U':
	case 'u':
		for (int i = y; i > y - length; i--)
			if (gameBoard[x][i] == '#' || i < 0) return false;
		for (int i = y; i > y - length; i--)
			gameBoard[x][i] = shipChar;
		break;
	case 'D':
	case 'd':
		for (int i = y; i < y + length; i++)
			if (gameBoard[x][i] == '#' || i > 9) return false;
		for (int i = y; i < y + length; i++)
			gameBoard[x][i] = shipChar;
		break;
	case 'R':
	case 'r':
		for (int i = x; i < x + length; i++)
			if (gameBoard[i][y] == '#' || i > 9) return false;
		for (int i = x; i < x + length; i++)
			gameBoard[i][y] = shipChar;
		break;
	case 'L':
	case 'l':
		for (int i = x; i > x - length; i--)
			if (gameBoard[i][y] == '#' || i < 0) return false;
		for (int i = x; i > x - length; i--)
			gameBoard[i][y] = shipChar;
		break;
	}
	return true;
}

void playerSetup(char gameBoard[10][10], struct shipType ship[5])
{
	char gameBoardTemp[10][10];
	int xCoord;
	int yCoord;
	char direction;
	bool inputError = false;
	bool spaceOccupied = false;
	string playerInput;

	for (int i = 0; i < 5; i++)
	{
		memcpy(gameBoardTemp, gameBoard, sizeof(gameBoardTemp));
		for (int j = 0; j < 3; j++)
		{
			printBoard(gameBoardTemp);
			cout << endl << ship[i].name << endl;
			cout << "Length: " << ship[i].length << endl;
			switch (j)
			{
			case 0:
				if (inputError) cout << "\t[COORDINATE POINT OUT OF RANGE: 1-10]\r";
				if (spaceOccupied) cout << "\t[SPACE ALREADY OCCUPIED]\r";
				cout << "X: ";

				if (!(cin >> xCoord) || xCoord < 1 || xCoord > 10)
				{
					cin.clear();
					cin.ignore( 999, '\n');
					inputError = true;
				}
				else
				{
					inputError = false;
					spaceOccupied = true;
					for (int y = 0; y < 10; y++)
					{
						if (placeShip(xCoord - 1, y, ship[i].length, 'U', '~', gameBoard) ||
							placeShip(xCoord - 1, y, ship[i].length, 'D', '~', gameBoard) ||
							placeShip(xCoord - 1, y, ship[i].length, 'L', '~', gameBoard) ||
							placeShip(xCoord - 1, y, ship[i].length, 'R', '~', gameBoard))
						{
							spaceOccupied = false;
						}
					}
				}

				if (inputError || spaceOccupied) j = j - 1;
				else
				{
					for (int y = 0; y < 10; y++) if (gameBoardTemp[xCoord - 1][y] == '~') gameBoardTemp[xCoord - 1][y] = '|';
				}
				break;
			case 1:
				cout << "X: " << xCoord << endl;
				if (inputError) cout << "\t[COORDINATE POINT OUT OF RANGE: 1-10]\r";
				if (spaceOccupied) cout << "\t[SPACE ALREADY OCCUPIED]\r";
				cout << "Y: ";

				if (!(cin >> yCoord) || yCoord < 1 || yCoord > 10)
				{
					cin.clear();
					cin.ignore( 999, '\n');
					inputError = true;
				}
				else
				{
					inputError = false;
					if (placeShip(xCoord - 1, yCoord - 1, ship[i].length, 'U', '~', gameBoard) ||
						placeShip(xCoord - 1, yCoord - 1, ship[i].length, 'D', '~', gameBoard) ||
						placeShip(xCoord - 1, yCoord - 1, ship[i].length, 'L', '~', gameBoard) ||
						placeShip(xCoord - 1, yCoord - 1, ship[i].length, 'R', '~', gameBoard))
					{
						spaceOccupied = false;
					}
					else spaceOccupied = true;
				}

				if (inputError || spaceOccupied) j = j - 1;
				else
				{
					for (int x = 0; x < 10; x++) if (gameBoardTemp[x][yCoord - 1] == '~') gameBoardTemp[x][yCoord - 1] = '-';
					placeShip(xCoord - 1, yCoord - 1, ship[i].length, 'U', 'U', gameBoardTemp);
					placeShip(xCoord - 1, yCoord - 1, ship[i].length, 'D', 'D', gameBoardTemp);
					placeShip(xCoord - 1, yCoord - 1, ship[i].length, 'L', 'L', gameBoardTemp);
					placeShip(xCoord - 1, yCoord - 1, ship[i].length, 'R', 'R', gameBoardTemp);
					gameBoardTemp[xCoord - 1][yCoord - 1] = 'X';
				}
				break;
			case 2:
				cout << "X: " << xCoord << endl;
				cout << "Y: " << yCoord << endl;
				if (spaceOccupied) cout << "\t\t[SPACE ALREADY OCCUPIED]\r";
				if (inputError) cout << "\t\t[INVALID DIRECTION SELECTION: U-D-L-R ]\r";
				cout << "Direction: ";

				cin >> playerInput;
				direction = playerInput.at(0);

				if (!(direction == 'U' || direction == 'u' ||
					direction == 'D' || direction == 'd' ||
					direction == 'L' || direction == 'l' ||
					direction == 'R' || direction == 'r'))
				{
					inputError = true;
				}
				else
				{
					inputError = false;
					if (!(placeShip(xCoord - 1, yCoord - 1, ship[i].length, direction, '#', gameBoard))) spaceOccupied = true;
					else spaceOccupied = false;
				}

				if (inputError || spaceOccupied) j = j - 1;
				break;
			}
			cout << flush;
			system("CLS");
		}
	}
}

void computerSetup(char gameBoard[10][10], struct shipType ship[5])
{
	srand(time(0));

	for (int i = 0; i < 5; i++)
	{
		int x = rand() % 9;
		int y = rand() % 9;
		char direction;
		switch (rand() % 3)
		{
		case 0: direction = 'U';
			break;
		case 1: direction = 'D';
			break;
		case 2: direction = 'L';
			break;
		case 3: direction = 'R';
			break;
		}

		if (!(placeShip(x, y, ship[i].length, direction, '#', gameBoard))) i = i - 1;
	}

	//Debugging Printout
	if (debugging)
	{
		cout << "Computer Fleet: \n";
		printBoard(gameBoard);
		cout << flush;
		system("PAUSE");
		system("CLS");
	}
}

void playerFire(char playerShots[10][10], char computerFleet[10][10])
{
	int x;
	int y;
	bool inputError = false;
	bool spaceOccupied = false;
	char playerShotsTemp[10][10];

	memcpy(playerShotsTemp, playerShots, sizeof(playerShotsTemp));
	for (int i = 0; i < 3; i++)
	{
		printBoard(playerShotsTemp);
		cout << "\nTARGET COORDINATES\n";
		switch (i)
		{
		case 0:
			if (inputError) cout << "\t[COORDINATE POINT OUT OF RANGE: 1-10]\r";
			if (spaceOccupied) cout << "\t[SPACE ALREADY FIRED UPON]\r";
			cout << "X: ";

			if (!(cin >> x) || x > 10 || x < 0)
			{
				cin.clear();
				cin.ignore( 999, '\n');
				inputError = true;
			}
			else
			{
				inputError = false;
				spaceOccupied = true;
				for (int y1 = 0; y1 < 10; y1++) if (playerShots[x - 1][y1] == '~') spaceOccupied = false;
			}

			if (inputError || spaceOccupied) i = i - 1;
			else for (int y1 = 0; y1 < 10; y1++) if (playerShotsTemp[x - 1][y1] == '~') playerShotsTemp[x - 1][y1] = '|';
			break;
		case 1:
			cout << "X: " << x << endl;
			if (inputError) cout << "\t[COORDINATE POINT OUT OF RANGE: 1-10]\r";
			if (spaceOccupied) cout << "\t[SPACE ALREADY FIRED UPON]\r";
			cout << "Y: ";

			if (!(cin >> y) || y > 10 || y < 0)
			{
				cin.clear();
				cin.ignore( 999, '\n');
				inputError = true;
			}
			else
			{
				inputError = false;
				if (playerShots[x - 1][y - 1] != '~') spaceOccupied = true;
				else spaceOccupied = false;
			}
			if (inputError || spaceOccupied) i = i - 1;
			else
			{
				for (int x1 = 0; x1 < 10; x1++) if (playerShotsTemp[x1][y - 1] == '~') playerShotsTemp[x1][y - 1] = '-';
				if (computerFleet[x - 1][y - 1] == '#')
				{
					playerShotsTemp[x - 1][y - 1] = 'H';
					playerShots[x - 1][y - 1] = 'H';
					computerFleet[x - 1][y - 1] = 'H';
				}
				else
				{
					playerShotsTemp[x - 1][y - 1] = 'M';
					playerShots[x - 1][y - 1] = 'M';
					computerFleet[x - 1][y - 1] = 'M';
				}
			}
			break;
		case 2:
			cout << "X: " << x << endl;
			cout << "Y: " << y << endl << endl;
			if (computerFleet[x - 1][y - 1] == 'H') cout << "HIT!";
			else cout << "MISS!";

			//debugging printout
			if (debugging)
			{
				cout << "\n\nComputer Fleet:\n";
				printBoard(computerFleet);
			}

			cout << endl << endl << flush;
			system("PAUSE");
			break;
		}
		cout << flush;
		system("CLS");
	}
}

bool computerFire(char computerShots[10][10], char playerFleet[10][10])
{
	int x;
	int y;
	bool invalidShot;
	do
	{
		x = rand() % 9;
		y = rand() % 9;
		if (computerShots[x][y] != '~') invalidShot = true;
		else
		{
			invalidShot = false;
			if (playerFleet[x][y] == '#')
			{
				computerShots[x][y] = 'H';
				playerFleet[x][y] = 'H';
			}
			else
			{
				computerShots[x][y] = 'M';
				playerFleet[x][y] = 'M';
			}
		}
	} while (invalidShot);

	string playerInput;
	bool inputError = false;
	do
	{
		printBoard(playerFleet);
		if (playerFleet[x][y] == 'H') cout << "\nCOMPUTER HIT!\n\n";
		else cout << "\nCOMPUTER MISSED!\n\n";

		if (inputError) cout << "\t\t\t\t\t[Invalid Response: Y/N]\r";
		cout << "Surrender? : ";
		cin >> playerInput;
		switch (playerInput.at(0))
		{
		case 'y':
		case 'Y':
			cout << flush;
			system("CLS");
			return true;
		case 'n':
		case 'N':
			cout << flush;
			system("CLS");
			return false;
		default:
			inputError = true;
		}

		// debugging printout
		if (debugging)
		{
			cout << "\n\nComputer Shots:\n";
			printBoard(computerShots);
		}

		cout << endl << endl << flush;
		system("PAUSE");
		system("CLS");
	} while (inputError);
}

bool checkWin(char gameBoard[10][10])
{
	bool win = true;
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			if (gameBoard[x][y] == '#') win = false;
		}
	}
	return win;
}

bool playAgain(bool win, int wins, int losses)
{
	bool inputError = false;
	bool thanks = false;
	string playerInput;

	do {
		if (win)
		{
			cout << "\t /$$    /$$ /$$$$$$  /$$$$$$  /$$$$$$$$  /$$$$$$ /$$$$$$$  /$$    /$$  /$$\n";
			cout << "\t| $$   | $$|_  $$_/ /$$__  $$|__  $$__//$$__  $$| $$__  $$|  $$   /$$/| $$\n";
			cout << "\t| $$   | $$  | $$  | $$  \\__/   | $$  | $$  \\ $$| $$  \\ $$ \\  $$ /$$/ | $$\n";
			cout << "\t|  $$ / $$/  | $$  | $$         | $$  | $$  | $$| $$$$$$$/  \\  $$$$/  | $$\n";
			cout << "\t \\  $$ $$/   | $$  | $$         | $$  | $$  | $$| $$__  $$   \\  $$/   |__/\n";
			cout << "\t  \\  $$$/    | $$  | $$    $$   | $$  | $$  | $$| $$  \\ $$    | $$        \n";
			cout << "\t   \\  $/    /$$$$$$|  $$$$$$/   | $$  |  $$$$$$/| $$  | $$    | $$     /$$\n";
			cout << "\t    \\_/    |______/ \\______/    |__/   \\______/ |__/  |__/    |__/    |__/\n\n";
			cout << "------------------------------------------------------------------------------------------\n\n";
			cout << "\t\t\t\tWINS: " << wins << "\t\tLOSSES: " << losses << endl << endl << endl;
		}
		else
		{
			cout << "\t /$$$$$$$  /$$$$$$$$ /$$$$$$$$ /$$$$$$$$  /$$$$$$  /$$$$$$$$ /$$\n";
			cout << "\t| $$__  $$| $$_____/| $$_____/| $$_____/ /$$__  $$|__  $$__/| $$\n";
			cout << "\t| $$  \\ $$| $$      | $$      | $$      | $$  \\ $$   | $$   | $$\n";
			cout << "\t| $$  | $$| $$$$$   | $$$$$   | $$$$$   | $$$$$$$$   | $$   | $$\n";
			cout << "\t| $$  | $$| $$__/   | $$__/   | $$__/   | $$__  $$   | $$   |__/\n";
			cout << "\t| $$  | $$| $$      | $$      | $$      | $$  | $$   | $$       \n";
			cout << "\t| $$$$$$$/| $$$$$$$$| $$      | $$$$$$$$| $$  | $$   | $$    /$$\n";
			cout << "\t|_______/ |________/|__/      |________/|__/  |__/   |__/   |__/\n\n";
			cout << "--------------------------------------------------------------------------------\n\n";
			cout << "\t\t\tWINS: " << wins << "\t\tLOSSES: " << losses << endl << endl << endl;
		}
		if (inputError) cout << "\t\t\t\t\t[Invalid Response: Y/N]\r";
		if (thanks)
		{
			cout << "Would you like to play again? : " << playerInput.at(0) << "\tThanks for playing!\n\n\n";
			return false;
		}
		cout << "Would you like to play again? : ";
		cin >> playerInput;
		switch (playerInput.at(0))
		{
		case 'y':
		case 'Y':
			cout << flush;
			system("CLS");
			return true;
		case 'n':
		case 'N':
			inputError = false;
			thanks = true;
			break;
		default:
			inputError = true;
		}
		cout << flush;
		system("CLS");
	} while (inputError || thanks);
}