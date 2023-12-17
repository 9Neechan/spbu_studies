// Вычисление определенного интеграла методом квадратов

#include <bits/stdc++.h>
#include<iostream>
#include<cmath>

using namespace std;

// f(x)
float f(float x) {
    return 1+sin(x)/(1+cos(x)) ;
}

float serial(int a, int b, int n) {
    float h, S, x;
    h = (b - a) * 1.0 / n;
    S = 0;

    for(int i = 0; i < n-1; i++) {
        x = a + i * h;
        S = S + f(x);
    }
    S = h * S;
    return S;
}

float parallel(int a, int b, int n, int num_thr) {
    float h, S, x;
    h = (b - a) * 1.0 / n;
    S = 0;

#pragma omp parallel for reduction(+:S) num_threads(num_thr) // не надо # pragma omp ordered
    for(int i = 0; i < n-1; i++) {
        x = a + i * h;
        S = S + f(x);
    }
    S = h * S;
    return S;
}

int main() { 
    string head = "кол-во потоков,10000,20000,30000,40000,50000,60000,70000,80000,90000,100000,\n";

    ofstream myfile;
    myfile.open ("./tables/table3.csv");
    myfile << head;

    for (int k = 1; k <= 8; k++) {
        int N = 10000;

        myfile << k << ",";

        for (int j = 0; j < 10; j++) {
            
            auto start_time = chrono::high_resolution_clock::now(); 
            int result_parallel = parallel(1, 5, N, k); 
            auto end_time = chrono::high_resolution_clock::now(); 
            chrono::duration<double> parallel_duration = end_time - start_time; 

            string ap = to_string(parallel_duration.count()*pow(10, 6));
            myfile << ap << ",";

            N = N + 10000;
        }
        myfile << "\n";
    }
    myfile.close();
  
    return 0; 
}
