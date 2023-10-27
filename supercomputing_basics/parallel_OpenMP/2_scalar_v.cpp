//Скалярное произведение векторов

//#include <chrono> 
#include <bits/stdc++.h>

using namespace std;
  
int serial(vector<int> v1, vector<int> v2) { 
    int out = 0;
    for (int i = 0; i < v1.size(); i++) { 
        out+=v1[i]*v2[i];
    } 
    return out; 
} 
  
int parallel(vector<int> v1, vector<int> v2, int num_thr) { 
    int out = 0;
#pragma omp parallel for reduction(+:out) num_threads(num_thr)
    for (int i = 0; i < v1.size(); i++) { 
        out += v1[i]*v2[i];
    } 
    return out;  
} 

int main() { 
    ofstream myfile;
    myfile.open ("./tables/table2.csv");
    myfile << "1_thr,2_thr,3_thr,4_thr,5_thr,6_thr,7_thr,8_thr,\n";

    vector<string> s(10);

    for (int k = 1; k <= 8; k++) {
        int N = 1000;

        cout << "Кол-во потоков: " << k << endl;

        for (int j = 0; j < 10; j++) {
            vector<int> v1(N);
            vector<int> v2(N);
    
            for (int i = 0; i < N; i++) {
                v1[i] = rand() % 101 - 100;
                v2[i] = rand() % 101 - 100;
            }
            
            auto start_time = chrono::high_resolution_clock::now(); 
            int result_parallel = parallel(v1, v2, k); 
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


/*
#include <omp.h>
#include <vector>
 
using namespace std;
 
int main()
{
	vector<int> v = {1, 2, 3, 5, 645, 44, 7, 5, 454, 96, 78, 32, 23, 0, -2, -4, -6, -85, -9};
	
	#pragma omp parallel //num_threads(4)
	{

	}
}
*/



/*
#include <omp.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

int main(int argc, char* argv[])
{
	printf("Starting Program!\n");

	#pragma omp parallel //num_threads(4)
	{
		printf("Running on multiple threads%d\n", omp_get_thread_num());
	}

	printf("Finished!\n");

	cin.get();
	return 0;
}
*/
