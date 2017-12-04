#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include "TSPGenome.hh"

using namespace std;

// constructor 
TSPGenome::TSPGenome(int num){
	
	//pre-initialize circuitLength to a dummy value
	circuitLength = -1;
	
	for(int i = 0; i < num; i++){
		orderOfPoints.push_back(i);
	}
	
	//seed the random generator 
	srand(unsigned time(0));
	
	//shuffle vector (use random_shuffle())
	random_shuffle(orderOfPoints.begin(), orderOfPoints.end());
	
}

// constructor 
TSPGenome::TSPGenome(const vector<int> &order){
	
	//set circuit length to a dummy value initially. could also be a really large number, i.e. 1e9 (10^9)
	circuitLength = -1;
	
	for(unsigned int i = 0; i < order.size(); i++){
		orderOfPoints.push_back(order[i]);
	}
}

//destructor
TSPGenome::~TSPGenome(){
}

//getters
vector<int> TSPGenome::getOrder() const{
	return orderOfPoints;	
}

double TSPGenome::getCircuitLength() const{
	return circuitLength;
}

//calculate circuit length
void TSPGenome::computeCircuitLength(const vector<Point> &points){
	
	double circuitLen = 0;
	unsigned int size = orderOfPoints.size();
	
	for(unsigned int i = 0; i < size; i++){
		
		int fromIndex = orderOfPoints[i];
		int toIndex = orderOfPoints[(i + 1) % size]; //circular array indexing
		circuitLength += points[fromIndex].distanceTo(points[toIndex]);
	}
	
	this.circuitLength = circuitLen;
}

// mutate a vector of points
// swap two points randomly 
void mutate(vector<int> pointOrder){
	
	int rand1 = rand() % pointOrder.size();
	int rand2 = rand() % pointOrder.size();
    
    //ensure two random points selected	
	while(rand2 == rand1){
		rand2 = rand() % pointOrder.size();
	}
	
	int temp = pointOrder[rand1];
	pointOrder[rand1] = pointOrder[rand2];
	pointOrder[rand2] = temp;
	
}




