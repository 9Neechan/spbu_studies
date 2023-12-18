//Скалярное произведение векторов

#include <bits/stdc++.h>
#include <omp.h>

using namespace std;
  
int serial(vector<int> v1, vector<int> v2) { 
    int out = 0;
    for (int i = 0; i < v1.size(); i++) { 
        out+=v1[i]*v2[i];
    } 
    return out; 
} 
  
int parallel(vector<int> v1, vector<int> v2, int num_thr) { 
    int out = 0;
#pragma omp parallel for reduction(+:out) num_threads(num_thr)
    for (int i = 0; i < v1.size(); i++) { 
        out += v1[i]*v2[i];
    } 
    return out;  
} 

int main() { 
    string head = "кол-во потоков,10000000,20000000,3000000,40000000,50000000,60000000,70000000,80000000,90000000,100000000,\n";
    
    ofstream myfile;
    myfile.open ("./tables/table2.csv");
    myfile << head;

    for (int k = 1; k <= 16; k++) {
        int N = 10000000;

        myfile << k << ",";
        cout << k << endl;

        for (int j = 0; j < 10; j++) {
            vector<int> v1(N);
            vector<int> v2(N);
    
            for (int i = 0; i < N; i++) {
                v1[i] = rand() % 101 - 100;
                v2[i] = rand() % 101 - 100;
            }
            
            auto start_time = chrono::high_resolution_clock::now(); 
            int result_parallel = parallel(v1, v2, k); 
            auto end_time = chrono::high_resolution_clock::now(); 
            chrono::duration<double> parallel_duration = end_time - start_time; 

            string ap = to_string(parallel_duration.count()*pow(10, 6));
            myfile << ap << ",";

            N = N + 10000000;
        }
        myfile << "\n";
    }
    myfile.close();
  
    return 0; 
}
