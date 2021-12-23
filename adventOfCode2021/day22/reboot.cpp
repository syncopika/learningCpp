// g++ -Wall -std=c++14 reboot.cpp -o reboot
#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <regex>
#include <unordered_map>
#include <set>
#include <string>
#include <vector>
#include <stdlib.h>

struct Vec3 {
    int x;
    int y;
    int z;
    bool isOn;
    
    Vec3(int x, int y, int z){
        this->x = x;
        this->y = y;
        this->z = z;
        this->isOn = false;
    }
    
    bool operator==(const Vec3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    
    // https://stackoverflow.com/questions/11181760/implementing-operator-for-x-y-z-coordinate
    bool operator<(const Vec3& other) const {
        if(x != other.x) return x < other.x;
        if(y != other.y) return y < other.y;
        return z < other.z;
    }
};

std::vector<std::string> split(const char* delimiter, std::string& line){
    std::regex reg(delimiter);
    std::sregex_token_iterator iter(line.begin(), line.end(), reg, -1);
    std::sregex_token_iterator end;
    std::vector<std::string> vec(iter, end);
    return vec;
}

// we want something like {'x': {0, 10}, 'y': {0, 10}, ...} where {0, 10} are the min and max for the range
std::unordered_map<char, std::pair<int, int>> getCoordRange(std::string& str){
    // split on ,
    // in order x, y, z -> expect 3 strings
    // for each string, match digits
    std::unordered_map<char, std::pair<int, int>> coordRanges;
    
    std::vector<std::string> xyz = split(",", str);
    
    assert(xyz.size() == 3);
    
    std::vector<char> coords = {'x', 'y', 'z'};
    
    for(int i = 0; i < 3; i++){
        std::regex reg("(-?\\d+)..(-?\\d+)"); // 2 capture groups for 2 digits
        std::smatch matches;
        std::regex_search(xyz[i], matches, reg);
        
        //std::cout << xyz[i] << '\n';
        assert(matches.size() == 3); // https://stackoverflow.com/questions/44706989/c-regex-finds-only-1-sub-match
        
        coordRanges[coords[i]] = {
            atoi(std::string(matches[1]).c_str()),
            atoi(std::string(matches[2]).c_str())
        };
    }
    
    return coordRanges;
}

bool inRange(int min, int max, int x, int y, int z){
    return x >= min && x <= max &&
           y >= min && y <= max &&
           z >= min && z <= max;
}

long checkSet(std::set<Vec3>& vec3Set){
    long onCount = 0;
    for(Vec3 v : vec3Set){
        if(v.isOn) onCount++;
    }
    return onCount;
}

void processInstruction(std::string& str, std::set<Vec3>& vec3Set){
    // split on space to get 'on' or 'off'
    // then process the 2nd substring to extract the coords
    std::vector<std::string> parts = split(" ", str);
    assert(parts.size() == 2);
    std::unordered_map<char, std::pair<int, int>> coords = getCoordRange(parts[1]);
    
    std::pair<int, int> x = coords['x'];
    std::pair<int, int> y = coords['y'];
    std::pair<int, int> z = coords['z'];
    
    // part 1
    if(!inRange(-50, 50, x.first, y.first, z.first) || !inRange(-50, 50, x.second, y.second, z.second)){
        return;
    }
    
    //std::cout << "turn " << parts[0] << "; x: " << x.first << "->" << x.second
    //<< ", y: " << y.first << "->" << y.second << ", z: " << z.first << "->" << z.second << '\n';
    
    bool turnCubeOn = parts[0] == "on";
    //long totalCubes = 0;
    
    for(int i = x.first; i <= x.second; i++){
        for(int j = y.first; j <= y.second; j++){
            for(int k = z.first; k <= z.second; k++){
                // check if coord in vec3 set
                // otherwise create vector3
                // set it on or off
                Vec3 v{i, j, k};
                if(vec3Set.find(v) == vec3Set.end()){
                    v.isOn = turnCubeOn;
                    vec3Set.insert(v);
                    //totalCubes++;
                }else{
                    // it might've been an error choosing to use a set :/
                    // https://stackoverflow.com/questions/7340434/how-to-update-an-existing-element-of-stdset
                    Vec3& vf = const_cast<Vec3&>(*vec3Set.find(v)); // based on coord only
                    vf.isOn = turnCubeOn;
                }
            }
        }
    }
    
    //std::cout << "total cubes so far: " << totalCubes << '\n';
    //long currOn = checkSet(vec3Set);
    //std::cout << "there are currently: " << currOn << " cubes on.\n";
}

void part1(){
    std::ifstream file("input.txt");
    std::string line;
    
    std::set<Vec3> vec3Set;
    
    if(file.is_open()){
        while(std::getline(file, line)){
            processInstruction(line, vec3Set);
        }
    }
    
    std::cout << "num cubes on: " << checkSet(vec3Set) << '\n';
}

int main(){
    part1();
    return 0;
}

