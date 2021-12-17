// g++ -Wall -std=c++14 chiton.cpp -o chiton
#include <climits>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>

// https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
// https://stackoverflow.com/questions/41648480/how-can-i-create-a-stdset-of-structures
struct Coord {
    int x;
    int y;
    
    bool operator==(const Coord& other) const {
        return other.x == x && other.y == y;
    }
    
    // necessary when using with std::set
    bool operator<(const Coord& other) const {
        return (y < other.y && !(other.x < x)) || (x < other.x);
    }
};

// compare functor for priority queue since we want a min-heap and it's a max-heap by default
struct Compare {
	bool operator()(const std::pair<int, Coord> a, const std::pair<int, Coord> b){
		return a.first > b.first;
	}
};

// need a hash functor to be able to use Coord as a key in map
// interesting: https://stackoverflow.com/questions/682438/hash-function-providing-unique-uint-from-an-integer-coordinate-pair
// and https://stackoverflow.com/questions/22826326/good-hashcode-function-for-2d-coordinates
// and https://stackoverflow.com/questions/9209323/easiest-way-of-using-min-priority-queue-with-key-update-in-c
// append a new hash function for Coord to the std namespace
namespace std {
    template <>
    struct hash<Coord> {
        std::size_t operator()(const Coord& coord) const {
            int tmp = coord.y + ((coord.x+1)/2);
            return coord.x + (tmp * tmp);
        }
    };
}

int main(){
    std::ifstream file("input.txt");
    std::string line;
 
    std::priority_queue<std::pair<int, Coord>, std::vector<std::pair<int, Coord>>, Compare> queue;
    std::set<Coord> visited;
    std::vector<std::vector<int>> grid;
    std::unordered_map<Coord, int> cost;
    std::unordered_map<Coord, Coord> previous;
 
    if(file.is_open()){
        int y = 0;
        while(std::getline(file, line)){
            std::vector<int> row;
            
            int x = 0;
            for(char c : line){
                row.push_back(c - 48);
                
                Coord co{x, y};
                Coord undefined{-1, -1};
                
                if(x == 0 && y == 0){
                    cost[co] = 0;
                    queue.push({0, co});
                }else{
                    cost[co] = INT_MAX;
                }
                
                previous[co] = undefined;
                x++;
            }
            
            grid.push_back(row);
            y++;
        }
        
        file.close();
    }

    // try Dijkstra's algorithm
    while(!queue.empty()){
        Coord curr = queue.top().second;
        
        if(visited.find(curr) != visited.end()){
            queue.pop();
            continue;
        }
        
        visited.insert(curr);
        
        std::vector<Coord> adjNodes;
        
        // check left
        if(curr.x - 1 >= 0){
            Coord left{curr.x - 1, curr.y};
            if(visited.find(left) == visited.end()){
                adjNodes.push_back(left);
            }
        }
        
        // check right
        if(curr.x + 1 < (int)grid[0].size()){
            Coord right{curr.x + 1, curr.y};
            if(visited.find(right) == visited.end()){
                adjNodes.push_back(right);
            }
        }
        
        // check up
        if(curr.y - 1 >= 0){
            Coord up{curr.x, curr.y - 1};
            if(visited.find(up) == visited.end()){
                adjNodes.push_back(up);
            }
        }
        
        // check down
        if(curr.y + 1 < (int)grid.size()){
            Coord bottom{curr.x, curr.y + 1};
            if(visited.find(bottom) == visited.end()){
                adjNodes.push_back(bottom);
            }
        }
        
        for(Coord& neighbor : adjNodes){
            if(cost[curr] + grid[neighbor.y][neighbor.x] < cost[neighbor]){
                cost[neighbor] = cost[curr] + grid[neighbor.y][neighbor.x];
                previous[neighbor] = curr;
                queue.push({cost[neighbor], neighbor});
            }
        }
        
        queue.pop();
    }
    
    int endY = (int)grid.size() - 1;
    int endX = (int)grid[0].size() - 1;
    
    Coord end{endY, endX};
    Coord prev = previous[end];
    while(prev.x != -1){
        std::cout << "prev to end: " << prev.y << ", " << prev.x << " cost: " << cost[prev] << '\n';
        prev = previous[prev];
    }
    std::cout << "lowest cost: " << cost[end] << '\n';
    
    return 0;
}

