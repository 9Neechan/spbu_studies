// Поиск минимума в векторе

#include <bits/stdc++.h>
#include <omp.h>

using namespace std;
  
int serial(vector<int> v) { 
    int min = INT_MAX; 
    for (int i = 0; i < v.size(); ++i) { 
        if (v[i] < min) min = v[i];
    } 
    return min; 
} 

// reduction - Для каждой переменной создаются локальные копии в каждом потоке.
// Над локальными копиями переменных после выполнения всех операторов параллельной области выполняется заданный оператор. 

int parallel(vector<int> v, int num_thr) { 
    int min = INT_MAX; 

    #pragma omp parallel for reduction(min:min) num_threads(num_thr)
        for (int i = 0; i < v.size(); ++i) { 
            if (v[i] < min) min = v[i];
        } 
    return min; 
} 

int parallel2(vector<int> v, int num_thr) {
    int min = INT_MAX; 
    #pragma omp parallel num_threads(num_thr)
    {
        int min_local = min;  
        #pragma omp for nowait // позволить потокам двигаться вперед, не дожидаясь, пока все они завершат соответствующий блок
        for (int i = 0; i < v.size(); i++) {        
            if (v[i] < min_local) min_local = v[i];
        }
        #pragma omp critical // ждать пока все потоки завершат свои операции до этого момента
        {
            if (min_local < min) min = min_local;
        }
    }
    return min;
}

int main() { 
    string head = "кол-во потоков,10000000,20000000,30000000,40000000,50000000,60000000,70000000,80000000,90000000,100000000,\n";
    
    ofstream myfile;
    myfile.open ("./tables/table1_reduction.csv");
    myfile << head;

    ofstream myfile2;
    myfile2.open ("./tables/table1_noreduction.csv");
    myfile2 << head;

    for (int k = 1; k <= 16; k++) { // num threads
        int N = 10000000;

        myfile << k << ",";
        myfile2 << k << ",";
        cout << k << endl;
        
        for (int j = 0; j < 10; j++) {
            vector<int> v(N);
    
            for (int i = 0; i < N; i++)
                v[i] = rand() % 101 - 100;
            
            auto start_time = chrono::high_resolution_clock::now(); 
            int result_parallel = parallel(v, k); 
            auto end_time = chrono::high_resolution_clock::now(); 
            chrono::duration<double> parallel_duration = end_time - start_time; 

            string ap = to_string(parallel_duration.count()*pow(10, 6));
            myfile << ap << ",";

            //------------------------------------------------------------------

            start_time = chrono::high_resolution_clock::now(); 
            result_parallel = parallel2(v, k); 
            end_time = chrono::high_resolution_clock::now(); 
            parallel_duration = end_time - start_time; 

            ap = to_string(parallel_duration.count()*pow(10, 6));
            myfile2 << ap << ",";

            N = N + 10000000; // len vec
        }
        myfile << "\n";
        myfile2 << "\n";
    }
    myfile.close();
    myfile2.close();

    return 0; 
}