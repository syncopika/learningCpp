// g++ -Wall -std=c++14 hydrothermal.cpp -o hydrothermal

#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
//#include <utility>
#include <regex>
#include <string>
#include <vector>
#include <assert.h>
#include <stdlib.h>

struct Line {
    //std::pair<int, int> start;
    //std::pair<int, int> end;
    int x1;
    int y1;
    int x2;
    int y2;
    
    Line(int a, int b, int c, int d){
        x1 = a;
        y1 = b;
        x2 = c;
        y2 = d;
    }

    Line(std::vector<int>& list){
        x1 = list[0];
        y1 = list[1];
        x2 = list[2];
        y2 = list[3];
    }
    
    void printLine(){
        std::cout << "x1: " << std::to_string(x1) << ", y1: " << std::to_string(y1) << ", x2: " << std::to_string(x2) << ", y2: " << std::to_string(y2) << '\n';
    }
    
    /*
    bool overlaps(Line& line){
        int minLineX = std::min(line.x1, line.x2);
        //int maxLineX = max(line.x1, line.x2);
        int minX = std::min(x1, x2);
        int maxX = std::max(x1, x2);
        
        int minLineY = std::min(line.y1, line.y2);
        //int maxLineY = max(line.y1, line.y2);
        int minY = std::min(y1, y2);
        int maxY = std::max(y1, y2);
        
        bool xOverlap = (minLineX >= minX && maxX >= minLineX);
        bool yOverlap = (minY <= minLineY && maxY >= minLineY);
        
        return xOverlap && yOverlap;
    }*/
};

std::vector<std::string> getTokens(const char* delimiter, std::string& str){
    std::regex reg(delimiter); // break on all spaces \\s+
    std::sregex_token_iterator iter(str.begin(), str.end(), reg, -1);
    std::sregex_token_iterator end;
    std::vector<std::string> vec(iter, end);
    return vec;
}

std::vector<int> getLineData(std::string& str){
    // expect str to be like: "x,y -> x2,y2"
    std::vector<std::string> coords = getTokens("\\s->\\s", str);
    assert((int)coords.size() == 2);
    
    std::vector<std::string> point1 = getTokens(",", coords[0]);
    std::vector<std::string> point2 = getTokens(",", coords[1]);
    
    std::vector<int> coordInts;
    
    std::for_each(point1.begin(), point1.end(), [&](std::string& s){
        coordInts.push_back(atoi(s.c_str()));
    });
    
    std::for_each(point2.begin(), point2.end(), [&](std::string& s){
        coordInts.push_back(atoi(s.c_str()));
    });
    
    return coordInts;
}

void setupDiagram(std::vector<std::vector<int>>& diagram, int maxX, int maxY){
    for(int i = 0; i <= maxY; i++){
        std::vector<int> row;
        for(int j = 0; j <= maxX; j++){
            row.push_back(0);
        }
        diagram.push_back(row);
    }
}

void addLinesToDiagram(std::vector<std::vector<int>>& diagram, std::vector<Line>& lines){
    for(Line& ln : lines){
        int startX = ln.x1;
        int startY = ln.y1;
        int endX = ln.x2;
        int endY = ln.y2;
        
        // only consider horizontal and vertical lines
        // - where x1 == x2 || y1 == y2
        if(startX == endX || startY == endY){
            // figure out min and max x and y's so it'll be easier on us for the loop
            int xStart = std::min(startX, endX);
            int xEnd = std::max(startX, endX);
            int yStart = std::min(startY, endY);
            int yEnd = std::max(startY, endY);
            
            for(int i = xStart; i <= xEnd; i++){
                std::vector<int>& row = diagram[i];
                for(int j = yStart; j <= yEnd; j++){
                    row[j] += 1;
                }
            }
        }else if(std::abs(endX - startX) == std::abs(endY - startY)){
            // consider diagonal lines for part 2
            // we can know if a 45 deg diagonal line is formed
            // if the resulting vector of point2 - point1 has
            // the same absolute x and y values.
            
            // normalize the vector so that it's <(+ or -)1, (+ or -)1>
            int xStep = (endX - startX) / std::abs(endX - startX);
            int yStep = (endY - startY) / std::abs(endY - startY);
            
            // be careful about off-by-1 errors! super easy to make them here.
            for(int i = startX; i != endX + xStep; i += xStep){
                std::vector<int>& row = diagram[i];
                row[startY] += 1;
                startY += yStep;
            }
        }
    }
}

int evaluateDiagram(std::vector<std::vector<int>>& diagram){
    // return number of overlaps (so indexes with at least 2)
    int overlaps = 0;
    
    for(int i = 0; i < (int)diagram.size(); i++){
        std::vector<int> row = diagram[i];
        for(int j = 0; j < (int)row.size(); j++){
            if(row[j] >= 2) overlaps++;
        }
    }
    
    return overlaps;
}

void printDiagram(std::vector<std::vector<int>>& diagram){
    for(int i = 0; i < (int)diagram.size(); i++){
        std::vector<int> row = diagram[i];
        std::string s;
        for(int j = 0; j < (int)row.size(); j++){
            s += std::to_string(row[j]) + " ";
        }
        std::cout << s << '\n';
    }
}

int main(){
    std::ifstream file("input.txt");
    std::string line;
    
    std::vector<Line> lines;
    
    int maxXCoord = 0;
    int maxYCoord = 0;
    if(file.is_open()){
        while(std::getline(file, line)){
            std::vector<int> coords = getLineData(line);
            
            maxXCoord = std::max(maxXCoord, std::max(coords[0], coords[2]));
            maxYCoord = std::max(maxYCoord, std::max(coords[1], coords[3]));
            
            Line newLine(coords);
            lines.push_back(newLine);
        }
        
        file.close();
    }
    
    std::vector<std::vector<int>> diagram;
    setupDiagram(diagram, maxXCoord, maxYCoord);
    addLinesToDiagram(diagram, lines);
    int overlaps = evaluateDiagram(diagram);
    
    std::cout << "max X: " << maxXCoord << ", max Y: " << maxYCoord << '\n';
    std::cout << "overlaps: " << overlaps << '\n';
    
    //printDiagram(diagram);
    
    return 0;
}
