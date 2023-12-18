// Поиск максимального значения среди минимальных элементов строк матрицы

#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

int parallel4(vector<vector<int>> v, int num_thr, int N) { 
    int min_max = 0;

#pragma omp parallel for shared(v) reduction(max:min_max) num_threads(num_thr)
    for (int i = 0; i < N; ++i) {
        int my_min = INT_MAX;
        for (int j = 0; j < N; ++j) {
            if (v[i][j] < my_min){
                my_min = v[i][j];
            }
        }
        if (min_max < my_min){
            min_max = my_min;
        }
    }

    return min_max;
}

vector<int> read(ifstream &file, int N, vector<int> v){
    for (int i=0; i<N; i++) {
        string s;
        getline(file, s);
        v[i] = atoi(s.c_str());
    }
    return v;
}

int main() { 
    /*
    ifstream myfile2("./tables/data.txt", std::ifstream::binary);
    myfile2.seekg(0, myfile2.end);
    int length = myfile2.tellg();
    myfile2.seekg(0, myfile2.beg);*/

    string head = "кол-во потоков,1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,\n";

    ofstream myfile;
    myfile.open ("./tables/table4.csv");
    myfile << head;

    for (int k = 1; k <= 8; k++) {
        int N = 1000;

        myfile << k << ",";
        cout << k << endl;

        for (int j = 0; j < 10; j++) {
            vector<vector<int>> v(N, vector<int>(N));

            for (int i = 0; i < N; i++) {
                for (int m = 0; m < N; m++) {
                    v[i][m] = rand() % 101 - 100;
                }
            }
            /*
            vector <vector<int>> v_arr(0);   
    
            for (int i = 0; i < N; i++) {
                vector<int> v(N);
                v = read(myfile2, N, v);
                v_arr.push_back(v);
            }
            cout << "=" << endl;
            */
            
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

