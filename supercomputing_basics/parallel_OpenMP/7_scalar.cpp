// Скаляроное произведение для последовательного набора векторов
// с использованием sections

#include <omp.h>
#include <bits/stdc++.h>

using namespace std;

int multiply_v_serial(vector<int> v1, vector<int> v2) { 
    int out = 0;
    for (int i = 0; i < v1.size(); i++) { 
        out+=v1[i]*v2[i];
    } 
    return out; 
} 

int multi_parallel(vector<int> v1, vector<int> v2) { 
    int out = 0;
#pragma omp parallel for reduction(+:out) //num_threads(num_thr)
    for (int i = 0; i < v1.size(); i++) { 
        out += v1[i]*v2[i];
    } 
    return out;  
} 

vector<int> read(ifstream &file, int N, vector<int> v){
    for (int i=0; i<N; i++) {
        string s;
        getline(file, s);
        v[i] = atoi(s.c_str());
    }
    return v;
}


int parallel7(int num_thr, int N) {
    ifstream myfile2("data.txt", std::ifstream::binary);
    myfile2.seekg(0, myfile2.end);
    int length = myfile2.tellg();
    myfile2.seekg(0, myfile2.beg);
   
    vector <vector<int>> v_arr(0);    
    
    int ans = 0;
    int index = 0;
    #pragma omp parallel shared(v_arr, index) num_threads(num_thr)
    {
        // sections - секции выполняются разными потоками параллельно
        // завершение директивы по умолчанию синхронизируется
        #pragma omp sections nowait
        {
            #pragma omp section
                for (int i = 0; i < 10; ++i){
                    vector<int> v(N);
                    v = read(myfile2, N, v);
                    v_arr.push_back(v);

                    if (i % 2 == 1) index += 2; // уведомляем, когда записаны 2 вектора
                    #pragma omp flush(index) // точка синхронизации переменных между потоками в памяти
                }

            #pragma omp section
                for (int i = 0; i < 10; i += 2){
                    while (index < i + 2) { 
                       #pragma omp flush(index) // умножаем, когда пришло уведомление
                    }
                    //ans = multiply_v_serial(v_arr[i], v_arr[i+1]);
                    ans = multi_parallel(v_arr[i], v_arr[i+1]);
                }        
        } 
    } 
    return ans;
}

int main(){
    string head = "кол-во потоков,100000,200000,300000,400000,500000,600000,700000,800000,900000,1000000,\n";
    
    ofstream myfile;
    //myfile.open ("./tables/table7_serial.csv");
    myfile.open ("./tables/table7_parallel.csv");
    myfile << head;

    for (int k = 1; k <= 12; k++) {
        int N = 100000;

        myfile << k << ",";

        for (int j = 0; j < 10; j++) {          
            auto start_time = chrono::high_resolution_clock::now(); 
            int result_parallel = parallel7(k, N); 
            auto end_time = chrono::high_resolution_clock::now(); 
            chrono::duration<double> parallel_duration = end_time - start_time; 

            string ap = to_string(parallel_duration.count()*pow(10, 8));
            myfile << ap << ",";

            N = N + 100000;
        }
        myfile << "\n";
    }
    myfile.close();
  
    return 0; 
}
