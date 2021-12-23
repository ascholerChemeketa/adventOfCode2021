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

using byte = uint8_t;

const int BOUNDS = INT_MAX;

enum Dimension {
    X, Y, Z
};

enum Dir {
    MIN, MAX
};

struct Cuboid {
    int coords[3][2] = {};  //DIMENSION x,y,z  DIR min,max
    bool on = true;

    Cuboid* children[3][2] = {};  //DIMENSION x,y,z  DIR min,max

    Cuboid() {}
    Cuboid(const Cuboid& other) {
        memcpy(coords, other.coords, 6 * sizeof(int));
        on = other.on;
    }
    ~Cuboid() {
        for(int i = 0; i < 3; i++) {
            if( children[i][MIN] ) delete children[i][MIN];
            if( children[i][MAX] ) delete children[i][MAX];
        }
    }
    Cuboid& operator=(const Cuboid& other);

    //Find cuboid containing bits extending beyond c in dimension/direction pair
    // If expansive, other two dimesions can extend past c
    // If not expansive, contrain other 2 dimensions to c's dimensions
    Cuboid* partBeyond(const Cuboid* c, Dimension d, Dir dir) {
        if(dir == MAX && (coords[d][MAX] <= c->coords[d][MAX]))
                return nullptr;
        if(dir == MIN && (coords[d][MIN] >= c->coords[d][MIN]))
                return nullptr;

        Cuboid* part = new Cuboid(*this);

        //Copy all dimensions, contraining if needed
        for(int i = 0; i < 3; i++) {
            //X cuts expand in Y and Z; Y cuts expand in Z
            if(i >= d) {
                //Expand dimension as far as possible
                part->coords[i][MIN] = coords[i][MIN];
                part->coords[i][MAX] = coords[i][MAX];
            } else {
                //Constrain dimension to cut
                part->coords[i][MIN] = max(coords[i][MIN], c->coords[i][MIN]);
                part->coords[i][MAX] = min(coords[i][MAX], c->coords[i][MAX]);
                if(part->coords[i][MIN] > part->coords[i][MAX]) {
                    delete part;
                    return nullptr;
                }
            }
        }

        //Now crop off the key dimension
        if(dir == MAX) {
            part->coords[d][MIN] = max(coords[d][MIN], c->coords[d][MAX] + 1);
            part->coords[d][MAX] = coords[d][MAX];
        } else {
            part->coords[d][MAX] = min(coords[d][MAX], c->coords[d][MIN] - 1);
            part->coords[d][MIN] = coords[d][MIN];
        }
        return part;
    }

    uint64_t getVolume() const {
        uint64_t v = 1;
        for(int i = 0; i < 3; i++) {
            v *= max(coords[i][MAX] - coords[i][MIN] + 1, 0);
        }
        return v;
    }

    uint64_t getNumOn() {
        uint64_t v = 0;
        for(Dimension i = X; i <= Z; i = static_cast<Dimension>(i + 1)) {
            if(children[i][MIN]) v += children[i][MIN]->getNumOn();
            if(children[i][MAX]) v += children[i][MAX]->getNumOn();
        }
        if(on)
            v += getVolume();
        return v;
    }
};

//Smashes a cuboid into another, by removing overlap and attaching parts to parent as children
// Return parent
Cuboid* pushInto(Cuboid* parent, Cuboid* target) {
    if(parent == nullptr)       //If no parent, this is now the thing
        return target;
    if(target == nullptr)
        return parent;          //If nothing there, nevermind

    for(Dimension i = X; i <= Z; i = static_cast<Dimension>(i + 1)) {
        parent->children[i][MIN] = pushInto(parent->children[i][MIN], target->partBeyond(parent, i, MIN));
        parent->children[i][MAX] = pushInto(parent->children[i][MAX], target->partBeyond(parent, i, MAX));
    }
    delete target;
    return parent;
}

ostream& operator<<(ostream& os, const Cuboid* c) {
    if(c)
        for(int i = 0; i < 3; i++) {
            os << c->coords[i][MIN] << "->" << c->coords[i][MAX] << "  ";
        }
    else
        os << "null";
    return os;
}

Cuboid parseLine(const string& line) {
    Cuboid c;
    string_view s(line);
    if(s[1] == 'f') c.on = false;

    auto i = s .find('=');
    s.remove_prefix(i + 1);
    i = s.find('.');
    from_chars(s.begin(), s.begin() + i, c.coords[X][MIN]);
    s.remove_prefix(i + 2);
    i = s.find(',');
    from_chars(s.begin(), s.begin() + i, c.coords[X][MAX]);
    s.remove_prefix(i + 3);
    i = s.find('.');
    from_chars(s.begin(), s.begin() + i, c.coords[Y][MIN]);
    s.remove_prefix(i + 2);
    i = s.find(',');
    from_chars(s.begin(), s.begin() + i, c.coords[Y][MAX]);
    s.remove_prefix(i + 3);
    i = s.find('.');
    from_chars(s.begin(), s.begin() + i, c.coords[Z][MIN]);
    s.remove_prefix(i + 2);
    from_chars(s.begin(), s.end(), c.coords[Z][MAX]);

    for(int i = 0; i < 3; i++) {
        c.coords[i][MIN] = max(c.coords[i][MIN], -BOUNDS);
        c.coords[i][MAX] = min(c.coords[i][MAX], BOUNDS);
    }
    return c;
}


uint64_t getVolume(const Cuboid* p) {
    return p ? p->getVolume() : 0;
}

void day22p1() {
}


int main()
{
    deque<Cuboid> steps;

    ifstream in("../Input.txt");
    string line;
    while(getline(in, line)) {
        steps.emplace_front( parseLine(line) );
    }

    Cuboid* root = nullptr;
    for(auto& c : steps) {
        root = pushInto(root, new Cuboid(c));
    }
    cout << root->getNumOn() << endl;
    delete root;
}
