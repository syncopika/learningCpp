#include "Matrix.hh"
#include <cassert>

// default constructor
Matrix::Matrix(){
	rows = 0;
	cols = 0;
	matrix = new int[rows*cols];
}

// constructor with parameters
Matrix::Matrix(int row, int col){
	
	//if negative values given, exit
	assert(row >= 0 && col >= 0);
	
	rows = row;
	cols = col;
	
	//remember that this can represent a 2d array!
	//to access arr[i][j], it would be: arr[i*col + j]
	matrix = new int[row*col];
	
	//set everything to 0
	for(int i = 0; i < rows*cols; i++){
		matrix[i] = 0;
	}
}

// copy constructor
Matrix::Matrix(Matrix &m){
	
	rows = m.getrows();
	cols = m.getcols();
	
	matrix = new int[rows*cols];
	
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			matrix[i*cols + j] = m.getelem(i, j);
		}
	}
	
}

// destructor
Matrix::~Matrix(){
	delete [] matrix;
}
	
// getters
// the presence of 'const' indicates that the member function does not change 
// the object it's called on 
int Matrix::getrows() const{
	return rows;
}

int Matrix::getcols() const{
	return cols;
}

int Matrix::getelem(int row, int col) const{
	return matrix[row*cols + col];
}
	
// setter
void Matrix::setelem(int row, int col, int value){
	
	//if invalid values given, exit
	assert(row <= rows && col <= cols);
	
	matrix[row*cols + col] = value;
}
	
// mutators
void Matrix::add(Matrix &m){
	
	bool matchRows = (m.getrows() == rows);
	bool matchCols = (m.getcols() == cols);
	
	if(matchRows && matchCols){
		for(int i = 0; i < rows; i++){
			for(int j = 0; j < cols; j++){
				//add the corresponding coordinate's value
				//with this array's coordinate value
				matrix[i*cols + j] += m.getelem(i, j);
			}
		}
	}
}

void Matrix::subtract(Matrix &m){
	
	bool matchRows = (m.getrows() == rows);
	bool matchCols = (m.getcols() == cols);
	
	if(matchRows && matchCols){
		for(int i = 0; i < rows; i++){
			for(int j = 0; j < cols; j++){
				//subtract the corresponding coordinate's value
				//from this array's coordinate values
				matrix[i*cols + j] -= m.getelem(i, j);
			}
		}
	}
}
	
// equals method
bool Matrix::equals(Matrix &m){
	
	bool matchRows = (m.getrows() == rows);
	bool matchCols = (m.getcols() == cols);
	
	if(matchRows && matchCols){
		for(int i = 0; i < rows; i++){
			for(int j = 0; j < cols; j++){
				//check if each coordinate in both matrices
				//contain the same value
				if(matrix[i*cols + j] != m.getelem(i, j)){
					return false;
				}
			}
		}
	}else{
		// if matchrows != matchCols
		return false;
	}
	return true;
}
