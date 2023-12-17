// Поиск максимального значения среди минимальных элементов строк матрицы
// c использованием распараллеливания разного уровня вложенности

#include <bits/stdc++.h>

using namespace std;


int parallel1(vector<vector<int>> v, int num_thr, int N) { 
    int min_max = 0;

#pragma omp parallel for shared(v) reduction(max:min_max) num_threads(num_thr) 
    for (int i = 0; i < N; ++i) {
        int my_min = INT_MAX;
        for (int j = 0; j < N; ++j) {
            if (v[i][j] < my_min) my_min = v[i][j];
        }
        if (min_max < my_min) min_max = my_min;
    }
    return min_max;
}

int parallel2(vector<vector<int>> v, int num_thr, int N) { 
    int min_max = 0;
    int my_min = INT_MAX;

    for (int i = 0; i < N; ++i) {
        #pragma omp parallel for shared(v) reduction(max:min_max) num_threads(num_thr) 
        for (int j = 0; j < N; ++j) {
            if (v[i][j] < my_min) my_min = v[i][j];
        }
        if (min_max < my_min) min_max = my_min;
    }
    return min_max;
}

int main() { 
    string head = "кол-во потоков,1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,\n";

    ofstream myfile;
    myfile.open ("./tables/table_real10_2.csv");
    myfile << head;

    for (int k = 1; k <= 8; k++) {
        int N = 1000;

        myfile << k << ",";
        cout << k << endl;

        for (int j = 0; j < 10; j++) {
            vector<vector<int> > v(N, vector<int>(N));
    
            for (int i = 0; i < N; i++) {
                for (int m = 0; m < N; m++) {
                    v[i][m] = rand() % 101 - 100;
                }
            }
            
            auto start_time = chrono::high_resolution_clock::now(); 
            int result_parallel = parallel2(v, k, N); 
            auto end_time = chrono::high_resolution_clock::now(); 
            chrono::duration<double> parallel_duration = end_time - start_time; 

            string ap = to_string(parallel_duration.count()*pow(10, 6));
            myfile << ap << ",";

            N = N + 1000;
        }
        myfile << "\n";
    }
    myfile.close();
  
    return 0; 
}

