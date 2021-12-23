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
#include <deque>
#include <charconv>
#include <climits>
#include <cstring>

using namespace std;

int getCost(char c) {
    switch(c) {
        case 'A': return 1;
        case 'B': return 10;
        case 'C': return 100;
        case 'D': return 1000;
    }
    return 0;
}

const vector<int> validRests = {0,1,3,5,7,9,10};

struct Grid {
    vector<vector<char>> rooms;
    vector<char> hallway;

    int costSoFar = 0;

    Grid() {
        rooms.insert(rooms.end(), 4, {'.', '.'});
        hallway.insert(hallway.end(), 11, '.');
    }

    //inclusive of to, not of from
    bool clearPath(int from, int to) {
        if(from > to) {
            swap(--from, --to);
        }
        return all_of(hallway.begin() + from + 1, hallway.begin() + to + 1, [](char c){return c =='.';});
    }

    bool tryHome(int hallLoc) {
        char type = hallway[hallLoc];
        int home = type - 'A';
        if((rooms[home][1] != '.' && rooms[home][1] != type) || rooms[home][0] != '.')
            return false;

        int homeLoc = home * 2 + 2;

        if(!clearPath(hallLoc, homeLoc)) {
            return false;
        }

        int homeIndex = rooms[home][1] == '.' ? 1 : 0;
        hallway[hallLoc] = '.';
        rooms[home][homeIndex] = type;

        int distance = abs(hallLoc - homeLoc);
        costSoFar += getCost(type) * (distance + (homeIndex + 1));

        return true;
    }

    bool solved(int room) {
        int correct = 'A' + room;
        if(rooms[room][0] == correct && rooms[room][1] == correct)
            return true;
        return false;
    }

    bool hasIssue(int room) {
        int correct = 'A' + room;
        if((rooms[room][0] != correct && rooms[room][0] != '.') ||
           (rooms[room][1] != correct && rooms[room][1] != '.'))
            return true;
        return false;
    }

    bool solved() {
        for(int i = 0; i < 4; i++) {
            if(!solved(i))
                return false;
        }
        return true;
    }

    void moveOut(int num, int index, int destination) {
        char type = rooms[num][index];

        rooms[num][index] = '.';
        hallway[destination] = type;

        int roomLoc = num * 2 + 2;
        int distance = abs(destination - roomLoc);
        costSoFar += getCost(type) * (distance + (index + 1));
    }

    vector<int> validExitLocations(int room) {
        int roomLoc = room * 2 + 2;
        vector<int> exits;
        for(int i : validRests) {
            if(clearPath(roomLoc, i))
                exits.push_back(i);
        }
        return exits;
    }

};

istream& operator>>(istream& is, Grid& g) {
    for(int i = 0; i < 4; i++) {
        is >> g.rooms[i][0];
        is >> g.rooms[i][1];
    }
    return is;
}
ostream& operator<<(ostream& os, const Grid& g) {
    for(const char& c : g.hallway)
        os << c;
    for(int j = 0; j < 2; j++) {
        os << endl << " ";
        for(int i = 0; i < 4; i++) {
        os << "#" << g.rooms[i][j];
        }
        os << "#";
    }
    cout << endl << g.costSoFar << endl;
    return os;
}


int solve(Grid& gr) {
    vector<Grid> searchStack;
    searchStack.push_back(gr);

    int minCompletedCost = INT_MAX;

    while(!searchStack.empty()) {
        Grid g = searchStack.back();
        searchStack.pop_back();
        //cout << g << endl;

        //always send anyone home if possible - keep doing so as long as possible
        bool done = false;
        while(!done) {
            done = true;
            for(int i : validRests) {
                if(g.hallway[i] != '.' && g.tryHome(i)) {
                    done = false;
                }
            }
        }

        if(g.solved()) {
            minCompletedCost = min(minCompletedCost, g.costSoFar);
            //cout << g << endl;
            continue;
        }

        for(int room = 0; room < 4; room++) {
            if(g.hasIssue(room)) {
                int indexOfTop = g.rooms[room][0] == '.' ? 1 : 0;
                vector<int> destinations = g.validExitLocations(room);
                for(const int& i : destinations) {
                    searchStack.emplace_back(Grid(g));
                    searchStack.back().moveOut(room, indexOfTop, i);
                }
            }
        }
    }
    return minCompletedCost;
}

int main()
{
    Grid g;
    ifstream in("../input.txt");
    in >> g;

    cout << solve(g) << endl;
}
