//#include <chrono> 
#include <bits/stdc++.h>

using namespace std;

int min_in_v(vector<int> v, int num_thr, int l_ind, int r_ind) { 
    int min = INT_MAX; 
#pragma omp parallel for reduction(min:min) num_threads(num_thr)
    for (int i = l_ind; i < r_ind; ++i) { 
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

int parallel(vector<vector<int>> v, int num_thr, int N, string type, int l) { 
    vector<int> v_min(N);
    int counter_r = l;
    int counter_l = 0;
    int counter = 0;

    #pragma omp parallel for num_threads(num_thr) shared(counter_r, counter_l, counter)
        for (int i = 0; i < N; i++) {
            if (type == "treyg")
                v_min[i] = min_in_v(v[i], num_thr, i, N);

            if (type == "lent") {
                if (counter >= l) 
                    counter_l++;
                if (counter_r < N)
                    counter_r++;
                
                v_min[i] = min_in_v(v[i], num_thr, counter_l, counter_r);
                counter++;
            }
                

        }
        #pragma omp barrier
    
    int ans = max_in_v(v_min, num_thr);
    return ans;
} 

// сгенерировать ленточную матрицу с шагом l и размером матрицы NxN
vector<vector<int>> lent_matrix(int l, int N) {
    vector<vector<int> > v(N, vector<int>(N));
    int counter_r = l;
    int counter_l = 0;
    int counter = 0;

    for (int i = 0; i < N; i++) {
        for (int m = 0; m < N; m++) {
            if (counter >= l) 
                counter_l++;
            if (counter_r < N)
                counter_r++;
            
            if (m >= counter_l && m < counter_r) {
                v[i][m] = rand() % 101 - 100;
            } else {
                v[i][m] = 0;
            }
            counter++;
        }
    }
    return v;
}

int main() { 
    // треугольные матрицы //////////////////////////////////////////////////////////
    ofstream myfile;
    myfile.open ("./tables/table5_treyg.csv");
    myfile << "1_thr,2_thr,3_thr,4_thr,5_thr,6_thr,7_thr,8_thr,\n";

    vector<string> s(10);

    for (int k = 1; k <= 8; k++) { // k - колво потоков
        int N = 1000;

        cout << "Кол-во потоков: " << k << endl;

        for (int j = 0; j < 10; j++) { // размерность матрицы +(1000*j)
            vector<vector<int> > v(N, vector<int>(N));
    
            // заполнили треуг матрицу
            for (int i = 0; i < N; i++) {
                for (int m = 0; m < N; m++) {
                    if (m < i) {
                        v[i][m] = 0;
                    } else {
                        v[i][m] = rand() % 101 - 100;
                    } 
                }
            }
            
            auto start_time = chrono::high_resolution_clock::now(); 
            int result_parallel = parallel(v, k, N, "treyg", 0); 
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

    // ленточные матрицы ////////////////////////////////////////////////////////////
    ofstream myfile2;
    myfile2.open ("./tables/table5_lent.csv");
    myfile2 << "1_thr,2_thr,3_thr,4_thr,5_thr,6_thr,7_thr,8_thr,\n";

    vector<string> s2(10);
    int l = 100;

    for (int k = 1; k <= 8; k++) {
        int N = 1000;

        cout << "Кол-во потоков: " << k << endl;

        for (int j = 0; j < 10; j++) {
            vector<vector<int> > v(N, vector<int>(N));
            v = lent_matrix(l, N);
            
            auto start_time = chrono::high_resolution_clock::now(); 
            int result_parallel = parallel(v, k, N, "lent", l); 
            auto end_time = chrono::high_resolution_clock::now(); 
            chrono::duration<double> parallel_duration = end_time - start_time; 

            cout << "N = " << N; // << ". Min = " << result_parallel << endl; 
            cout << " Duration: " << parallel_duration.count() << " seconds" << endl; 
            cout << endl; 

            string ap = to_string(parallel_duration.count()) + ",";
            s2[j].append(ap);

            N = N + 1000;
        }
        cout << endl << endl;
    }

    for (int l = 0; l < 10; l++) {
        s2[l].append("\n");
        myfile2 << s2[l];
    }
    myfile2.close();
  
    return 0; 
}

