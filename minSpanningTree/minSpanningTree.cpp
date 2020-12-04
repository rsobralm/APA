#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits>
#include <sys/timeb.h>
#include <sys/resource.h>

using namespace std;

double CalcDistEuc ( double *X, double *Y, int I, int J )
{
    return
    sqrt ( pow ( X[I] - X[J], 2 ) + pow ( Y[I] - Y[J], 2 ) );
}

void readData( int argc, char** argv, int* Dimension, double ***Mdist ){

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

    int N;
    string arquivo, ewt;

     char *instancia;
     instancia = argv[1];


    ifstream in( instancia, ios::in);

	if (!in) {
		cout << "arquivo nao pode ser aberto\n";
		exit(1);
    }

    while ( arquivo.compare("DIMENSION:") != 0 && arquivo.compare("DIMENSION" ) != 0 ) {
        in >> arquivo;
    }

    if ( arquivo.compare("DIMENSION" ) == 0 )  in >> arquivo;

    in >> N;
    //N = 4;

    double *x = new double [N+1];
    double *y = new double [N+1];

    // Alocar matriz 2D
    double **dist = new double*[N+1];

    for ( int i = 0; i < N+1; i++ ) {
        dist [i] = new double [N+1];
    }

    /*double distancia[4][4] = {{0,10,6,5},
    {10,0,10000,15},
    {6,10000,0,4},
    {5,15,4,0},
    };*/


    while ( arquivo.compare("DISPLAY_DATA_SECTION") != 0 ) {
        in >> arquivo;
    }
    // ler coordenadas
    int tempCity;
    for ( int i = 1; i < N+1; i++ ) {
        in >> tempCity >> x[i] >> y[i];
    }

    // Calcular Matriz Distancia (Euclidiana)
    for ( int i = 1; i < N+1; i++ ) {
        for ( int j = 1; j < N+1; j++ ) {
            dist[i][j] = floor ( CalcDistEuc ( x, y, i, j ) + 0.5 );
            //dist[i][j] = distancia[i-1][j-1];
        }
    }

    *Dimension = N;
    *Mdist = dist;
    

}


int dimension;
double ** matrizAdj; // matriz de adjacencia

/*double **matrizAdj = {
    {0,10,6,5},
    {10,0,10000,15},
    {6,10000,0,4},
    {5,15,4,0},
    };
int dimension = 0;*/

void printData() {
  cout << "dimension: " << dimension << endl;
  for (size_t i = 1; i <= dimension; i++) {
    for (size_t j = 1; j <= dimension; j++) {
      cout << matrizAdj[i][j] << " ";
    }
    cout << endl;
  }
}


struct Edge
{
    int u, v;
    double cost;
};

vector<Edge> edges;
vector<int> set;

void initializeEdges(){
    for (int i = 1; i <= dimension; i++) {
        for (int j = i+1; j <= dimension; j++) {
            edges.push_back((Edge){i, j, matrizAdj[i][j]});
        }
    }
}

void maxHeapify(vector<Edge> &edges, int n,  int i){
     int left = 2 * i + 1;
     int right = 2 * i + 2;
     int largest = i;

    if(left < n && edges[left].cost > edges[largest].cost)
        largest = left;
    if(right < n && edges[right].cost > edges[largest].cost)
        largest = right;
    if(largest != i){
        swap(edges[i], edges[largest]);
        maxHeapify(edges, n ,largest);
    }
}

void BuildMaxHeap(vector<Edge> &edges){
    int n = edges.size();
    for(int i = n/2 - 1; i >= 0; i--){
        maxHeapify(edges, n, i);
    }
}

void heapSort(vector<Edge> &edges){
    int heapSize = edges.size();
    BuildMaxHeap(edges);
    for(int i = heapSize - 1; i > 0; i--){
        swap(edges[0], edges[i]);
        maxHeapify(edges, i, 0);
    }
}

void countingSort(vector<Edge> &A, int n){

    vector<Edge> B(n);
    vector<int> C;
    int max = A[0].cost;

    for (int i = 1; i < n; i++) {
        if (A[i].cost > max)
            max = A[i].cost;
    }

    for (int i = 0; i <= n; ++i) {
        C.push_back(0);
    }

    for (int i = 0; i < n; i++) {
        C[A[i].cost]++;
    }

    for (int i = 1; i <= n; i++) {
        C[i] += C[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        B[C[A[i].cost] - 1] = A[i];
        C[A[i].cost]--;
    }

    for (int i = 0; i < n; i++) {
        A[i] = B[i];
    }
}



void makeSet(int v){
    set[v] = v; // indice representa o vertice, valor o conjunto
}

 int findSet(int v){
    if(v == set[v]){
        return v;
    }
    set[v] = findSet(set[v]);
    return set[v];
}

void unionSet(int u,  int v){
    set[v] = u;
}

double MST_Kruskal(){
    
    vector<Edge> s;
    double cost = 0;
    set.resize(dimension);

    for(int i = 1; i <= dimension; i++){ // cria conjunto para cada vértice
        makeSet(i);
    }

    heapSort(edges); // ordena as arestas pelo custo
    //countingSort(edges, edges.size());

    for(Edge e: edges){
        int u = findSet(e.u); // verifica em qual conjunto o vertice está
        int v = findSet(e.v);
        if(u != v){
            cost += e.cost; // computa custo da aresta adicionada
            s.push_back(e); // adiciona aresta a solução
            unionSet(u, v); // une os conjuntos disjuntos
        }
    }

    //cout << "cost: " << cost << endl;
    return cost;

}

double cpuTime() {
	static struct rusage usage;
	getrusage(RUSAGE_SELF, &usage);
	return ((double)usage.ru_utime.tv_sec)+(((double)usage.ru_utime.tv_usec)/((double)1000000));
}

int main(int argc, char** argv){

    double averageExecTime = 0;
    double averageSolution = 0;


    readData(argc, argv, &dimension, &matrizAdj);
    //printData();
    initializeEdges();
    for(unsigned int i = 0; i < 10; i++){
        double init = cpuTime();
        //cout << "edges: " << edges.size();
        //coutingSort(edges, edges.size());
        //heapSort(edges);
        averageSolution += MST_Kruskal();
        double end = cpuTime();
        //cout << "time: " << end - init << endl;
        averageExecTime += (end - init);
    }
    cout << "Average Solution: " << averageSolution/10 << endl;
    cout << "Average Execution Time (s): " << averageExecTime/10 << endl;
    return 0;
}