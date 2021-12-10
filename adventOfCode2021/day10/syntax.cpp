// g++ -Wall -std=c++14 syntax.cpp -o syntax

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <stack>

char getNext(std::stack<char>& stack){
    char next = stack.top();
    stack.pop();
    return next;
}

int main(){
    std::ifstream file("input.txt");
    std::string line;
    
    std::unordered_map<char, int> points{
        {'(', 3},
        {'[', 57},
        {'{', 1197},
        {'<', 25137}
    };
    
    int totalPoints = 0;
    
    if(file.is_open()){
        int lineCount = 1;
        while(std::getline(file, line)){
            std::stack<char> stack;
            int linePoints = 0;
            bool error = false;
            
            for(char c : line){
                if(c == ']'){
                    char next = getNext(stack);
                    if(next != '['){
                        linePoints += points['['];
                        std::cout << "line " << lineCount << ", error: expected " << "[ but saw " << next << '\n';
                        error = true;
                        break;
                    }
                }else if(c == '}'){
                    char next = getNext(stack);
                    if(next != '{'){
                        linePoints += points['{'];
                        std::cout << "line " << lineCount << ", error: expected " << "{ but saw " << next << '\n';
                        error = true;
                        break;
                    }
                }else if(c == ')'){
                    char next = getNext(stack);
                    if(next != '('){
                        linePoints += points['('];
                        std::cout << "line " << lineCount << ", error: expected " << "( but saw " << next << '\n';
                        error = true;
                        break;
                    }
                }else if(c == '>'){
                    char next = getNext(stack);
                    if(next != '<'){
                        linePoints += points['<'];
                        std::cout << "line " << lineCount << ", error: expected " << "< but saw " << next << '\n';
                        error = true;
                        break;
                    }
                }else{
                    // push all '[', '(', '<', '{' to stack
                    stack.push(c);
                }
            }
            lineCount++;
            
            if(error) totalPoints += linePoints;
            //if(!stack.empty() && !error) std::cout << "line " << lineCount << " is incomplete\n";
        }
        
        file.close();
    }
    
    std::cout << "total points: " << totalPoints << '\n';
    
    return 0;
}
