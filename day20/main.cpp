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

const int INIT_ROWS = 100;
const int INIT_COLS = INIT_ROWS;

struct Bounds {
    int minRow = 0;
    int maxRow = INIT_ROWS;
    int minCol = 0;
    int maxCol = INIT_COLS;
};

struct ImageMap {
    map<pair<int, int>, char> data;
    Bounds bounds;
    char boundary = '.';

    char getValue(int row, int col) const {
        if(row < bounds.minRow || row >= bounds.maxRow || col < bounds.minCol || col >= bounds.maxCol)
            return boundary;

        auto mapPointer = data.find({row, col});
        if(mapPointer == data.end())
            return '.';
        return mapPointer->second;
    }

    void growBounds() {
        bounds.minRow-=1;
        bounds.maxRow+=1;
        bounds.minCol-=1;
        bounds.maxCol+=1;
    }

    int getBinaryValue(int row, int col) const {
        string s;
        for(int i = -1; i <= 1; i++) {
            for(int j = -1; j <= 1; j++) {
                s += getValue(row + i, col + j) == '#' ? '1' : '0';
            }
        }
        return stoi(s, nullptr, 2);
    }


    ImageMap refine(const string& al) const {
        ImageMap m2;
        m2.bounds = bounds;
        m2.growBounds();
        m2.boundary = (boundary == '.') ? '#' : '.';
        for(int i = m2.bounds.minRow; i < m2.bounds.maxRow; i++) {
            for(int j = m2.bounds.minCol; j < m2.bounds.maxCol; j++) {
                int binVal = getBinaryValue(i, j);
                char c = al.at(binVal);
                if(c == '#')
                    m2.data[{i, j}] = c;
            }
        }
        return m2;
    }
};

ostream& operator<<(ostream& os, const ImageMap& m) {
    for(int i = m.bounds.minRow - 5; i < m.bounds.maxRow + 5; i++) {
        for(int j = m.bounds.minCol - 5; j < m.bounds.maxCol + 5; j++) {
            os << m.getValue(i, j);
        }
        os << endl;
    }
    os << endl;
    return os;
}

void getImage(ifstream& in, ImageMap& image) {
    for(int i = 0; i < INIT_ROWS; i++) {
        for(int j = 0; j < INIT_COLS; j++) {
            char c;
            in >> c;
            if(c == '#')
                image.data[{i, j}] = c;
        }
    }
}

int main()
{
    string algorithm;
    ImageMap image;
    ifstream in("../Input.txt");
    in >> algorithm;

    getImage(in, image);

    ImageMap step1 = image.refine(algorithm);
    ImageMap step2 = step1.refine(algorithm);

    cout << step2.data.size() << endl;
    cout << "-----------------------------------" << endl;

    ImageMap enhanced = image;
    for(int i = 0; i < 50; i++) {
        enhanced = enhanced.refine(algorithm);
    }
    cout << enhanced.data.size() << endl;
}
