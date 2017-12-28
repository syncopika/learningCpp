#include "maze.hh"
#include <iostream>
#include <cassert>

// implementation of class Maze 

// private helper methods 
	
// convert 2D expanded coordinates (from Location obj) to 1D array index
// use formula: row*N_cols + col
int Maze::get1DIndex(const Location &loc) const{
	int x = loc.col;
	int y = loc.row;
	// the index of loc in a 1D array should be y*numCols+x
	// the numCols is 2*numCols + 1 because this is the expanded number of columns! 
	return (y*(2*numCols + 1)) + x;
}

// convert regular maze matrix coord into expanded 2D coordinates 
Location Maze::getExpandedCoords(int row, int col) const{
	// given the coordinates of a cell (i.e. in a regular n x n matrix; no expanded stuff)
	// convert the coordinates into expanded coordinates
	// r_exp = 2 * r_cell + 1, c_exp = 2 * c_cell + 1 
	int rowExpanded = (2*row) + 1;
	int colExpanded = (2*col) + 1;
	
	Location loc = Location(rowExpanded, colExpanded);
	return loc; 
}

// get the expanded coordinates of the wall on a particular side of a cell
// given its regular 1D coords 
// row, col, direction
Location Maze::getWallCoords(int row, int col, Direction dir) const{
	
	if(dir == Direction::NORTH){
		// convert cell coord to expanded coord first. then subtract 1 from the row.
		int expRow = (2*row);
		int expCol = (2*col) + 1;
		return Location(expRow, expCol);
	
	}else if(dir == Direction::SOUTH){
		
		int expRow = (2*row) + 2;
		int expCol = (2*col) + 1;
		return Location(expRow, expCol);
		
	}else if(dir == Direction::EAST){

		int expRow = (2*row) + 1;
		int expCol = (2*col) + 2;
		return Location(expRow, expCol);
	
	}else{
		
		// direction is WEST
		int expRow = (2*row) + 1;
		int expCol = (2*col);
		return Location(expRow, expCol);
	}
}

// get expanded size of maze (i.e. size of cells variable)
int Maze::getMazeSize() const{
	return ((2*numRows) + 1)*((2*numCols) + 1);
}


// constructor 
Maze::Maze(int rows, int cols){
	
	// initialize member vars
	numRows = rows;
	numCols = cols;
	
	// dynamically allocate an array of enum objects
	// this needs to be deleted in the destructor!
	int size = getMazeSize();
	
	// cells is a 1D representation of the expanded maze (including walls)
	cells = new MazeCell[size];

	// initialize everything to empty 
	clear();
	
}

// copy constructor 
Maze::Maze(const Maze &m){
	
	// get number of rows and columns 
	numRows = m.getNumRows();
	numCols = m.getNumCols();
	
	// set start and end 
	start = m.getStart();
	end = m.getEnd();
	
	// make a deep copy of the expanded representation of the maze 
	int size = getMazeSize();
	cells = new MazeCell[size];
	
	// initialize maze to empty first 
	clear();
	
	for(int i = 0; i < numRows; i++){
		for(int j = 0; j < numCols; j++){
			// each i,j coord represents a cell in the maze (not expanded! - so not including walls)
			// for each cell, need to know if a wall exists around it.
			// convert that wall's coords to 1D array coords and add it 
			// to 'cells' variable
			Location expandedCellCoords = getExpandedCoords(i, j);
			cells[get1DIndex(expandedCellCoords)] = m.getCell(i, j);
			
			// then check walls in N,S,E,W
			if(m.hasWall(i, j, Direction::NORTH)){
				Location north = getWallCoords(i, j, Direction::NORTH);
				cells[get1DIndex(north)] = MazeCell::WALL;
			}
				
			if(m.hasWall(i, j, Direction::SOUTH)){
				Location south = getWallCoords(i, j, Direction::SOUTH);
				cells[get1DIndex(south)] = MazeCell::WALL;
			}
			
			if(m.hasWall(i, j, Direction::EAST)){
				Location east = getWallCoords(i, j, Direction::EAST);
				cells[get1DIndex(east)] = MazeCell::WALL;
			}
			
			if(m.hasWall(i, j, Direction::WEST)){
				Location west = getWallCoords(i, j, Direction::WEST);
				cells[get1DIndex(west)] = MazeCell::WALL;
			}
		}
	}
	
}

