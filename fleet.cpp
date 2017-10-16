//----------------------------------------------------------------------------
// File:	fleet.cpp
// 
// Functions: 
//		setShipInfo()	
//		allocMem() 
//		deleteMem()
//		printShip() 
//		printGrid() 
//		initializePlayer() 
//		setships()	      
//		saveGrid()
//		getGrid()
//		resetGrid()
//		getCoord()
//		validLocation() 
//		header() 
//		endBox() 
//----------------------------------------------------------------------------
#include "fleet.h"
#define BOW_ROW players[whichPlayer].m_ships[j].m_bowLocation.m_row
#define BOW_COL players[whichPlayer].m_ships[j].m_bowLocation.m_col
#define COL_HORIZ players[whichPlayer].m_ships[j].m_bowLocation.m_col + k
#define ROW_VERT players[whichPlayer].m_ships[j].m_bowLocation.m_row + k

const char* shipNames[SHIP_SIZE_ARRAYSIZE] =
{ "No Ship", "Mine Sweeper", "Submarine", "Frigate",
"Battleship", "Aircraft Carrier" };
const int TOTALPIECES = 17; // total pieces in all ships

//----------------------------------------------------------------------------
// Function:	initializePlayer()
// Title:		Initialize Player 
// Description:
//				sets initial values for m_ships and m_piecesLeft
//
// Programmer:	Paul Bladek
//				David G Landry
//				Jason M Gautama
// 
// Date:		1/30/13
//
// Version:		1.1
// 
// Environment: Hardware: i7
//              Software: OS: Windows 10; 
//              Compiles under Microsoft Visual C++ 2013
//
// Calls:		setShipInfo()
//
// Called By:	main()
//
// Parameters:	playerPtr: Player*;	a pointer to the current Player
// 
// Returns:		void
//
// History Log: 
//		9/12/06 PB completed v 1.0
//		1/30/16 DL & JM v1.1; added setShipInfo into the Calls part 
//		of the header 
//----------------------------------------------------------------------------
void initializePlayer(Player* playerPtr)
{
	for (short i = 0; i < SHIP_SIZE_ARRAYSIZE; i++)
		setShipInfo(playerPtr->m_ships + i, static_cast<Ship>(i));

	playerPtr->m_piecesLeft = TOTALPIECES;
}

//----------------------------------------------------------------------------
// Function:	setShipInfo()
// Title:		Set ShipInfo
// Description:
//				Sets struct ShipInfo fields
// Programmer:	David G Landry
//				Jason M Gautama
// 
// Date:		1/30/16
//
// Version:		1.0
// 
// Environment: Hardware: i7
//              Software: OS: Windows 10; 
//              Compiles under Microsoft Visual C++ 2013
//
// Calls:		n/a
//
// Called By:	initializePlayer()
//
// Parameters:	shipInfoPtr: ShipInfo *; pointer to the ShipInfo to be set
//				name: Ship;	enumerated name of type of ship; default: NOSHIP
//				orientation: Direction;	enumerated direction; 
//						default: HORIZONTAL
//				row: unsigned short; row-coordinate in grid; default: 0
//				col: unsigned short; column-coordinate in grid; default: 0
// 
// Returns:		void
//
// History Log:
//		12/20/05 PB completed v 0.1
//		1/30/16 DL & JM v1.0; fixed the header of the function   
//----------------------------------------------------------------------------
void setShipInfo(ShipInfo * shipInfoPtr, Ship name, Direction orientation,
	unsigned short row, unsigned short col)
{
	shipInfoPtr->m_bowLocation = { row, col };
	shipInfoPtr->m_name = name;
	shipInfoPtr->m_orientation = orientation;
	shipInfoPtr->m_piecesLeft = shipSize[name];
}

#include "fleet.h"

//---------------------------------------------------------------------------------
// Function:	allocMem()
// Title:		Allocate Memory
// Description:
//		allocates memory for current grids
// Programmer:	Paul Bladek
// modified by:	
// 
// Date:	9/13/06
//
// Version:	1.01
// 
// Environment: Hardware: i3 
//              Software: OS: Windows 7; 
//              Compiles under Microsoft Visual C++ 2012
//
// Calls:
//
// Called By:	main()
//
// Parameters:	players: Player[]; 	the array of 2 players in the game
//		size: char;	'	S' or 'L' (small or large)
// 
// Returns:	void
//
// History Log:
//		12/20/05 PB completed v 1.0 
//		9/13/06  PB completed v 1.01
//
//---------------------------------------------------------------------------------
void allocMem(Player players[], char size)
// Array of 2 players.  Player is a struct containing Ship, ShipInfo, & short int.
// Each player has 2 arrays.  0 keeps track of ship location.  1 keeps track of hits & misses.
// 0 has NOSHIP or one of several types of ships.
// 1 has NOSHIP or hit or miss.
// size is either small or large
{
	short numberOfRows = (toupper(size) == 'L') ? LARGEROWS : SMALLROWS;
	short numberOfCols = (toupper(size) == 'L') ? LARGECOLS : SMALLCOLS;

	try
	{
		for (short i = 0; i < NUMPLAYERS; ++i)
		{
			players[i].m_gameGrid[0] = nullptr;
			players[i].m_gameGrid[0] = new Ship*[numberOfRows];
			players[i].m_gameGrid[1] = nullptr;
			players[i].m_gameGrid[1] = new Ship*[numberOfRows];
			for (short j = 0; j < numberOfRows; ++j)
			{
				players[i].m_gameGrid[0][j] = nullptr;
				players[i].m_gameGrid[0][j] = new Ship[numberOfCols];
				players[i].m_gameGrid[1][j] = nullptr;
				players[i].m_gameGrid[1][j] = new Ship[numberOfCols];
				for (short k = 0; k < numberOfCols; ++k)
				{
					players[i].m_gameGrid[0][j][k] = NOSHIP;
					players[i].m_gameGrid[1][j][k] = NOSHIP;
				} // end for k
			} // end for j
		} // end for i
	}
	catch (bad_alloc e)
	{
		deleteMem(players, size);
		cerr << "exception: " << e.what() << endl;
		cout << "shutting down" << endl;
		cin.ignore(BUFFER_SIZE, '\n');
		exit(EXIT_FAILURE);
	}
	catch (exception e)
	{
		deleteMem(players, size);
		cerr << "exception: " << e.what() << endl;
		cout << "shutting down" << endl;
		cin.ignore(BUFFER_SIZE, '\n');
		exit(EXIT_FAILURE);
	}
}

