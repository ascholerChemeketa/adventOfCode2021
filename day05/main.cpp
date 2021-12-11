#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

const int ROWS = 1000;
const int COLS = 1000;
const int INPUT_LINES = 500;

//Get the int at location (row, col)
int getValue(int grid[], int row, int col) {
    return grid[row * COLS + col];
}

//++ value at location (row, col)
void incrementValue(int grid[], int row, int col) {
    grid[row * COLS + col]++;
}

void printGrid(int grid[]) {
    for(int r = 0; r < ROWS; r++) {
        for(int c = 0; c < COLS; c++) {
            cout << setw(3) << getValue(grid, r, c);
        }
        cout << endl;
    }
}

void day5() {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

    int* grid = new int[ROWS * COLS] {};

    for(int i = 0; i < INPUT_LINES; i++) {
        int x1, y1, x2, y2;
        char charDump;
        in >> x1 >> charDump >> y1 >> charDump >> charDump >> x2 >> charDump >> y2;

//        //Part1 - skip if not horizontal or vertical
//        if( !(x1 == x2 || y1 == y2) )
//            continue;

        //Part2 - skip if not horizontal, vertical or diagonal
        if( !(x1 == x2 || y1 == y2 || abs(x1 - x2) == abs(y1 - y2)) )
            continue;

        //Now connect the dots
        int xStep = x1 == x2 ? 0 : (x1 < x2 ? 1 : -1);  //xStep is 0 if x1==x2, 1 if x1 is < x2, -1 if x1 is > x2
        int yStep = y1 == y2 ? 0 : (y1 < y2 ? 1 : -1);  //Same logic for y

        while(x1 != x2 || y1 != y2) {
            incrementValue(grid, y1, x1);
            x1 += xStep;
            y1 += yStep;
        }
        incrementValue(grid, y2, x2);
    }

    int multipleCount = 0;
    for(int r = 0; r < ROWS; r++) {
        for(int c = 0; c < COLS; c++) {
            if(getValue(grid, r, c) > 1)
                multipleCount++;
        }
    }

    cout << multipleCount << endl;
    delete [] grid;
}



int main()
{
    day5();
}
