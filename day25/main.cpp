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
#include <iterator>

using namespace std;

//true if any moces happen
bool attemptMoves(vector<string>& grid) {
    vector<string> newGrid = grid;
    bool modified = false;
    for(size_t i = 0; i < grid.size(); i++) {
        for(size_t j = 0; j < grid[0].size(); j++) {
            if(grid[i][j] == '>') {
                size_t newCol = j < grid[0].size() - 1 ? j + 1 : 0;
                if(grid[i][newCol] == '.') {
                    modified = true;
                    swap(newGrid[i][j], newGrid[i][newCol]);
                }
            }
        }
    }
    grid = newGrid;
    for(size_t i = 0; i < grid.size(); i++) {
        for(size_t j = 0; j < grid[0].size(); j++) {
            if(grid[i][j] == 'v') {
                size_t newRow = i < grid.size() - 1 ? i + 1 : 0;
                if(grid[newRow][j] == '.') {
                    modified = true;
                    swap(newGrid[i][j], newGrid[newRow][j]);
                }
            }
        }
    }
    grid = move(newGrid);
    return modified;
}

void printGrid(const vector<string>& grid) {
    for(const auto& row : grid) {
        cout << row << endl;
    }
}

int main()
{
    ifstream in("../Input.txt");
    vector<string> grid;
    string line;
    while(in >> line) {
        grid.push_back(line);
    }


    bool done = false;
    int stepCount = 0;
    while(!done) {
        done = !attemptMoves(grid);
        stepCount++;
    }
    printGrid(grid);
    cout << stepCount << endl;
}
