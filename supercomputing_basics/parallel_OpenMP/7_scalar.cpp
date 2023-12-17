#include <omp.h>
#include <iostream>
#include <random>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

int multiply_v(vector<int> v1, vector<int> v2) { 
    int out = 0;
    for (int i = 0; i < v1.size(); i++) { 
        out+=v1[i]*v2[i];
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
    ifstream myfile2("example.txt", std::ifstream::binary);
    myfile2.seekg(0, myfile2.end);
    int length = myfile2.tellg();
    myfile2.seekg(0, myfile2.beg);

    //vector <Vect *> vect_arr(0);    
    vector <vector<int>> v_arr(0);    
    
    int ans = 0;
    uint16_t index = 0;
    #pragma omp parallel shared(v_arr, index) num_threads(num_thr)
    {
        #pragma omp sections nowait
        {
            #pragma omp section
                for (int i = 0; i < 10; ++i){
                    vector<int> v(N);
                    v = read(myfile2, N, v);
                    v_arr.push_back(v);
                    //v_arr.back() -> Read(myfile2, N, v);
                    //cout << "readok" << endl;
                    if (i%2 == 1) index+=2;
                    #pragma omp flush(index)
                }

            #pragma omp section
                for (int i = 0; i <10; i += 2){
                    while (index < i + 2) {
                       #pragma omp flush(index)
                        //cout << index << endl;
                    }
                    //cout << (*(vect_arr[i])) * (*(vect_arr[i+1])) << endl;
                    ans = multiply_v(v_arr[i], v_arr[i+1]);
                }
                    
        } // end of sections
    } // end of parallel section
    return ans;
}

int main(){
    // https://www.openmp.org/wp-content/uploads/openmp-examples-4.0.2.pdf
    // стр. 13

    string head = "кол-во потоков,100,200,300,400,500,600,700,800,900,1000,\n";
    
    ofstream myfile;
    myfile.open ("./tables/table7.csv");
    myfile << head;

    for (int k = 1; k <= 8; k++) {
        int N = 100;

        myfile << k << ",";

        for (int j = 0; j < 10; j++) {          
            auto start_time = chrono::high_resolution_clock::now(); 
            int result_parallel = parallel7(k, N); 
            auto end_time = chrono::high_resolution_clock::now(); 
            chrono::duration<double> parallel_duration = end_time - start_time; 

            string ap = to_string(parallel_duration.count()*pow(10, 8));
            myfile << ap << ",";

            N = N + 100;
        }
        myfile << "\n";
    }
    myfile.close();
  
    return 0; 
}
