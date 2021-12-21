// g++ -Wall -std=c++14 polymers.cpp -o polymers
#include <climits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <stdlib.h>

std::vector<std::string> split(const char* delimiter, std::string& line){
    std::regex reg(delimiter);
    std::sregex_token_iterator iter(line.begin(), line.end(), reg, -1);
    std::sregex_token_iterator end;
    std::vector<std::string> vec(iter, end);
    return vec;
}

// here we're actually creating the new string at each step. this will get expensive performance-wise for long strings.
std::string step(
    std::string& input, 
    std::unordered_map<std::string, std::string>& map
    ){
    std::string output;
    for(int i = 0; i < (int)input.length() - 1; i++){
        std::string currPair = input.substr(i, 2);
        
        if(i == 0) output += input[i]; // only at i == 0 otherwise we'll add a bunch of letters twice
        
        if(map.find(currPair) != map.end()){
            // get character to insert based on map
            output += map[currPair];
        }
        
        output += input[i+1];
    }
    
    return output;
}

// for part 1 we can just iterate through the string to keep track of counts.
void countLetters(std::string& str, std::unordered_map<char, long long>& freq){
    for(char c : str){
        if(freq.find(c) == freq.end()){
            freq[c] = 1;
        }else{
            freq[c]++;
        }
    }
}

// part 2: we don't actually need to create the string at each step to figure out character counts.
// instead, we can just keep track of pairs at each step and then figure out the number of new pairs
// that will occur. from the pairs we can figure out individual character counts.
void part2(std::string& inputStr, std::unordered_map<std::string, std::string>& map, int numSteps){
    // initialize our char pair counter
    std::unordered_map<std::string, long long> pairTracker;
    
    for(std::pair<std::string, std::string> kv : map){
        // map should already contain all the possible pairs to see
        pairTracker[kv.first] = 0;
    }
    
    for(int i = 0; i < (int)inputStr.length() - 1; i++){
        std::string pair = inputStr.substr(i, 2);
        pairTracker[pair]++;
    }
    
    // keep track of character counts
    std::unordered_map<char, long long> charCounts;
    for(char c : inputStr){
        if(charCounts.find(c) != charCounts.end()){
            charCounts[c]++;
        }else{
            charCounts[c] = 1;
        }
    }
    
    // then go through the steps
    for(int i = 0; i < numSteps; i++){
        std::unordered_map<std::string, long long> tempPairTracker;
        for(std::pair<std::string, std::string> kv : map){
            tempPairTracker[kv.first] = 0;
        }
        
        // collect pairs to add (don't update map while going through the pairs)
        for(std::pair<std::string, long long> kv : pairTracker){
            std::string pair = kv.first;
            if(pairTracker[pair] > 0){
                std::string charToInsert = map[pair];
                
                charCounts[charToInsert[0]] += kv.second;
                
                // create 2 new pairs using this current pair and the char to insert
                tempPairTracker[pair.substr(0, 1) + charToInsert] += kv.second;
                tempPairTracker[charToInsert + pair.substr(1, 1)] += kv.second;
            }
        }
        
        // we want to look at the pairs we created from the previous iteration
        pairTracker = tempPairTracker;
    }
    
    // find max and min counts
    std::pair<char, long long> maxChar{'a', -1};
    std::pair<char, long long> minChar{'a', LLONG_MAX};
    for(std::pair<char, long long> kv : charCounts){
        char c = kv.first;
        long long count = kv.second;
        if(maxChar.second < count){
            maxChar.first = c;
            maxChar.second = count;
        }
        if(minChar.second > count){
            minChar.first = c;
            minChar.second = count;
        }
    }
    
    std::cout << "max char: " << maxChar.first << ", count: " << maxChar.second << '\n';
    std::cout << "min char: " << minChar.first << ", count: " << minChar.second << '\n';
    std::cout << "diff: " << maxChar.second - minChar.second << '\n';
}

int main(){
    std::ifstream file("input.txt");
    std::string line;
    
    std::unordered_map<std::string, std::string> map; // map particular char pairs to the char that should be inserted between them
    std::unordered_map<char, long long> freq;
    std::string inputLine;
    
    if(file.is_open()){
        int count = 0;
        while(std::getline(file, line)){
            if(count == 0){
                inputLine = line;
            }else if(line != ""){
                std::vector<std::string> tokens = split("\\s->\\s", line);
                map[tokens[0]] = tokens[1];
            }
            count++;
        }
        
        file.close();
    }
    
    /* part 1
    int numSteps = 10;
    std::cout << inputLine << '\n';
    for(int i = 0; i < numSteps; i++){
        inputLine = step(inputLine, map);
    }
    std::cout << "step " << numSteps-1 << " length: " << inputLine.length() << '\n';
    
    long long maxOccur = 0;
    long long minOccur = -1;
    
    countLetters(inputLine, freq);
    
    for(auto& pair : freq){
        maxOccur = std::max(maxOccur, pair.second);
        minOccur = (minOccur == -1) ? pair.second : std::min(minOccur, pair.second);
    }
    
    std::cout << "max: " << maxOccur << ", min: " << minOccur << ", diff: " << (maxOccur - minOccur) << '\n';
    */
    
    // part 2
    part2(inputLine, map, 40);
    
    return 0;
}
