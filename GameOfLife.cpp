// Author: Mario Talevski
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>

//Move OS defines up here to be used in different places
#if defined(_WIN32) || defined(WIN32) || defined(__MINGW32__) || defined(__BORLANDC__)
 #define OS_WIN
 //WINDOWS COLORS 
  #define COLOR_RED SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED)
  #define COLOR_WARNING SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN) 

  #define COLOR_BLUE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE)

  #define COLOR_RESET SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15)

#elif defined(linux) || defined(__CYGWIN__)
  #define OS_LINUX

  #define COLOR_RED "\033[31m"
  #define COLOR_GREEN "\033[32m" 
  #define COLOR_BLUE "\033[34m"
  #define COLOR_RESET "\033[0m"

#elif (defined(__APPLE__) || defined(__OSX__) || defined(__MACOS__)) && defined(__MACH__)//To ensure that we are running on a mondern version of macOS
  #define OS_MAC

  #define COLOR_RED "\033[31m"
  #define COLOR_GREEN "\033[32m" 
  #define COLOR_BLUE "\033[34m"
  #define COLOR_RESET "\033[0m"

#endif

#if defined(OS_WIN)
  #include <windows.h> // Use for windows
#endif

using namespace std;

const int gridSize = 25;
void printGrid(bool gridOne[gridSize+1][gridSize+1]);
void determineState(bool gridOne[gridSize+1][gridSize+1]);
void clearScreen(void);


int main(){

    // system( "color A" );//LGT green
    cout << COLOR_RED;
    clearScreen();
    bool gridOne[gridSize+1][gridSize+1] = {};
    int x,y,n;
    string nc;
    string start;
    string filename;
    cout << "                        CSSS FALL HACKATHON - THE GAME OF ... - Implementation in C++" << endl;
	 // TODO:  write introduction and get rid of this one
    cout << endl;
    cout << endl;
    cout << endl;
    cout << "Also known simply as life, " << endl;
    cout << "is a cellular automaton devised by the British mathematician John Horton Conway in 1970." << endl;
    cout << endl;
    cout << "Rules" << endl;
    cout << "The universe of the Game of life is an infinite two-dimensional orthogonal grid of square cells," << endl;
    cout << "each of which is in one of two possible states, life or dead. Every" << endl;
    cout << "cell interacts with its eight neighbours, which are the cells that are horizontally, vertically, or diagonally adjacent." << endl;
    cout << "At each step in time, the following transitions occur:" << endl;
    cout << "1. Any live cell with fewer than two live neighbours dies, as if caused by under-population." << endl;
    cout << "2. Any live cell with two or three live neighbours lives on to the next generation." << endl;
    cout << "3. Any live cell with more than three live neighbours dies, as if by over-population." << endl;
    cout << "4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction." << endl;
    cout << endl;
    cout << "O - living cell" << endl;
    cout << ". - dead cell" << endl;
    cout << endl;
    cout << "Enter the number of cells, or 'r' to read cells from file: ";
    cin >> nc;
    cout << endl;

    if ( nc == "r" )
      {
	while (true)
	  {
	    
	    cout << "Enter name of file to read from: "<<endl;
	    cin  >> filename;
		
		/*
		TODO: another potential improvement, right now the input file just specifies coordinates for the cells like
			5 4
			7 4
			5 5
			6 5
			5 6

		might be cooler for the input to be like
			. . . . . 
			. o . . . 
			. o . . o
			o . o . .
			o o . . o

		instead	
		*/
	    
	    ifstream readfile(filename);
	    if ( readfile.is_open() )
	      {
		string fileline;
    		int yCoord = 0;

		while (getline(readfile,fileline))
		  {
		   	stringstream ss(fileline);	    
        		int xCoord = 0;
        		char cell;

        		while (ss.get(cell)) {

			  if( cell == 'o' ) {
			    cout << "adding live cell to coord " << xCoord << ", " << yCoord << "!" << endl;
			    gridOne[xCoord][yCoord] = true;
			  }
			  if( cell == ' ') {
			    // don't count spaces
			    continue;
			  }
			  xCoord++;
			}
			yCoord++;
		  }
		break;
	      } else {
	      cout << "No such file, try again." << endl;
	    }
	  }
      }
    else
      {
	
	for(int i=0;i<stoi(nc);i++)
	  {
	    cout <<stoi(nc)<< "Enter the coordinates of cell " << i+1 << " : ";
	    cin >> x >> y;
	    gridOne[x][y] = true;
	    printGrid(gridOne);
	  }
      }
    cout << "Grid setup is done. Start the game ? (y/n)" << endl;
	
	// TODO: let the player configure game parameters (that's what makes it the Game of ... rather than the Game of Life
	
    printGrid(gridOne);
    cin >> start;
    if( start == "y" || start == "Y" )
      {
        while (true)
	  {
            printGrid(gridOne);
            determineState(gridOne);
            usleep(200000);
            clearScreen();
	  }
      }
    else
      {
        cout << COLOR_RESET;
        clearScreen();
        return 0;
      }   
}