//---------------------------------------------------------------------------------
// Function:	deleteMem()
// Title:		Delete Memory
// Description:
//		Safely deletes memory for grids
// Programmer:
// 
// Date:	2/22/2016
//
// Version:	1.1
// 
// Environment: Hardware: i3 
//              Software: OS: Windows 7; 
//              Compiles under Microsoft Visual C++ 2012
//
// Calls:
//
// Called By:	main()
//
// Parameters:	players: Player[]; 	the array of the 2 Players
//		size: char;	'	S' or 'L'
// 
// Returns:	void
//
// History Log:
//		12/20/05 PB completed v 0.1
//		2/22/16 DL & JM completed v 1.1- We streamlined the code, deleting
//					repetitive code for ease of reading.
//---------------------------------------------------------------------------------
void deleteMem(Player players[], char size)
{
	short numberOfRows = (toupper(size) == 'L') ? LARGEROWS : SMALLROWS;
	short numberOfCols = (toupper(size) == 'L') ? LARGECOLS : SMALLCOLS;

	// your code goes here ...
	// delete[] in reverse order of allocMem()
	// be sure to check if the memory was allocated (!nullptr) BEFORE deleting

	// First, check to see if the pointer to pointer is null
	try
	{
		for (short i = 0; i < NUMPLAYERS; ++i)
		{
			for (short h = 0; h < NUMPLAYERS; ++h)
			{
				if (players[i].m_gameGrid[h] != nullptr)
				{
					// Free memory of each individual row for grid 0
					// (grid of ships)
					for (short j = 0; j < numberOfRows; ++j)
					{
						if (players[i].m_gameGrid[h][j] != nullptr)
						{
							delete[] players[i].m_gameGrid[h][j];
							players[i].m_gameGrid[h][j] = nullptr;
						}
					}
				}
			}
			
		} // end for i
		for (short i = 0; i < NUMPLAYERS; ++i)
		{
			// Clears the memory allocated to the players.
			if (players[i].m_gameGrid[0] != nullptr)
			{
				delete[] players[i].m_gameGrid[0];
				players[i].m_gameGrid[0] = nullptr;
			}
			if (players[i].m_gameGrid[1] != nullptr)
			{
				delete[] players[i].m_gameGrid[1];
				players[i].m_gameGrid[1] = nullptr;
			}
		}// end for i
	}// end try block.


	catch (bad_alloc e)
	{
		deleteMem(players, size);
		cerr << "exception: " << e.what() << endl;
		cout << "shutting down" << endl;
		cin.ignore(BUFFER_SIZE, '\n');
		exit(EXIT_FAILURE);
	}

	catch (exception e)
	{
		deleteMem(players, size);
		cerr << "exception: " << e.what() << endl;
		cout << "shutting down" << endl;
		cin.ignore(BUFFER_SIZE, '\n');
		exit(EXIT_FAILURE);
	}
}

//----------------------------------------------------------------------------
// Function:	printShip()
// Title:		Print Ship 
// Description:
//				Print grid element for the proper ship
// Programmer:	Paul Bladek
//				David G Landry
//				Jason M Gautama
// 
// Date:		1/30/16
//
// Version:		1.1
// 
// Environment: Hardware: i7
//              Software: OS: Windows 10; 
//              Compiles under Microsoft Visual C++ 2013
//
// Output:		three characters representing one ship to sout
//
// Calls:		n/a
//
// Called By:	printGrid()
//
// Parameters:	sout: ostream&;	the stream to print to
//				thisShip: Ship;	the Ship to be printed in the grid format
// 
// Returns:		void
//
// History Log: 
//		9/12/06 PB completed v 1.0
//		1/30/16 DL & JM v1.1; fixed header information
//     
//----------------------------------------------------------------------------
void printShip(ostream & sout, Ship thisShip)
{
	sout << ' ';
	switch (thisShip)
	{
	case NOSHIP: sout << ' ';
		break;
	case MINESWEEPER: sout << 'M';
		break;
	case SUB: sout << 'S';
		break;
	case FRIGATE: sout << 'F';
		break;
	case BATTLESHIP: sout << 'B';
		break;
	case CARRIER: sout << 'C';
		break;
	case HIT: sout << 'H';
		break;
	case MISSED: sout << MISS;
		break;
	default: sout << 'X';
	}
	sout << VERT;
}

