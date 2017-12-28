#include "maze.hh"
#include <iostream>

int main(){

	Maze m1 = Maze(5, 5);
	m1.setAllWalls();
	
	m1.setStart(0, 0);
	m1.setEnd(3, 3);
	
	// this is before clearing a wall 
	std::cout << "is there a wall to the south of 2,0? " << m1.hasWall(2, 0, Direction::SOUTH) << std::endl;
	
	m1.clearWall(2, 2, Direction::EAST);
	
	// std::cout is an ostream type 
	m1.print(std::cout);

	std::cout << "is there a wall to the east of 2,2? " << m1.hasWall(2, 2, Direction::EAST) << std::endl; //should be 0
	std::cout << "is there a wall to the south of 2,2? " << m1.hasWall(2, 2, Direction::SOUTH) << std::endl; // should be 1
	
	// south wall is disappearing here... (row 2, col 0) :< - should be 1
	std::cout << "is there a wall to the south of 2,0? " << m1.hasWall(2, 0, Direction::SOUTH) << std::endl;
	
	std::cout << "start location: " << (m1.getStart().row) << ", " << (m1.getStart().col) << std::endl;
	std::cout << "start is at 0,0: " << (m1.getStart() == Location(0,0)) << std::endl;
	
	return 0;
}