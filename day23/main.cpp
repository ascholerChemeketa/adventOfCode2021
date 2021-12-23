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

const int ROOM_SIZE = 4;

bool isOpen(char c) {
    return c == '.';
}

struct Grid {
    vector<vector<char>> rooms;
    vector<char> hallway;

    int costSoFar = 0;

    Grid() {
        vector<char> room(ROOM_SIZE, '.');
        rooms.insert(rooms.end(), 4, room);
        hallway.insert(hallway.end(), 11, '.');
    }

    int deepestClearSpot(int room) {
        return distance(find(rooms[room].rbegin(), rooms[room].rend(), '.'), rooms[room].rend()) - 1;
    }

    bool partiallySolved(int room) {
        char correct = 'A' + room;
        return all_of(rooms[room].begin(), (rooms[room].end()), [correct](char c) { return c == correct || c == '.'; });
    }

    bool solved(int room) {
        char correct = 'A' + room;
        return all_of(rooms[room].begin(), (rooms[room].end()), [correct](char c) { return c == correct; });
    }

    bool hasIssue(int room) {
        int correct = 'A' + room;
        return any_of(rooms[room].begin(), rooms[room].end(), [correct](char c) { return c != correct && c != '.'; });
    }

    bool solved() {
        for(int i = 0; i < 4; i++) {
            if(!solved(i))
                return false;
        }
        return true;
    }

    //inclusive of to, not of from
    bool clearPath(int from, int to) {
        if(from > to) {
            swap(--from, --to);
        }
        return all_of(hallway.begin() + from + 1, hallway.begin() + to + 1, isOpen);
    }

    bool tryHome(int hallLoc) {
        char type = hallway[hallLoc];
        int home = type - 'A';
        if(!partiallySolved(home))
            return false;

        int homeLoc = home * 2 + 2;

        if(!clearPath(hallLoc, homeLoc)) {
            return false;
        }

        int homeIndex = deepestClearSpot(home);
        hallway[hallLoc] = '.';
        rooms[home][homeIndex] = type;

        int distance = abs(hallLoc - homeLoc);
        costSoFar += getCost(type) * (distance + (homeIndex + 1));

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
        for(int j = 0; j < ROOM_SIZE; j++) {
            is >> g.rooms[i][j];
        }
    }
    return is;
}

ostream& operator<<(ostream& os, const Grid& g) {
    for(const char& c : g.hallway)
        os << c;
    for(int j = 0; j < ROOM_SIZE; j++) {
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
        } else {
            if(g.costSoFar > minCompletedCost)
                continue;
        }

        for(int room = 3; room >= 0; room--) {
            if(g.hasIssue(room)) {
                int indexOfTop = g.deepestClearSpot(room) + 1;
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

    auto start = clock();
    cout << solve(g) << endl;
    cout << (clock() - start) / 1000 << endl;
}