// destructor 
Maze::~Maze(){
	// need to deallocate cells variable 
	delete cells; 
}

// assignment operator overloading 
Maze & Maze::operator=(const Maze &m){
	// check for self-assignment! 
	// 'this' is a pointer, so the the pointer to m
	if(this != &m){
		
		// delete current mem allocations for this object 
		delete cells;
		
		// get the current data from m 
		numRows = m.getNumRows();
		numCols = m.getNumCols();
		start = m.getStart();
		end = m.getEnd();
		
		// then reallocate data and set up new maze
		int size = getMazeSize();
		cells = new MazeCell[size];
		
		// initialize maze to empty first 
		clear();
		
		for(int i = 0; i < numRows; i++){
			for(int j = 0; j < numCols; j++){
				// each i,j coord represents a cell in the maze (not expanded! - so not including walls)
				// for each cell, need to know if a wall exists around it.
				// convert that wall's coords to 1D array coords and add it 
				// to 'cells' variable
				Location expandedCellCoords = getExpandedCoords(i, j);
				cells[get1DIndex(expandedCellCoords)] = m.getCell(i, j);
				
				// then check walls in N,S,E,W
				if(m.hasWall(i, j, Direction::NORTH)){
					Location north = getWallCoords(i, j, Direction::NORTH);
					cells[get1DIndex(north)] = MazeCell::WALL;
				}
					
				if(m.hasWall(i, j, Direction::SOUTH)){
					Location south = getWallCoords(i, j, Direction::SOUTH);
					cells[get1DIndex(south)] = MazeCell::WALL;
				}
				
				if(m.hasWall(i, j, Direction::EAST)){
					Location east = getWallCoords(i, j, Direction::EAST);
					cells[get1DIndex(east)] = MazeCell::WALL;
				}
				
				if(m.hasWall(i, j, Direction::WEST)){
					Location west = getWallCoords(i, j, Direction::WEST);
					cells[get1DIndex(west)] = MazeCell::WALL;
				}
			}
		}	
	}
	return *this; // return the reference to this 
}

// getters 
int Maze::getNumRows() const{
	return numRows;
}

int Maze::getNumCols() const{
	return numCols;
}

Location Maze::getStart() const{
	return start;
}

Location Maze::getEnd() const{
	return end;
}

// only use this function for getting cells that are in the maze (not wall cells)
MazeCell Maze::getCell(int cellRow, int cellCol) const{
	Location loc = getExpandedCoords(cellRow, cellCol);
	return cells[get1DIndex(loc)];
}

bool Maze::hasWall(int cellRow, int cellCol, Direction direction) const{
	Location wallLoc = getWallCoords(cellRow, cellCol, direction);
	int index = get1DIndex(wallLoc);
	return (cells[index] == MazeCell::WALL);
}

bool Maze::isVisited(int cellRow, int cellCol) const{
	Location cellLoc = getExpandedCoords(cellRow, cellCol);
	int index = get1DIndex(cellLoc);
	return (cells[index] == MazeCell::VISITED);
}

Location Maze::getNeighborCell(int cellRow, int cellCol, Direction direction) const{
	if(direction == Direction::NORTH){
		assert(cellRow - 1 >= 0 && cellRow < numRows);
		return Location(cellRow - 1, cellCol);
	}else if(direction == Direction::SOUTH){
		assert(cellRow + 1 < numRows && cellRow >= 0);
		return Location(cellRow + 1, cellCol);
	}else if(direction == Direction::EAST){
		assert(cellCol + 1 < numCols && cellCol >= 0);
		return Location(cellRow, cellCol + 1);
	}else{
		// assuming no invalid directions are passed in 
		assert(cellCol - 1 >= 0 && cellCol < numCols);
		assert(direction == Direction::WEST);
		return Location(cellRow, cellCol - 1);
	}
}

