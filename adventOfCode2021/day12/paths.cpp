// g++ -Wall -std=c++14 paths.cpp -o paths
#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<std::string> split(std::string& str){
    std::regex reg("-");
    std::sregex_token_iterator iter(str.begin(), str.end(), reg, -1);
    std::sregex_token_iterator end;
    std::vector<std::string> vec(iter, end);
    return vec;
}

bool isLowercase(std::string& str){
    // for this exercise I think just checking the first char is sufficient
    return str[0] - 97 >= 0; // b/c the ASCII lowercase letters start at 97
}

// part 1
// in this case, at each point where we want to move to a new cave, we want to pass a new copy
// of paths (and not a reference) because we will want to make different modifications to paths
// depending on the current route.
void traversePath(
    std::string currCave,
    std::unordered_map<std::string, std::vector<std::string>> paths,
    std::unordered_map<std::string, bool> visited,
    std::string currPath,
    int& pathCount
    ){
        
    if(currCave == "end"){
        // we found the end! we are done, yay
        pathCount++;
        //std::cout << currPath << "-end\n";
        return;
    }
    
    // update visited map (only applies to small caves)
    if(visited.find(currCave) != visited.end()) visited[currCave] = true;
    
    // traverse the next caves if any
    for(std::string& nextCave : paths[currCave]){
        //std::cout << currPath + "-" + currCave << ", next cave: " << nextCave << '\n';
        // if big cave or un-visited small cave yet
        if(visited.find(nextCave) == visited.end() || !visited[nextCave]){
            std::string curr = currPath + "-" + currCave;
            traversePath(nextCave, paths, visited, curr, pathCount);
        }
    }
}

// part 2
void traversePath2(
    std::string currCave,
    std::unordered_map<std::string, std::vector<std::string>> paths,
    std::unordered_map<std::string, int> visitedCounts,
    std::string currPath,
    long& pathCount
    ){
    
    if(currCave == "end"){
        pathCount++;
        //std::cout << currPath << "-end\n";
        return;
    }
    
    if(visitedCounts.find(currCave) != visitedCounts.end()){
        visitedCounts[currCave]++;
        
        // set a "twice" flag so we know no subsequent path can re-enter a small cave more than once
        if(visitedCounts[currCave] == 2) visitedCounts["twice"] = 1;
    }
    
    // traverse the next caves if any
    for(std::string& nextCave : paths[currCave]){
        //std::cout << currPath + "-" + currCave << ", next cave: " << nextCave << '\n';
        if(nextCave == "start") continue; // can only go through start once, which we do first in main
        
        bool isBigCave = visitedCounts.find(nextCave) == visitedCounts.end();
        bool hasTwice = visitedCounts.find("twice") != visitedCounts.end();
        if(isBigCave || (hasTwice && visitedCounts[nextCave] < 1) || (visitedCounts[nextCave] < 2 && !hasTwice)){
            std::string curr = currPath + "-" + currCave;
            traversePath2(nextCave, paths, visitedCounts, curr, pathCount);
        }
    }
}

// add new key and value pair
void addKV(
    std::string& key, 
    std::string& val, 
    std::unordered_map<std::string, std::vector<std::string>>& paths
    ){
    if(paths.find(key) == paths.end()){
        std::vector<std::string> newVal;
        newVal.push_back(val);
        paths[key] = newVal;
    }else{
        paths[key].push_back(val);
    }
}

/*

start-A
start-b
A-c
A-b
b-d
A-end
b-end

    start
    /   \
c--A-----b--d
    \   /
     end

expected map:
{
    start: [A, b],
    A: [c, b, end],
    b: [d, end],
    c: [A],
    b: [A, d],
    d: [b]
}

*/

int main(){
    std::ifstream file("input.txt");
    std::string line;
    
    int pathCount = 0;
    std::unordered_map<std::string, std::vector<std::string>> paths;
    std::unordered_map<std::string, bool> visitedCaves;
    std::unordered_map<std::string, int> visitedCounts; // part 2
    
    if(file.is_open()){
        while(std::getline(file, line)){
            std::vector<std::string> tokens = split(line);
            
            // paths are bidirectional! (but can only pass through a small cave once)
            addKV(tokens[0], tokens[1], paths);
            addKV(tokens[1], tokens[0], paths);
            
            if(isLowercase(tokens[0])){
                // we want to keep track of visited small caves (and start)
                visitedCaves[tokens[0]] = false;
                visitedCounts[tokens[0]] = 0;
            }
            
            if(isLowercase(tokens[1])){
                visitedCaves[tokens[1]] = false;
                visitedCounts[tokens[1]] = 0;
            }
        }
    }
    
    //for(auto p : visitedCaves){
    //    std::cout << p.first << ": " << p.second << '\n';
    //}
    
    std::string start("start");
    std::string currPath("");
    
    traversePath(start, paths, visitedCaves, currPath, pathCount);
    std::cout << "total successful paths: " << pathCount << '\n';
 
    // part 2
    long pathCount2 = 0;
    traversePath2(start, paths, visitedCounts, currPath, pathCount2);
    std::cout << "part 2 - total successful paths: " << pathCount2 << '\n';

    return 0;
}
