// header file for Matrix class
//http://courses.cms.caltech.edu/cs11/material/cpp/mike/lab2/lab2.html

class Matrix{

public:

	// default constructor
	Matrix();
	
	// other constructor
	Matrix(int row, int col);
	
	// copy constructor
	Matrix(Matrix &m);
	
	// destructor
	~Matrix();
	
	// getters
	int getrows() const;
	int getcols() const;
	int getelem(int row, int col) const;
	
	// setter
	void setelem(int row, int col, int value);
	
	// mutators
	void add(Matrix &m);
	void subtract(Matrix &m);
	
	// equals method
	bool equals(Matrix &m);


private:

	// instance variables
	int rows;
	int cols;
	
	//the matrix (notice it's a pointer)
	//this is so it can be allocated memory
	//also note:
	// a 2d matrix can be seen as 1 big 1d array!!
	int *matrix;

}; //need semicolon!
