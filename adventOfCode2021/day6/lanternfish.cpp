// g++ -Wall -std=c++14 lanternfish.cpp -o lanternfish

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

std::string getStateString(std::vector<int>& fish){
    std::string state("");
    for(int& f : fish){
        state += std::to_string(f) + ",";
    }
    return state;
}

// simple way to handle smaller amounts of fish
void checkFish(std::vector<int>& fish, int numDays){
    int newFishToAdd = 0;
    for(int i = 0; i < numDays; i++){
        for(int j = 0; j < (int)fish.size(); j++){
            if(fish[j] == 0){
                // reset fish
                fish[j] = 6;
                newFishToAdd++;
            }else{
                fish[j]--;
            }
        }
        
        for(int k = 0; k < newFishToAdd; k++){
            fish.push_back(8);
        }
        newFishToAdd = 0;
        
        //std::cout << "state after " << (i+1) << " days: " << getStateString(fish) << '\n';
    }
    std::cout << "final num fish using checkFish: " << fish.size() << '\n';
}

// handle more fish?
void checkFish2(std::unordered_map<int, long long>& fish, int numDays){
    for(int i = 0; i < numDays; i++){
        long long newFishToAdd = 0;
        long long resetCount = 0;
        for(int j = 0; j < 9; j++){
            if(j == 0){
                // reset (but after all changes have been made to the other states)
                resetCount = fish[j];
                // new fish to add
                newFishToAdd = fish[j];
            }else{
                fish[j-1] += fish[j];
            }
            fish[j] = 0;
        }
        fish[6] += resetCount;
        fish[8] = newFishToAdd;
    }
    
    long long sum = 0;
    for(auto& kv : fish){
        //std::cout << "state " << kv.first << ", count: " << kv.second << '\n';
        sum += kv.second;
    }
    
    std::cout << "final num fish using checkFish2: " << sum << '\n';
}

int main(){
    std::ifstream file("input.txt");
    std::string line;
    
    std::vector<int> fish;
    std::unordered_map<int, long long> fish2;
    
    for(int i = 0; i < 9; i++){
        fish2[i] = 0;
    }
    
    if(file.is_open()){
        while(std::getline(file, line)){
            for(int i = 0; i < (int)line.length(); i++){
                if(line[i] != ','){
                    fish.push_back(line[i] - 48); // subtract 48 to convert char to int
                    fish2[line[i] - 48]++;
                }
            }
        }
    
        file.close();
    }
    
    //std::cout << "initial state: " << getStateString(fish) << '\n';
    
    //checkFish(fish, 80);
    checkFish2(fish2, 256);
    
    return 0;
}
