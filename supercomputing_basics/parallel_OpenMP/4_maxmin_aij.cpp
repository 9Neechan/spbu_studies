//#include <chrono> 
#include <bits/stdc++.h>

using namespace std;

int min_in_v(vector<int> v, int num_thr) { 
    int min = INT_MAX; 
#pragma omp parallel for reduction(min:min) num_threads(num_thr)
    for (int i = 0; i < v.size(); ++i) { 
        if (v[i] < min) min = v[i];
    } 
    return min; 
} 

int max_in_v(vector<int> v, int num_thr) { 
    int max = INT_MIN; 
#pragma omp parallel for reduction(max:max) num_threads(num_thr)
    for (int i = 0; i < v.size(); ++i) { 
        if (v[i] > max) max = v[i];
    } 
    return max; 
} 

int parallel(vector<vector<int>> v, int num_thr, int N) { 
    vector<int> v_min(N);

    #pragma omp parallel for num_threads(num_thr)
    
        for (int i = 0; i < N; i++) {
            v_min[i] = min_in_v(v[i], num_thr);
        }
        #pragma omp barrier
    
    int ans = max_in_v(v_min, num_thr);
    return ans;
} 

int main() { 
    ofstream myfile;
    myfile.open ("./tables/table4.csv");
    myfile << "1_thr,2_thr,3_thr,4_thr,5_thr,6_thr,7_thr,8_thr,\n";

    vector<string> s(10);

    for (int k = 1; k <= 8; k++) {
        int N = 1000;

        cout << "Кол-во потоков: " << k << endl;

        for (int j = 0; j < 10; j++) {
            vector<vector<int> > v(N, vector<int>(N));
    
            for (int i = 0; i < N; i++) {
                for (int m = 0; m < N; m++) {
                    v[i][m] = rand() % 101 - 100;
                }
            }
            
            auto start_time = chrono::high_resolution_clock::now(); 
            int result_parallel = parallel(v, k, N); 
            auto end_time = chrono::high_resolution_clock::now(); 
            chrono::duration<double> parallel_duration = end_time - start_time; 

            cout << "N = " << N; // << ". Min = " << result_parallel << endl; 
            cout << " Duration: " << parallel_duration.count() << " seconds" << endl; 
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

