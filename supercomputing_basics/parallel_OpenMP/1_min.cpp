// Минимум в векторе
// как сделать без педукции
// рассчитать ускорение и построить графики

#include <chrono> 
#include <bits/stdc++.h>
#include <omp.h>
#include <vector>

using namespace std;
  
int serial(vector<int> v) { 
    int min = INT_MAX; 
    for (int i = 0; i < v.size(); ++i) { 
        if (v[i] < min) min = v[i];
    } 
    return min; 
} 
  
int parallel(vector<int> v, int num_thr) { 
    int min = INT_MAX; 

    #pragma omp parallel for reduction(min:min) num_threads(num_thr)
        for (int i = 0; i < v.size(); ++i) { 
            if (v[i] < min) min = v[i];
        } 
    //printf("Running on multiple threads%d\n", omp_get_thread_num());
    return min; 
} 

int parallel2(vector<int> v, int num_thr) {
    int index = 0;
    int min = v[0];
    #pragma omp parallel
    {
        int index_local = index;
        float min_local = min;  
        #pragma omp for nowait
        for (int i = 1; i < v.size(); i++) {        
            if (v[i] < min_local) {
                min_local = v[i];
                index_local = i;
            }
        }
        #pragma omp critical 
        {
            if (min_local < min) {
                min = min_local;
                index = index_local;
            }
        }
    }
    return min;
}

int main() { 
    ofstream myfile;
    myfile.open ("./tables/table1.csv");
    myfile << "1_thr,2_thr,3_thr,4_thr,5_thr,6_thr,7_thr,8_thr,\n";

    vector<string> s(10);

    for (int k = 1; k <= 8; k++) { // num threads
        int N = 10000000;

        cout << "Кол-во потоков: " << k << endl;

        for (int j = 0; j < 10; j++) {
            vector<int> v(N);
    
            for (int i = 0; i < N; i++)
                v[i] = rand() % 101 - 100;
            
            auto start_time = chrono::high_resolution_clock::now(); 
            int result_parallel = parallel2(v, k); 
            auto end_time = chrono::high_resolution_clock::now(); 
            chrono::duration<double> parallel_duration = end_time - start_time; 

            cout << "N = " << N; // << ". Min = " << result_parallel << endl; 
            cout << " Duration: " << parallel_duration.count() << " seconds" << endl; 
            cout << endl; 

            string ap = to_string(parallel_duration.count()) + ",";
            s[j].append(ap);

            N = N + 1000; // len vec
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
    

	/*
    auto start_time = std::chrono::high_resolution_clock::now(); 
  
	// !!!
    int result_serial = serial(v); 
  
    auto end_time = std::chrono::high_resolution_clock::now(); 
  
    std::chrono::duration<double> serial_duration = end_time - start_time; 

    start_time = std::chrono::high_resolution_clock::now(); 
  
  	// !!!
    int result_parallel = parallel(v); 
    
	end_time = std::chrono::high_resolution_clock::now(); 
    std::chrono::duration<double> parallel_duration = end_time - start_time; 
  
    std::cout << "Serial result: " << result_serial << std::endl; 
    std::cout << "Parallel result: " << result_parallel << std::endl; 
    std::cout << "Serial duration: " << serial_duration.count() << " seconds" << std::endl; 
    std::cout << "Parallel duration: " << parallel_duration.count() << " seconds" << std::endl; 
    std::cout << "Speedup: " << serial_duration.count() << parallel_duration.count()  << std::endl; 
    */
  


