// g++ -Wall -std=c++14 origami.cpp -o origami

#include <cmath>
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <utility>
#include <stdlib.h>

std::vector<std::string> split(const char* delimiter, std::string& line){
    std::regex reg(delimiter);
    std::sregex_token_iterator iter(line.begin(), line.end(), reg, -1);
    std::sregex_token_iterator end;
    std::vector<std::string> vec(iter, end);
    return vec;
}

void makeMap(std::vector<std::vector<char>>& map, int maxX, int maxY){
    for(int i = 0; i <= maxY; i++){
        std::vector<char> row;
        for(int j = 0; j <= maxX; j++){
            row.push_back('.');
        }
        map.push_back(row);
    }
}

void addHashes(std::vector<std::vector<char>>& map, std::vector<std::pair<int, int>>& coords){
    for(int i = 0; i < (int)coords.size(); i++){
        int x = coords[i].first;
        int y = coords[i].second;
        map[y][x] = '#';
    }
}

void applyFold(std::string& fold, std::vector<std::vector<char>>& map){
    std::vector<std::string> instruction = split("=", fold); // e.g. "x=7"
    std::string dir = instruction[0];
    int coord = atoi(instruction[1].c_str());
    
    if(dir == "y"){
        std::vector<char>& row = map[coord];
        for(int i = 0; i < (int)row.size(); i++){
            row[i] = '-';
        }
        
        for(int j = coord+1; j < (int)map.size(); j++){
            // find the row above the fold line to collapse with
            int rowToCollapseWith = (-1 * j) + (int)map.size() - 1; // if equal halves
            if((int)map.size() - coord != coord + 1){
                // if not equal-sized halves
                rowToCollapseWith = (-1 * j) + (int)map.size();
            }
            
            //std::cout << (int)map.size() << '\n';
            //std::cout << "curr row: " << j << ", row to collapse with: " << rowToCollapseWith << '\n';
            for(int k = 0; k < (int)map[rowToCollapseWith].size(); k++){
                char curr = map[j][k];
                if(curr == '#'){
                    map[rowToCollapseWith][k] = '#';
                }
                
                // clear lines
                map[j][k] = ' ';
            }
        }
    }else if(dir == "x"){
        for(int i = 0; i < (int)map.size(); i++){
            map[i][coord] = '|';
        }
        
        for(int j = coord+1; j < (int)map[0].size(); j++){
            // find the row above the fold line to collapse with
            int colToCollapseWith = (-1 * j) + (int)map[0].size() - 1;
            if((int)map[0].size() - coord != coord + 1){
                // if not equal-sized halves
                colToCollapseWith = (-1 * j) + (int)map[0].size();
            }
            
            for(int k = 0; k < (int)map.size(); k++){
                char curr = map[k][j];
                //char other = map[k][colToCollapseWith];
                if(curr == '#'){
                    map[k][colToCollapseWith] = '#';
                }
                
                // clear lines
                map[k][j] = ' ';
            }
        }
    }
}

int countDots(std::vector<std::vector<char>>& map){
    int count = 0;
    for(int i = 0; i < (int)map.size(); i++){
        for(int j = 0; j < (int)map[i].size(); j++){
            if(map[i][j] == '#'){
                count++;
            }
        }
    }
    return count;
}

void printMap(std::vector<std::vector<char>>& map){
    for(int i = 0; i < (int)map.size(); i++){
        std::string row;
        for(int j = 0; j < (int)map[i].size(); j++){
            row += (map[i][j]);
        }
        std::cout << row << '\n';
    }
}

void drawMap(std::vector<std::vector<char>>& map){
    std::ofstream newfile;
    newfile.open("output.txt");
    
    for(int i = 0; i < (int)map.size(); i++){
        std::string row;
        for(int j = 0; j < (int)map[i].size(); j++){
            row += (map[i][j]);
        }
        //std::cout << row << '\n';
        newfile << row << '\n';
    }
    
    newfile.close();
}

int main(){
    std::ifstream file("input.txt");
    std::string line;
    
    std::vector<std::pair<int, int>> coords;
    std::vector<std::string> folds;
    std::vector<std::vector<char>> map;
    
    int maxX = 0;
    int maxY = 0;
    
    if(file.is_open()){
        while(std::getline(file, line)){
            if(line[0] != 'f' && line != ""){
                std::vector<std::string> coord = split(",", line);
                int x = atoi(coord[0].c_str());
                int y = atoi(coord[1].c_str());
                coords.push_back({x, y});
                
                maxX = std::max(maxX, x);
                maxY = std::max(maxY, y);
            }else if(line[0] == 'f'){
                // get the folds
                std::string foldInstruction = split("\\s", line)[2];
                folds.push_back(foldInstruction);
            }
        }
        
        file.close();
    }
    
    std::cout << "max x: " << maxX << ", max y: " << maxY << '\n';
    makeMap(map, maxX, maxY);
    addHashes(map, coords);
    //printMap(map);
    //std::cout << "==========================\n";
    
    // part 1
    //applyFold(folds[0], map);
    //std::cout << "total dots: " << countDots(map) << '\n';
    //printMap(map);
    
    // part 2
    for(std::string& str : folds){
        applyFold(str, map);
    }
    
    drawMap(map);
    
    return 0;
}
