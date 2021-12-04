#include <algorithm>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <regex>
#include <set>
#include <string>
#include <stdlib.h>

struct BingoBoard {
    std::vector<std::vector<int>> board;
    
    void printBoard(){
        for(int i = 0; i < (int)board.size(); i++){
            std::string s = "";
            for(int j = 0; j < (int)board[i].size(); j++){
                //std::cout << board[i][j] << '\n';
                s += std::to_string(board[i][j]) + " "; 
            }
            std::cout << s << std::endl;
        }
        /*
        for(auto& row : board){
            std::string r("");
            for(int& n : row){
                r += n + " ";
            }
            std::cout << r << '\n';
        }
        */
    }
};


void markBingoBoards(std::vector<std::unique_ptr<BingoBoard>>& boards, int numToMark){
    for(std::unique_ptr<BingoBoard>& b : boards){
        // remember that assignment will implicitly call the copy constructor, 
        // so modifying without using a reference will modify the copy, which is not what we want
        std::vector<std::vector<int>>& theBoard = b->board;
        for(int i = 0; i < (int)theBoard.size(); i++){
            std::vector<int>& row = theBoard[i]; // b/c we use a reference to the original vector, we can modify it
            for(int j = 0; j < (int)row.size(); j++){
                if(row[j] == numToMark){
                    row[j] = -1; // mark with -1
                }
            }
        }
    }
}

std::vector<int> checkBingoBoards(std::vector<std::unique_ptr<BingoBoard>>& boards){
    std::vector<int> winners;
    for(int x = 0; x < (int)boards.size(); x++){
        std::vector<std::vector<int>> theBoard = boards[x]->board;
        
        //bool hasBingoRowVert = true;
        for(int i = 0; i < (int)theBoard.size(); i++){
            std::vector<int> row = theBoard[i];
            
            // check horizontal
            bool hasBingoRowHorz = true;
            for(int j = 0; j < (int)row.size(); j++){
                hasBingoRowHorz = hasBingoRowHorz && (row[j] == -1);
            }
            
            if(hasBingoRowHorz){
                winners.push_back(x);
                break;
            }
            
            // check vertical
            bool hasBingoRowVert = true;
            for(std::vector<int>& row : theBoard){
                hasBingoRowVert = hasBingoRowVert && (row[i] == -1);
            }
            if(hasBingoRowVert) winners.push_back(x);
            
            // doing this here would be checking for diagonals, which is a no-go for this exercise
            //hasBingoRowVert = hasBingoRowVert && (row[i] == -1);
        }
    }
    
    return winners;
}

int sumUnmarkedNumsOnBoard(std::unique_ptr<BingoBoard>& b){
    int sum = 0;

    std::vector<std::vector<int>>& theBoard = b->board;
    for(int i = 0; i < (int)theBoard.size(); i++){
        std::vector<int>& row = theBoard[i];
        for(int j = 0; j < (int)row.size(); j++){
            if(row[j] != -1){
                sum += row[j];
            }
        }
    }

    return sum;
}

std::vector<std::string> getTokens(const char* delimiter, std::string& str){
    std::regex reg(delimiter); // break on all spaces \\s+
    std::sregex_token_iterator iter(str.begin(), str.end(), reg, -1);
    std::sregex_token_iterator end;
    std::vector<std::string> vec(iter, end);
    return vec;
}

void addNumsToList(const char* toSplit, std::string& str, std::vector<int>& list){
    std::vector<std::string> tokens = getTokens(toSplit, str);
    std::for_each(tokens.begin(), tokens.end(), [&](std::string& s){
        if(s != ""){
            list.push_back(atoi(s.c_str()));
            //std::cout << s << '\n';
        }
    });
    //std::cout << "========================\n";
}


int main(){
    std::ifstream file("input.txt");
    std::string line;

    if(file.is_open()){
        int counter = 0;
        
        std::vector<std::unique_ptr<BingoBoard>> boardList;
        
        int currBoardIndex = -1;
        
        std::vector<int> bingoNums;
        
        while(std::getline(file, line)){
            if(counter == 0){
                // get all the bingo nums to try
                addNumsToList(",", line, bingoNums);
            }else if(line == ""){
                // start a new board
                //std::cout << "making a new board...\n";
                // note: make_unique is a c++14 feature!
                boardList.push_back(std::move(std::make_unique<BingoBoard>()));
                currBoardIndex++;
            }else{
                // add new rows to board
                std::vector<int> newRow;
                addNumsToList("\\s+", line, newRow);
                boardList[currBoardIndex]->board.push_back(newRow);
            }
            counter++;
        }
        
        std::vector<int> winnerOrder;
        std::set<int> seen;
        int numsDrawn = 0;
        for(int& bingoNum : bingoNums){
            //std::cout << "round " << numsDrawn << ": \n";
            //std::cout << "drawing: " << bingoNum << '\n';
            markBingoBoards(boardList, bingoNum);
            
            if(numsDrawn >= (int)boardList[0]->board.size()){
                // after we've drawn x numbers,
                // where x should be at least the number of rows,
                // which should also be the same number of columns of the board
                // we can check the boards for possible bingo
                std::vector<int> winners = checkBingoBoards(boardList);
                if(winners.size() > 0){
                    for(int& bingo : winners){
                        if(seen.find(bingo) == seen.end()){
                            int sum = sumUnmarkedNumsOnBoard(boardList[bingo]);
                            std::cout << "board num: " << bingo << " got bingo with the last number being: " << bingoNum << ", final score: " << sum * bingoNum << '\n';
                            seen.insert(bingo);
                            winnerOrder.push_back(bingo);
                        }
                    }
                }
            }
            
            numsDrawn++;
        }
        
        std::cout << "first winning board: " << winnerOrder.front() << '\n';
        std::cout << "last winning board: " << winnerOrder.back() << '\n';
        
        file.close();
    }
    
    return 0;
}
