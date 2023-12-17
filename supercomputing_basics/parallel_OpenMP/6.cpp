// Реализация редукции с использованием разных способв организации взаимоисключения
// atomic, critical, lock

#include <bits/stdc++.h>
#include <omp.h>
#include <stdint.h>

using namespace std;

int parallel_l(vector<int> v, int num_thr, int N){
    omp_lock_t lock;
    omp_init_lock(&lock);
    int sum = 0;

    #pragma omp parallel for shared(v, lock) num_threads(num_thr) reduction(+:sum) 
    for (int i = 0; i < N; i++) {
        omp_set_lock(&lock);
        sum += v[i]; // только один поток одновременно может выполнить это
        omp_unset_lock(&lock);
    }
    
    omp_destroy_lock (&lock);
    return sum;
}

int parallel_a(vector<int> v, int num_thr, int N){
    int sum = 0;

    #pragma omp parallel for shared(v) num_threads(num_thr) reduction(+:sum) 
    for (int i = 0; i < N; i++) {
        #pragma omp atomic
            sum += v[i];
    }
    return sum;
}

int parallel_c(vector<int> v, int num_thr, int N){
    int sum = 0;

    #pragma omp parallel for shared(v) num_threads(num_thr) reduction(+:sum) 
    for (int i = 0; i < N; i++) {
        #pragma omp critical
            sum += v[i];
    }
    return sum;
}

// atomic - определяет переменную, доступ к которй должен быть выполнен как неделимая операция

// critical - определяет фрагмент кода, который должен выполянться только одним потоком в каждый текущий момент

int main() { 
    string head = "кол-во потоков,100000,200000,300000,400000,500000,600000,700000,800000,900000,1000000,\n";

    ofstream myfile;
    myfile.open ("./tables/table6_a.csv");
    myfile << head;

    ofstream myfile2;
    myfile2.open ("./tables/table6_c.csv");
    myfile2 << head;

    ofstream myfile3;
    myfile3.open ("./tables/table6_l.csv");
    myfile3 << head;

    for (int k = 1; k <= 8; k++) {
        int N = 100000;

        myfile << k << ",";
        myfile2 << k << ",";
        myfile3 << k << ",";
        cout << k << endl;

        for (int j = 0; j < 10; j++) {
            vector<int > v(N);
    
            for (int i = 0; i < N; i++) {
                v[i] = rand() % 101 - 100; //1 //dist(gen);   
            }
            
            auto start_time = chrono::high_resolution_clock::now(); 
            int result_parallel = parallel_a(v, k, N); 
            auto end_time = chrono::high_resolution_clock::now(); 
            chrono::duration<double> parallel_duration = end_time - start_time; 

            string ap = to_string(parallel_duration.count()*pow(10, 6));
            myfile << ap << ",";


            start_time = chrono::high_resolution_clock::now(); 
            result_parallel = parallel_c(v, k, N); 
            end_time = chrono::high_resolution_clock::now(); 
            parallel_duration = end_time - start_time; 

            ap = to_string(parallel_duration.count()*pow(10, 6));
            myfile2 << ap << ",";


            start_time = chrono::high_resolution_clock::now(); 
            result_parallel = parallel_l(v, k, N); 
            end_time = chrono::high_resolution_clock::now(); 
            parallel_duration = end_time - start_time; 

            ap = to_string(parallel_duration.count()*pow(10, 6));
            myfile3 << ap << ",";


            N = N + 100000;
        }
        myfile << "\n";
        myfile2 << "\n";
        myfile3 << "\n";
    }
    myfile.close();
    myfile2.close();
    myfile3.close();
  
    return 0; 
}