//----------------------------------------------------------------------------
// Function:	printGrid()
// Title:		Print Ship 
// Description:
//		Print grid element for the proper ship
// Programmer:	David G Landry
//				Jason M Gautama
// 
// Date:		1/30/16
//
// Version:		1.0
// 
// Environment: Hardware: i7
//              Software: OS: Windows 10; 
//              Compiles under Microsoft Visual C++ 2013
//
// Output:		a single grid to sout
//
// Calls:		printShip()
//
// Called By:	main()
//				setships()
//				saveGrid()
//				getGrid()
//
// Parameters:	sout: ostream&;	the stream to print to
//		grid: Ship**;	the 2-D array of Ships 
//		size: char;	'S' or 'L'
// 
// Returns:	void
//
// History Log:
//		12/20/05 PB completed v 0.1
//		1/30/16 DL & JM v1.0; added the getGrid() info to Called By header
//     
//----------------------------------------------------------------------------
void printGrid(ostream& sout, Ship** grid, char size)
{
	short numberOfRows = (toupper(size) == 'L') ? LARGEROWS : SMALLROWS;
	short numberOfCols = (toupper(size) == 'L') ? LARGECOLS : SMALLCOLS;
	char rowHeader = 'A';
	system("cls");
	// Print the column headers- the numbers 1 to numberOfCols
	sout << ' ';
	for (short j = 1; j <= numberOfCols; ++j)
		sout << setw(3) << j;
	sout << endl;
	sout << HORIZ << CROSS;
	for (short j = 1; j <= numberOfCols - 1; ++j)
		sout << HORIZ << HORIZ << CROSS;
	sout << HORIZ << HORIZ << CL;
	sout << endl;

	// Print the rest of the grid- left to right, top to bottom
	for (short j = 0; j <= numberOfRows - 2; ++j)
	{
		sout << rowHeader;		// Prints the row header: A to numberOfRows
		sout << VERT;			// Prints a vertical line.
		for (short k = 0; k <= numberOfCols - 1; ++k)
		{
			// Alternate between blank space, ship (if it exists), and
			// a vertical line.
			printShip(sout, grid[j][k]);
		}
		sout << endl;
		sout << HORIZ << CROSS << HORIZ << HORIZ << CROSS;
		for (short l = 1; l <= numberOfCols - 2; ++l)
		{
			sout << HORIZ << HORIZ << CROSS;
		}
		sout << HORIZ << HORIZ << CL;
		sout << endl;
		rowHeader++;
	}

	// Print the final row, done separately due to the different lower border.
	sout << rowHeader << VERT;
	for (short k = 0; k <= numberOfCols - 1; ++k)
	{
		printShip(sout, grid[numberOfRows - 1][k]);
	}
	sout << endl;
	sout << HORIZ << CU;		// Gets the lower border started.
	for (short l = 1; l <= numberOfCols - 1; ++l)
	{
		// Fill the rest of the lower border.
		sout << HORIZ << HORIZ << CU;
	}
	// Put in the lower right border corner.
	sout << HORIZ << HORIZ << LR;
	sout << endl;
}

