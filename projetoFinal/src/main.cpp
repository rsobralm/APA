#include <iostream>
#include <chrono>

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

    // // cout << data.numMachines << " " << data.numJobs << endl;

    // for (int i = 0; i < data.numJobs; i++)
    // {
    //     for (int j = 0; j < data.numMachines; j++)
    //     {
    //         // cout << data.processingTime[i][j] << " ";
    //     }
    //     // cout << endl;
    // }

    Solution s;
    construction(&data, &s);    
    int constructionSol = s.objValue;

    auto end = chrono::system_clock::now();
    chrono::duration<double> elapsed = end - start;
    timeConstruction = elapsed.count();
 
    // cout << "Construction Obj value: " << constructionSol << endl;
    // cout << "Construction time: " << timeConstruction << endl;
    // print(s);

    // checkSolution(&data, &s);

    while(movSwap(&data, &s));
    end = chrono::system_clock::now();
    elapsed = end -  start;
    timeNeighborhood = elapsed.count();

    // cout << "Neighborhood Obj value: " << s.objValue << endl;
    // cout << "Neighborhood time: " << timeNeighborhood << endl;
    // checkSolution(&data, &s);

    //latex
    cout << "instancia & " << constructionSol << " & " << timeConstruction << " & " << s.objValue << " & " << timeNeighborhood << " \\" << endl;


    return 0;
}

