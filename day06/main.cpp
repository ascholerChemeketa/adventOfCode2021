#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

const int NUM_DAYS = 256;

void day6() {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

    //numFishAtStage[x] == number of fish that are x days from reproducing
    uint64_t numFishAtStage[9]= {};

    while(!in.eof()) {
        int nextFish;
        in >> nextFish;
        if(in.fail())
            break;

        numFishAtStage[nextFish]++;

        char comma;
        in >> comma;
    }

    for(int day = 0; day < NUM_DAYS; day++) {
        uint64_t numNewFish = numFishAtStage[0];
        for(int stage = 0; stage < 9; stage++) {
            numFishAtStage[stage] = numFishAtStage[stage + 1];
        }
        numFishAtStage[6] += numNewFish;
        numFishAtStage[8] = numNewFish;
    }

    uint64_t totalFish = 0;
    for(int stage = 0; stage < 9; stage++) {
        totalFish += numFishAtStage[stage];
    }
    cout << totalFish;
}



int main()
{
    day6();
}
