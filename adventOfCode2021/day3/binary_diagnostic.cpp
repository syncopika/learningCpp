// g++ -Wall -std=c++14 binary_diagnostic.cpp -o binary_diagnostic
#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>
#include <map> // keep in mind this is backed by a tree (so O(lg(n))) whereas unordered map is a hashtable (O(1))
#include <string>
#include <utility> // for pair
#include <vector>
#include <math.h>

// using a map helps keep keys in sorted order which is helpful here
// although maybe a vector of pairs would suffice as well...
// an unordered_map could work too I think as long as we remember how long the binary strings are
// so we know how many indices we could expect to find in the map and then just look up 0 to n.
void getBinCounts(std::map<int, std::pair<int, int>>& map, std::string& line){
    for(int i = 0; i < (int)line.length(); i++){
        // each index maps to a pair
        // we keep the counts of 0 and 1, respectively, in the pair
        int val = line[i] - 48; // 0 or 1
        if(map.find(i) == map.end()){
            // adding for the first time
            (val == 0) ? map.insert({i, {1, 0}}) : map.insert({i, {0, 1}});
            // alternative way: map[i] = (val == 0) ? std::make_pair(1, 0) : std::make_pair(0, 1);
        }else{
            if(val == 0){
                map[i].first += 1;
            }else{
                map[i].second += 1;
            }
        }
    }
}

int binToDecimal(std::string& binaryStr){
    int binNum = 0;
    // ex.
    // 0011 => 3 b/c 1*2^0 + 2*2^1 + 0*2^2 + 0*2^3 (notice it's right to left)
    // 1000 => 8 b/c 0*2^0 + 0*2^1 + 0*2^2 + 0*2^2 + 1*2^3 
    // 
    // reverse the string (or start at the end)
    // each index represents 2^index
    // sum each index
    int exp = 0;
    
    // using size_t did not cooperate when indexing the string via binaryStr[i]
    // gdb is helpful!
    for(int i = (int)binaryStr.length()-1; i >= 0; i--){
        binNum += (binaryStr[i] - 48)*pow(2, exp++); 
    }
    
    return binNum;
}
 
enum Type { o2, co2 };

bool keep(int zeroFreq, int oneFreq, char charAtIdx, Type type){
    if(type == o2){
        // for o2
        if(zeroFreq > oneFreq){
            return charAtIdx == '0';
        }else{
            return charAtIdx == '1';
        }
    }else{
        // for co2
        if(zeroFreq > oneFreq){
            return charAtIdx == '1';
        }else{
            return charAtIdx == '0';
        }
    }
}

std::vector<std::string> filterMatches(std::vector<std::string>& bestMatches, int index, Type type){
    // we have to recalculate the frequency of 0s and 1s at each index!
    std::map<int, std::pair<int, int>> binCounts;
    for(std::string& str : bestMatches){
        getBinCounts(binCounts, str);
    }
    
    int zeroFreq = binCounts[index].first;
    int oneFreq = binCounts[index].second;
  
    std::vector<std::string> filtered;
    
    std::copy_if(
        bestMatches.begin(),
        bestMatches.end(),
        std::back_inserter(filtered),
        [=](std::string s){
            char charAtIdx = s[index];
            return keep(zeroFreq, oneFreq, charAtIdx, type);
        }
    );
    
    return filtered;
}

void diagnostic(){
    std::ifstream file("input.txt");
    
    std::string line;
    
    // for our map we want something like:
    /*
    {
       0: {2, 3}, // means 0 appeared twice, 1 appeared three times at the 0th index
       1: {0, 5},
       ...
    }
    */
    std::map<int, std::pair<int, int>> binCounts;
    
    if(file.is_open()){
        while(std::getline(file, line)){
            getBinCounts(binCounts, line);
        }
    }
    
    // part 1: find gamma and epsilon rate
    std::string gammaBinary("");
    std::string epsilonBinary("");
    
    for(auto& x : binCounts){
        int zeroFreq = x.second.first; // x.second is the std::pair holding the frequencies of 0 and 1 at a particular index, respectively
        int oneFreq = x.second.second;
        if(zeroFreq > oneFreq){
            gammaBinary += "0";
            epsilonBinary += "1";
        }else{
            gammaBinary += "1";
            epsilonBinary += "0";
        }
    }
    
    int gammaDecimal = binToDecimal(gammaBinary);
    int epsilonDecimal = binToDecimal(epsilonBinary);
    
    std::cout << "gammaBinary: " << gammaBinary << ", decimal: " << gammaDecimal << '\n';
    std::cout << "epsilonBinary: " << epsilonBinary << ", decimal: " << epsilonDecimal << '\n';
    std::cout << "power consumption: " << gammaDecimal * epsilonDecimal << '\n';
    
    
    // part 2: o2 and co2 life support
    std::vector<std::string> bestOxyMatches;
    std::vector<std::string> bestCo2Matches;
    std::string bestOxyMatch;
    std::string bestCo2Match;
    
    if(file.is_open()){
        // need to reset the file so we can read at the beginning again
        file.clear();
        file.seekg(0);
        
        // collect all the initial matches
        while(std::getline(file, line)){
            int zeroFreq = binCounts[0].first;
            int oneFreq = binCounts[0].second;
            char charAtIdx = line[0];
            
            if(keep(zeroFreq, oneFreq, charAtIdx, o2)) bestOxyMatches.push_back(line);
            if(keep(zeroFreq, oneFreq, charAtIdx, co2)) bestCo2Matches.push_back(line);
        }
        
        file.close();
    }
    
    // then we can do the filtering
    for(int i = 1; i < (int)binCounts.size(); i++){
        bestOxyMatches = filterMatches(bestOxyMatches, i, o2);        
        if(bestOxyMatches.size() == 1){
            bestOxyMatch = bestOxyMatches[0];
            break;
        }
    }
    
    for(int i = 1; i < (int)binCounts.size(); i++){
        bestCo2Matches = filterMatches(bestCo2Matches, i, co2);     
        if(bestCo2Matches.size() == 1){
            bestCo2Match = bestCo2Matches[0];
            break;
        }
    }
    
    int o2GenRating = binToDecimal(bestOxyMatch);
    int co2ScrubRating = binToDecimal(bestCo2Match);
    
    std::cout << "o2 generator => " << "binary: " << bestOxyMatch << ", decimal: " << o2GenRating << '\n';
    std::cout << "co2 scrubber => " << "binary: " << bestCo2Match << ", decimal: " << co2ScrubRating << '\n';
    std::cout << "life support rating: " << o2GenRating * co2ScrubRating << '\n';
}


int main(){
    //std::map<int, std::pair<int, int>> map;
    //std::string bin("01001");
    //std::string bin2("01110");
    //getBinCounts(map, bin);
    //getBinCounts(map, bin2);
    
    //std::string bin("0011");
    //std::string bin2("1011");
    //int num = binToDecimal(bin); // 3
    //int num2 = binToDecimal(bin2); // 11
    //std::cout << num << ", " << num2 << '\n';
    
    diagnostic();
}
