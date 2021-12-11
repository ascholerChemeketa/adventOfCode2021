#include <iostream>
#include <fstream>

using namespace std;

const int DATA_SIZE = 200;

void day8p1() {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

    int specialDigitCount = 0;
    for(int i = 0; i < DATA_SIZE; i++) {
        for(int j = 0; j < 15; j++) {
            string value;
            in >> value;

            if(j < 11)
                continue;  //skip before |

            if((value.length() >= 2 && value.length() <= 4) ||
               (value.length() == 7)) {
                specialDigitCount++;
            }
        }
    }

    cout << specialDigitCount << endl;
}

enum MappingIndex {
    A, B, C, D, E, F, G
};

bool contains(const string& s, char c) {
    if(s.find(c) != string::npos)
        return true;
    return false;
}

//remove c from source if present
string removeChar(string source, char c) {
    if(contains(source, c))
        source.erase(source.find(c), 1);
    return source;
}

//Finds all chars from possibilities in just one of the strings in the array
string inOne(const string& possibilities, const string arr[]) {
    string result = "";
    for(string::size_type i = 0; i < possibilities.length(); i++) {
        char curChar = possibilities.at(i);
        int inCount = 0;
        for(int j = 0; j < 3; j++) {
            if(contains(arr[j], curChar))
                inCount++;
        }
        if(inCount == 1)
            result += curChar;
    }
    return result;
}

//Finds all chars from source that are NOT in filter
string keepNotIn(const string& source, const string& filter) {
    string result = "";
    for(string::size_type i = 0; i < source.length(); i++) {
        char curChar = source.at(i);
        if(!contains(filter, curChar))
            result += curChar;
    }
    return result;
}

//Finds all chars from source that are in filter
string keepIn(const string& source, const string& filter) {
    string result = "";
    for(string::size_type i = 0; i < source.length(); i++) {
        char curChar = source.at(i);
        if(contains(filter, curChar))
            result += curChar;
    }
    return result;
}


int getDigit(string segments, char mapping[7]) {
    string::size_type len = segments.length();
    if(len == 2) {
        return 1;
    } else if(len == 3) {
        return 7;
    } else if(len == 4) {
        return 4;
    } else if(len == 7) {
        return 8;
    }
    //must be 5 or 6...

    //first remap
    for(string::size_type i = 0; i < segments.length(); i++) {
        char curValue = segments.at(i);
        segments.at(i) = mapping[curValue - 'a'];
    }

    if(len == 5) {
        if(!contains(segments, 'f'))
            return 2;
        else if(contains(segments, 'b'))
            return 5;
        else
            return 3;
    }

    if(len == 6) {
        if(!contains(segments, 'd'))
            return 0;
        else if(!contains(segments, 'c'))
            return 6;
        else
            return 9;
    }
    return -1;  //ruh roh
}

