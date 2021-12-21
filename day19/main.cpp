#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <numeric>
#include <vector>
#include <map>
#include <set>
#include <stack>

using namespace std;

struct Transform {
    int matrix[3][3] = {};
};

const Transform IDENTITY = {{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}};
const Transform SPIN = {{{0, -1, 0}, {1, 0, 0}, {0, 0, 1}}};
const Transform ROTATE = {{{1, 0, 0}, {0, 0, 1}, {0, -1, 0}}};

Transform operator*(const Transform& t1, const Transform& t2) {
    Transform result;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            for(int k = 0; k < 3; k++) {
                result.matrix[i][j] += t1.matrix[i][k] * t2.matrix[k][j];
            }
        }
    }
    return result;
}


//all 24 possible transform matricies
vector<Transform> getTransforms() {
    vector<Transform> vt;

    Transform temp = IDENTITY;
    for(int rot = 0; rot < 4; rot++) {
        for(int spin = 0; spin < 4; spin++) {
            vt.emplace_back(temp);
            temp = SPIN * temp;
        }
        temp = ROTATE * temp;
    }

    temp = ROTATE * SPIN;
    for(int spin = 0; spin < 4; spin++) {
        vt.emplace_back(temp);
        temp = SPIN * temp;
    }

    temp = ROTATE * SPIN * SPIN * SPIN;
    for(int spin = 0; spin < 4; spin++) {
        vt.emplace_back(temp);
        temp = SPIN * temp;
    }

    return vt;
}


struct Location {
    int x = 0, y = 0, z = 0;
    Location operator-(const Location& other) const { return Location({x - other.x, y - other.y, z - other.z}); }
    Location operator+(const Location& other) const { return Location({x + other.x, y + other.y, z + other.z}); }
    bool operator<(const Location& other) const {
        if(x < other.x) return true;
        else if(x > other.x) return false;
        else
            if(y < other.y) return true;
            else if(y > other.y) return false;
            else
                if(z < other.z) return true;
                else if(z > other.z) return false;
        return false;
    }
    bool operator==(const Location& other) const { return x == other.x && y == other.y && z == other.z; }
};
ostream& operator<<(ostream& os, const Location& l) {
    os << l.x << "," << l.y << "," << l.z << endl;
    return os;
}

Location operator*(const Transform& t, const Location& l) {
    Location result;
    result.x += t.matrix[0][0] * l.x + t.matrix[0][1] * l.y + t.matrix[0][2] * l.z;
    result.y += t.matrix[1][0] * l.x + t.matrix[1][1] * l.y + t.matrix[1][2] * l.z;
    result.z += t.matrix[2][0] * l.x + t.matrix[2][1] * l.y + t.matrix[2][2] * l.z;
    return result;
}


struct ScannerData {
    vector<Location> beacons;
};


struct Relation {
    bool overlap = false;
    Location offset;
    Transform orientation;
};


void parseInput(vector<ScannerData>& sd) {
    ifstream in("../Input.txt");
    while(!in.eof()) {
        string line;
        getline(in, line);

        if(line.length() == 0)
            continue;

        if(line.length() > 2 && line.substr(0, 3) == "---") {
            sd.emplace_back( ScannerData() );
            continue;
        }

        string::size_type comma1 = line.find(",");
        string::size_type comma2 = line.find(",", comma1 + 1);

        Location loc;
        loc.x = stoi(line.substr(0, comma1));
        loc.y = stoi(line.substr(comma1 + 1, comma2 - comma1 - 1));
        loc.z = stoi(line.substr(comma2 + 1));

        sd.back().beacons.push_back(loc);
    }
}



Relation getRelation(const ScannerData& sd1, const ScannerData& sd2, const vector<Transform> allTransforms) {
    Relation r;

    map<Location, int> seenCounts;

    for(const Transform& t : allTransforms) {
        for(const Location& sd1Loc : sd1.beacons) {
            for(const Location& sd2Loc : sd2.beacons) {
                Location sd2Adjusted = t * sd2Loc;
                Location diff = sd1Loc - sd2Adjusted;
                seenCounts[diff]++;
                if(seenCounts[diff] >= 12) {
                    r.offset = diff;
                    r.overlap = true;
                    r.orientation = t;
                    return r;
                }
            }
        }
    }

    return r;
}


map<pair<int,int>, Relation> findRelations(const vector<ScannerData>& sd) {

    map<pair<int,int>, Relation> relations;

    vector<Transform> allTransforms = getTransforms();
    for(size_t i = 0; i < sd.size(); i++) {
        for(size_t j = 0; j < sd.size(); j++) {
            Relation r = getRelation(sd[i], sd[j], allTransforms);
            if(r.overlap) {
                relations[{i, j}] = r;
                cout << i << "->" << j << endl;
                cout << "   " << r.offset << endl;
            }
        }
    }
    return relations;
}

void remapCoords(const Relation& r, vector<Location>& beacons) {
    for(Location& b : beacons) {
        b = r.orientation * b + r.offset;
        //cout << b;
    }
}

//connect out the parent map so that everything connects to 0 explictly
void connectAllToZero(const vector<ScannerData>& scanData, map<pair<int,int>, Relation>& overlaps) {
    vector<int> parents(scanData.size(), -1);
    parents[0] = INT_MAX;
    stack<int> todo;
    todo.push(0);
    while(!todo.empty()) {
        int cur = todo.top();
        todo.pop();
        for(size_t i = 0; i < parents.size(); i++) {
            if(parents[i] != -1)
                continue;

            auto relation = overlaps.find({cur, i});
            if(relation != overlaps.end()) {
                parents[i] = cur;
                todo.push(i);
            }
        }
    }

    for(size_t i = 1; i < parents.size(); i++) {
        int curLocation = parents[i];
        Relation newRel = overlaps[{curLocation, i}];
        while(curLocation != 0) {
            int parentLoc = parents[curLocation];
            Relation parentRel = overlaps[{parentLoc, curLocation}];
            newRel.offset = parentRel.orientation * newRel.offset + parentRel.offset;
            newRel.orientation = parentRel.orientation * newRel.orientation;
            curLocation = parentLoc;
        }
        overlaps[{0, i}] = newRel;
    }

}

int distance(const Location& l1, const Location& l2) {
    Location diff = l1 - l2;
    return abs(diff.x) + abs(diff.y) + abs(diff.z);
}

int main()
{
    vector<ScannerData> scanData;
    parseInput(scanData);

    map<pair<int,int>, Relation> overlaps = findRelations(scanData);
    connectAllToZero(scanData, overlaps);

    for(size_t i = 0; i < scanData.size(); i++) {
        Relation& r = overlaps[{0, i}];
        remapCoords(r, scanData[i].beacons);
    }

    set<Location> trueLocs;
    for(auto& scanner : scanData) {
        for(auto& beacon : scanner.beacons)
            trueLocs.insert(beacon);
    }
    cout << trueLocs.size() << endl;
    cout << "-----------------------------------" << endl;

    int maxDistance = 0;

    for(size_t i = 0; i < scanData.size(); i++) {
        for(size_t j = i + 1; j < scanData.size(); j++) {
            Relation& r1 = overlaps[{0, i}];
            Relation& r2 = overlaps[{0, j}];
            int dist = distance(r1.offset, r2.offset);
            if(dist > maxDistance)
                maxDistance = dist;
        }
    }

    cout << maxDistance << endl;
}
