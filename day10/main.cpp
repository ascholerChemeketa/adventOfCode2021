#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

const int DATA_SIZE = 94;

int getValue(char c) {
    switch(c){
        case ')': return 3;
        case ']': return 57;
        case '}': return 1197;
        case '>': return 25137;
    }
    return 0;
}

int getScore(char c) {
    switch(c){
        case ')': return 1;
        case ']': return 2;
        case '}': return 3;
        case '>': return 4;
    }
    return 0;
}

bool isOpen(char c) {
    return string("[{(<").find(c) != string::npos;
}

char getMatch(char c) {
    switch(c){
        case '(': return ')';
        case '[': return ']';
        case '{': return '}';
        case '<': return '>';
    }
    return 0;
}

bool isMatch(char c, char c2) {
    if( getMatch(c) == c2 )
        return true;

    return false;
}

void day10p1() {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

    int sum = 0;
    for(int i = 0; i < DATA_SIZE; i++) {
        string line;
        in >> line;

        string rebuiltLine;
        for(string::size_type charIndex = 0; charIndex < line.length(); charIndex++) {
            char curChar = line.at(charIndex);
            if(isOpen(curChar))
                rebuiltLine += curChar;
            else {
                if(!isMatch( rebuiltLine.at(rebuiltLine.length()-1), curChar )) {
                    sum += getValue(curChar);
                    break;
                } else {
                    rebuiltLine.erase(rebuiltLine.length() - 1);
                }
            }
        }
    }
    cout << sum << endl;
}

void selectSort(uint64_t scores[], int numScores) {
    for(int pass = 0; pass < numScores; pass++) {
        int lowestIndex = pass;
        for(int j = pass + 1; j < numScores; j++) {
            if(scores[j] < scores[lowestIndex])
                lowestIndex = j;
        }
        uint64_t temp = scores[pass];
        scores[pass] = scores[lowestIndex];
        scores[lowestIndex] = temp;
    }
}

void day10p2() {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

    uint64_t scores[94] = {}; //wont need all of these
    int numScores = 0;
    for(int i = 0; i < DATA_SIZE; i++) {
        string line;
        in >> line;

        string chunkStack;
        bool incomplete = true;
        for(string::size_type charIndex = 0; charIndex < line.length(); charIndex++) {
            char curChar = line.at(charIndex);
            if(isOpen(curChar))
                chunkStack += curChar;
            else {
                if(!isMatch( chunkStack.back(), curChar )) {  //back is last char
                    incomplete = false;
                    break;
                } else {
                    chunkStack.pop_back();  //remote last char
                }
            }
        }

        if(incomplete) {
            string closing;
            uint64_t score = 0;
            for(int charIndex = chunkStack.length() - 1; charIndex >= 0; charIndex--) {
                char curChar = chunkStack.at(charIndex);
                char newChar = getMatch(curChar);
                score = score * 5 + getScore(newChar);
            }
            scores[numScores++] = score;
        }
    }
    selectSort(scores, numScores);
    cout << scores[numScores / 2] << endl;
}

int main()
{
    day10p1();
    cout << "-------------------------" << endl;
    day10p2();
}
