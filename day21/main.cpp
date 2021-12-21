#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <numeric>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <stack>

using namespace std;


struct Die {
    int nextVal = 0;
    int totalRolls = 0;
    int roll() {
        totalRolls++;
        nextVal = nextVal % 100 + 1;
        return nextVal;
    }
};

const int BOARD_SIZE = 10;

struct Player {
    int score = 0;
    int position;       //zero index

    void move(int amt) {
        position += amt;
        position %= BOARD_SIZE;
        score += position + 1;
    }
};

void day21p1(int p1Start, int p2Start) {
    Player players[2];
    players[0].position = p1Start;
    players[1].position = p2Start;

    Die d;

    int curPlayer = 1;
    while(players[curPlayer].score < 1000) {
        curPlayer = 1 - curPlayer;
        players[curPlayer].move(d.roll() + d.roll() + d.roll());
    }
    cout << players[1- curPlayer].score << endl;
    cout << d.totalRolls << endl;
    cout << players[1- curPlayer].score * d.totalRolls << endl;
}





struct QuantumState {
    Player players[2];
    int lastTurn = 1;
    uint64_t copies = 1;

    bool isDone() const {
        return players[lastTurn].score >= 21;
    }

    QuantumState getResultState(int roll, int splits) const {
        QuantumState s = *this;
        s.lastTurn = 1 - lastTurn;
        s.copies *= splits;
        s.players[s.lastTurn].move(roll);
        return s;
    }
};



void day21p2(int p1Start, int p2Start) {
    QuantumState initial({{{0, p1Start}, {0, p2Start}}, 1, 1});

    uint64_t wins[2] = {};

    vector<pair<int, int>> rollTypes = {{3,1}, {4,3}, {5,6}, {6,7}, {7,6}, {8,3}, {9,1}};

    list<QuantumState> activeGames({initial});
    while(activeGames.size() != 0) {
        auto& state = *activeGames.begin();
        for(const auto& r : rollTypes) {
            QuantumState q = state.getResultState(r.first, r.second);
            if(q.isDone()) {
                wins[q.lastTurn] += q.copies;
            }
            else
                activeGames.push_back(move(q));
        }
        activeGames.pop_front();
    }
    cout << wins[0] << endl;
    cout << wins[1] << endl;
}


int main()
{
    int p1, p2;
    ifstream in("../Input.txt");
    in >> p1 >> p2;
    //zero index positions
    p1--; p2--;
    day21p1(p1, p2);
    cout << "----------------------------------------" << endl;
    day21p2(p1, p2);
}