void clearScreen(void) {
  // Tested and working on Ubuntu and Cygwin
  #if defined(OS_WIN)
    HANDLE hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = { 0, 0 };

    if (hStdOut == INVALID_HANDLE_VALUE) return;

    /* Get the number of cells in the current buffer */
    GetConsoleScreenBufferInfo( hStdOut, &csbi );
    cellCount = csbi.dwSize.X *csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    FillConsoleOutputCharacter(hStdOut, (TCHAR) ' ', cellCount, homeCoords, &count);

    /* Fill the entire buffer with the current colors and attributes */
    FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count);

    SetConsoleCursorPosition( hStdOut, homeCoords );

  #elif defined(OS_LINUX) || defined(OS_MAC)
    cout << "\033[2J;" << "\033[1;1H"; // Clears screen and moves cursor to home pos on POSIX systems
  #endif

}


void printGrid(bool gridOne[gridSize+1][gridSize+1])
{
  for (int i=0; i < gridSize; i++)
  {
    for (int x=0; x<gridSize; x++)
    {
      if (gridOne[i][x]==true)
      {
        cout << "o ";
      }
      else
      {
        cout << ". ";
      }
    }
    cout << endl;
  }
}

void copyGrid (bool gridOne[gridSize+1][gridSize+1], bool gridTwo[gridSize+1][gridSize+1]){
	for(int i=0; i<gridSize+1; i++) //row
	{
		for(int j=0; j<gridSize+1; j++)//column
		{
			gridTwo[i][j] = gridOne[i][j];
		}
	}
}

/*
param:  gridOne, the boolean grid with the alive (true) and dead (false) cells  
        i, the integer index of the row of gridOne
        j, the integer index of the column of gridOne
return: the number of adjacent cells (including diagonal) to gridOne[i][j]
        that are alive (true).
*/
int liveNeighbours (bool gridOne[gridSize+1][gridSize+1], int i, int j){
    // count all trues from grid[i-1][j-1] to grid[i+1][j+1]
    int count = 0;
    
    // avoid accessing negative indices or indices past gridSize
    int lower_bound_cols = max(1, i-1);
    int upper_bound_cols = min(gridSize-1, i+1);
    int lower_bound_rows = max(1, j-1);
    int upper_bound_rows = min(gridSize-1, j+1);

    for (int a = lower_bound_cols; a <= upper_bound_cols; a++){
        for (int b = lower_bound_rows; b <= upper_bound_rows; b++){
            if (gridOne[a][b] == true && (a != i || b != j)){
                count++;
            }
        }
    }
    return count;
}

/*
param:  gridOne, the boolean grid with the alive (true) and dead (false) cells 
return: void, but modifies gridOne with trues and falses to simulate an iteration
*/
void determineState(bool gridOne[gridSize+1][gridSize+1]){
    bool gridCopy[gridSize+1][gridSize+1];

    // make an unchanged copy of gridOne so changes made to gridOne
    // won't affect liveNeighbours
    copyGrid(gridOne, gridCopy);

    for (int i = 1; i < gridSize; i++){
        for (int j = 1; j < gridSize; j++){
            int alive = liveNeighbours(gridCopy, i, j);

            // TODO: Make it so that these rules can be changed
            /* Rules:
            Any cell with less than 2 live neighbours dies.
            Any cell with more than 3 live neighbours dies.
            Any cell with 2-3 live neighbours stays unchanged.
            Any cell with 3 live neighbours is alive.
            */

            if (alive < 2){
                gridOne[i][j] = false;
            }
            else if (alive > 3){
                gridOne[i][j] = false;
            }
            else if (alive == 3){
                gridOne[i][j] = true;
            }
        }
    }
}

