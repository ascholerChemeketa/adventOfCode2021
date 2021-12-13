#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

const int NUM_DOTS = 907;
const int NUM_FOLDS = 12;

struct Dot {
    int x;
    int y;
};

struct Fold {
    char dimension;
    int position;
};

void getData(Dot dots[NUM_DOTS], Fold folds[NUM_FOLDS]) {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

    char trash;
    for(int i = 0; i < NUM_DOTS; i++) {
        in >> dots[i].y;
        in >> trash;
        in >> dots[i].x;
    }

    string foldLine;
    getline(in, foldLine);  //end of dots
    getline(in, foldLine);  //empty
    for(int i = 0; i < NUM_FOLDS; i++) {
        getline(in, foldLine);
        string::size_type eqLoc = foldLine.find("=");
        folds[i].dimension = foldLine.at(eqLoc - 1);
        folds[i].position = stoi(foldLine.substr(eqLoc + 1));
    }
}

//see if i needs to fold and do so
void checkAndChange(int& pos, int foldPosition) {
    if(pos > foldPosition) {
        pos = foldPosition - (pos - foldPosition);
    }
}

void doFold(Dot dots[NUM_DOTS], const Fold& f) {
    for(int i = 0; i < NUM_DOTS; i++) {
        if(f.dimension == 'x')
            checkAndChange(dots[i].y, f.position);
        else
            checkAndChange(dots[i].x, f.position);
    }
}

int countUniqueDots(Dot dots[NUM_DOTS]) {
    int count = 0;
    for(int i = 0; i < NUM_DOTS; i++) {
        Dot& curDot = dots[i];
         //only count dots that do not have a match after them
        bool hasLaterMatch = false;
        for(int j = i + 1; j < NUM_DOTS; j++) {
            Dot& otherDot = dots[j];
            if(curDot.x == otherDot.x && curDot.y == otherDot.y)
                hasLaterMatch = true;
        }
        if(!hasLaterMatch) {
            count++;
        } else {
        }
    }
    return count;
}

void day13p1() {
    Dot dots[NUM_DOTS];
    Fold folds[NUM_FOLDS];
    getData(dots, folds);

    doFold(dots, folds[0]);

    cout << countUniqueDots(dots) << endl;
}

void day13p2() {
    Dot dots[NUM_DOTS];
    Fold folds[NUM_FOLDS];
    getData(dots, folds);

    for(int i = 0; i < NUM_FOLDS; i++)
        doFold(dots, folds[i]);

    //Determined size from last folds in each dimension
    const int FINAL_ROWS = 6;
    const int FINAL_COLS = 40;
    char finalGrid[FINAL_ROWS][FINAL_COLS] {};

    for(int i = 0; i < NUM_DOTS; i++) {
        //code coordinates appears to be transposed?!?!?
        if(dots[i].x < FINAL_ROWS  && dots[i].y < FINAL_COLS)
            finalGrid[ dots[i].x ][ dots[i].y ] = '#';
    }

    for(int i = 0; i < FINAL_ROWS; i++) {
        for(int j = 0; j < FINAL_COLS; j++) {
            cout << finalGrid[ i ][ j ];
        }
        cout << endl;
    }

}


int main()
{
    day13p1();
    cout << "-------------------------------" << endl;
    day13p2();
}
