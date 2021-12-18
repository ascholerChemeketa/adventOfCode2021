#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <numeric>
#include <vector>
#include <charconv>
#include <string_view>


using namespace std;

struct Target{
    int x1, x2, y1, y2;
};

enum ResultCategory { UNDER, OVER, FAIL_TO_REACH, HIT, OTHER };

struct Result {
    bool hit = false;
    int maxHeight = INT_MIN;
    int finalY = 0;
    int finalX = 0;
    int finalXVel = 0;
    int finalYVel = 0;
    ResultCategory kind = OTHER;
};

int getMaxSteps(int xVel, int maxX) {
    int count = 0;
    int pos = 0;
    while(pos < maxX) {
        pos += xVel--;
        count++;
    }
    return count;
}

Result fire(int xVel, int yVel, const Target& t) {
    int x = 0, y = 0;
    Result r;
    for(int i = 0; true; i++) {
        x += xVel;
        if(xVel > 0) xVel--;
        if(xVel < 0) xVel++;

        y += yVel--;
        if(y > r.maxHeight)
            r.maxHeight = y;

        if(t.x1 <= x && x <= t.x2 && t.y1 <= y && y <= t.y2) {
            r.hit = true;
            r.kind = HIT;
        }

        if(r.hit && y < 0)
            break;   //Once hit, wait for it to stop going up
        else if(x > t.x2 && xVel > 0) {
            break;   //Overshot horizontal
        } else if(y < t.y1 && yVel < 0) {
            break;   //Overshot vertically
        }
    }

    //Actually does OK without this complexity
    if(x < t.x1 && xVel == 0)
        r.kind = FAIL_TO_REACH;             //never made it
    else if(y < t.y1 && x < t.x1)
        r.kind = UNDER;                     //undershot
    else if(y > t.y2 && x > t.x2)
        r.kind = OVER;                      //overshot

    return r;
}


void day17p1(const Target& t) {
    int maxHeightSeen = 0;
    //No point in speeds sufficient to move past target in one step
    for(int xVel = 1; xVel <= t.x2; xVel++) {
        for(int yVel = 0; yVel <= -t.y1; yVel++) {
            Result r = fire(xVel, yVel, t);
            if(r.hit) {
                if(r.maxHeight > maxHeightSeen)
                    maxHeightSeen = r.maxHeight;
            } else {
                //Runs fine without!!!
                if(r.kind == OVER || r.kind == FAIL_TO_REACH)
                    break; //no need to keep trying higher y speeds
            }
        }
    }
    cout << maxHeightSeen << endl;
}


void day17p2(const Target& t) {
    int count = 0;
    //No point in speeds sufficient to move past target in one step
    for(int xVel = 1; xVel <= t.x2; xVel++) {
        //Try up first
        //No point in speeds that on return will be moving faster than gap to target (upper bound -t.y1)
        for(int yVel = 0; yVel <= -t.y1; yVel++) {
            Result r = fire(xVel, yVel, t);
            if(r.hit) {
                count++;
            } else {
                if(r.kind == OVER || r.kind == FAIL_TO_REACH)
                    break; //no need to keep trying higher y speeds
            }
        }

        //Now down
        //No point in speeds sufficient to move past target in one step (lower bound t.y1)
        for(int yVel = -1;  yVel >= t.y1; yVel--) {
            Result r = fire(xVel, yVel, t);
            if(r.hit) {
                count++;
            } else {
                if(r.kind == UNDER || r.kind == FAIL_TO_REACH)
                    break; //undershot, no need to keep trying higher speeds
            }
        }
    }
    cout << count << endl;
}

int main()
{
    ifstream in("../input.txt");
    Target t;
    in >> t.x1 >> t.x2 >> t.y1 >> t.y2;

    //fire(12,-4,t);

    day17p1(t);
    cout << "-------------------------------" << endl;
    day17p2(t);
}