//----------------------------------------------------------------------------
// Function:		setships()
// Title:			Set Ships 
// Description:
//					Allows user to put ships in grid
// Programmers:		Paul Bladek
//
// modified by:		David G Landry
//					Jason M Gautama
// 
// Date:			1/12/06
//
// Version:			1.0
// 
// Environment:		Hardware: i7
//					Software: OS: Windows 10; 
//					Compiles under Microsoft Visual C++ 2013
//
// Input:			location and orientation using getCoord from cin
//
// Output:			prompts to cout
//
// Calls:			printGrid()
//					safeChoice()
//					getCoord()
//					saveGrid()
//
// Called By:		main()
//
// Parameters:		players: Player[];	the array of 2 players 
//					size: char;		'S' or 'L'
//					whichPlayer: short;	the player number (0 or 1)
// 
// Returns:			void
//
// History Log: 
//		9/12/06		PB completed v 0.5
//		1/25/16		JM & DL, v 1.0;	Finished code for placing ship and for
//					calling the saveGrid function
//     
//----------------------------------------------------------------------------
void setships(Player players[], char size, short whichPlayer)
{
	// Variable initializations:
	char input = 'V';
	char ok = 'Y';
	char save = 'N';
	ostringstream outSStream;
	Cell location = { 0, 0 };
	short j = 0;					// Used to represent the current ship.
	short k = 0;					// Used to calculate the location of each
									// of the ship's pieces.

	// Macros used:
	// BOW_ROW is used for current ship's bow row coordinate.  Substitutes:
	//		players[whichPlayer].m_ships[j].m_bowLocation.m_row
	// BOW_COL is used for current ship's bow column coordinate.  Substitutes:
	//		players[whichPlayer].m_ships[j].m_bowLocation.m_col
	// COL_HORIZ is used for current ship's non-bow location when oriented
	//		horizontally.  Substitutes:
	//		players[whichPlayer].m_ships[j].m_bowLocation.m_col + k
	// ROW_VERT is used for current ship's non-bow location when oriented
	//		vertically.  Substitutes:
	//		players[whichPlayer].m_ships[j].m_bowLocation.m_row + k

	for (j = 1; j < SHIP_SIZE_ARRAYSIZE; j++)
	{
		do
		{
			system("cls");
			printGrid(cout, players[whichPlayer].m_gameGrid[0], size);
			outSStream.str("");
			outSStream << "Player " << whichPlayer + 1 << " Enter "
				<< shipNames[j] << " orientation";
			input = safeChoice(outSStream.str(), 'V', 'H');
			players[whichPlayer].m_ships[j].m_orientation
				= (input == 'V') ? VERTICAL : HORIZONTAL;
			printGrid(cout, players[whichPlayer].m_gameGrid[0], size);
			cout << "Player " << whichPlayer + 1 << " Enter " << shipNames[j]
				<< " bow coordinates <row letter><col #>.\n";
			players[whichPlayer].m_ships[j].m_bowLocation =
				getCoord(cin, size);

			// if ok
			if (!validLocation(players[whichPlayer], j, size))
			{
				cout << "invalid location. Press <enter>";
				cin.get();
				j--; // redo
				continue;
			}
			else
			{
				players[whichPlayer].m_gameGrid[0][BOW_ROW][BOW_COL]
					= static_cast<Ship>(j);
				if (players[whichPlayer].m_ships[j].m_orientation ==
					HORIZONTAL)
				{
					for (k = 1; k < shipSize[j]; k++)
						players[whichPlayer].m_gameGrid[0][BOW_ROW][COL_HORIZ]
						= static_cast<Ship>(j);
				}
				if (players[whichPlayer].m_ships[j].m_orientation == VERTICAL)
				{
					for (k = 1; k < shipSize[j]; k++)
						players[whichPlayer].m_gameGrid[0][ROW_VERT][BOW_COL]
						= static_cast<Ship>(j);
				}
				printGrid(cout, players[whichPlayer].m_gameGrid[0], size);
				cout << "Player " << whichPlayer + 1 << " " << shipNames[j];
				input = safeChoice(" OK?");
			}// End else if block.
			if (input == 'N')
			{
				// Clear the grid of the current ship
				players[whichPlayer].m_gameGrid[0][BOW_ROW][BOW_COL]
					= static_cast<Ship>(0);
				if (players[whichPlayer].m_ships[j].m_orientation ==
					HORIZONTAL)
				{
					for (k = 1; k < shipSize[j]; k++)
						players[whichPlayer].m_gameGrid[0][BOW_ROW][COL_HORIZ]
						= static_cast<Ship>(0);
				}
				if (players[whichPlayer].m_ships[j].m_orientation == VERTICAL)
				{
					for (k = 1; k < shipSize[j]; k++)
						players[whichPlayer].m_gameGrid[0][ROW_VERT][BOW_COL]
						= static_cast<Ship>(0);
				}
			}// End if input == 'N'
		} while (input == 'N');
	} // end for j
	save = safeChoice("\nSave starting grid?", 'Y', 'N');
	if (save == 'Y')
		saveGrid(players, whichPlayer, size);
}

