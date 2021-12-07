#include <iostream>
#include <fstream>

using namespace std;

const int DATA_SIZE = 1000;

//helper for part 2
int costForSteps(int steps) {
    int cost = 0;
    for(int i = 1; i <= steps; i++) {
        cost += i;
    }
    return cost;
}

void day7() {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

    int crabLocations[DATA_SIZE];
    for(int i = 0; i < DATA_SIZE; i++) {
        in >> crabLocations[i];
        char comma;
        in >> comma;
    }

    //Start from position 0. Calculate cost for each location.
    //Keep going while cost is dropping. Once we see first increase, stop
    int location = 0;
    int curCost = 0;
    int lowestCost = INT_MAX;
    while(curCost <= lowestCost) {
        curCost = 0;
        for(int i = 0; i < DATA_SIZE; i++) {
            curCost += costForSteps(abs(crabLocations[i] - location));  //part2
            //curCost += abs(crabLocations[i] - location); //part 1
        }
        if(curCost < lowestCost) {
            lowestCost = curCost;
            location++;
        }
    }

    location--;  //Back up from the one step too far
    cout << "Location: " << location << endl;
    cout << "Cost: " << lowestCost << endl;
}


int main()
{
    day7();
}
