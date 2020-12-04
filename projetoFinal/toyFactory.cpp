#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

void readData(int argc, char** argv, int* n, int* m, double ***Mdist){

    if (argc < 2) {
        cout << "\nFaltando parametros\n";
        cout << " ./exec [Instancia] "<< endl;
        exit(1);
    }

    if (argc > 2) {
        cout << "\nMuitos parametros\n";
        cout << " ./exec [Instancia] " << endl;
        exit(1);
    }

    int N, M;
    string arquivo, ewt;

     char *instancia;
     instancia = argv[1];


    ifstream in( instancia, ios::in);

	if (!in) {
		cout << "arquivo nao pode ser aberto\n";
		exit(1);
    }

    while(arquivo.compare("maquinas") != 0){
        in >> arquivo;
    }
    if ((arquivo.compare("maquinas") == 0))
        in >> M;

    while(arquivo.compare("brinquedos") != 0){
        in >> arquivo;
    }
    if ((arquivo.compare("brinquedos") == 0))
        in >> N;

    while(arquivo.compare("BxM") != 0){
        in >> arquivo;
    }

    double *x = new double [M+1];
    double *y = new double [N+1];

    double **dist = new double*[N+1];

    for ( int i = 0; i < N+1; i++ ) {
        dist [i] = new double [M+1];
    }

    for (int i = 1; i < N+1; i++){
        for(int j = 1; j < M+1; j++){
            in >> dist[i][j];
        }
    }

    *n = N;
    *m = M;
    *Mdist = dist;
}

int main(int argc, char** argv){
    double **matriz;
    int m, n;
    readData(argc, argv, &n, &m, &matriz);

    cout << m << " " << n << endl;
    
    for (int i = 1; i < n+1; i++){
        for(int j = 1; j < m+1; j++){
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}