// g++ -Wall -std=c++14 segment_search.cpp -o segment_search

#include <iostream>
#include <fstream>
#include <regex>
#include <unordered_map>
#include <set>
#include <string>
#include <vector>
#include <stdlib.h>

int countUnique(std::string& str){
    std::set<char> set;
    for(char c : str){
        set.insert(c);
    }
    return (int)set.size();
}

std::vector<std::string> getTokens(const char* delimiter, std::string& str){
    std::regex reg(delimiter);
    std::sregex_token_iterator iter(str.begin(), str.end(), reg, -1);
    std::sregex_token_iterator end;
    std::vector<std::string> vec(iter, end);
    return vec;
}

// notes
// part2 potential strategy?
// for each line, sort the left side of the | by length? so we can establish 
// the order of segments for 1 at least.
// keep a vector<char> for each segment, arrange in clockwise order?
// every time we decide on a change in order of segments, we just swap chars?
// also, important to remember: the left side of | contains one pattern for
// each digit (there are 10, 0 -> 9)

int main(){
    std::ifstream file("input.txt");
    std::string line;
    
    std::unordered_map<int, int> uniqueNumSegments{
        {2, 1},
        {4, 4},
        {3, 7},
        {7, 8}
    };
    
    int numDigitsWithUniqueSegments = 0;
    
    if(file.is_open()){
        while(std::getline(file, line)){
            std::vector<std::string> tokens = getTokens("\\s\\|\\s", line);
            //std::cout << tokens[1] << '\n';
            std::vector<std::string> rightSide = getTokens("\\s+", tokens[1]);
            
            for(std::string& segment : rightSide){
                int uniqueCount = countUnique(segment);
                if(uniqueNumSegments.find(uniqueCount) != uniqueNumSegments.end()){
                    numDigitsWithUniqueSegments++;
                    //std::cout << segment << '\n';
                }
            }
        }
        
        file.close();
    }
    
    std::cout << "num digits with unique segments: " << numDigitsWithUniqueSegments << '\n';
    
    return 0;
}
