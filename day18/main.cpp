#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <numeric>
#include <vector>
#include <list>
#include <stack>
#include <charconv>
#include <string_view>
#include <memory>


using namespace std;

struct SnailNum {
    weak_ptr<SnailNum> parent;
    SnailNum(weak_ptr<SnailNum> p) : parent(p) {}
    virtual int getMagnitude() { return 0; }
    virtual string toString() { return "???"; }
    virtual bool isPair() { return false; }
    virtual bool isValue() { return false; }
    shared_ptr<SnailNum> getParent() { return parent.lock(); }
};

struct Pair : SnailNum {
    shared_ptr<SnailNum> left;
    shared_ptr<SnailNum> right;
    Pair(weak_ptr<SnailNum> p) : SnailNum(p) {}
    virtual int getMagnitude() {
        return 3 * left->getMagnitude() + 2 * right->getMagnitude();
    }
    virtual string toString() {
        string out = "[";
        out += left->toString();
        out += ",";
        out += right->toString();
        out += "]";
        return out;
    }
    virtual bool isPair() { return true; }
};

struct Value : SnailNum {
    int value;
    Value(int v, weak_ptr<SnailNum> p) : SnailNum(p), value(v) {}
    virtual int getMagnitude() { return value; }
    virtual string toString() { return to_string(value); }
    virtual bool isValue() { return true; }
};



int NUM_ADDITIONS = 1;

int extractValue(string_view& s) {
    int val;
    size_t found = s.find_first_of(",]");
    from_chars(s.begin(), s.begin() + found, val);
    s.remove_prefix(found);
    return val;
}

shared_ptr<SnailNum> parseNum(string_view& s, weak_ptr<SnailNum> parent) {
    shared_ptr<SnailNum> newNum;
    if(s.at(0) == '[') {
        shared_ptr<Pair>pairPtr = make_shared<Pair>( Pair(parent) );
        s.remove_prefix(1);  //[
        pairPtr->left = parseNum(s, pairPtr);
        s.remove_prefix(1);  //comma
        pairPtr->right = parseNum(s, pairPtr);
        s.remove_prefix(1);  //]
        newNum = pairPtr;
    } else {
        //value type, must have pair parent
        //dynamic_pointer_cast<Pair>(parent.lock())->children.emplace_back( extractValue(s) );
        newNum = make_shared<Value>( Value(extractValue(s), parent) );
    }
    return newNum;
}

void pushLeft(shared_ptr<SnailNum> s, int val) {
    auto parent = dynamic_pointer_cast<Pair>(s->getParent());
    auto current = s;
    while(parent) {
        if(current == parent->right)
            break;
        current = parent;
        parent = dynamic_pointer_cast<Pair>(parent->getParent());
    }
    if(parent) {
        auto target = parent->left;
        while(target->isPair())
            target = dynamic_pointer_cast<Pair>(target)->right;
        dynamic_pointer_cast<Value>(target)->value += val;
    }
}

void pushRight(shared_ptr<SnailNum> s, int val) {
    auto parent = dynamic_pointer_cast<Pair>(s->getParent());
    auto current = s;
    while(parent) {
        if(current == parent->left)
            break;
        current = parent;
        parent = dynamic_pointer_cast<Pair>(parent->getParent());
    }
    if(parent) {
        auto target = parent->right;
        while(target->isPair())
            target = dynamic_pointer_cast<Pair>(target)->left;
        dynamic_pointer_cast<Value>(target)->value += val;
    }
}

bool explode(shared_ptr<SnailNum> s, int curDepth) {
    auto asPair = dynamic_pointer_cast<Pair>(s);
    if(asPair) {
        //Is pair of values deeper than 4?
        if(curDepth >= 4) {
            //left
            auto asPair = dynamic_pointer_cast<Pair>(s);
            int leftToMove = dynamic_pointer_cast<Value>(asPair->left)->value;
            int rightToMove = dynamic_pointer_cast<Value>(asPair->right)->value;

            //left
            pushLeft(s, leftToMove);
            //right
            pushRight(s, rightToMove);

            //remove from parent
            auto parent = dynamic_pointer_cast<Pair>(s->getParent());
            if(parent->left == s)
                parent->left = make_shared<Value>( Value(0, parent) );
            else
                parent->right = make_shared<Value>( Value(0, parent) );

            return true;
        }

        //Check children - if they explode, explode is done
        if( explode(asPair->left, curDepth + 1) )
            return true;
        if( explode(asPair->right, curDepth + 1) )
            return true;
    }

    return false;
}

