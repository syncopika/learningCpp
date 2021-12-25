// g++ -Wall -std=c++14 octopus.cpp -o octopus

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void printGrid(std::vector<std::vector<int>>& grid){
    for(std::vector<int>& r : grid){
        std::string row;
        for(int i : r){
            row += std::to_string(i) + " ";
        }
        std::cout << row << '\n';
    }
}

void increaseEnergy(std::vector<std::vector<int>>& grid){
    for(int i = 0; i < (int)grid.size(); i++){
        std::vector<int>& row = grid[i];
        for(int j = 0; j < (int)row.size(); j++){
            row[j]++;
        }
    }
}

bool checkSynchronized(std::vector<std::vector<int>>& grid){
    int energy = grid[0][0];
    for(int i = 0; i < (int)grid.size(); i++){
        for(int j = 0; j < (int)grid[i].size(); j++){
            if(energy != grid[i][j]){
                return false;
            }
        }
    }
    return true;
}

// use recursion on octopuses with > 9 energy level
void propagateEnergy(int row, int col, int& flashTotal, std::vector<std::vector<int>>& grid){
    if(grid[row][col] + 1 > 9){
        grid[row][col] = 0; // flash
        flashTotal += 1;
        
        // propagate to neighbors
        // north
        if(row - 1 >= 0){
            // nw, ne
            if(col - 1 >= 0){
                propagateEnergy(row-1, col-1, flashTotal, grid);
            }
            if(col + 1 < (int)grid[row-1].size()){
                propagateEnergy(row-1, col+1, flashTotal, grid);
            }
            // north
            propagateEnergy(row-1, col, flashTotal, grid);
        }
        // south
        if(row + 1 < (int)grid.size()){
            // sw, se
            if(col - 1 >= 0){
                propagateEnergy(row+1, col-1, flashTotal, grid);
            }
            if(col + 1 < (int)grid[col+1].size()){
                propagateEnergy(row+1, col+1, flashTotal, grid);
            }
            // south
            propagateEnergy(row+1, col, flashTotal, grid);
        }
        // east
        if(col + 1 < (int)grid[row].size()){
            propagateEnergy(row, col+1, flashTotal, grid);
        }
        // west
        if(col - 1 >= 0){
            propagateEnergy(row, col-1, flashTotal, grid);
        }
    }else if(grid[row][col] != 0){
        // skip 0s so we don't increment octopuses that have already flashed
        grid[row][col]++;
    }
}

void iteration(std::vector<std::vector<int>>& grid, int& flashTotal){
    // strategy:
    // do 1 sweep of the grid to increase each octopus' energy by 1
    // do another sweep of the grid to figure out who is > 9 energy and propagate energy
    increaseEnergy(grid);
    for(int i = 0; i < (int)grid.size(); i++){
        for(int j = 0; j < (int)grid[i].size(); j++){
            if(grid[i][j] > 9){
                propagateEnergy(i, j, flashTotal, grid);
            }
        }
    }
}

void part2(std::vector<std::vector<int>>& grid){
    int iterationCount = 0;
    int flashTotal = 0;
    while(!checkSynchronized(grid)){
        iterationCount++;
        iteration(grid, flashTotal);
    }
    std::cout << "sync step: " << iterationCount << '\n';
}

int main(){
    std::ifstream file("input.txt");
    std::string line;
    
    int flashTotal = 0;
    int numIterations = 100;
    std::vector<std::vector<int>> grid;
    
    if(file.is_open()){
        while(std::getline(file, line)){
            std::vector<int> row;
            for(char c : line){
                row.push_back(c - 48);
            }
            grid.push_back(row);
        }
    }
    
    //printGrid(grid);
    //std::cout << "--------------\n";
    //increaseEnergy(grid);
    //printGrid(grid);
    
    // part 1
    for(int i = 0; i < numIterations; i++){
        iteration(grid, flashTotal);
    }
    std::cout << "total flashes: " << flashTotal << " after " << numIterations << " iterations.\n";
    
    // do separately from part 1 because we're using the same grid that would
    // be modified by part 1
    //part2(grid);
    
    return 0;
}