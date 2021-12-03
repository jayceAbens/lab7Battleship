﻿/*
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

	shipType ship[5];	// struct of ship names and lengths

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

	bool pWin;	// player win
	bool cWin;	// computer win
	int wins = 0;	// total wins
	int losses = 0;	// total losses

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
				if(computerFire(gameBoard[cShots], gameBoard[pFleet]) || checkWin(gameBoard[pFleet])) // computerFire returns true if player surrenders
				{
					cWin = true;
					losses++;
				}
			}
		} while (!pWin && !cWin);
	} while (playAgain(pWin, wins, losses));

	return 0;
}


// Precondition :	using namespace std, the iostream library is included
// Postcondition :	the indroduction page is printed, cleared after a keystroke, and the ship placement instructions are printed
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
	system("PAUSE");	// waits for keystroke
	system("CLS");		// clears screen
	
	cout << "\t ____ _____  _    ____ _____   _       ____  _                  _____ _           _   \n";
	cout << "\t/ ___|_   _|/ \\  / ___| ____| / |  _  |  _ \\| | __ _  ___ ___  |  ___| | ___  ___| |_ \n";
	cout << "\t\\___ \\ | | / _ \\| |  _|  _|   | | (_) | |_) | |/ _` |/ __/ _ \\ | |_  | |/ _ \\/ _ \\ __|\n";
	cout << "\t ___) || |/ ___ \\ |_| | |___  | |  _  |  __/| | (_| | (_|  __/ |  _| | |  __/  __/ |_ \n";
	cout << "\t|____/ |_/_/   \\_\\____|_____| |_| (_) |_|   |_|\\__,_|\\___\\___| |_|   |_|\\___|\\___|\\__|\n\n";  
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << "\t\t1. Enter the coordinate points of each ship placement. (1-10)\n\n";
	cout << "\t\t2. Enter the direction to place the ship. (U-D-L-R)\n\n\n\n";

	cout << flush;
	system("PAUSE");	// waits for keystroke
	system("CLS");		// clears screen
}

// Precondition :	using namespace std, the iostream library is included
// Postcondition :	the transition page and firing instructions are printed, then cleared after keystroke
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

// Precondition :	a 3 diminsional character array 4*10*10 is passed by reference
// Postcondition :	the game boards are set to all "~" to represent water
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

// Precondition :	using namespace std, the iostream library is included, one game board (2 diminsional character array 10*10) is passed by reference
// Postcondition :	the game board is printed to the console accompanied by x and y axis labels
void printBoard(char gameBoard[10][10])
{
	cout << "   1 2 3 4 5 6 7 8 9 10\n"; // x axis label
	for (int y = 0; y < 10; y++)
	{
		cout << setw(2) << y + 1; // y axis laebel
		for (int x = 0; x < 10; x++)
		{
			cout << " " << gameBoard[x][y];
		}
		cout << endl;
	}
}

// Precondition :	integer coordinates x&y (1-10) are passed as parameters, a direction character(U,u,D,d,L,l,R,r) is passed by parameter,
//					a character to be printed to represent the ship is passed as a parameter, one game board (2 diminsional character array 10*10) is passed by reference
// Postcondition :	the ship placement loaction is first checked for validity and the function should return false if placement is invalid game board should then be modified, 
//					placing the ship (represented by '#') according to the coordinates, direction, and length.
bool placeShip(int x, int y, int length, char direction, char shipChar, char gameBoard[10][10])
{
	switch (direction)
	{
	case 'U':
	case 'u':
		for (int i = y; i > y - length; i--)
			if (gameBoard[x][i] == '#' || i < 0) return false; // checks upward ship placement, returns false if already occupied
		for (int i = y; i > y - length; i--)
			gameBoard[x][i] = shipChar; // sets gameboard to shipChar at ship placement location
		break;
	case 'D':
	case 'd':
		for (int i = y; i < y + length; i++)
			if (gameBoard[x][i] == '#' || i > 9) return false; // checks downward ship placement, returns false if already occupied
		for (int i = y; i < y + length; i++)
			gameBoard[x][i] = shipChar;	// sets gameboard to shipChar at ship placement location
		break;
	case 'R':
	case 'r':
		for (int i = x; i < x + length; i++)
			if (gameBoard[i][y] == '#' || i > 9) return false; // checks right ship placement, returns false if already occupied
		for (int i = x; i < x + length; i++)
			gameBoard[i][y] = shipChar;	// sets gameboard to shipChar at ship placement location
		break;
	case 'L':
	case 'l':
		for (int i = x; i > x - length; i--)	
			if (gameBoard[i][y] == '#' || i < 0) return false; // checks left ship placement, returns false if already occupied
		for (int i = x; i > x - length; i--)
			gameBoard[i][y] = shipChar;	// sets gameboard to shipChar at ship placement location
		break;
	}
	return true;
}

// Precondition :	using namespace std, the iostream library is included, the player fleet board (2 diminsional character array 10*10) is passed by reference,
//					a struct array of ship names and lengths is passed as a parameter
// Postcondition :	the player fleet is modified, placing the 5 ship types represented by "#", the player selects the x and y coordinates and direction each ship,
//					any errors in player entry and or ship placement location should be handled and the player prompted to reenter values.
void playerSetup(char gameBoard[10][10], struct shipType ship[5])
{
	char gameBoardTemp[10][10];	// temporary game board to print placement crosshair and direction options
	int xCoord;
	int yCoord;
	char direction;
	bool inputError = false;	// true if unexpected input type/character from the player
	bool spaceOccupied = false;	// true if the placement location is already occupied by a ship or is off the board
	string playerInput;

	for (int i = 0; i < 5; i++) // increments through 5 ship types
	{
		memcpy(gameBoardTemp, gameBoard, sizeof(gameBoardTemp));	// actual player fleet board is copied to gameBoardTemp
		for (int j = 0; j < 3; j++)	// increments through three input cases ( x, y, direction )
		{
			printBoard(gameBoardTemp);
			cout << endl << ship[i].name << endl;			// prints ship name
			cout << "Length: " << ship[i].length << endl;	// prints ship length
			switch (j)
			{
			case 0:	// x coordinate input
				if (inputError) cout << "\t[COORDINATE POINT OUT OF RANGE: 1-10]\r";	// error message printed if last input was out of range
				if (spaceOccupied) cout << "\t[SPACE ALREADY OCCUPIED]\r";				// error message printed if x coodinate collumn is occupied
				cout << "X: ";	// prompts for x coordinate input

				if (!(cin >> xCoord) || xCoord < 1 || xCoord > 10)	// check if cin fails, x < 1, or x > 10
				{
					cin.clear();				// clears input stream to remove invalid input
					cin.ignore( 999, '\n');		// ignores the entire line of input (up to 999 characters)
					inputError = true;
				}
				else
				{
					inputError = false;
					spaceOccupied = true;
					for (int y = 0; y < 10; y++) // checks evey possible placement loaction and direction in x coordinate selection
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

				if (inputError || spaceOccupied) j = j - 1;	// repeeats y input case
				else
				{
					for (int y = 0; y < 10; y++) if (gameBoardTemp[xCoord - 1][y] == '~') gameBoardTemp[xCoord - 1][y] = '|'; // prints vertical crosshair on temp board
				}
				break;
			case 1: // y coordinate input
				cout << "X: " << xCoord << endl;
				if (inputError) cout << "\t[COORDINATE POINT OUT OF RANGE: 1-10]\r";	// error message printed if last input was out of range
				if (spaceOccupied) cout << "\t[SPACE ALREADY OCCUPIED]\r";				// error message printed if placement location is occupied
				cout << "Y: ";	// prompts for y coordinate input

				if (!(cin >> yCoord) || yCoord < 1 || yCoord > 10) // checks if cin fails, y < 1, or y > 10
				{
					cin.clear();				// clears input stream to remove invalid input
					cin.ignore( 999, '\n');		// ignores the entire line of input (up to 999 characters)
					inputError = true;
				}
				else
				{
					inputError = false;
					if (placeShip(xCoord - 1, yCoord - 1, ship[i].length, 'U', '~', gameBoard) ||
						placeShip(xCoord - 1, yCoord - 1, ship[i].length, 'D', '~', gameBoard) ||
						placeShip(xCoord - 1, yCoord - 1, ship[i].length, 'L', '~', gameBoard) ||
						placeShip(xCoord - 1, yCoord - 1, ship[i].length, 'R', '~', gameBoard))		// checks if placement is possible in any direction at coordinates
					{
						spaceOccupied = false;
					}
					else spaceOccupied = true;
				}

				if (inputError || spaceOccupied) j = j - 1;	// repeeats y input case
				else
				{
					for (int x = 0; x < 10; x++) if (gameBoardTemp[x][yCoord - 1] == '~') gameBoardTemp[x][yCoord - 1] = '-'; // prints horizontal crosshair on temp board
					placeShip(xCoord - 1, yCoord - 1, ship[i].length, 'U', 'U', gameBoardTemp);
					placeShip(xCoord - 1, yCoord - 1, ship[i].length, 'D', 'D', gameBoardTemp);
					placeShip(xCoord - 1, yCoord - 1, ship[i].length, 'L', 'L', gameBoardTemp);
					placeShip(xCoord - 1, yCoord - 1, ship[i].length, 'R', 'R', gameBoardTemp); // prints UDLR characters in all possible placement directions on the croshair
					gameBoardTemp[xCoord - 1][yCoord - 1] = 'X'; // prints X at center of crosshair on temp board
				}
				break;
			case 2:	// direction input
				cout << "X: " << xCoord << endl;
				cout << "Y: " << yCoord << endl;
				if (spaceOccupied) cout << "\t\t[SPACE ALREADY OCCUPIED]\r";				// error message printed if placement location is occupied
				if (inputError) cout << "\t\t[INVALID DIRECTION SELECTION: U-D-L-R ]\r";	// error message printed if last input character was invalid
				cout << "Direction: ";	// prompts user for direction input

				cin >> playerInput;		// temporarily stores user input in a string
				direction = playerInput.at(0);	// sets direction to first character of string ( not a perfect solution, but removes input error problems and accept yes/no full word answers)

				if (!(direction == 'U' || direction == 'u' ||
					direction == 'D' || direction == 'd' ||
					direction == 'L' || direction == 'l' ||
					direction == 'R' || direction == 'r')) // checks if direction character is valid
				{
					inputError = true;
				}
				else
				{
					inputError = false;
					if (!(placeShip(xCoord - 1, yCoord - 1, ship[i].length, direction, '#', gameBoard))) spaceOccupied = true; // checks if valid placement location, and places ship on player fleet board
					else spaceOccupied = false;
				}

				if (inputError || spaceOccupied) j = j - 1; // repeats direction selection case
				break;
			}
			cout << flush;
			system("CLS"); // clears screen after each input
		}
	}
}

// Precondition :	using namespace std, "time.h" is included, the iostream library is included (for debugging printout),  the player computer board board
//					(2 diminsional character array 10*10) is passed by reference, a struct array of ship names and lengths is passed as a parameter
// Postcondition :	all five ship types are placed on the computer fleet board are random coordinates and directions, the ships should not overlap or extend beyond the boundaries of the board
void computerSetup(char gameBoard[10][10], struct shipType ship[5])
{
	srand(time(NULL)); // sets random number seed to current time

	for (int i = 0; i < 5; i++) // increments through all 5 ship types
	{
		int x = rand() % 9; // generates random coordinates and direction selection
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

		if (!(placeShip(x, y, ship[i].length, direction, '#', gameBoard))) i = i - 1; // repeats ship type if the space is already occupied
	}

	//Debugging Printout of computers fleet board
	if (debugging)
	{
		cout << "Computer Fleet: \n";
		printBoard(gameBoard);
		cout << flush;
		system("PAUSE");
		system("CLS");
	}
}

// Precondition :	using namespace std, the iostream library is included, the player shots board (2 diminsional character array 10*10) is passed by reference,
//					the computer fleet board (2 diminsional character array 10*10) is passed by reference and has been set up to contain the computers ships represented by #
// Postcondition :	following the player input, the playerShots ans computerFleet gameboard should be marked miss 'M' or hit 'H' if a computer ship was present at the location, 
//					any errors in player entry and or shot fire location should be handled and the player prompted to reenter values.
void playerFire(char playerShots[10][10], char computerFleet[10][10])
{
	int x;
	int y;
	bool inputError = false;		// true if unexpected input type/character from the player
	bool spaceOccupied = false;		// true if the placement location is already occupied by a ship or is off the board
	char playerShotsTemp[10][10];	// temporary game board to print crosshair

	memcpy(playerShotsTemp, playerShots, sizeof(playerShotsTemp)); // copies player shots to temporary board
	for (int i = 0; i < 3; i++) // increments through three input cases ( x, y, direction )
	{
		printBoard(playerShotsTemp);	// prints temp shots board
		cout << "\nTARGET COORDINATES\n";
		switch (i)
		{
		case 0: // x coordinate input
			if (inputError) cout << "\t[COORDINATE POINT OUT OF RANGE: 1-10]\r"; // error message printed if last input was out of range
			if (spaceOccupied) cout << "\t[SPACE ALREADY FIRED UPON]\r";		 // error message printed if shot location is occupied
			cout << "X: "; // prompts player for x input

			if (!(cin >> x) || x > 10 || x < 0) // check if cin fails, x < 1, or x > 10
			{
				cin.clear();				// clears input stream to remove invalid input
				cin.ignore(999, '\n');		// ignores the entire line of input (up to 999 characters)
				inputError = true;
			}
			else
			{
				inputError = false;
				spaceOccupied = true;
				for (int y1 = 0; y1 < 10; y1++) if (playerShots[x - 1][y1] == '~') spaceOccupied = false; // checks if entire column has been fired upon
			}

			if (inputError || spaceOccupied) i = i - 1; // repeats x case
			else for (int y1 = 0; y1 < 10; y1++) if (playerShotsTemp[x - 1][y1] == '~') playerShotsTemp[x - 1][y1] = '|'; // prints vertical crosshair to tempoaray shots board
			break;
		case 1: // y coordinate input
			cout << "X: " << x << endl;
			if (inputError) cout << "\t[COORDINATE POINT OUT OF RANGE: 1-10]\r"; // error message printed if last input was out of range
			if (spaceOccupied) cout << "\t[SPACE ALREADY FIRED UPON]\r";		 // error message printed if shot location is occupied
			cout << "Y: "; // prompts player for y input

			if (!(cin >> y) || y > 10 || y < 0) // checks if cin fails, y < 1, or y > 10
			{
				cin.clear();				// clears input stream to remove invalid input
				cin.ignore(999, '\n');		// ignores the entire line of input (up to 999 characters)
				inputError = true;
			}
			else
			{
				inputError = false;
				if (playerShots[x - 1][y - 1] != '~') spaceOccupied = true; // checks if space has already been fired upon
				else spaceOccupied = false;
			}
			if (inputError || spaceOccupied) i = i - 1;	// repeats y case
			else
			{
				for (int x1 = 0; x1 < 10; x1++) if (playerShotsTemp[x1][y - 1] == '~') playerShotsTemp[x1][y - 1] = '-'; // prints horizontal crosshair to tempoaray shots board
				if (computerFleet[x - 1][y - 1] == '#') // sets all corresponding boards to hit 'H' or miss 'M'
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
		case 2: // shot result printout
			cout << "X: " << x << endl;
			cout << "Y: " << y << endl << endl;
			if (computerFleet[x - 1][y - 1] == 'H') cout << "HIT!"; // prints resuts HIT/MISS after each
			else cout << "MISS!";

			//debugging printout of player shots on computer fleet board
			if (debugging)
			{
				cout << "\n\nComputer Fleet:\n";
				printBoard(computerFleet);
			}

			cout << endl << endl << flush;
			system("PAUSE"); // waits for keystroke to continue
			break;
		}
		cout << flush;
		system("CLS"); // clears screen after each input/printout
	}
}

// Precondition :	using namespace std, the iostream library is included, the computer shots board (2 diminsional character array 10*10) is passed by reference,
//					the player fleet board (2 diminsional character array 10*10) is passed by reference and has been set up to contain the computers ships represented by #
// Postcondition :	the computer should generate random target coordinates, check them as a valid location, and the computerShots and playerFleet gameboard should be marked
//					miss 'M' or hit 'H' if a computer ship was present at the location. After firing, the playerFleet board should be printed displaying the current state of
//					their fleet. The player shold then be allowed to surrender if desired, handling any invalid user resonses appropriately. The function should return true
//					if the player whishes to surrender
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

		// debugging printout of computer shots board
		if (debugging)
		{
			cout << "\n\nComputer Shots:\n";
			printBoard(computerShots);
			cout << endl << endl;
		}

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
	} while (inputError);
}

// Precondition :	a fleet gameboard (2 diminsional character array 10*10, containing ships '#', hits 'H', and misses 'M') is passed by reference.  
// Postcondition :	the array should be checked for the presence of any '#', indicating the fleet is still alive. I
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

// Precondition :	using namespace std, the iostream library is included, the player 'win' boolean, and wins and losses integers are passed by value
// Postcondition :	a win or loss screen should be printed accoriding to the 'win' boolean, along with a counter of total wins and losses. The player should then be asked to
//					play again, if yes, the function should return true. Any error in player input should be handled appropiately. 
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