//----------------------------------------------------------------------------
// Function:		saveGrid()
// Title:			Save Grid 
// Description:
//					Saves the ship grid to a file
// Programmer:		David G Landry & Jason M Gautama
// 
// Date:			1/26/16
//
// Version:			1.0
// 
// Environment:		Hardware: i7
//					Software: OS: Windows 10;
//					Compiles under Microsoft Visual C++ 2013
//
// Input:			user writes the filename for the grid
//					to be saved
//
// Output:			grid to specified file
//
// Calls:			printGrid()
//
// Called By:		setships()
//
// Parameters:		player: sPlayer[];	the array of 2 players
//					whichPlayer: short; the player number (0 or 1) 
//					size: char;	'S' or 'L'
// 
// Returns:			void
//
// History Log:
//					12/20/05 PB completed v 0.1
//					1/26/16 JM & DL; Finished inserting prompts and entered 
//						code to check if the filename is valid and to save 
//						the grid to the given valid file name.
//     
//----------------------------------------------------------------------------
void saveGrid(Player players[], short whichPlayer, char size)
{
	short numberOfRows = (toupper(size) == 'L') ? LARGEROWS : SMALLROWS;
	short numberOfCols = (toupper(size) == 'L') ? LARGECOLS : SMALLCOLS;
	ofstream sout;
	string filename = "";
	bool validFile = false;
	char rowHeader = 'A';
	int testValue = 0;

	do
	{
		cout << "Please enter a file name to save to.\n" <<
			"The .shp will be added automatically: ";
		cin >> filename;
		if ((testValue = filename.find("\\", 0)) >= 0)

		{
			cout << endl << "File could not be saved because it contained " <<
				"illegal characters.\nPlease try again.\n";
			validFile = false;
			continue;

		}
		else if ((testValue = filename.find("<", 0)) >= 0)
		{
			cout << endl << "File could not be saved because it contained " <<
				"illegal characters.\nPlease try again.\n";
			validFile = false;
			continue;

		}
		else if ((testValue = filename.find(">", 0)) >= 0)
		{
			cout << endl << "File could not be saved because it contained " <<
				"illegal characters.\nPlease try again.\n";
			validFile = false;
			continue;

		}
		else if ((testValue = filename.find("*", 0)) >= 0)
		{
			cout << endl << "File could not be saved because it contained " <<
				"illegal characters.\nPlease try again.\n";
			validFile = false;
			continue;

		}
		else if ((testValue = filename.find("|", 0)) >= 0)
		{
			cout << endl << "File could not be saved because it contained " <<
				"illegal characters.\nPlease try again.\n";
			validFile = false;
			continue;

		}
		else if ((testValue = filename.find("/", 0)) >= 0)
		{
			cout << endl << "File could not be saved because it contained " <<
				"illegal characters.\nPlease try again.\n";
			validFile = false;
			continue;

		}
		else if ((testValue = filename.find("?", 0)) >= 0)
		{
			cout << endl << "File could not be saved because it contained " <<
				"illegal characters.\nPlease try again.\n";
			validFile = false;
			continue;

		}
		else if ((testValue = filename.find("\"", 0)) >= 0)
		{
			cout << endl << "File could not be saved because it contained " <<
				"illegal characters.\nPlease try again.\n";
			validFile = false;
			continue;

		}
		else if ((testValue = filename.find(":", 0)) >= 0)
		{
			cout << endl << "File could not be saved because it contained " <<
				"illegal characters.\nPlease try again.\n";
			validFile = false;
			continue;
		}
		validFile = true;
	} while (validFile == false);
	filename += ".shp";
	try
	{
		sout.open(filename);
		// At this point, the file should have been successfully opened.
		// We will begin by saving the grid size.
		sout.put(size);
		sout.put('\n');

		// Now we'll write the grid to the file.
		// Print the column headers- the numbers 1 to numberOfCols
		sout << ' ';
		for (short j = 1; j <= numberOfCols; ++j)
			sout << setw(3) << j;
		sout << endl;
		sout << HORIZ << CROSS;
		for (short j = 1; j <= numberOfCols - 1; ++j)
			sout << HORIZ << HORIZ << CROSS;
		sout << HORIZ << HORIZ << CL;
		sout << endl;

		// Print the rest of the grid- left to right, top to bottom
		for (short j = 0; j <= numberOfRows - 2; ++j)
		{
			sout << rowHeader;	// Prints the row header: A to numberOfRows

			sout << VERT;		// Prints a vertical line.

			for (short k = 0; k <= numberOfCols - 1; ++k)
			{
				// Alternate between blank space, ship (if it exists), and
				// a vertical line.
				printShip(sout, players[whichPlayer].m_gameGrid[0][j][k]);
			}
			sout << endl;
			sout << HORIZ << CROSS << HORIZ << HORIZ << CROSS;
			for (short l = 1; l <= numberOfCols - 2; ++l)
			{
				sout << HORIZ << HORIZ << CROSS;
			}
			sout << HORIZ << HORIZ << CL;
			sout << endl;
			rowHeader++;
		}

		// Print the final row, done separately due to the different lower
		// border.
		sout << rowHeader << VERT;
		for (short k = 0; k <= numberOfCols - 1; ++k)
		{
			printShip(sout, players[whichPlayer].m_gameGrid[0]
				[numberOfRows - 1][k]);
		}
		sout << endl;
		sout << HORIZ << CU;		// Gets the lower border started.
		for (short l = 1; l <= numberOfCols - 1; ++l)
		{
			// Fill the rest of the lower border.
			sout << HORIZ << HORIZ << CU;
		}
		// Put in the lower right border corner.
		sout << HORIZ << HORIZ << LR;
		sout << endl << endl;

		// Close the file and call it good.
		sout.close();
		cout << "File successfully saved!  Press \"Enter\" to continue.  ";
		cin.get();
		cin.ignore(FILENAME_MAX, '\n');
	}
	catch (exception e)
	{
		cout << "Error saving file.  Please try again.";
	}
}

//----------------------------------------------------------------------------
// Function:		getGrid()
// Title:			GetGrid 
// Description:
//					Reads grid from a file and properly sets the ships
// Programmer:		Paul Bladek
// modified by:		David G Landry
//					Jason M Gautama
// 
// Date:			2/22/16
//
// Version:			1.2
// 
// Environment:		Hardware: i7
//					Software: OS: Windows 10; 
//					Compiles under Microsoft Visual C++ 2013
//
// Input:			grid from specified file
//
// Output:			prompts to cout
//
// Calls:			printGrid()
//
// Called By:		main()
//
// Parameters:		players: Player[];	the array of 2 players
//					whichPlayer: short;	the player number  (0 or 1) 
//					size: char; 'S' or 'L'
//					string fileName: the name of the file to be opened
//					for reading
// 
// Returns:			bool -- true if the file is opened and read;
//					false otherwise
//
// History Log: 
//		9/12/06 PB comleted v 0.5
//		1/30/16 DL & JM v1.0; Finished writing up the code
//		2/1/16 DL	Fixed an issue where the filename checking would not
//					properly check for invalid file names.  v. 1.1
//		2/22/16 DL & JM V.1.2; Fixed a corrupted memory error that would
//					cause the program to crash after loading a grid from a
//					file
//----------------------------------------------------------------------------
#include "fleet.h"

