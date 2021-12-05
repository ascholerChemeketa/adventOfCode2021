#include <iostream>
#include <fstream>

using namespace std;

const int DATA_SIZE = 2000;

void day1p1() {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

    int lastValue, curValue;
    in >> lastValue;

    int numUp = 0;
    for(int i = 1; i < DATA_SIZE; i++) {
        in >> curValue;
        if(curValue > lastValue) {
            numUp++;
        }
        lastValue = curValue;
    }

    cout << numUp << endl;
}

void day1p2() {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

    int value1, value2, value3, value4;
    in >> value1 >> value2 >> value3;

    int numUp = 0;
    for(int i = 0; i < DATA_SIZE - 3; i++) {
        in >> value4;
        int oldSum = value1 + value2 + value3;
        int newSum = value2 + value3 + value4;
        if(newSum > oldSum) {
            numUp++;
        }

        value1 = value2;
        value2 = value3;
        value3 = value4;
    }

    cout << numUp << endl;
}

void day2p1() {
    ifstream in("input.txt");

    int horizontalPos = 0;
    int depth = 0;

    for(int i = 0; i < DATA_SIZE; i++) {
        string dir;
        in >> dir;

        int amt;
        in >> amt;

        if(dir == "forward") {
            horizontalPos += amt;
        } else if(dir == "down"){
            depth += amt;
        } else {
            depth -= amt;
        }
    }

    cout << depth * horizontalPos << endl;
}


int main()
{
    day1p1();
    cout << "---------------------" << endl;
    day1p2();
}
