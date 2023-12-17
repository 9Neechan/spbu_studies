// Поиск максимального значения среди минимальных элементов строк матрицы
// c использованием и без использования вложенного параллелизма 

#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

int min_in_v(vector<int> v, int num_thr) { 
    int min = INT_MAX; 
#pragma omp parallel for shared(v) reduction(min:min) num_threads(num_thr)
    for (int i = 0; i < v.size(); ++i) { 
        if (v[i] < min) min = v[i];
    } 
    return min; 
} 

int max_in_v(vector<int> v, int num_thr) { 
    int max = INT_MIN; 
#pragma omp parallel for shared(v) reduction(max:max) num_threads(num_thr)
    for (int i = 0; i < v.size(); ++i) { 
        if (v[i] > max) max = v[i];
    } 
    return max; 
} 

int min_in_v_serial(vector<int> v) { 
    int min = INT_MAX; 
    for (int i = 0; i < v.size(); ++i) { 
        if (v[i] < min) min = v[i];
    } 
    return min; 
} 

int max_in_v_serial(vector<int> v) { 
    int max = INT_MIN; 
    for (int i = 0; i < v.size(); ++i) { 
        if (v[i] > max) max = v[i];
    } 
    return max; 
} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

int parallel(vector<vector<int>> v, int num_thr, int N) { 
    int min_max = 0;

#pragma omp parallel for shared(v) reduction(max:min_max) num_threads(num_thr) // schedule(guided, 100)
    for (int i = 0; i < N; ++i) {
        int my_min = INT_MAX;
        for (int j = 0; j < N; ++j) {
            if (v[i][j] < my_min) my_min = v[i][j];
        }
        if (min_max < my_min) min_max = my_min;
    }
    return min_max;
}


int parallel1(vector<vector<int>> v, int num_thr, int N) { 
    vector<int> v_min(N);
    int ans;

    #pragma omp parallel num_threads(num_thr)
    {
        #pragma omp for 
        for (int i = 0; i < N; i++) {
            v_min[i] = min_in_v(v[i], num_thr);
            //v_min[i] = min_in_v_serial(v[i]);
        }
        #pragma omp barrier
        ans = max_in_v(v_min, num_thr);
        //ans = max_in_v_serial(v_min);
    }
    return ans;
}

// два разных паралльлельных области
int parallel2(vector<vector<int>> v, int num_thr, int N) { 
    vector<int> v_min(N);
    int ans;

    for (int i = 0; i < N; i++) {
        v_min[i] = min_in_v(v[i], num_thr);
    }
    ans = max_in_v(v_min, num_thr);

    return ans;
}


int parallel3(vector<vector<int>> v, int num_thr, int N) { 
    vector<int> v_min(N);
    int ans;

    #pragma omp parallel num_threads(num_thr)
    {
        #pragma omp for 
        for (int i = 0; i < N; i++) {
            //v_min[i] = min_in_v(v[i], num_thr);
            v_min[i] = min_in_v_serial(v[i]);
        }
        #pragma omp barrier
        ans = max_in_v(v_min, num_thr);
        //ans = max_in_v_serial(v_min);
    }
    return ans;
}


int parallel4(vector<vector<int>> v, int num_thr, int N) { 
    vector<int> v_min(N);
    int ans;

    #pragma omp parallel num_threads(num_thr)
    {
        #pragma omp for 
        for (int i = 0; i < N; i++) {
            v_min[i] = min_in_v(v[i], num_thr);
            //v_min[i] = min_in_v_serial(v[i]);
        }
        #pragma omp barrier
        //ans = max_in_v(v_min, num_thr);
        ans = max_in_v_serial(v_min);
    }
    return ans;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() { 
    string head = "кол-во потоков,1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,\n";

    ofstream myfile;
    myfile.open ("./tables/table10_4.csv");
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
            int result_parallel = parallel4(v, k, N); 
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

