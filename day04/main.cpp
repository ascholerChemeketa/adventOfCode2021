#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

//601 lines in input, numbers list is one line, each blank + board is 6 lines
const int DATA_SIZE = 100;


struct BingoCell {
    int num;
    bool marked = false;
};

struct BingoBoard {
    BingoCell squares[5][5];
};


//See if given board has won
bool isWinner(BingoBoard boards[DATA_SIZE], int boardNum) {
    //Check rows
    for(int r = 0; r < 5; r++) {
        bool winner = true;
        for(int c = 0; c < 5; c++) {
            if(boards[boardNum].squares[r][c].marked == false) {
                winner = false;
                break;
            }
        }
        if(winner)
            return true;
    }

    //Check cols
    for(int c = 0; c < 5; c++) {
        bool winner = true;
        for(int r = 0; r < 5; r++) {
            if(boards[boardNum].squares[r][c].marked == false) {
                winner = false;
                break;
            }
        }
        if(winner)
            return true;
    }

    return false;
}

//Find which board is winner
int getWinner(BingoBoard boards[DATA_SIZE]) {
    for(int b = 0; b < DATA_SIZE; b++) {
        bool wins = isWinner(boards, b);
        if(wins)
            return b;
    }
    return -1;
}


//Fine how many boards are winners
int numWinners(BingoBoard boards[DATA_SIZE]) {
    int numWinners = 0;
    for(int b = 0; b < DATA_SIZE; b++) {
        bool wins = isWinner(boards, b);

        if(wins)
            numWinners++;
    }
    return numWinners;
}

//Mark given number on all boards
void markBoards(BingoBoard boards[DATA_SIZE], int num) {
    for(int b = 0; b < DATA_SIZE; b++) {
        for(int r = 0; r < 5; r++) {
            for(int c = 0; c < 5; c++) {
                if(boards[b].squares[r][c].num == num) {
                    boards[b].squares[r][c].marked = true;
                }
            }
        }
    }
}

int getSumOfUnmarked(const BingoBoard& board) {
    int sum = 0;
    for(int r = 0; r < 5; r++) {
        for(int c = 0; c < 5; c++) {
            if(!board.squares[r][c].marked)
                sum += board.squares[r][c].num;
        }
    }
    return sum;
}

//Debug helper
void printBoard(const BingoBoard& board) {
    for(int r = 0; r < 5; r++) {
        for(int c = 0; c < 5; c++) {
            cout << board.squares[r][c].num;
            if(board.squares[r][c].marked)
                cout << "X";

            cout << "\t";
        }
        cout << endl;
    }
}

void day4p1() {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

    //Get first line into stringstream so we can parse it using getline later
    string firstLine;
    getline(in, firstLine);
    stringstream firstStream(firstLine);

    //Read in all boards
    BingoBoard boards[DATA_SIZE];
    for(int b = 0; b < DATA_SIZE; b++) {
        for(int r = 0; r < 5; r++) {
            for(int c = 0; c < 5; c++) {
                in >> boards[b].squares[r][c].num;
            }
        }
    }

    //Start reading numbers until we find a winner
    int winningBoardNum = -1;
    int curNum = 0;
    do {
        string nextNum;
        getline(firstStream, nextNum, ',');
        curNum = stoi(nextNum);
        markBoards(boards, curNum);
        winningBoardNum = getWinner(boards);
    }
    while(winningBoardNum == -1);

    int sum = getSumOfUnmarked(boards[winningBoardNum]);

    cout << sum * curNum << endl;
}

void day4p2() {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

    //Get first line into stringstream so we can parse it using getline later
    string firstLine;
    getline(in, firstLine);
    stringstream firstStream(firstLine);

    //Read in all boards
    BingoBoard boards[DATA_SIZE];
    for(int b = 0; b < DATA_SIZE; b++) {
        for(int r = 0; r < 5; r++) {
            for(int c = 0; c < 5; c++) {
                in >> boards[b].squares[r][c].num;
            }
        }
    }

    //Start reading numbers until every board wins
    int winners = 0;
    int lastWinner = -1;
    int curNum = 0;
    do {
        string curNumString;
        getline(firstStream, curNumString, ',');
        curNum = stoi(curNumString);

        markBoards(boards, curNum);
        winners = numWinners(boards);

        //Only one non-winner, check who it is
        if(winners == DATA_SIZE - 1 && lastWinner == -1) {
            for(int b = 0; b < DATA_SIZE; b++) {
                if(!isWinner(boards, b))
                    lastWinner = b;
            }
        }
    }
    while(winners < DATA_SIZE);

    int sum = getSumOfUnmarked(boards[lastWinner]);

    cout << curNum * sum << endl;
}




int main()
{
    day4p1();
    cout << "---------------------" << endl;
    day4p2();
}
