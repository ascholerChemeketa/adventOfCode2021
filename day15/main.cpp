#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <queue>


using namespace std;

const int SIZE = 100;

void getData(int grid[SIZE][SIZE]) {
    ifstream in("../input.txt");
    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++) {
            char c;
            in >> c;
            grid[i][j] = c - '0';
        }
}

void findCosts(int grid[SIZE][SIZE], int costs[SIZE][SIZE], int r, int c, int costOnPath) {
    if(r < 0 || c < 0 || r == SIZE || c == SIZE)
        return;

    int costViaPath = costOnPath + grid[r][c];
    if(costViaPath < costs[r][c]) {
        costs[r][c] = costViaPath;
        findCosts(grid, costs, r - 1, c, costViaPath);
        findCosts(grid, costs, r + 1, c, costViaPath);
        findCosts(grid, costs, r, c - 1, costViaPath);
        findCosts(grid, costs, r, c + 1, costViaPath);
    }
}

void day14p1() {
    int grid[SIZE][SIZE];
    getData(grid);

    int costs[SIZE][SIZE] = {};
    fill(&costs[0][0], &costs[0][0] + SIZE * SIZE, INT_MAX);

    findCosts(grid, costs, 0, 0, -grid[0][0]);

    cout << costs[SIZE - 1][SIZE - 1] << endl;
}

//------------------------------------------------------

const int EXPANDED_SIZE = 5 * SIZE;

//cost is cost for all steps but the last
struct Step {
    pair<int, int> from;
    pair<int, int> to;
    int fromCost = 0;
};

struct StepComparer {
    bool operator() (const Step& a, const Step& b)
    {
        return a.fromCost > b.fromCost;
    }
};

ostream& operator<<(ostream& out, const Step& s) {
    out << s.from.first << "," << s.from.second << "->" << s.to.first << "," << s.to.second << ":" << s.fromCost << endl;
    return out;
}


void getDataP2(int grid[EXPANDED_SIZE][EXPANDED_SIZE]) {
    ifstream in("../input.txt");
    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++) {
            char c;
            in >> c;
            grid[i][j] = c - '0';
        }

    for(int vertReps = 0; vertReps < 5; vertReps++)
        for(int horzReps = 0; horzReps < 5; horzReps++) {
            for(int i = 0; i < SIZE; i++)
                for(int j = 0; j < SIZE; j++) {
                    grid[vertReps * SIZE + i][horzReps * SIZE + j] = grid[i][j] + vertReps + horzReps;
                    if(grid[vertReps * SIZE + i][horzReps * SIZE + j] > 9)
                        grid[vertReps * SIZE + i][horzReps * SIZE + j] -= 9;
                }
        }
}

void calcCosts(int grid[EXPANDED_SIZE][EXPANDED_SIZE],
              int costs[EXPANDED_SIZE][EXPANDED_SIZE]) {

    int minCost = INT_MAX;
    priority_queue<Step, std::vector<Step>, StepComparer> steps;
    steps.emplace(Step({{0,0}, {1,0}, 200}));  //first two possible moves
    steps.emplace(Step({{0,0}, {0,1}, 200}));

    while(!steps.empty()) {
        Step s = steps.top();
        steps.pop();

        if(s.to.first < 0 || s.to.second < 0 || s.to.first == EXPANDED_SIZE || s.to.second == EXPANDED_SIZE)
            continue;

        int toCost = costs[s.from.first][s.from.second] + grid[s.to.first][s.to.second];
        if(toCost >= costs[s.to.first][s.to.second])
            continue;

        costs[s.to.first][s.to.second] = toCost;

        if(s.from.first > 0)
            steps.emplace(Step({{ s.to.first, s.to.second }, {s.to.first - 1, s.to.second}, toCost}));
        if(s.from.first < EXPANDED_SIZE - 1)
            steps.emplace(Step({{ s.to.first, s.to.second }, {s.to.first + 1, s.to.second}, toCost}));
        if(s.from.second > 0)
            steps.emplace(Step({{ s.to.first, s.to.second }, {s.to.first, s.to.second - 1}, toCost}));
        if(s.from.second < EXPANDED_SIZE - 1)
            steps.emplace(Step({{ s.to.first, s.to.second }, {s.to.first, s.to.second + 1}, toCost}));
    }
}

void day14p2() {
    int grid[EXPANDED_SIZE][EXPANDED_SIZE];
    getDataP2(grid);

    int costs[EXPANDED_SIZE][EXPANDED_SIZE] = {};
    fill(&costs[0][0], &costs[0][0] + EXPANDED_SIZE * EXPANDED_SIZE, INT_MAX);
    costs[0][0] = 0;

    calcCosts(grid, costs);

    cout << costs[EXPANDED_SIZE - 1][EXPANDED_SIZE - 1] << endl;
}



int main()
{
    clock_t start = clock();
    day14p1();
    cout << "elapsed time: " << clock() - start << endl;
    cout << "-------------------------------" << endl;
    start = clock();
    day14p2();
    cout << "elapsed time: " << clock() - start << endl;
}
