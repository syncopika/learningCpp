/// g++ -Wall -std=c++14 heightmap.cpp -o heightmap

#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <utility>
#include <vector>

void drawMap(std::vector<std::vector<int>>& theMap){
    for(std::vector<int>& r : theMap){
        std::string row;
        for(int i : r){
            row += std::to_string(i) + " ";
        }
        std::cout << row << '\n';
    }
}

// part 1
int evalMinPoints(std::vector<std::vector<int>>& theMap){
    int minSum = 0;
    
    for(int i = 0; i < (int)theMap.size(); i++){
        std::vector<int>& row = theMap[i]; 
        for(int j = 0; j < (int)row.size(); j++){
            // check left, right, top and down
            int left = j - 1;
            int top = i - 1;
            int right = j + 1;
            int bottom = i + 1;
            
            int currVal = row[j];
            bool isMin = true;
            if(left >= 0){
                isMin = isMin && currVal < row[left];
            }
            if(right <= (int)row.size() - 1){
                isMin = isMin && currVal < row[right];
            }
            if(top >= 0){
                isMin = isMin && currVal < theMap[top][j];
            }
            if(bottom <= (int)theMap.size() - 1){
                isMin = isMin && currVal < theMap[bottom][j]; 
            }
            
            if(isMin){
                //std::cout << "found a min: " << currVal << '\n';
                minSum += (currVal + 1);
            }
        }
    }
    
    return minSum;
}

// part 2 - dfs
int findBasinTotal(std::vector<std::vector<int>>& theMap){
    std::priority_queue<int> maxHeap; // by default the priority queue is a max heap (largest nums at the top)
    
    for(int i = 0; i < (int)theMap.size(); i++){
        
        std::vector<int>& row = theMap[i];
        
        for(int j = 0; j < (int)row.size(); j++){
            if(theMap[i][j] == 9 || theMap[i][j] == -1){
                continue;
            }
            
            std::stack<std::pair<int, int>> stack;
            stack.push({i, j});
            
            int totalBasinSize = 0;
            
            while(!stack.empty()){
                std::pair<int, int> currCoord = stack.top();
                stack.pop();
                
                if(theMap[currCoord.first][currCoord.second] == -1){
                    // probably should actually store coords of spots we will check
                    // so we don't have the chance of revisiting a spot that gets marked as -1 :/ 
                    continue;
                }
                
                totalBasinSize++;
                
                //std::cout << "visiting i: " << currCoord.first << ", j: " << currCoord.second << ", value: " << theMap[currCoord.first][currCoord.second] << "\n";
               
                // check left, right, top and down
                int left = currCoord.second - 1;
                int top = currCoord.first - 1;
                int right = currCoord.second + 1;
                int bottom = currCoord.first + 1;
                
                if(left >= 0 && theMap[currCoord.first][left] != 9 && theMap[currCoord.first][left] != -1){
                    //std::cout << "adding left - i: " << currCoord.first << ", j: " << left << '\n';
                    stack.push({currCoord.first, left});
                }
                if(right < (int)row.size() && theMap[currCoord.first][right] != 9 && theMap[currCoord.first][right] != -1){
                    //std::cout << "adding right - i: " << currCoord.first << ", j: " << right << '\n';
                    stack.push({currCoord.first, right});
                }
                if(top >= 0 && theMap[top][currCoord.second] != 9 && theMap[top][currCoord.second] != -1){
                    //std::cout << "adding top - i: " << top << ", j: " << currCoord.second << '\n';
                    stack.push({top, currCoord.second});
                }
                if(bottom < (int)theMap.size() && theMap[bottom][currCoord.second] != 9 && theMap[bottom][currCoord.second] != -1){
                    //std::cout << "adding bottom - i: " << bottom << ", j: " << currCoord.second << '\n';
                    stack.push({bottom, currCoord.second});
                }
                
                // mark as -1
                theMap[currCoord.first][currCoord.second] = -1;
            }
            
            //std::cout << "basin found size: " << totalBasinSize << '\n';
            //drawMap(theMap);
            //std::cout << "----------------------\n";
            maxHeap.push(totalBasinSize);
        }
    }
    
    int first = maxHeap.top();
    maxHeap.pop();
    int second = maxHeap.top();
    maxHeap.pop();
    int third = maxHeap.top();
    maxHeap.pop();
    std::cout << "first: " << first << ", second: " << second << ", third: " << third << '\n';
    return (first * second * third);
}

int main(){
    std::ifstream file("input.txt");
    std::string line;
    
    std::vector<std::vector<int>> rows;
    
    if(file.is_open()){
        while(std::getline(file, line)){
            std::vector<int> row;
            for(char c : line){
                row.push_back(c - 48);
            }
            rows.push_back(row);
        }
        
        file.close();
    }
    
    /*
    for(std::vector<int>& r : rows){
        std::string row;
        for(int i : r){
            row += std::to_string(i) + " ";
        }
        std::cout << row << '\n';
    }*/
    
    //int res = evalMinPoints(rows); // part 1
    int res = findBasinTotal(rows); // part 2
    std::cout << "result: " << res << '\n';
    
    return 0;
}