// setters
void Maze::clear(){
	int size = getMazeSize();
	for(int i = 0; i < size; i++){
		// set everything to empty 
		cells[i] = MazeCell::EMPTY;
	}
}

void Maze::clearWall(int cellRow, int cellCol, Direction direction){
	Location wallLoc = getWallCoords(cellRow, cellCol, direction);
	int index = get1DIndex(wallLoc);
	cells[index] = MazeCell::EMPTY;
}

void Maze::setCell(int cellRow, int cellCol, MazeCell val){
	Location cellLoc = getExpandedCoords(cellRow, cellCol);
	int index = get1DIndex(cellLoc);
	cells[index] = val;
}

void Maze::setAllWalls(){
	for(int i = 0; i < numRows; i++){
		for(int j = 0; j < numCols; j++){
			// for each cell, add a wall in N,S,E,W 
			setWall(i, j, Direction::NORTH);
			setWall(i, j, Direction::SOUTH);
			setWall(i, j, Direction::EAST);
			setWall(i, j, Direction::WEST);
		}
	}
}

void Maze::setStart(int row, int col){
	start = Location(row, col);
}

void Maze::setEnd(int row, int col){
	end = Location(row, col);
}

void Maze::setWall(int cellRow, int cellCol, Direction direction){
	Location wallLoc = getWallCoords(cellRow, cellCol, direction);
	int index = get1DIndex(wallLoc);
	cells[index] = MazeCell::WALL;
}

void Maze::setVisited(int cellRow, int cellCol){
	Location cellLoc = getExpandedCoords(cellRow, cellCol);
	int index = get1DIndex(cellLoc);
	cells[index] = MazeCell::VISITED;
}

// print function 
void Maze::print(ostream &os) const{
	// first output number of rows and columns 
	os << numRows << " " << numCols << std::endl;
	
	string topRow = "+";
	
	Location expandedStart = getExpandedCoords(start.row, start.col);
	Location expandedEnd = getExpandedCoords(end.row, end.col);
	
	for(int k = 0; k < numRows; k++){
			
		string bottomRow = "+";
		string currRow = "";
	
		for(int l = 0; l < numCols; l++){
			
			if(k == 0){
				// get top row (only for 1st row!) the subsequent rows' 'top' row will be the 
				// the previous row's 'bottom' row. these top and bottom rows are where walls go. 
				if(hasWall(k, l, Direction::NORTH)){
					topRow += "---+";
				}else{
					topRow += "   +";
				}
			}
			
			// check left walls for each cell in this row
			if(hasWall(k, l, Direction::WEST)){
				currRow += "|";
			}else{
				currRow += " ";
			}
			
			// check if this cell is start, end or empty 
			Location loc = getExpandedCoords(k, l);
			if(loc.row == expandedStart.row && loc.col == expandedStart.col){
				// found the start cell! 
				currRow += " S ";
			}else if(loc.row == expandedEnd.row && loc.col == expandedEnd.col){
				// found the end cell!
				currRow += " E ";
			}else if(cells[get1DIndex(loc)] == MazeCell::EMPTY){
				// cell is empty 
				currRow += "   ";
			}
			
			// if last cell for the row, check the right wall 
			if(l == (numCols - 1)){
				if(hasWall(k, l, Direction::EAST)){
					currRow += "|\n";	// add a new line!
				}else{
					currRow += "\n";
				}
			}
			
			// take care of the bottom wall row here
			if(hasWall(k, l, Direction::SOUTH)){
				bottomRow += "---+";
			}else{
				bottomRow += "   +";
			}
			
		}
		
		// set very first row
		if(k == 0){
			os << topRow << std::endl;
		}
		
		os << currRow << bottomRow << std::endl;
		
	}
}