bool getGrid(Player players[], short whichPlayer, char size, string fileName)
{
	// Initialize variables to default values.
	string line;
	ifstream ifs;
	Ship ship = NOSHIP;
	short shipCount[SHIP_SIZE_ARRAYSIZE] = { 0 };
	char cell = ' ';
	char fsize = 'S';
	short rows = 0;
	short cols = 0;
	short rowTest = 0;
	short colTest = 0;

	// Check if the board size is small or large.  If large, use the constants
	// for the large board.  If small, use the constants for small board.
	short numberOfRows = (toupper(size) == 'L') ? LARGEROWS : SMALLROWS;
	short numberOfCols = (toupper(size) == 'L') ? LARGECOLS : SMALLCOLS;

	try
	{
		// Try to open the file.
		ifs.open(fileName.c_str());
		if (!ifs)
		{
			cout << "could not open file " << fileName << endl
				<< " press <enter> to continue" << endl;
			cin.ignore(BUFFER_SIZE, '\n');
			return false;
		}
	}
	catch (exception e)
	{
		cout << "could not open file " << fileName << endl
			<< " press <enter> to continue" << endl;
		cin.ignore(BUFFER_SIZE, '\n');
		return false;
	}
	(ifs >> fsize).get(cell);
	if (fsize != size)
	{
		cout << "The board you loaded is not the correct size.  Please " <<
			"try again.  Press \"Enter\" to continue.";
		cin.ignore(FILENAME_MAX, '\n');
		return false;
	}
	for (rows = 0; rows < (numberOfRows * 2) + 1; rows++)
	{
		ifs.get(cell);
		if (cell != 'A' && cell != 'B' && cell != 'C' && cell != 'D' &&
			cell != 'E' && cell != 'F' && cell != 'G' && cell != 'H')
		{
			getline(ifs, line);
			continue;
		}
		ifs.get(cell);
		for (cols = 0; cols < (numberOfCols * 3) + 1; cols++)
		{
			rowTest = ((rows / 2) - 1);
			colTest = ((cols / 3));
			ifs.get(cell);
			if (cell == 'M')
				players[whichPlayer].m_gameGrid[0][(rows / 2) - 1][cols / 3]
				= MINESWEEPER;
			if (cell == 'S')
				players[whichPlayer].m_gameGrid[0][(rows / 2) - 1][cols / 3]
				= SUB;
			if (cell == 'F')
				players[whichPlayer].m_gameGrid[0][(rows / 2) - 1][cols / 3]
				= FRIGATE;
			if (cell == 'B')
				players[whichPlayer].m_gameGrid[0][(rows / 2) - 1][cols / 3]
				= BATTLESHIP;
			if (cell == 'C')
				players[whichPlayer].m_gameGrid[0][(rows / 2) - 1][cols / 3]
				= CARRIER;
		}
	} //End for loop block

	// Close the file and print the grid.
	ifs.close();
	printGrid(cout, players[whichPlayer].m_gameGrid[0], size);
	return true;
}

//----------------------------------------------------------------------------
// Function:		resetGrid()
// Title:			reset Grid 
// Description:
//					Clears the grid of all ships for the current player.
//
// Programmer:		David G Landry
//					Jason M Gautama
// 
// Date:			1/28/16
//
// Version:			1.0
// 
// Environment:		Hardware: i7
//					Software: OS: Windows 10; 
//					Compiles under Microsoft Visual C++ 2013
//
// Input:			n/a
//
// Output:			n/a
//
// Calls:			n/a
//
// Called By:		main()
//
// Parameters:		players[]: the array of players; size: 'L' or 'S' ;
//					whichPlayer: Player 1 or 2
// 
// Returns:			void
//					
// History Log: 
//					9/12/06 PB comleted v 0.5
//					1/28/16 DL & JM v1.0; Finished writing up the code
//----------------------------------------------------------------------------
void resetGrid(Player players[], char size, short whichPlayer)
{
	short numberOfRows = (toupper(size) == 'L') ? LARGEROWS : SMALLROWS;
	short numberOfCols = (toupper(size) == 'L') ? LARGECOLS : SMALLCOLS;

	for (short rows = 0; rows < (numberOfRows); rows++)
	{
		for (short cols = 0; cols < (numberOfCols); cols++)
		{
			players[whichPlayer].m_gameGrid[0][rows][cols] = NOSHIP;
			players[whichPlayer].m_gameGrid[1][rows][cols] = NOSHIP;
		}
	}
}

