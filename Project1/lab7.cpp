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

using namespace std;

void resetGame(char gameBoard[4][10][10]);
void printBoard(char board[10][10]);
void playerSetup();
void placeShip();
void computerSetup();
void playerFire();
void computerFire();
bool checkWin();

int main()
{
	char gameBoard[4][10][10];

	//bool win;
	bool playAgain = false;

	do {
		resetGame(gameBoard);
		printBoard(gameBoard[1]);
		playerSetup();
		/*
		while (win == false)
		{
						
		}*/
	} while (playAgain == true);
	
	return 0;
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
	for (int y = 1; y < 11; y++)
	{
		cout << setw(2) << y;
		for (int x = 0; x < 10; x++)
		{
			cout << " " << gameBoard[x][y];
		}
		cout << endl;
	}
}

void playerSetup()
{
	int xCoordinate;
	int yCoordinate;
	char direction;
	bool inputError = false;

	cout << "Before begining play, you must place each ship of your fleet on the game board.\n";
	cout << "First, enter the x and y coordinates (1-10), then the direction of the ship (U,D,L,R).\n\n";
	
	cout << "~ ~ ~ ~ ~ ~ ~\n";
	cout << "~ # # # # # ~\n";
	cout << "~ ~ ~ ~ ~ ~ ~\n";
	cout << "Ship: Carrier\n";
	cout << "Length: 5 Units\n\n";

	do {
		cout << "X: ";
		cin >> xCoordinate;
		if (xCoordinate > 10 || xCoordinate < 0)
		{

		}
	}
	


	cout << "~ ~ ~ ~ ~ ~\n";
	cout << "~ # # # # ~\n";
	cout << "~ ~ ~ ~ ~ ~\n";
	cout << "Ship: Battleship\n";
	cout << "Length: 4 Units\n\n";

	cout << "~ ~ ~ ~ ~\n";
	cout << "~ # # # ~\n";
	cout << "~ ~ ~ ~ ~\n";
	cout << "Ship: Cruiser\n";
	cout << "Length: 3 Units\n\n";

	cout << "~ ~ ~ ~ ~\n";
	cout << "~ # # # ~\n";
	cout << "~ ~ ~ ~ ~\n";
	cout << "Ship: Submarine\n";
	cout << "Legnth: 3 Units\n\n";

	cout << "~ ~ ~ ~\n";
	cout << "~ # # ~\n";
	cout << "~ ~ ~ ~\n";
	cout << "Ship: Destroyer\n";
	cout << "Length: 2 Units\n\n";
}

void placeShip(string name, int length)
{
	for (int i = 0; i < (length + 2); i++) { cout << "~ "; }
	cout << endl << "~ ";
	for (int i = 0; i < length; i++) { cout << "# "; }
	cout << "~ " << 

	cout << "Ship: " << name << endl;
	cout << "Length: " << length << " Units\n";

}