//----------------------------------------------------------------------------
// File:	SinkTheFleetleet.cpp
// 
// Function:
//      main()
//----------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include "fleet.h"

// The following are definitions used within the game itself.  They refer to
// array members of arrays of structs used for enemy ship targeting.
#define SHOT_LOC game[whichPlayer].m_gameGrid[1][tempLoc.m_row][tempLoc.m_col]
#define E_TRGT game[!whichPlayer].m_gameGrid[0][tempLoc.m_row][tempLoc.m_col]
#define SHIP_PIECES_LEFT game[!whichPlayer].m_ships[E_TRGT].m_piecesLeft
#define SHIP_NAME game[!whichPlayer].m_ships[E_TRGT].m_name


using namespace std;

extern const char* shipNames[7];
//----------------------------------------------------------------------------
// Function:	main()
// Title:		Set ShipInfo
// Description:
//				Runs the Sink the Fleet Game
// Programmer:	Paul Bladek
// modified by:
// 
// Date:		2/1/2016
//
// Version:		1.1
// 
// Environment: Hardware: i7
//              Software: OS: Windows 10; 
//              Compiles under Microsoft Visual C++ 2013
//
// Input:		Grid size selection (whether small or large)
//				Whether the current player wants to read the starting grid
//					from a file.
//				The prompt for a file name when the player wants to read from
//					a file.
//				Prompt whether the player is ok with the grid layout from a
//					file.
//				Coordinates for firing at opponent's ship.
//				Whether to play again once the game is over.
//				
// Output:		Prompts for above inputs.
//
// Calls:		initializePlayer()
//				allocMem()
//				safeChoice()
//				getGrid()
//				printGrid()
//				resetGrid()
//				setships()
//				header()
//				getCoord()
//				deleteMem()
//				endBox()
//
// Called By:	n/a
//
// Parameters:	void
// 
// Returns:		int -- EXIT_SUCCESS upon successful completion
//
// History Log: 
//				12/9/2012 PB completed v 0.5
//				1/20/2016 JM added some minor changes v 0.6
//				1/29/2016	Completed v 1.0
//				2/1/2016 DL	Fixed an issue where the game would progress even
//							after a player enters an invalid file name.
//								v. 1.1
//----------------------------------------------------------------------------
int main(void)
{
	// Declare variables and initialize
	short numRows = SMALLROWS;			// total # of rows in the array
	short numCols = SMALLCOLS;			// total # of columns in the array
	char again = 'N';
	char gridSize = 'S';
	short whichPlayer = 0;
	short printShip = 0;
	bool isGoodFile = true;				// True if the file player is reading
										// is valid.
	bool gameOver = false;
	Cell coord = { 0, 0 };
	string message;
	string filename;
	Ship shipHit = NOSHIP;
	Player game[NUMPLAYERS];			// the two players in an array
	string prompt;
	Cell tempLoc = { 0, 0 };			// Set of coordinates used in
	// targeting enemy ships.

	// Begin game setup.
	do
	{
		gameOver = false;
		system("cls");
		cout << endl;
		header(cout);
		gridSize = safeChoice("Which size grid do you want to use", 'S', 'L');

		if (gridSize == 'L')
		{
			numRows = LARGEROWS;
			numCols = LARGECOLS;
		}
		else
		{
			numRows = SMALLROWS;
			numCols = SMALLCOLS;
		}
		// Allocate memory for the rows of the array
		allocMem(game, gridSize);

		// Initialize players' ship grids.
		initializePlayer(game);
		initializePlayer(game + 1);
		
		// Set up the board for each player.
		for (whichPlayer = 0; whichPlayer < NUMPLAYERS; whichPlayer++)
		{
			do
			{
				resetGrid(game, gridSize, whichPlayer);
				system("cls");
				prompt = "Player " + to_string(whichPlayer + 1) +
					", would you like to read starting from a grid file? ";
				char selection = safeChoice(prompt);
				if (selection == 'Y')
				{
					do
					{
						cout << "Enter file name: ";
						(cin >> prompt).get();
						prompt = prompt + ".shp";
						isGoodFile = getGrid(game, whichPlayer, gridSize,
							prompt);
					} while (!isGoodFile);
					if (safeChoice
						("Are you ok with your grid layout?") == 'N')
						isGoodFile = false;
				}
				else
				{
					setships(game, gridSize, whichPlayer);
				}
			} while ((!isGoodFile));
		}// End for whichPlayer block (the loop that sets up players' ship
		//	grid).

		// Clear the screen and display the begin game screen.
		system("cls");
		cout << endl;
		header(cout);
		cout << "Press <enter> to start the battle!";
		cin.ignore(FILENAME_MAX, '\n');
		whichPlayer = 0;

		// The game itself begins here.
		while (!gameOver)
		{
			// Accessing arrays of structs of arrays takes up a lot of
			// characters!  Therefore, we have declared some #define
			// macros to simplify access.
			// SHOT_LOC, short for Shot Location, replaces
			// game[whichPlayer].m_gameGrid[1][tempLoc.m_row][tempLoc.m_col].
			// E_TRGT, short for enemy target (referring to the enemy ship if
			// hit), replaces
			// game[!whichPlayer].m_gameGrid[0][tempLoc.m_row][tempLoc.m_col].
			// SHIP_PIECES_LEFT replaces
			// game[!whichPlayer].m_ships[E_TRGT].m_piecesLeft.
			// SHIP_NAME replaces
			// game[!whichPlayer].m_ships[E_TRGT].m_name.
			printGrid(cout, game[whichPlayer].m_gameGrid[1], gridSize);
			prompt = "Player " + to_string(whichPlayer + 1) +
				", enter coordinates for firing:\n";
			cout << prompt;
			tempLoc = getCoord(cin, gridSize);
			if (SHOT_LOC == HIT || SHOT_LOC == MISSED)
			{
				cout << "You already fired there. Try again.";
				cin.ignore(FILENAME_MAX, '\n');
				continue;
			}
			if (E_TRGT > 0)
			{
				SHOT_LOC = HIT;
				printGrid(cout, game[whichPlayer].m_gameGrid[1], gridSize);
				cout << "HIT!" << endl;
				--SHIP_PIECES_LEFT;
				--game[!whichPlayer].m_piecesLeft;
				if (game[!whichPlayer].m_piecesLeft == 0)
					gameOver = true;
				if (SHIP_PIECES_LEFT == 0)
				{
					cout << shipNames[SHIP_NAME] << " SUNK!" << endl;
				}
			}
			else
			{
				SHOT_LOC = MISSED;
				printGrid(cout, game[whichPlayer].m_gameGrid[1], gridSize);
				cout << "MISS!" << endl;
				whichPlayer = !whichPlayer;  // switch players
			}
			cout << "Press <enter> to continue.";
			cin.ignore(FILENAME_MAX, '\n');
		} // End the while(!gameOver) block.

		// Once a player's fleet has been sunk completely, bring up the
		// congratulations screen and ask if the players would like to play
		// again.
		system("CLS");
		endBox(whichPlayer);
		cout << endl;
		again = safeChoice("Would you like to play again?", 'Y', 'N');
		try
		{
			deleteMem(game, gridSize);
		}
		catch (exception e)
		{
			cerr << "oops!";
			cerr << e.what();
			cin.get();
		}
		// function safeChoice in safeio.cpp
	} while (again == 'Y');

	// If the players do not want to play again, exit the game.
	system("cls");
	cout << endl;
	header(cout);
	cout << "Thanks for playing! Press <enter> to exit.";
	cin.ignore(FILENAME_MAX, '\n');
	return EXIT_SUCCESS;
}