//----------------------------------------------------------------------------
// Function:		getCoord()
// Title:			Get Coordinates 
// Description:
//					Returns a cell with coordinates set by user
// Programmer:		Paul Bladek
// 
// Date:			9/12/06
//
// Version:			1.0
// 
// Environment:		Hardware: i3 
//					Software: OS: Windows 7; 
//					Compiles under Microsoft Visual C++ 2012
//
// Input:			cell coordinates (in the form "A13" from sin
//
// Output:			prompts to cout
//
// Calls:			none
//
// Called By:		main()
//					setships()
//
// Parameters:		sin: istream&;	the stream to read from
//					size: char;	'S' or 'L'
// 
// Returns:			Cell location -- a cell containing the input coordinates
//
// History Log: 
//		9/12/06 PB completed v 1.0
//      1/30/16 JM & DL v1.1; Improved the header information.
//----------------------------------------------------------------------------
Cell getCoord(istream& sin, char size)
{
	short numberOfRows = (toupper(size) == 'L') ? LARGEROWS : SMALLROWS;
	short numberOfCols = (toupper(size) == 'L') ? LARGECOLS : SMALLCOLS;
	char highChar = static_cast<char>(numberOfRows - 1) + 'A';
	char row = 'A';
	short col = 0;
	Cell location = { 0, 0 };
	do
	{
		col = 0;
		cout << "Row must be a letter from A to " << highChar
			<< " and column must be from 1 to " << numberOfCols << ": ";
		while ((row = toupper(sin.get())) < 'A' || row > highChar)
		{
			sin.ignore(BUFFER_SIZE, '\n');
			cout << "Row must be a letter from A to " << highChar
				<< " and column must be from 1 to " << numberOfCols << ": ";
		}
		sin >> col;
		if (!sin)
			sin.clear();
		sin.ignore(BUFFER_SIZE, '\n');
	} while (col < 1 || col > numberOfCols);

	location.m_col = col - 1;
	location.m_row = static_cast<short>(row - 'A');
	return location;
}

//----------------------------------------------------------------------------
// Function:		validLocation()
// Title:			Valid Location 
// Description:
//					Can the ship legitimately go there?
// Programmer:		David G Landry
//					Jason M Gautama
// 
// Date:			1/19/16
//
// Version:			1.0
// 
// Environment:		Hardware: i7
//					Software: OS: Windows 10; 
//					Compiles under Microsoft Visual C++ 2013
//
// Calls:			n/a
//
// Called By:		setships()
//
// Parameters:		player: const Player&;	a reference to the specific Player
//					shipNumber: short;	the number of the ship (1 - 5)
//					in the array player.m_ships[]
//					size: char;		'S' or 'L'
// 
// Returns:			bool -- true if the ship would not go off the edge
//					or cross another ship; 
//					false otherwise
//
// History Log:
//		12/20/05 PB completed v 0.1
//		1/19/16	 DL & JM v1.0; Completed writing the code
//----------------------------------------------------------------------------
bool validLocation(const Player& player, short shipNumber, char size)
{
	// Variable and constant declarations:
	short numberOfRows = (toupper(size) == 'L') ? LARGEROWS : SMALLROWS;
	short numberOfCols = (toupper(size) == 'L') ? LARGECOLS : SMALLCOLS;
	// The following are constants for the current ship, getting its 
	// coordinates.
	const short shipColumn =
		player.m_ships[shipNumber].m_bowLocation.m_col;
	const short shipRow =
		player.m_ships[shipNumber].m_bowLocation.m_row;
	// -----------------------------------------------------------------------
	// Tests if our current ship is vertical oriented.
	if (player.m_ships[shipNumber].m_orientation == VERTICAL)
	{
		// Test for row out-of-bounds
		if ((shipColumn < 0) || (shipColumn > numberOfCols - 1))
			return false;
		for (short i = shipRow; i < shipRow + shipSize[shipNumber]; i++)
		{
			// Test for out-of-bounds
			if ((i < 0) || (i > numberOfRows - 1))
				return false;

			// Test for other ships
			// First, obtain the other ship's code number one by one.
			for (short otherShips = 1; otherShips < shipNumber; otherShips++)
			{
				// Assign easier-to-work-with variables for the ship info.
				short otherShipCol =
					player.m_ships[otherShips].m_bowLocation.m_col;
				short otherShipRow =
					player.m_ships[otherShips].m_bowLocation.m_row;
				Direction otherShipOrientation =
					player.m_ships[otherShips].m_orientation;
				// Testing: our ship vertical, other ship vertical.
				if (otherShipOrientation == VERTICAL)
				{
					if (shipColumn == otherShipCol)
					{
						for (short j = otherShipRow; j < otherShipRow +
							shipSize[otherShips]; j++)
						{
							if (j == i)
								return false;
						}// End for j = otherShipRow to otherShipRow + 
							//shipSize[otherShips]
					}// End if shipColumn == otherShipCol
				}// End if otherShipOrientation == VERTICAL

				// Testing: our ship vertical, other ship horizontal.
				if (otherShipOrientation == HORIZONTAL)
				{
					for (short j = otherShipCol; j < otherShipCol + shipSize
						[otherShips]; j++)
					{
						if (shipColumn == j)
						{
							if (i == otherShipRow)
								return false;
						}// End if shipColumn == j
					}// End for j = otherShipCol to otherShipCol + 
						//shipSize[otherShips]
				}// End if otherShipOrientation == HORIZONTAL
			}// End for otherShips (end testing for other ships)
		}// End for i = shipRow to shipRow + shipSize[shipNumber]
	}// End if ship orientation == VERTICAL

	// -----------------------------------------------------------------------
	// Now we'll run tests for if our current ship is oriented horizontally.
	if (player.m_ships[shipNumber].m_orientation == HORIZONTAL)
	{
		// Test for row out-of-bounds
		if ((shipRow < 0) || (shipRow > numberOfRows - 1))
			return false;
		for (short i = shipColumn; i < shipColumn + shipSize[shipNumber]; i++)
		{
			// Test for out-of-bounds
			if ((i < 0) || (i > numberOfCols - 1))
				return false;

			// Test other ships:
			for (short otherShips = 1; otherShips < shipNumber; otherShips++)
			{
				// Assign easier-to-work-with variables for the ship info.
				short otherShipCol =
					player.m_ships[otherShips].m_bowLocation.m_col;
				short otherShipRow =
					player.m_ships[otherShips].m_bowLocation.m_row;
				Direction otherShipOrientation =
					player.m_ships[otherShips].m_orientation;

				// Testing: our ship horizontal, other ship horizontal.
				if (otherShipOrientation == HORIZONTAL)
				{
					if (shipRow == otherShipRow)
					{
						for (short j = otherShipCol; j < otherShipCol +
							shipSize[otherShips]; j++)
						{
							if (j == i)
								return false;
						}// End for j = otherShipCol to otherShipCol + 
							//shipSize[otherShips]
					}// End if shipRow == otherShipRow
				}// End if otherShipOrientation == HORIZONTAL

				// Testing: our ship horizontal, other ship vertical.
				if (otherShipOrientation == VERTICAL)
				{
					for (short j = otherShipRow; j < otherShipRow + 
						shipSize[otherShips]; j++)
					{
						if (shipRow == j)
						{
							if (i == otherShipCol)
								return false;
						}// End if shipRow == j
					}// End for j = otherShipRow to otherShipRow + 
						//shipSize[otherShips]
				}// End if otherShipOrientation == VERTICAL
			}// End for otherShips (testing against other ships)
		}// End for i = shipColumn to shipColumn + shipSize[shipNumber]
	}// End if ship orientation == HORIZONTAL

	// If after all that gobbledegook all is well, we can return true,
	// allowing the game to continue.
	return true;
}