void day8p2() {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

    int totalSum = 0;

    for(int i = 0; i < DATA_SIZE; i++) {
        string twoLetterPattern;
        string threeLetterPattern;
        string fourLetterPattern;
        string fiveLetterPatterns[3];
        string sixLetterPatterns[3];
        string sevenLetterPattern;
        int fiveLetterPatternsSeen = 0;
        int sixLetterPatternsSeen = 0;

        for(int j = 0; j < 10; j++) {
            string value;
            in >> value;
            string::size_type len = value.length();
            if(len == 2)
                twoLetterPattern = value;
            else if(len == 3)
                threeLetterPattern = value;
            else if(len == 4)
                fourLetterPattern = value;
            else if(len == 5)
                fiveLetterPatterns[fiveLetterPatternsSeen++] = value;
            else if(len == 6)
                sixLetterPatterns[sixLetterPatternsSeen++] = value;
            else if(len == 7)
                sevenLetterPattern = value;
        }

        //mapping stores location of each char
        //mapping[A] = 'd' indicates that the 'a' segment is misswired to 'd'
        char mapping[7] = {};

        //the chars still unmapped
        string stillAvailable = "abcdefg";

        //A is the one letter that is three letter pattern but not two
        mapping[A] = keepNotIn(threeLetterPattern, twoLetterPattern)[0];
        stillAvailable = removeChar(stillAvailable, mapping[A]);

        //B and E are in just one of the five letter patterns
        string beList = inOne(stillAvailable, fiveLetterPatterns);

        //B is in the four letter
        string bList = keepIn(beList, fourLetterPattern);
        mapping[B] = bList[0];
        stillAvailable = removeChar(stillAvailable, mapping[B]);

        //E is not in four letter
        string eList = keepNotIn(beList, fourLetterPattern);
        mapping[E] = eList[0];
        stillAvailable = removeChar(stillAvailable, mapping[E]);

        //D and G are in all three five letters
        string allFives = keepIn(stillAvailable, fiveLetterPatterns[0]);
        allFives = keepIn(allFives, fiveLetterPatterns[1]);
        allFives = keepIn(allFives, fiveLetterPatterns[2]);

        //D is also in four letter
        string dList = keepIn(allFives, fourLetterPattern);
        mapping[D] = dList[0];
        stillAvailable = removeChar(stillAvailable, mapping[D]);

        //G is not in four letter
        string gList = keepNotIn(allFives, fourLetterPattern);
        mapping[G] = gList[0];
        stillAvailable = removeChar(stillAvailable, mapping[G]);

        //F is in all sixes
        string allSixes = keepIn(stillAvailable, sixLetterPatterns[0]);
        allSixes = keepIn(allSixes, sixLetterPatterns[1]);
        allSixes = keepIn(allSixes, sixLetterPatterns[2]);
        mapping[F] = allSixes[0];
        stillAvailable = removeChar(stillAvailable, mapping[F]);

        //C is left over
        mapping[C] = stillAvailable[0];

        //reverseMapping[A] = 'd' indicates that 'a' in the garbled version is 'd' in real
        char reverseMapping[7] = {};
        for(int j = 0; j < 7; j++) {
            char curChar = mapping[j];
            reverseMapping[curChar - 'a'] = static_cast<char>('a' + j);
        }

        string trash;
        in >> trash;  // |

        //Now get the four actual digits
        int output = 0;
        int digitPlaceMultiplier = 1000;
        for(int j = 0; j < 4; j++) {
            string value;
            in >> value;

            output += getDigit(value, reverseMapping) * digitPlaceMultiplier;
            digitPlaceMultiplier /= 10;
        }
        totalSum += output;
    }
    cout << totalSum << endl;
}



void day8p2Take2() {
    //Assume working directory is ./Debug/
    ifstream in("../input.txt");
    if(!in)
        cout << "Error opening file";

    int totalSum = 0;

    for(int i = 0; i < DATA_SIZE; i++) {
        string twoLetterPattern;
        string fourLetterPattern;

        //How often each letter appears
        //0 = a, 1 = b, etc...
        int heatMap[7] = {};

        for(int j = 0; j < 10; j++) {
            string value;
            in >> value;
            string::size_type len = value.length();
            if(len == 2)
                twoLetterPattern = value;
            else if(len == 4)
                fourLetterPattern = value;

            for(string::size_type k = 0; k < value.length(); k++) {
                heatMap[value.at(k) - 'a']++;
            }
        }

        //reverseMapping[A/0] = 'd' indicates that 'a' in the garbled version is 'd' in real
        char reverseMapping[7] = {};

        for(int j = 0; j < 7; j++) {
            int numOccurances = heatMap[j];
            char mapCharValue = static_cast<char>('a' + j);

            if(numOccurances == 4)          //e segment should appear 4 times
                reverseMapping[j] = 'e';
            else if(numOccurances == 6)     //b appears 6
                reverseMapping[j] = 'b';
            else if(numOccurances == 9)     //...
                reverseMapping[j] = 'f';
            else if(numOccurances == 8) {   //a and c appear 8
                if(twoLetterPattern.find(mapCharValue) != string::npos)
                    reverseMapping[j] = 'c';    //c is in the twoLetterPattern
                else
                    reverseMapping[j] = 'a';    //a is not
            } else if(numOccurances == 7) {   //g and d appear 7 times
                if(fourLetterPattern.find(mapCharValue) != string::npos)
                    reverseMapping[j] = 'd';    //d is in the twoLetterPattern
                else
                    reverseMapping[j] = 'g';    //g is not
            }
        }

        string trash;
        in >> trash;  // |

        //Now get the four actual digits
        int output = 0;
        int digitPlaceMultiplier = 1000;
        for(int j = 0; j < 4; j++) {
            string value;
            in >> value;

            output += getDigit(value, reverseMapping) * digitPlaceMultiplier;
            digitPlaceMultiplier /= 10;
        }
        totalSum += output;
    }
    cout << totalSum << endl;
}


int main()
{
    day8p1();
    cout << "-------------------------" << endl;
    day8p2();
    cout << "-------------------------" << endl;
    day8p2Take2();
}
