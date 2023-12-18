//Скалярное произведение векторов. 
// исследование режимов распараллеливания цикла for  (static, dynamic, guided): 
// при использовании цикла с неравномерной нагрузкой итераций

#include <bits/stdc++.h>
#include <omp.h>

using namespace std;
  
int def(int out, vector<int> v1, vector<int> v2, int i) {
    if (i % 3 == 0) {
        int k = rand() % 101 - 100;
        out += v1[i] * (v2[i]+k);
    } else {
        out += v1[i] * (v2[i]);
    }
    return out;
}

int parallel(vector<int> v1, vector<int> v2, int num_thr, string param1, int param2) { 
    int out = 0;

    if (param1 == "static") {
        #pragma omp parallel for schedule(static, param2) reduction(+:out) num_threads(num_thr)
        for (int i = 0; i < v1.size(); i++) {
            out = def(out, v1, v2, i);
        }
    } else if (param1 == "dynamic") {
        #pragma omp parallel for schedule(dynamic, param2) reduction(+:out) num_threads(num_thr)
        for (int i = 0; i < v1.size(); i++) {
            out = def(out, v1, v2, i);
        }
    } else {
        #pragma omp parallel for schedule(guided, param2) reduction(+:out) num_threads(num_thr)
        for (int i = 0; i < v1.size(); i++) {
            out = def(out, v1, v2, i);
        }
    }

    return out;  
} 

// static - Если вы не укажете переменную размера фрагмента, 
// OpenMP разделит итерации на фрагменты, которые примерно равны по размеру, 
// и распределит фрагменты по потокам по порядку 
// Если вы укажете переменную chunk-size, итерации будут разделены на фрагменты iter_size / chunk_size.
// если iter_size / chunk_size больше, чем потоков, то рапределит по потокам в круговом порядке

// dynamic - OpenMP по-прежнему будет разбивать задачу на фрагменты iter_size/chunk_size, 
// но распределять итерации по потокам динамически без какого-либо определенного порядка.

// Плюсы: тип динамического планирования подходит, когда итерации требуют разных вычислительных затрат. 
// Это означает, что итерации не так сбалансированы, как статический метод, между собой. 
// Минусы: тип динамического планирования имеет более высокие накладные расходы, чем статический тип планирования, 
// поскольку он динамически распределяет итерации во время выполнения.

// guided - Размер chunk является динамическим
// размер chunk пропорционален количеству неназначенных итераций, деленных на количество потоков, 
// и размер будет уменьшен до chunk-size (но последний chunk может быть меньше chunk-size)

int main() { 
    string head = "кол-во потоков,static-1,static-300,static-500,static-1000,dynamic-1,dynamic-300,dynamic-500,dynamic-1000,guided-1,guided-300,guided-500,guided-1000,\n";
    vector<string> params1 {"static", "dynamic", "guided"}; 
    vector<int> params2 {1, 300, 500, 1000};
    
    ofstream myfile;
    myfile.open ("./tables/table11.csv");
    myfile << head;

    int N = 20000;
    vector<int> v1(N);
    vector<int> v2(N);

    for (int i = 0; i < N; i++) {
        v1[i] = rand() % 101 - 100;
        v2[i] = rand() % 101 - 100;
    }

    for (int k = 1; k <= 12; k++) {
        myfile << k << ",";
        cout << k << endl;

        for (int j = 0; j < 3; j++) {
            for (int m = 0; m < 4; m++) {
                auto start_time = chrono::high_resolution_clock::now(); 
                int result_parallel = parallel(v1, v2, k, params1[j], params2[m]); 
                auto end_time = chrono::high_resolution_clock::now(); 
                chrono::duration<double> parallel_duration = end_time - start_time; 

                string ap = to_string(parallel_duration.count()*pow(10, 6));
                myfile << ap << ",";
            }
        }
        myfile << "\n";
    }
    myfile.close();
  
    return 0; 
}