//----------------------------------------------------------------------------
// Function:		header()
// Title:			header 
// Description:
//					Prints opening graphic
// Programmer:		Paul Bladek
// Modified by:		David Landry & Jason Maxwell Gautama
//						(team "Battle at C")
// 
// Date:			1/12/16
//
// Version:			1.1
// 
// Environment:		Hardware: i3 
//					Software: OS: Windows 7; 
//					Compiles under Microsoft Visual C++ 2012
//
// Output:	
//
// Calls:			boxTop()
//					boxLine()
//					boxBottom()
//
// Called By:		main()
//
// Parameters:		sout: ostream&;	the stream to print to
// 
// Returns:			void
//
// History Log: 
//		9/12/06 PB	completed v 1.0
//		1/12/16 DL	Minor cosmetic improvements.	v. 1.1
//     
//----------------------------------------------------------------------------
void header(ostream& sout)
{
	const string empty;
	const string sink("SINK THE FLEET!");
	const string theDevelopers("David Landry & Jason Maxwell Gautama");
	const string devTeam("(Team \"Battle at C\")");
	const string by("Edmonds Community College CS 132");
	boxTop(sout, BOXWIDTH);
	boxLine(sout, empty, BOXWIDTH);
	boxLine(sout, sink, BOXWIDTH, 'C');
	boxLine(sout, empty, BOXWIDTH);
	boxLine(sout, theDevelopers, BOXWIDTH, 'C');
	boxLine(sout, devTeam, BOXWIDTH, 'C');
	boxLine(sout, empty, BOXWIDTH);
	boxLine(sout, by, BOXWIDTH, 'C');
	boxLine(sout, empty, BOXWIDTH);
	boxBottom(sout, BOXWIDTH);
}

//----------------------------------------------------------------------------
// Function:		endBox()
// Title:			End Box 
// Description:
//					prints closinging graphic
// Programmer:		Paul Bladek
// Modified by:		David G Landry
//					Jason M Gautama
//
// Date:			1/26/16
//
// Version:			1.1
// 
// Environment:		Hardware: i7
//					Software: OS: Windows 10; 
//					Compiles under Microsoft Visual C++ 2013
//
// Output:			Congratulation message to the winner
//
// Calls:			boxTop()
//					boxLine()
//					boxBottom()
//
// Called By:		main()
//
// Parameters:		player: short; the number of the winner (0 or 1)
// 
// Returns:			void
//
// History Log: 
//					9/12/06 PB completed v 1.0
//					1/26/16 DL & JM v1.1; Finished the code for this function
//----------------------------------------------------------------------------
void endBox(short player)
{
	const string empty;
	ostringstream msg;
	msg << "Congratulations player " << player + 1 << "!";
	boxTop(cout, BOXWIDTH);
	boxLine(cout, empty, BOXWIDTH);
	boxLine(cout, msg.str(), BOXWIDTH, 'C');
	boxLine(cout, empty, BOXWIDTH);
	boxBottom(cout, BOXWIDTH);
}