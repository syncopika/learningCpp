// g++ -Wall -std=c++14 dice.cpp -o dice
#include <cmath>
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <stdlib.h>

int getNumber(std::string& str){
    std::regex reg("\\d+$"); // extract digits at the end of the string only
    
    std::smatch matches;
    std::regex_search(str, matches, reg); // note it's regex_search, not regex_match
    
    // there should only be one number in the resulting vector
    return atoi(std::string(matches[0]).c_str());
}

void part1(int player1PosStart, int player2PosStart){
    int player1Score = 0;
    int player2Score = 0;
    
    int p1Pos = player1PosStart;
    int p2Pos = player2PosStart;
    
    int currDiceNum = 1;
    long diceRollCount = 0;
    
    while(player1Score < 1000 && player2Score < 1000){
        for(int i = 0; i < 3; i++){
            if(currDiceNum == 101){
                // only a 100-sided die
                currDiceNum = 1;
            }
            p1Pos += currDiceNum;
            diceRollCount++;
            currDiceNum++;
        }
        p1Pos = (p1Pos % 10 == 0) ? 10 : p1Pos % 10; // wraparound after 10 spaces
        player1Score += p1Pos;
        
        if(player1Score >= 1000){
            break;
        }
        
        // same thing for player 2
        for(int i = 0; i < 3; i++){
            if(currDiceNum == 101){
                currDiceNum = 1;
            }
            p2Pos += currDiceNum;
            diceRollCount++;
            currDiceNum++;
        }
        p2Pos = (p2Pos % 10 == 0) ? 10 : p2Pos % 10;
        player2Score += p2Pos;
    }
    
    std::cout << "player 1 score: " << player1Score << '\n';
    std::cout << "player 2 score: " << player2Score << '\n';
    std::cout << "num dice rolls: " << diceRollCount << '\n';
    std::cout << "answer: " << diceRollCount * std::min(player1Score, player2Score) << '\n';
}

void part2(){
    // TODO
}

int main(){
    std::ifstream file("input.txt");
    std::string line;
    
    int player1StartPos;
    int player2StartPos;
    
    if(file.is_open()){
        int count = 0;
        while(std::getline(file, line)){
            if(count++ == 0){
                player1StartPos = getNumber(line);
            }else{
                player2StartPos = getNumber(line);
            }
        }
    }
    
    std::cout << "player 1 start pos: " << player1StartPos << '\n';
    std::cout << "player 2 start pos: " << player2StartPos << '\n';
    
    part1(player1StartPos, player2StartPos);

    return 0;
}
