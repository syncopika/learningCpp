// g++ -Wall -std=c++14 polymers.cpp -o polymers

#include <cmath>
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <stdlib.h>

std::vector<std::string> split(const char* delimiter, std::string& line){
    std::regex reg(delimiter);
    std::sregex_token_iterator iter(line.begin(), line.end(), reg, -1);
    std::sregex_token_iterator end;
    std::vector<std::string> vec(iter, end);
    return vec;
}

std::string step(
    std::string& input, 
    std::unordered_map<std::string, std::string>& map
    ){
    std::string output;
    for(int i = 0; i < (int)input.length() - 1; i++){
        std::string currPair = input.substr(i,2);
        
        if(i == 0) output += input[i]; // only at i == 0 otherwise we'll add a bunch of letters twice
        
        if(map.find(currPair) != map.end()){
            // get character to insert based on map
            output += map[currPair];
        }
        
        output += input[i+1];
    }
    
    return output;
}

void countLetters(std::string& str, std::unordered_map<char, long long>& freq){
    for(char c : str){
        if(freq.find(c) == freq.end()){
            freq[c] = 1;
        }else{
            freq[c]++;
        }
    }
}

int main(){
    std::ifstream file("input.txt");
    std::string line;
    
    std::unordered_map<std::string, std::string> map;
    std::unordered_map<char, long long> freq;
    std::string inputLine;
    
    if(file.is_open()){
        int count = 0;
        while(std::getline(file, line)){
            if(count == 0){
                inputLine = line;
            }else if(line != ""){
                std::vector<std::string> tokens = split("\\s->\\s", line);
                //std::cout << tokens[0] << ", " << tokens[1] << '\n';
                map[tokens[0]] = tokens[1];
            }
            count++;
        }
        
        file.close();
    }
    
    int numSteps = 40;
    std::cout << inputLine << '\n';
    for(int i = 0; i < numSteps; i++){
        std::cout << "working on step: " << i << '\n';
        //if(i < 5) std::cout << "step " << i << ": " << inputLine << " length: " << inputLine.length() << '\n';
        inputLine = step(inputLine, map);
    }
    std::cout << "step " << numSteps-1 << " length: " << inputLine.length() << '\n';
    
    long long maxOccur = 0;
    long long minOccur = -1;
    
    countLetters(inputLine, freq);
    
    for(auto pair : freq){
        maxOccur = std::max(maxOccur, pair.second);
        minOccur = (minOccur == -1) ? pair.second : std::min(minOccur, pair.second);
    }
    
    std::cout << "max: " << maxOccur << ", min: " << minOccur << ", diff: " << (maxOccur - minOccur) << '\n';
    
    return 0;
}
