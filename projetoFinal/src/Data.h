#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

struct Data
{
    int numJobs, numMachines;
    vector<vector<double>> processingTime;
};


void readData(int , char**, Data *data);
#endif // DATA_H_INCLUDED