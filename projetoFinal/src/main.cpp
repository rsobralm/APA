#include <iostream>
#include <chrono>
#include <limits>

#include "Data.h"
#include "Solution.h"
#include "Heuristics.h"

using namespace std;

int main(int argc, char **argv)
{
    srand(time(NULL));
    double timeConstruction = 0;
    double timeNeighborhood = 0;

    auto start = chrono::system_clock::now();
    Data data;
    readData(argc, argv, &data);

    Solution s;

    construction(&data, &s);
    int constructionSol = s.objValue;

    auto end = chrono::system_clock::now();
    chrono::duration<double> elapsed = end - start;
    timeConstruction = elapsed.count();

    while (movSwap(&data, &s));
        
    end = chrono::system_clock::now();
    elapsed = end - start;
    timeNeighborhood = elapsed.count();

    //latex
    cout << "instancia & " << constructionSol << " & " << timeConstruction << " & " << s.objValue << " & " << timeNeighborhood << " \\\\" << endl;

    return 0;
}
