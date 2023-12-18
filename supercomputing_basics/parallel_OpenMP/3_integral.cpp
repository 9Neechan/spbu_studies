// Вычисление определенного интеграла методом квадратов

#include <bits/stdc++.h>
#include <omp.h>

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

#pragma omp parallel for reduction(+:S) num_threads(num_thr) 
    for(int i = 0; i < n-1; i++) {
        x = a + i * h;
        S = S + f(x);
    }
    S = h * S;
    return S;
}

int main() { 
    string head = "кол-во потоков,10000000,20000000,30000000,40000000,50000000,60000000,70000000,80000000,90000000,100000000,\n";

    ofstream myfile;
    myfile.open ("./tables/table3.csv");
    myfile << head;

    for (int k = 1; k <= 12; k++) {
        int N = 10000000;

        myfile << k << ",";
        cout << k << endl;

        for (int j = 0; j < 10; j++) {
            
            auto start_time = chrono::high_resolution_clock::now(); 
            int result_parallel = parallel(1, 5, N, k); 
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
