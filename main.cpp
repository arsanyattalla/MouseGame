
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
using namespace std;

#define MAX_NUM_MOVES 100
#define MAX_NUM_LIVES 100
#define MOUSE 1
#define WATER -1
#define LAND 0
#define MAX_ROWS 20
#define MAX_COLUMNS 20
#define NUM_DIRECTIONS 4
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

struct MazeInfo
{
    int maze[MAX_ROWS][MAX_COLUMNS];
    int numRows;
    int numCols;
    int mouseI;
    int mouseJ;
};

void copyMap(struct MazeInfo & mazeInfo, int copy[MAX_ROWS][MAX_COLUMNS])
{
    for( int i=0; i<mazeInfo.numRows; i++)
        for( int j=0; j<mazeInfo.numCols; j++ )
            copy[i][j] = mazeInfo.maze[i][j];
}

bool isValidMapLocation(int i, int j,struct MazeInfo & mazeInfo )
{
    return (i>=0 && j>=0 && i<mazeInfo.numRows && j<mazeInfo.numCols);
}

void moveMouse(struct MazeInfo & mazeInfo, int copy[MAX_ROWS][MAX_COLUMNS],int & mouseI, int & mouseJ )
{
    int direction = rand() % NUM_DIRECTIONS;
    if( direction == NORTH )
    {
        if( isValidMapLocation(mouseI-1, mouseJ, mazeInfo) )
        {
            copy[mouseI][mouseJ] = LAND;
            mouseI--;
        }
    }

    if( direction == EAST )
    {
        if( isValidMapLocation(mouseI, mouseJ+1, mazeInfo) )
        {
            copy[mouseI][mouseJ] = LAND;
            mouseJ++;
        }
    }

    if( direction == SOUTH )
    {
        if( isValidMapLocation(mouseI+1, mouseJ, mazeInfo) )
        {
            copy[mouseI][mouseJ] = LAND;
            mouseI++;
        }
    }

    if( direction == WEST )
    {
        if( isValidMapLocation(mouseI, mouseJ-1, mazeInfo) )
        {
            copy[mouseI][mouseJ] = LAND;
            mouseJ--;
        }
    }

}

void readMap(struct MazeInfo & mazeInfo,const char * inputFileName)
{
    ifstream input(inputFileName);
    if( !input )
    {
        cout << "Cannot open file " << inputFileName << endl;
        return;
    }

    input >> mazeInfo.numRows;
    input >> mazeInfo.numCols;

    for( int i=0; i<mazeInfo.numRows; i++)
        for( int j=0; j<mazeInfo.numCols; j++ )
            input >> mazeInfo.maze[i][j];

    for( int i=0; i<mazeInfo.numRows; i++)
    {
        for( int j=0; j<mazeInfo.numCols; j++ )
        {
            if( mazeInfo.maze[i][j] == MOUSE )
            {
                mazeInfo.mouseI = i;
                mazeInfo.mouseJ = j;
                break;
            }
        }
    }

    input.close();
}

int printToFile(struct MazeInfo & mazeInfo,const char * outputFileName,const char * inputFileName )
{
    ofstream output(outputFileName);
    if( !output )
    {
        cout << "Cannot open file " << outputFileName << endl;
        return 0;
    }

    output << "Input file name : " << inputFileName << endl;
    output << "Output file name : " << outputFileName << endl;
    output << "Map : " << endl;
    for( int i=0; i<mazeInfo.numRows; i++)
    {
        for( int j=0; j<mazeInfo.numCols; j++ )
        {
            output << mazeInfo.maze[i][j] << " ";
        }
        output << endl;
    }
    output << endl;

    int numEscaped = 0;
    int numStarved = 0;
    int numDrowned = 0;

    for( int numLives = 0; numLives < MAX_NUM_LIVES; numLives++ )
    {
        int copy[MAX_ROWS][MAX_COLUMNS];
        copyMap(mazeInfo, copy);

        int mouseI = mazeInfo.mouseI;
        int mouseJ = mazeInfo.mouseJ;
        copy[mouseI][mouseJ] = LAND;

        int moves;
        for( moves = 0; moves < MAX_NUM_MOVES; moves++ )
        {

            moveMouse(mazeInfo, copy, mouseI, mouseJ);

            if( copy[mouseI][mouseJ] == WATER )
            {
                numDrowned++;

                output << "Life number " << (numLives + 1 ) << " : drowned " << endl;
                break;
            }
            else if( copy[mouseI][mouseJ] == LAND && (mouseI == mazeInfo.numRows - 1 || mouseJ == mazeInfo.numCols - 1) )
            {
                numEscaped++;
                output << "Life number " << (numLives + 1 ) << " : escaped " << endl;
                break;
            }
        }

        if( moves == MAX_NUM_MOVES )
        {
            output << "Life number " << (numLives + 1 ) << " : starved " << endl;

            numStarved++;
        }
    }

    output << endl;
    output << "Number of times drowned : " << numDrowned << endl;
    output << "Number of times escaped : " << numEscaped << endl;
    output << "Number of times starved : " << numStarved << endl;

    output.close();
    return 1;
}

int main(int argc, const char * argv[])
{
    if( argc < 3 )
    {
        cout << "Usage : <executable> <input_file_name> <output_file_name> <seed>" << endl;
        return 1;
    }

    struct MazeInfo mazeInfo;
    mazeInfo.numRows = -1;
    mazeInfo.numCols = -1;

    readMap(mazeInfo, argv[1]);

    if( mazeInfo.numCols == -1 )
    {
        return 2;
    }

    if( argc >= 4 )
    {
        srand((unsigned int)atol(argv[3]));
    }
    printToFile(mazeInfo, argv[2], argv[1]);

    return 0;
}
