#include "maze.hh"
#include <iostream>

int main(){

	Maze m1 = Maze(5, 5);
	m1.setAllWalls();
	
	m1.setStart(0, 0);
	m1.setEnd(3, 3);
	
	// std::cout is an ostream type 
	m1.print(std::cout);

	std::cout << "start location: " << (m1.getStart().row) << ", " << (m1.getStart().col) << std::endl;
	std::cout << "start is at 0,0: " << (m1.getStart() == Location(0,0)) << std::endl;
	
	return 0;
}