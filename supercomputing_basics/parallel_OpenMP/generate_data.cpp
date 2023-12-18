#include <iostream>
#include <random>
#include <fstream>
#include <omp.h>

using namespace std;

int main(){
    random_device rd;
    ofstream myfile;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(-100, 100);
    myfile.open ("data.txt");
    for (int i = 0; i < 10000000; i++) {
       myfile << dist(gen) << endl;
    }
    myfile.close();
  
    return 0; 
}
