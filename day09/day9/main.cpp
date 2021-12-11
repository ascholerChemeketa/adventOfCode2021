#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

const int ROWS = 100;
const int COLS = 100;


bool isLow(const int map[ROWS][COLS], int row, int col) {
    if((row > 0 && map[row][col] >= map[row - 1][col]) ||
       (row < ROWS - 1 && map[row][col] >= map[row + 1][col]) ||
       (col > 0 && map[row][col] >= map[row][col - 1]) ||
       (col < COLS - 1 && map[row][col] >= map[row][col + 1]))
        return false;
    else
        return true;
}

int getRisk(const int map[ROWS][COLS], int row, int col) {
    if(isLow(map, row, col))
        return map[row][col] + 1;
    else
        return 0;
}

void day9p1() {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

    int map[ROWS][COLS];

    for(int i = 0; i < ROWS; i++)
        for(int j = 0; j < COLS; j++) {
            char inChar;
            in >> inChar;
            map[i][j] = inChar - '0';
        }

    int riskSum = 0;
    for(int i = 0; i < ROWS; i++)
        for(int j = 0; j < COLS; j++) {
            riskSum += getRisk(map, i, j);
        }

    cout << riskSum << endl;

}

//-----------------------------------------------------

int countBasin(int map[ROWS][COLS], int row, int col) {
    if(row < 0 || row >= ROWS || col < 0 || col >= COLS
       || map[row][col] == -1 || map[row][col] == 9)
        return 0;

    map[row][col] = -1;

    return 1 + countBasin(map, row - 1, col)
             + countBasin(map, row + 1, col)
             + countBasin(map, row, col - 1)
             + countBasin(map, row, col + 1);

}

//Use 162 concepts
void day9p2Cheat() {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

    int map[ROWS][COLS];

    for(int i = 0; i < ROWS; i++)
        for(int j = 0; j < COLS; j++) {
            char inChar;
            in >> inChar;
            map[i][j] = inChar - '0';
        }

    vector<int> basinSizes;
    for(int i = 0; i < ROWS; i++)
        for(int j = 0; j < COLS; j++) {
            if(isLow(map, i, j)) {
                basinSizes.push_back(countBasin(map, i, j));
            }
        }

    //Since we are cheating wrt 161 content already...
    std::sort(basinSizes.begin(), basinSizes.end(), std::greater<int>());
    cout << basinSizes[0] * basinSizes[1] * basinSizes[2]  << endl;

}



//-----------------------------------------------------


//join neighboring basin if possible. Return true if that happens
bool expand(int map[ROWS][COLS], int row, int col) {
    if(map[row][col] == 9 || map[row][col] < 0)
        return false;

    int lowerVal = 0;
    if(row > 0 && map[row - 1][col] < 0)
        lowerVal = map[row - 1][col];
    else if(row < ROWS - 1 && map[row + 1][col] < 0)
        lowerVal = map[row + 1][col];
    else if(col > 0 && map[row][col - 1] < 0)
        lowerVal = map[row][col - 1];
    else if(col < COLS - 1 && map[row][col + 1] < 0)
        lowerVal = map[row][col + 1];

    if(lowerVal < 0) {
        map[row][col] = lowerVal;
        return true;
    } else
        return false;
}


//Grow the basins until no more growth possible
void growBasins(int map[ROWS][COLS]) {
    bool done = false;
    while(!done) {
        done = true;
        for(int i = 0; i < ROWS; i++)
            for(int j = 0; j < COLS; j++) {
                if(expand(map, i, j)) {
                    done = false;
                }
            }
    }
}


//Seed the basins - they will be marked with different negative values
//return number of last one
int seedBasins(int map[ROWS][COLS]) {
    int curBasinValue = -1;
    for(int i = 0; i < ROWS; i++)
        for(int j = 0; j < COLS; j++) {
            if(isLow(map, i, j)) {
                map[i][j] = curBasinValue;
                curBasinValue--;
            }
        }
    return curBasinValue + 1;
}

void day9p2() {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

    int map[ROWS][COLS];

    for(int i = 0; i < ROWS; i++)
        for(int j = 0; j < COLS; j++) {
            char inChar;
            in >> inChar;
            map[i][j] = inChar - '0';
        }

    //basins will use negative numbers starting from -1 to mark them
    //last basin will have the number of the last basin: -4 if there were 4 basins
    int lastBasin = seedBasins(map);

    growBasins(map);

    int largest = 0;
    int second = 0;
    int third = 0;
    for(int basin = -1; basin >= lastBasin; basin--) {
        int size = 0;
        for(int i = 0; i < ROWS; i++)
            for(int j = 0; j < COLS; j++) {
                if(map[i][j] == basin)
                    size++;
            }

        if(size > largest) {
            third = second;
            second = largest;
            largest = size;
        } else if(size > second) {
            third = second;
            second = size;
        }
        else if(size > third)
            third = size;
    }

    cout << largest * second * third << endl;
}


int main()
{
    day9p1();
    cout << "-------------------------" << endl;
    day9p2Cheat();
    cout << "-------------------------" << endl;
    day9p2();
}
