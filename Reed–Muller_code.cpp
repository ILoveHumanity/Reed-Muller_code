// Reed–Muller_code.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>

long long factorial(int n){
    return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}
long long C(int n, int k) {
    long long res = 1;
    for (int i = n - k + 1; i <= n; i++) { res *= i; }
    res /= factorial(k);
    return res;
}

int main()
{
    
    int R, M;
    std::cin >> R >> M;
    if (M <= 2 or R >= M) { return 0; } // Проверка
    //Размеры таблицы
    long long n_row = 1 + M, n_column = long long(1) << M;
    for (int i = 2; i <= R; i++) { n_row += C(M, i); };
    std::cout << n_row << ' ' << n_column << std::endl;
    std::vector <std::vector<bool>> G(n_row, std::vector<bool>(n_column));
    //G0
    for (long long j = 0; j < n_column; ++j) {
        G[0][j] = 1;
    }
    //G1
    for (long long i = 1, t1 = n_column, t2; i <= M; ++i) {
        bool a = 0;
        t1 = t1 >> 1;
        t2 = t1;
        for (long long j = 0; j < n_column; ++j) {
            if (t2) {
                G[i][j] = a;
                t2--;
            }
            else {
                t2 = t1 - 1;
                a = a ^ 1;
                G[i][j] = a;
            }
        }
    }
    //Gi
    int run_i = 0, G1_start = 1, G1_end = M + 1;
    std::vector <int> shiftsG1(n_row);
    for (int i = 0; i <= M; ++i) { shiftsG1[i] = i; }
    
    for (long long place_i = G1_end, get_i = 0; place_i < n_row; ++place_i) {
        while (run_i % G1_end == 0) {
            get_i++;
            run_i = G1_start + shiftsG1[get_i];
        }
        
        for (long long j = 0; j < n_column; ++j) {
            G[place_i][j] = G[get_i][j] & G[run_i][j];
        }
        shiftsG1[place_i] = run_i;
        
        run_i++;
    }

    //Вывод
    for (long long i = 0; i < n_row; ++i) {
        for (long long j = 0; j < n_column; ++j) {
            std::cout << G[i][j] << " ";
        }
        std::cout << "\t" << shiftsG1[i] << "\n";
    }

    std::cout << std::endl;


    //--------------------------------------- Декодирование ---------------------------------------// 
    /*
    int R, M;
    std::cin >> R >> M;
    if (M <= 2 or R >= M) { return 0; } // Проверка
    //Размеры таблицы
    long long n_row = 1 + M, n_column = long long(1) << M;
    */

    std::vector <std::vector <std::vector<bool>>> dec_G(R+1); // Матрица G
    std::vector <std::vector <int>> dec_shiftsG1(R+1); // Сдвиги для G1 при построении строк Gi (номер макс добавленной строчки)
    std::vector <std::vector <std::vector <std::vector <long long>>>> Closures(R + 1); // Замыкания


    //G0
    dec_G[0] = std::vector<std::vector<bool>>(1, std::vector<bool>(n_column, 1));
    dec_shiftsG1[0] = std::vector<int>(1, 0);
    Closures[0] = std::vector <std::vector <std::vector <long long>>>(1, std::vector <std::vector <long long>>(n_column, std::vector<long long>()));
    for (long long i = 0; i < n_column; i++) { Closures[0][0][i].push_back(i); }

    //G1
    dec_G[1] = std::vector<std::vector<bool>>(M, std::vector<bool>(n_column));
    dec_shiftsG1[1] = std::vector<int>(M);
    Closures[1] = std::vector <std::vector <std::vector <long long>>>(M, std::vector <std::vector <long long>>(n_column >> 1, std::vector<long long>()));

    for (long long i = 0, t1 = n_column, t2; i < M; ++i) {
        bool a = 0;
        t1 = t1 >> 1;
        t2 = t1;

        dec_shiftsG1[1][i] = i + 1;

        for (long long j = 0, k = 0; j < n_column; ++j) {
            if (t2) {
                dec_G[1][i][j] = a;
                if (a == 0) { 
                    Closures[1][i][k].push_back(Closures[0][0][j][0]);  
                    Closures[1][i][k].push_back(Closures[0][0][j + t1][0]);
                    k++;
                }
                t2--;
            }
            else {
                t2 = t1 - 1;
                a = a ^ 1;
                dec_G[1][i][j] = a;
                if (a == 0) {
                    Closures[1][i][k].push_back(Closures[0][0][j][0]);
                    Closures[1][i][k].push_back(Closures[0][0][j + t1][0]);
                    k++;
                }
            }

        }
    }

    //Gi
    for (int i = 2, run_i, G1_end = M; i <= R; i++) { 
        dec_G[i] = std::vector<std::vector<bool>>(C(M, i), std::vector<bool>(n_column));
        dec_shiftsG1[i] = std::vector<int>(C(M, i));
        Closures[i] = std::vector <std::vector <std::vector <long long>>>(C(M, i), std::vector <std::vector <long long>>(n_column >> i, std::vector<long long>()));

        run_i = dec_shiftsG1[i - 1][0];
        for (long long place_i = 0, get_i = 0; place_i < dec_G[i].size(); ++place_i) {
            while (run_i % G1_end == 0) {
                get_i++;
                run_i = dec_shiftsG1[i - 1][get_i];
            }

            for (long long j = 0; j < n_column; ++j) {
                dec_G[i][place_i][j] = dec_G[i - 1][get_i][j] & dec_G[1][run_i][j];
            }

            dec_shiftsG1[i][place_i] = run_i + 1;

            for (long long j = 0, t1 = n_column >> (run_i + 1), prev_j = 0; j < Closures[i][place_i].size(); ++j) {

                for (long long k = 0; k < Closures[i - 1][get_i][0].size(); ++k) {
                    Closures[i][place_i][j].push_back(Closures[i - 1][get_i][prev_j][k]);
                    Closures[i][place_i][j].push_back(Closures[i - 1][get_i][prev_j + t1][k]);
                }
                if (prev_j % t1 == t1 - 1) { prev_j += t1; }
                prev_j++;
            }
            run_i++;
        }
    };

    //Вывод
    for (int t = 0; t <= R; ++t) {
        for (long long i = 0; i < dec_G[t].size(); ++i) {
            for (long long j = 0; j < n_column; ++j) {
                std::cout << dec_G[t][i][j] << " ";
            }
            std::cout << "\t" << dec_shiftsG1[t][i] << "\n";
        }
    }
    std::cout << std::endl;
    for (int t = 0; t <= R; ++t) {
        for (long long i = 0; i < Closures[t].size(); ++i) {
            std::cout << "| ";
            for (long long j = 0; j < Closures[t][i].size(); ++j) {
                for (long long k = 0; k < Closures[t][i][j].size(); ++k) {
                    std::cout << Closures[t][i][j][k] + 1 << " ";
                }
                std::cout << "| ";
            }
            std::cout << std::endl;
        }
    }


    return 0;
}
