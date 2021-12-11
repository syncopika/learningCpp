// g++ -Wall -std=c++14 syntax.cpp -o syntax
#include <algorithm>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <stack>
#include <vector>

char getNext(std::stack<char>& stack){
    char next = stack.top();
    stack.pop();
    return next;
}

// for std::sort
bool compare(long long a, long long b){
    return a < b;
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
    std::vector<long long> results; // part 2
    
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
            
            // part 2
            if(!stack.empty() && !error){
                //std::cout << "line " << lineCount << " is incomplete\n";
                std::string rem;
                long long total = 0;
                while(!stack.empty()){
                    char next = getNext(stack);
                    rem += next;
                    
                    if(next == '['){
                        total = (total*5) + 2;
                    }else if(next == '('){
                        total = (total*5) + 1;
                    }else if(next == '{'){
                        total = (total*5) + 3;
                    }else if(next == '<'){
                        total = (total*5) + 4;
                    }
                }
                results.push_back(total);
                std::cout << "remaining on stack: " << rem << ", total: " << total << '\n';
            }
        }
        
        file.close();
    }
    
    std::cout << "total points: " << totalPoints << '\n';
    
    // part 2
    // needed long long again. whenever large negative numbers start showing up,
    // it's a sign :)
    std::sort(results.begin(), results.end(), compare);
    std::cout << "part 2 total: " << results[(int)results.size()/2] << '\n';
    
    return 0;
}
