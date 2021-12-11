#include <iostream>
#include <fstream>

using namespace std;

const int DATA_SIZE = 1000;

void day2p1() {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

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


void day2p2() {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

    int horizontalPos = 0;
    int depth = 0;
    int aim = 0;

    for(int i = 0; i < DATA_SIZE; i++) {
        string dir;
        in >> dir;

        int amt;
        in >> amt;

        if(dir == "forward") {
            horizontalPos += amt;
            depth += amt * aim;
        } else if(dir == "down"){
            aim += amt;
        } else {
            aim -= amt;
        }
    }

    cout << depth * horizontalPos << endl;
}


int main()
{
    day2p1();
    cout << "---------------------" << endl;
    day2p2();
}
