#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

const int DATA_SIZE = 22;
const int NUM_EDGES = 2 * DATA_SIZE;


const int MAX_PATHS = 1000000;

const string DEAD_PATH = "#";
//Each path will look like start,A,b... and eventually end with
// "end" or DEAD_PATH
struct PathList {
    string* paths = new string[MAX_PATHS];
    int numPaths = 0;
};

//Manage adding items to the PathList
void addToPathList(PathList& pl, const string& s) {
    if(pl.numPaths >= MAX_PATHS) {
        cout << "Out of path space!" << endl;
        exit(0);
    }

    pl.paths[pl.numPaths] = s;
    pl.numPaths++;
}

//returns number of copies of edge in path
int copiesIn(const string& path, string edge) {
    int copies = 0;
    edge = "," + edge;      //add comma to front so that we only detect complete matches
    string::size_type copyLoc = path.find(edge);
    while(copyLoc != string::npos) {
        copies++;
        copyLoc = path.find(edge, copyLoc + 1);
    }
    return copies;
}

//true if there is at least one lower case double cave
//only for part2
bool hasDoubleCave(const string& path) {
    string::size_type begin = 0;
    string::size_type end = -1;

    do {
        begin = end + 1;
        end = path.find(",", begin + 1);
        string cave;
        if(end != string::npos) {
            cave = path.substr(begin, (end - begin));
        } else {
            cave = path.substr(begin);
        }

        if(islower(cave[0]) && copiesIn(path, cave) >= 2)
            return true;

    } while(end != string::npos);

    return false;
}

//last location in a given path
string currentLocation(string& s) {
    string::size_type dashLoc = s.find_last_of(",");
    //forward edge
    return s.substr(dashLoc + 1);
}


struct Edge{
    string start;
    string end;
};


void getData(Edge edges[2*DATA_SIZE]) {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

    for(int i = 0; i < DATA_SIZE; i++) {
        string s;
        in >> s;
        string::size_type dashLoc = s.find("-");
        string first = s.substr(0, dashLoc);
        string second = s.substr(dashLoc + 1);
        //forward edge
        edges[2 * i].start = first;
        edges[2 * i].end = second;
        edges[2 * i + 1].start = second;
        edges[2 * i + 1].end = first;
    }
}

//allowDoubleCave use is difference between parts 1 and 2
void day12(bool allowDoubleCaveUse) {
    Edge edges[2 * DATA_SIZE];
    getData(edges);

    PathList inProgressPaths;

    addToPathList(inProgressPaths, "start");

    bool done = false;
    while(!done) {
        done = true;
        int numInProgress = inProgressPaths.numPaths;
        for(int i = 0 ; i < numInProgress; i++) {
            string curPath = inProgressPaths.paths[i];
            if(currentLocation(curPath) == DEAD_PATH || currentLocation(curPath) == "end")
                continue;

            bool hasExtended = false;
            for(int e = 0 ; e < NUM_EDGES; e++) {
                Edge curEdge = edges[e];
                if(curEdge.end == "start")
                    continue;
                if(curEdge.start == currentLocation(curPath)) {
                    //check if this is lower case and we already have it or if we are
                    if(islower(curEdge.end[0]) && curEdge.end != "end"){
                        int copies = copiesIn(curPath, curEdge.end);
                        if(!allowDoubleCaveUse && copies > 0)  //part1
                            continue;
                        if(allowDoubleCaveUse && (copies > 1 || (copies == 1 && hasDoubleCave(curPath)))) //part2
                            continue;
                    }

                    if(!hasExtended) {
                        //first match, replace what was there with new path
                        inProgressPaths.paths[i] = curPath + "," + curEdge.end;
                        hasExtended = true;
                    } else {
                        //add a new record for this match
                        string newPath = curPath + "," + curEdge.end;
                        addToPathList(inProgressPaths, newPath);
                    }
                }
            }
            if(!hasExtended) {
                //mark dead end
                inProgressPaths.paths[i] += "," + DEAD_PATH;
            } else {
                done = false;
            }
        }
    }

    int completePaths = 0;
    for(int i = 0 ; i < inProgressPaths.numPaths; i++) {
        if(currentLocation(inProgressPaths.paths[i]) == "end") {
            completePaths++;
        }
    }
    cout << completePaths << endl;
}


int main()
{
    day12(false);
    cout << "-------------------------------" << endl;
    day12(true);
}
