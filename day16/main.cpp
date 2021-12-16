#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <numeric>
#include <vector>
#include <charconv>
#include <string_view>


using namespace std;

string charToBinString(char c) {
    switch(c) {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
    }
    return "?";
}

string inputInBinary() {
    ifstream in("../input.txt");
    string input;
    in >> input;

    string bin;
    for(const char& c : input) {
        bin += charToBinString(c);
    }
    return bin;
}


struct Packet {
    int version;
    int type;
    long long value = 0;
    vector<Packet> subPackets;

    long long getValue() const;

    operator long long () const { return getValue(); }
};

long long Packet::getValue() const {
    switch(type) {
        case 0 : return accumulate(subPackets.begin(), subPackets.end(), 0LL);
        case 1 : return accumulate(subPackets.begin(), subPackets.end(), 1LL, multiplies<long long>());
        case 2 : return min_element(subPackets.begin(), subPackets.end())->getValue();
        case 3 : return max_element(subPackets.begin(), subPackets.end())->getValue();
        case 4 : return value;
        case 5 : return subPackets[0].getValue() > subPackets[1].getValue() ? 1 : 0;
        case 6 : return subPackets[0].getValue() < subPackets[1].getValue() ? 1 : 0;
        case 7 : return subPackets[0].getValue() == subPackets[1].getValue() ? 1 : 0;
    }
    return 0;
}


Packet parseInput(string_view& input) {
    //cout << "start" << input << endl;
    Packet p;
    from_chars(input.begin(), input.begin() + 3, p.version, 2);
    from_chars(input.begin() + 3, input.begin() + 6, p.type, 2);
    input.remove_prefix(6);

    if(p.type == 4) {
        string val;
        char contBit;
        do {
            contBit = input[0];
            val += input.substr(1, 4);
            input.remove_prefix(5);
        }
        while(contBit == '1');
        p.value = stoll(val, nullptr, 2);

    } else {
        char lengthT = input[0];
        if(lengthT == '0') {
            int subPLength;
            from_chars(input.begin() + 1, input.begin() + 16, subPLength, 2);
            input.remove_prefix(16);

            string_view subs = input.substr(0, subPLength);
            while(subs.length() != 0) {
                p.subPackets.emplace_back(parseInput(subs));
            }
            input.remove_prefix(subPLength);
        } else {
            int numSubPs;
            from_chars(input.begin() + 1, input.begin() + 12, numSubPs, 2);
            input.remove_prefix(12);
            while(numSubPs-- > 0) {
                p.subPackets.emplace_back(parseInput(input));
            }
        }
    }
    return p;
}

int versionSum(const Packet& p) {
    int sum = p.version;
    for(const Packet& other : p.subPackets)
        sum += versionSum(other);
    return sum;
}

int main()
{
    string input = inputInBinary();
    string_view s(input);
    Packet root = parseInput(s);

    cout << versionSum(root) << endl;
    cout << "-------------------------------" << endl;
    cout << root.getValue() << endl;
}
