#include <iostream>
#include <fstream>

using namespace std;

const int DATA_SIZE = 1000;

void day3p1() {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

    string data[DATA_SIZE];

    for(int i = 0; i < DATA_SIZE; i++) {
        in >> data[i];
    }

    string epsilon = "000000000000";
    string gamma = "000000000000";

    for(int i = 0; i < 12; i++) {
        int num1s = 0;
        for(int s = 0; s < DATA_SIZE; s++) {
            if(data[s][i] == '1')
                num1s++;
        }
        if(num1s > DATA_SIZE / 2) {
            gamma[i] = '1';
        } else {
            epsilon[i] = '1';
        }
    }

    //convert strings in base 2 to ints
    int gInt = stoi(gamma, 0, 2);
    int eInt = stoi(epsilon, 0, 2);

    cout << gInt * eInt << endl;

}


//-----------------------------------------------------

struct DiagnosticRecord {
    string data;
    bool inUse = true;
};

//Decide if '1' or '0' is more common in given position
char mostCommonValueInData(DiagnosticRecord records[DATA_SIZE], int position) {
    int num1s = 0;
    int num0s = 0;
    for(int i = 0; i < DATA_SIZE; i++) {
        if(records[i].inUse == true) {
            if(records[i].data[position] == '1')
                num1s++;
            else
                num0s++;
        }
    }
    if(num1s >= num0s) {
        //For both uses, a tie should resolve as 1
        return '1';
    } else {
        return '0';
    }
}

//Get number of dataRecords still in use
int numInUse(DiagnosticRecord records[DATA_SIZE]) {
    int num = 0;
    for(int s = 0; s < DATA_SIZE; s++) {
        if(records[s].inUse == true) {
           num++;
        }
    }
    return num;
}

//Eliminate records until only one remains, return its data
// eliminates records with least common value at each position if eliminateLeastCommon is true
// otherwise eliminates those with most common value
string findLastRecord(DiagnosticRecord records[DATA_SIZE], bool eliminateLeastCommon) {
    int position = 0;
    while(numInUse(records) > 1) {
        char c = mostCommonValueInData(records, position);

        for(int i = 0; i < DATA_SIZE; i++) {
            if((eliminateLeastCommon && records[i].data[position] == c) ||
               (!eliminateLeastCommon && records[i].data[position] != c)) {
                records[i].inUse = false;
            }
        }
        position++;
    }

    for(int i = 0; i < DATA_SIZE; i++) {
        if(records[i].inUse == true) {
           return records[i].data;
        }
    }
    return "????";
}

void day3p2() {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

    DiagnosticRecord records[DATA_SIZE];

    for(int i = 0; i < DATA_SIZE; i++) {
        in >> records[i].data;
    }

    string oxygen = findLastRecord(records, true);

    //Reset all records
    for(int i = 0; i < DATA_SIZE; i++) {
        records[i].inUse = true;
    }

    string CO2 = findLastRecord(records, false);

    int gInt = stoi(oxygen, 0, 2);
    int eInt = stoi(CO2, 0, 2);

    cout << gInt * eInt << endl;

}



int main()
{
    day3p1();
    cout << "---------------------" << endl;
    day3p2();
}
