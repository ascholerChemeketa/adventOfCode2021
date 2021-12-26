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

enum class Op { inp, add, mul, div, mod, eql };

struct Instruction {
    Op type;
    int oprd1;
    int oprd2;
    bool reg2reg;

    void execute(vector<int>& registers, int input) const {
        if(type == Op::inp) {
            registers[oprd1] = input;
            //registers[oprd1] = input.get();
            return;
        }

        int val2 = reg2reg ? registers[oprd2] : oprd2;
        switch(type) {
        case Op::add:
            registers[oprd1] += val2;
            break;
        case Op::mul:
            registers[oprd1] *= val2;
            break;
        case Op::div:
            registers[oprd1] /= val2;
            break;
        case Op::mod:
            registers[oprd1] %= val2;
            break;
        case Op::eql:
            registers[oprd1] = registers[oprd1] == val2 ? 1 : 0;
            break;
        }
    }
};

istream& operator>>(istream& is, Instruction& i) {
    string type;
    is >> type;
    if(type == "inp")
        i.type = Op::inp;
    else if(type == "add")
        i.type = Op::add;
    else if(type == "mul")
        i.type = Op::mul;
    else if(type == "div")
        i.type = Op::div;
    else if(type == "mod")
        i.type = Op::mod;
    else if(type == "eql")
        i.type = Op::eql;

    if(is.fail())
        return is;

    char oprd;
    is >> oprd;
    i.oprd1 = oprd - 'w';

    i.reg2reg = true;

    if(i.type != Op::inp) {
        string oprd2;
        is >> oprd2;
        if(oprd2[0] > '9') {
            i.oprd2 = oprd2[0] - 'w';
        } else {
            i.oprd2 = stoi(oprd2);
            i.reg2reg = false;
        }
    }

    return is;
}


//Stores an output of a given stage of the program and the digits used to get there
struct PathRecord {
    int output;
    vector<int> digitsToReach;
    bool operator<(const PathRecord& o) const { return output < o.output; }
    bool operator==(const PathRecord& o) const { return output == o.output; }
};

void search(const vector<Instruction>& program, int stage, const set<PathRecord>& possibleInputs, const vector<int>& digits) {
    const static int z = 3;
    set<PathRecord> possibleOutputs;
    vector<int> mem(4, 0);

    for(int d : digits) {
        for(auto& path : possibleInputs) {
            int i = path.output;
            mem[z] = i;
            for(auto it = program.begin() + stage * 18; it < program.begin() + (stage + 1) * 18; ++it) {
                it->execute(mem, d);
            }

            if(mem[z] == 0 && stage == 13) {
                copy(path.digitsToReach.begin(), path.digitsToReach.end(), ostream_iterator<int>(cout, ""));
                cout << d << endl;
            }

            if(mem[z] < 20000) {
                PathRecord pr2 = path;
                pr2.output = mem[z];
                pr2.digitsToReach.push_back(d);
                possibleOutputs.insert(pr2);
            }
        }
    }

    if(stage < 13)
        search(program, stage + 1, possibleOutputs, digits);
}



int main()
{
    vector<Instruction> program;
    ifstream in("../Input.txt");

    Instruction i;
    while(in >> i) {
        program.push_back(i);
    }


    PathRecord p1;
    p1.output = 0;

    set<PathRecord> pr;
    pr.insert(p1);


    vector<int> upCount = {1,2,3,4,5,6,7,8,9};
    vector<int> downCount = {9,8,7,6,5,4,3,2,1};
    cout << "Part 1:" << endl;
    search(program, 0, pr, downCount);
    cout << "Part 2:" << endl;
    search(program, 0, pr, upCount);
}
