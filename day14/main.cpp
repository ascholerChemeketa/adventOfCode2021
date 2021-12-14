#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>


using namespace std;

const int NUM_RULES = 100;
const int NUM_ITERATIONS_P1 = 10;
const int NUM_ITERATIONS_P2 = 40;

struct Rule {
    string pair;
    char insertion;
};

void getData(string& start, Rule rules[NUM_RULES]) {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

    in >> start;

    string line;
    getline(in, line);  //end of start
    getline(in, line);  //empty

    for(int i = 0; i < NUM_RULES; i++) {
        getline(in, line);
        rules[i].pair = line.substr(0, 2);
        rules[i].insertion = line.at(6);
    }
}

string doIteration(const string& curTemplate, Rule rules[NUM_RULES]) {
    string newTemplate;
    newTemplate.reserve(10000000000);
    for(unsigned int i = 0; i < curTemplate.length() - 1; i++) {
        string pattern = curTemplate.substr(i, 2);
        newTemplate += pattern.at(0);

        for(int ruleNum = 0; ruleNum < NUM_RULES; ruleNum++) {
            if(pattern == rules[ruleNum].pair) {
                newTemplate += rules[ruleNum].insertion;
                break;
            }
        }
    }

    newTemplate += curTemplate.back();
    return newTemplate;
}

void buildLetterCounts(const string& polyTemplate, int counts[26]) {
    for(unsigned int i = 0; i < polyTemplate.length(); i++) {
        int letterIndex = polyTemplate.at(i) - 'A';
        counts[letterIndex]++;
    }
}

int getMaxMinRange(const string& polyTemplate) {
    int counts[26] = {};
    buildLetterCounts(polyTemplate, counts);

    int min = INT_MAX;
    int max = 0;
    for(int i = 0; i < 26; i++) {
        if(counts[i] > max)
            max = counts[i];
        else if(counts[i] > 0 && counts[i] < min)
            min = counts[i];
    }
    //cout << max << " " << min << endl;
    return max - min;
}

void day13p1() {
    Rule rules[NUM_RULES];
    string curTemplate;

    getData(curTemplate, rules);

    for(int i = 0; i < NUM_ITERATIONS_P1; i++) {
        curTemplate = doIteration(curTemplate, rules);
        //cout << curTemplate << endl;
    }

    cout << getMaxMinRange(curTemplate) << endl;
}

///-------------------------------------------------------------------

typedef std::unordered_map<string, uint64_t> StringIntMap;
typedef std::unordered_map<string, char> StringCharMap;
typedef std::unordered_map<char, int> CharIntMap;

StringIntMap makePairs(const string& polyTemplate) {
    StringIntMap pairMap;
    for(unsigned int i = 0; i < polyTemplate.length() - 1; i++) {
        string pattern = polyTemplate.substr(i, 2);
        pairMap[pattern] = 1;
    }
    return pairMap;
}

StringCharMap makeRulePairs(Rule rules[NUM_RULES]) {
    StringCharMap pairMap;
    for(int i = 0; i < NUM_RULES; i++) {
        pairMap[ rules[i].pair ] = rules[i].insertion;
    }
    return pairMap;
}

void setOrModify(StringIntMap& pairs, const string& pair, uint64_t amount) {
    auto pairIt = pairs.find(pair);
    if(pairIt != pairs.end()) {
        pairIt->second += amount;
    } else
        pairs[pair] = amount;

    //cout << "  " << pair << " " << pairs[pair] << endl;
}

StringIntMap doMapIteration(const StringIntMap& pairs, const StringCharMap& ruleMap) {
    StringIntMap newPairs = pairs;
    for(const auto& kv : pairs) {
        string pair = kv.first;
        uint64_t copies = kv.second;
        if(copies == 0)
            continue;

        auto ruleIt = ruleMap.find(pair);
        if(ruleIt != ruleMap.end()) {
            string newPair1 = pair;
            newPair1[1] = ruleIt->second;
            string newPair2 = pair;
            newPair2[0] = ruleIt->second;
            //cout << pair << "-" << copies << " " << newPair1 << " " << newPair2 << endl;
            setOrModify(newPairs, newPair1, copies);
            setOrModify(newPairs, newPair2, copies);
            auto pairIt = newPairs.find(pair);
            pairIt->second -= copies;
            //cout << pair << "-" << copies << endl;
        }
    }
    return newPairs;
}

vector<uint64_t> getLetterCounts(const StringIntMap& pairs) {
    vector<uint64_t> counts(26, 0);
    for(const auto& kv : pairs) {
        string pair = kv.first;
        counts[pair[0] - 'A'] += kv.second;
    }
    return counts;
}


void day13p2() {
    Rule rules[NUM_RULES];
    string startTemplate;
    getData(startTemplate, rules);

    StringIntMap pairs = makePairs(startTemplate);
    StringCharMap ruleMap = makeRulePairs(rules);

    for(int i = 0; i < NUM_ITERATIONS_P2; i++) {
        //cout << "------------------------------------" << endl;
        //cout << i << endl;
        pairs = doMapIteration(pairs, ruleMap);
    }

    auto counts = getLetterCounts(pairs);
    //add one copy of last letter
    counts[startTemplate.back() - 'A']++;

    counts.erase(remove(counts.begin(), counts.end(), 0), counts.end());
    uint64_t min = *min_element(counts.begin(), counts.end());
    uint64_t max = *max_element(counts.begin(), counts.end());

    cout << (max - min) << endl;
}

int main()
{
    day13p1();
    cout << "-------------------------------" << endl;
    day13p2();
}