bool split(shared_ptr<SnailNum> s) {
    if(s->isValue()) {
        int value = s->getMagnitude();
        if(value >= 10) {
            int leftVal = value / 2;
            int rightVal = value - leftVal;

            auto parent = dynamic_pointer_cast<Pair>(s->getParent());
            auto newPair = make_shared<Pair>( Pair(parent) );
            newPair->left = make_shared<Value>( Value(leftVal, newPair) );
            newPair->right = make_shared<Value>( Value(rightVal, newPair) );

            if(parent->left == s)
                parent->left = newPair;
            else
                parent->right = newPair;

            return true;
        }
    }
    else if(s->isPair()) {
        auto asPair = dynamic_pointer_cast<Pair>(s);
        //Check children - if they split, split is done
        if( split(asPair->left) )
            return true;
        if( split(asPair->right) )
            return true;
    }

    return false;
}

shared_ptr<SnailNum> makeSum(shared_ptr<SnailNum> s1, shared_ptr<SnailNum> s2) {
    shared_ptr<Pair>pairPtr = make_shared<Pair>( Pair(weak_ptr<SnailNum>()) );
    pairPtr->left = s1;
    s1->parent = pairPtr;
    pairPtr->right = s2;
    s2->parent = pairPtr;


    bool modified = true;
    while(modified) {
        modified = explode(pairPtr, 0);
        if(!modified)
            modified = split(pairPtr);
        //cout << sum->toString() << endl;
    }

    return pairPtr;
}

void day18p1(list<shared_ptr<SnailNum>>& nums) {
    auto probIt = nums.begin();
    auto sum = *probIt;
    ++probIt;
    while(probIt != nums.end()) {
        sum = makeSum(sum, *probIt);
        //cout << sum->toString() << endl;
        //cout << "---------------------------------" << endl;
        ++probIt;
    }
    cout << sum->getMagnitude() << endl;
}

shared_ptr<SnailNum> deepCopy(shared_ptr<SnailNum> s, weak_ptr<SnailNum> p) {
    shared_ptr<SnailNum> result;
    if(s->isValue()) {
        result = make_shared<Value>( Value(s->getMagnitude(), p) );
    }
    else if(s->isPair()) {
        shared_ptr<Pair> asPair = dynamic_pointer_cast<Pair>(s);
        shared_ptr<Pair> newPair = make_shared<Pair>(p);
        newPair->left = deepCopy(asPair->left, newPair);
        newPair->right = deepCopy(asPair->right, newPair);
        result = newPair;
    }
    return result;
}

void day18p2(list<shared_ptr<SnailNum>>& nums) {
    int maxMagnitude = INT_MIN;
    for(auto p1 : nums) {
        for(auto p2 : nums) {
            if(p1 == p2)
                continue;

            auto p1Copy = deepCopy(p1, weak_ptr<SnailNum>());
            auto p2Copy = deepCopy(p2, weak_ptr<SnailNum>());
            auto sum = makeSum(p1Copy, p2Copy);
            int magnitude = sum->getMagnitude();

            if(magnitude > maxMagnitude) {
                cout << p1->toString() << endl;
                cout << p2->toString() << endl;
                cout << sum->toString() << endl;
                cout << "--------------------" << endl;
                maxMagnitude = magnitude;
            }
        }
    }
    cout << maxMagnitude << endl;
}


const int NUM_PROBLEMS = 100;
int main()
{
    ifstream in("../input.txt");
    list<shared_ptr<SnailNum>> nums;
    for(int i = 0; i < NUM_PROBLEMS; i++) {
        string s;
        in >> s;
        string_view sv(s);
        nums.emplace_back( parseNum(sv, weak_ptr<SnailNum>()) );
    }

    //Part1 will mess up data for part 2.... run one or other
    //day18p1(nums);
    cout << "-------------------------------" << endl;
    day18p2(nums);
}
