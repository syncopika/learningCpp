#include <chrono>
#include <iostream>
#include <list>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <vector>

#define SIZE 9000
#define NUM_TEST_RUNS 10

// trying to compare linked lists against vectors and regular arrays
// I guess it's a bit of a no-brainer that indexing (and traversal?) in arrays is faster
// but would be nice to have real data to prove it
// I'm interested in testing lookup and traversal speed

void prepDataStructures(int* array, std::vector<int>& vec, std::list<int>& list){
    for(int i = 0; i < SIZE; i++){
        array[i] = i;
        vec.push_back(i);
        list.push_back(i);
    }
}

template <typename T>
long sum(const T& thing){
    long s = 0;
    if(std::is_same<T, std::vector<int>>::value || std::is_same<T, std::list<int>>::value){
        // sum over vector or list
        for(int x : thing){
            s += x;
        }
    }else{
        throw std::invalid_argument("argument not std::vector or std::list");
    }
    return s;
}

double getTimeDiff(
    std::chrono::time_point<std::chrono::high_resolution_clock> start,
    std::chrono::time_point<std::chrono::high_resolution_clock> stop
){
    std::chrono::duration<double> diff = stop - start;
    return diff.count();
}

std::tuple<double, double, double> testRunSum(){
    int array[SIZE];
    std::vector<int> vector;
    std::list<int> list;
    
    prepDataStructures(array, vector, list);
    
    // test vector
    auto start = std::chrono::high_resolution_clock::now();
    long r1 = sum(vector);
    auto stop = std::chrono::high_resolution_clock::now();
    double res1 = getTimeDiff(start, stop);
    std::cout << "time for sum (" << r1 << ") over std::vector: " << res1 << " s. \n";
    
    // test linked list
    start = std::chrono::high_resolution_clock::now();
    long r2 = sum(list);
    stop = std::chrono::high_resolution_clock::now();
    double res2 = getTimeDiff(start, stop);
    std::cout << "time for sum (" << r2 << ") over std::list: " << res2 << " s. \n";
    
    // test regular array
    long arraySum = 0;
    start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < SIZE; i++){
        arraySum += array[i];
    }
    stop = std::chrono::high_resolution_clock::now();
    double res3 = getTimeDiff(start, stop);
    std::cout << "time for sum (" << arraySum << ") over array: " << res3 << " s. \n";

    return std::tuple<double, double, double>{res1, res2, res3};
}

int main(void){
    std::cout << "testing " << SIZE << " elements.\n";
    std::tuple<double, double, double> res(0, 0, 0);
    for(int i = 0; i < NUM_TEST_RUNS; i++){
        std::cout << "TEST RUN " << i+1 << '\n';
        std::tuple<double, double, double> testRunRes = testRunSum();
        std::cout << "--------------------\n";
        
        std::get<0>(res) += std::get<0>(testRunRes);
        std::get<1>(res) += std::get<1>(testRunRes);
        std::get<2>(res) += std::get<2>(testRunRes);
    }
    std::cout << "averages --------------------\n";
    std::cout << "std::vector time: " << std::get<0>(res) / NUM_TEST_RUNS << ".\n";
    std::cout << "std::list time: " << std::get<1>(res) / NUM_TEST_RUNS << ".\n";
    std::cout << "regular array time: " << std::get<2>(res) / NUM_TEST_RUNS << ".\n";
    return 0;
}