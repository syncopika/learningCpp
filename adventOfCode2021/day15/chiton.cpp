// g++ -Wall -std=c++14 chiton.cpp -o chiton
#include <climits>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>

// https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
struct Coord {
    int x;
    int y;
    
    bool operator==(const Coord& other) const {
        return other.x == x && other.y == y;
    }
};

// need a hash function to be able to use Coord as a key in map
// interesting: https://stackoverflow.com/questions/682438/hash-function-providing-unique-uint-from-an-integer-coordinate-pair
// and https://stackoverflow.com/questions/22826326/good-hashcode-function-for-2d-coordinates
// append a new hash function for Coord to the std namespace
template <>
struct std::hash<Coord> {
    std::size_t operator()(const Coord& coord) const {
        int tmp = coord.y + ((coord.x+1)/2);
        return coord.x + (tmp * tmp);
    }
};

//Coord getMinDistCoord(std::unordered_map<Coord, int>& map){
//}


int main(){
    std::ifstream file("input.txt");
    std::string line;
 
    std::vector<std::vector<int>> grid;
    std::unordered_map<Coord, bool> visited;
    std::unordered_map<Coord, int> distance;
    std::unordered_map<Coord, Coord> previous;
 
    if(file.is_open()){
        int y = 0;
        while(std::getline(file, line)){
            std::vector<int> row;
            std::vector<int> rowDist;
            std::vector<int> rowPrev;
            
            int x = 0;
            for(char c : line){
                row.push_back(c - 48);
                
                Coord co{y, x};
                Coord undefined{-1, -1};
                distance[co] = INT_MAX;
                previous[co] = undefined;
                x++;
            }
            grid.push_back(row);
            y++;
        }
        
        file.close();
    }
    
    // try dijkstra's algorithm
    distance[{0, 0}] = 0; // start at (0, 0)
    for(int i = 0; i < (int)grid.size(); i++){
        for(int j = 0 ; j < (int)grid[i].size(); j++){
            Coord currCoord{i, j};
            visited[currCoord] = true;
        }
    }
    
    return 0;
}