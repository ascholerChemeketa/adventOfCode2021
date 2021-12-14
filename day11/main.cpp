#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <iterator>

using namespace std;

const int ROWS = 10;
const int COLS = 10;
const int STEPS = 100;

enum OctopiStatus { INITIAL, PROCESSED, FLASHING, FLASHED };
struct Octopi {
    int val = 0;
    OctopiStatus status = INITIAL;

    Octopi() { }   //advanced
    Octopi(int v) { val = v; }   //advanced
};

void printAll(Octopi octopi[ROWS][COLS]) {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            cout << octopi[i][j].val;
        }
        cout << endl;
    }
    cout << "----------------------------" << endl;
}

void getData(Octopi data[ROWS][COLS]) {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            char c;
            in >> c;
            data[i][j].val = c - '0';
        }
    }
}

//return true if it just flashed
bool increment(Octopi octopi[ROWS][COLS], int row, int col) {
    if(row < 0 || row >= ROWS || col < 0 || col >= COLS)
        return false;

    if(octopi[row][col].status >= FLASHING)
        return false;

    //cout << row << " " << col << "changed" << endl;
    if(octopi[row][col].val < 9) {
        octopi[row][col].val++;
    }
    else {
        octopi[row][col].status = FLASHING;
        octopi[row][col].val = 0;
        return true;
    }
    return false;
}


//return number of flashes seen
int process(Octopi octopi[ROWS][COLS]) {
    int numFlashes = 0;
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            if(octopi[i][j].status == INITIAL) {
                octopi[i][j].status = PROCESSED;
                numFlashes += increment(octopi, i, j);
            }
            if(octopi[i][j].status == FLASHING) {
                //cout << "flashing " << i << " " << j << endl;
                octopi[i][j].status = FLASHED;

                numFlashes += increment(octopi, i - 1, j - 1);
                numFlashes += increment(octopi, i - 1, j);
                numFlashes += increment(octopi, i - 1, j + 1);
                numFlashes += increment(octopi, i, j - 1);
                numFlashes += increment(octopi, i, j + 1);
                numFlashes += increment(octopi, i + 1, j - 1);
                numFlashes += increment(octopi, i + 1, j);
                numFlashes += increment(octopi, i + 1, j + 1);
                //cout << "flashing done: " << numFlashes << endl;
            }
        }
    }
    //printAll(octopi);
    return numFlashes;
}

//reset all octopi, return number that had flashed
int resetAndCount(Octopi octopi[ROWS][COLS]) {
    int count = 0;
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            if(octopi[i][j].status == FLASHED) {
                count++;
            }
            octopi[i][j].status = INITIAL;
        }
    }
    return count;
}

void day11p1() {
    Octopi octopi[ROWS][COLS];

    getData(octopi);

    int totalFlashes = 0;
    for(int s = 0; s < STEPS; s++) {
        while(process(octopi) != 0) {
            //process handles all work
        }

        totalFlashes += resetAndCount(octopi);
    }
    cout << totalFlashes << endl;
}

void day11p2() {
    Octopi octopi[ROWS][COLS];

    getData(octopi);

    int stepNum = 0;
    int flashesSeen = 0;
    while(flashesSeen != ROWS * COLS) {
        stepNum++;
        while(process(octopi) != 0) {
            //process handles all work
        }
        flashesSeen = resetAndCount(octopi);
    }
    cout << stepNum << endl;
}

struct LocIterator {
    int row = 0;
    int col = 0;

    pair<int, int> operator*() {
        pair<int,int> ret = {row, col};
        if(++col == COLS)
            ++row, col = 0;
        return ret;
    }

    pair<int, int> operator++(int) {
        pair<int,int> ret = {row, col};
        if(++col == COLS)
            ++row, col = 0;
        return ret;
    }

    static pair<int, int> end() {
        return pair<int,int>(ROWS, 0);
    }
};


struct intPairHasher {
    size_t operator() (const pair<int,int>& p) const {
        return (p.first << 16) + p.second;
    }
};

void day11p2Advanced() {
    unordered_map<pair<int, int>, Octopi, intPairHasher> octopi;

    LocIterator loc;
    ifstream in("../input.txt");
    for_each(istream_iterator<char>(in), istream_iterator<char>(),
             [&](char c) { octopi.emplace(loc++, Octopi(c - '0')); }
    );

    for(LocIterator loc2; *loc2 != loc2.end(); loc2++) {
        cout << loc2.row <<  "" << loc2.col << ":" << octopi[*loc2].val << endl;
    }
}

int main()
{
    day11p1();
    cout << "-------------------------------" << endl;
    day11p2();
    cout << "-------------------------------" << endl;
    day11p2Advanced();
}
