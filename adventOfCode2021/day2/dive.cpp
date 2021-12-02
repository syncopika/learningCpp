// g++ -Wall -std=c++14 dive.cpp -o dive

#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>

#include <stdlib.h> // atoi

// https://stackoverflow.com/questions/53849/how-do-i-tokenize-a-string-in-c
std::vector<std::string> getTokens(std::string& str){
    std::regex reg("\\s+"); // break on all spaces
    std::sregex_token_iterator iter(str.begin(), str.end(), reg, -1);
    std::sregex_token_iterator end;
    std::vector<std::string> vec(iter, end);
    return vec;
}

void part1(){
    std::ifstream file("input.txt");
    std::string line;
    std::map<std::string, int> log{
        {"horizontal", 0},
        {"depth", 0}
    };
    
    if(file.is_open()){
        while(std::getline(file, line)){
            std::vector<std::string> tokens = getTokens(line);
            std::string direction(tokens[0]);
            int amount = atoi(tokens[1].c_str());
            
            //std::cout << "direction: " << direction << '\n';
            //std::cout << "amount: " << amount << '\n';
            
            if(direction == "forward"){
                log["horizontal"] += amount;
            }else if(direction == "up"){
                log["depth"] -= amount;
            }else{
                log["depth"] += amount;
            }
        }
        file.close();
    }

    std::cout << "horizontal * depth: " << log["horizontal"] * log["depth"] << '\n';
}

void part2(){
    std::ifstream file("input.txt");
    std::string line;
    std::map<std::string, int> log{
        {"horizontal", 0},
        {"depth", 0},
        {"aim", 0}
    };
    
    if(file.is_open()){
        while(std::getline(file, line)){
            std::vector<std::string> tokens = getTokens(line);
            std::string direction(tokens[0]);
            int amount = atoi(tokens[1].c_str());
            
            //std::cout << "direction: " << direction << '\n';
            //std::cout << "amount: " << amount << '\n';
            
            if(direction == "forward"){
                log["horizontal"] += amount;
                log["depth"] += (log["aim"] * amount);
            }else if(direction == "up"){
                log["aim"] -= amount;
            }else{
                log["aim"] += amount;
            }
        }
        file.close();
    }

    std::cout << "horizontal * depth: " << log["horizontal"] * log["depth"] << '\n';
}

int main(){
    part1();
    part2();
    return 0;
}
