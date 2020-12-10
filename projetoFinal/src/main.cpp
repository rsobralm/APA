#include <iostream>

#include "Data.h"
#include "Solution.h"
#include "Heuristics.h"

using namespace std;

int main(int argc, char **argv)
{
    srand(time(NULL));

    Data data;
    readData(argc, argv, &data);

    cout << data.numMachines << " " << data.numJobs << endl;

    for (int i = 0; i < data.numJobs; i++)
    {
        for (int j = 0; j < data.numMachines; j++)
        {
            cout << data.processingTime[i][j] << " ";
        }
        cout << endl;
    }

    Solution s;
    construction(&data, &s);

    cout << "Obj value: " << s.objValue << endl;
    // print(s);

    checkSolution(&data, &s);

    while(movSwap(&data, &s));

    cout << "Obj value: " << s.objValue << endl;

    return 0;
}