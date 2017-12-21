#include <iostream>
#include <vector>
#include <cstdlib>
#include <set>
#include <algorithm>
#include "tsp-ga.hh"

using namespace std;

// constructor 
TSPGenome::TSPGenome(int num){
	
	//pre-initialize circuitLength to a dummy value
	circuitLength = -1;
	
	for(int i = 0; i < num; i++){
		orderOfPoints.push_back(i);
	}
	
	//shuffle vector
	random_shuffle(orderOfPoints.begin(), orderOfPoints.end());
	
}

// constructor 
TSPGenome::TSPGenome(const vector<int> &order){
	
	//set circuit length to a dummy value initially. could also be a really large number, i.e. 1e9 (10^9)
	circuitLength = -1;
	
	int size = (int)order.size();
	
	for(int i = 0; i < size; i++){
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
	
	double length = 0;
	
	int size = (int)orderOfPoints.size(); // length of order of points to traverse in 
	
	for(int i = 0; i < size; i++){	
		int fromIndex = orderOfPoints[i];
		int toIndex = orderOfPoints[(i + 1) % size]; //circular array indexing
		length += points[fromIndex].distanceTo(points[toIndex]);
	}

	circuitLength = length;
}

// mutate a vector of points
// swap two points randomly 
void TSPGenome::mutate(){
	
	int size = (int)orderOfPoints.size();
	int rand1 = rand() % size;
	int rand2 = rand() % size;
    
    //ensure two random points selected	
	while(rand2 == rand1){
		rand2 = rand() % size;
	}
	
	int temp = orderOfPoints[rand1];
	orderOfPoints[rand1] = orderOfPoints[rand2];
	orderOfPoints[rand2] = temp;
	
}

// crosslink function. note that it's not a member function of the TSPGenome class!
TSPGenome crosslink(const TSPGenome &g1, const TSPGenome &g2){
	
	vector<int> offspringOrder;
	
	// keep a set to put integers in so we can prevent adding duplicate ints to offspringOrder
	set<int> s;
	
	// select a random index from the first genome's order list 
	int randomIndex = rand() % (int)g1.getOrder().size();
	
	// then copy all the elements from indices 0 to randomIndex - 1 in the first genome into offspringOrder 
	for(int i = 0; i < randomIndex - 1; i++){
		offspringOrder.push_back(g1.getOrder()[i]);
		s.insert(g1.getOrder()[i]);
	}
	
	// then copy all of the elements not already in the set from the second genome to offspring 
	for(int j = 0; j < (int)g2.getOrder().size(); j++){
		if(s.count(g2.getOrder()[j]) == 0){
			// since this element is not in the set yet, add it 
			offspringOrder.push_back(g2.getOrder()[j]);
			s.insert(g2.getOrder()[j]);
		}
	}
	
	// use the now-completed offspringOrder vector to create a new genome 
	return TSPGenome(offspringOrder);
}

// helper function to pass to std::sort in order to sort genomes by their circuit length 
bool isShorterPath(const TSPGenome &g1, const TSPGenome &g2){
	
	return g1.getCircuitLength() < g2.getCircuitLength();
	
}

// find a short path (might not be the shortest, but still good)
TSPGenome findAShortPath(const vector<Point> &points, 
                         int populationSize, 
						 int numGenerations, 
						 int keepPopulation, 
						 int numMutations){
							 
	// first generate an initial population of random genomes based on populationSize 
	// that is, create a vector of TSPGenomes, with each genome having the size equal to number of points
	// genome is a randomized list of ints representing some order of points to traverse 
	vector<TSPGenome> population;
	int genomeSize = (int)points.size(); 
	
	for(int i = 0; i < populationSize; i++){
		population.push_back(TSPGenome(genomeSize));
	}

	// keep a counter because the population needs to be iterated over based on numGenerations
	int counter = 0;
	
	while(counter < numGenerations){
		
		// calculate the circuitLength for each genome 
		for(int j = 0; j < populationSize; j++){
			population[j].computeCircuitLength(points);
		}
	
		sort(population.begin(), population.end(), isShorterPath);
		
		// keep track of what's going on by printing some info 
		if(counter % 10 == 0){
			cout << "Generation " << counter << ": shortest path is " << population[0].getCircuitLength() << endl;
		}
		
		// keep the top N fittest members of the population, but replace the rest with new genomes from the fittest 
		// the number of genomes to keep (N) is the keepPopulation arg 
		for(int k = keepPopulation; k < populationSize; k++){
			
			// select a random, but FIT genome 
			int randomFitGenomeIndex = rand() % keepPopulation;
			TSPGenome g1 = population[randomFitGenomeIndex];
			
			// select another random, fit genome to crosslink g1 with 
			// make sure it's not the same one! 
			int randomFitGenomeIndex2 = rand() % keepPopulation;
			while(randomFitGenomeIndex2 == randomFitGenomeIndex){
				randomFitGenomeIndex2 = rand() % keepPopulation;
			}
			TSPGenome g2 = population[randomFitGenomeIndex2];
			
			// replace the current genome with a more fit one 
			population[k] = crosslink(g1, g2);
		}
		
		// then mutate a random genome in the population (numMutations decides how many genomes to mutate)
		// don't mutate the genome at the 0th index since it's the most fit one 
		for(int l = 0; l < numMutations; l++){
			int randIndex = 1 + rand() % (populationSize - 1); // don't include index 0 
			population[randIndex].mutate();
		}
		
		counter++;
	}		 
	
	return population[0];
	
}




