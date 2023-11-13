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
    ofstream myfile;
    myfile.open ("./tables/table3.csv");
    myfile << "1_thr,2_thr,3_thr,4_thr,5_thr,6_thr,7_thr,8_thr,\n";

    vector<string> s(10);

    for (int k = 1; k <= 8; k++) {
        int N = 10;

        cout << "Кол-во потоков: " << k << endl;

        for (int j = 0; j < 10; j++) {
            
            auto start_time = chrono::high_resolution_clock::now(); 
            int result_parallel = parallel(1, 5, N, k); 
            auto end_time = chrono::high_resolution_clock::now(); 
            chrono::duration<double> parallel_duration = end_time - start_time; 
            //int result_serial = serial(1, 5, N);

            cout << "N = " << N; // << ". Min = " << result_parallel << endl; 
            cout << " Duration: " << parallel_duration.count() << " seconds" << endl; 
            //std::cout << "Serial result: " << result_serial << std::endl; 
            //std::cout << "Parallel result: " << result_parallel << std::endl; 
            cout << endl; 

            string ap = to_string(parallel_duration.count()) + ",";
            s[j].append(ap);

            N = N + 1000;
        }
        cout << endl << endl;
    }

    for (int l = 0; l < 10; l++) {
        s[l].append("\n");
        myfile << s[l];
    }
    myfile.close();
  
    return 0; 
}
