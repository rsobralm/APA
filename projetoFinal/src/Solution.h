#ifndef SOLUTION_H_INCLUDED
#define SOLUTION_H_INCLUDED

#include <algorithm>
#include "Data.h"

using namespace std;

struct Solution
{
    double objValue;
    vector<vector<double>> startTime; //inicio de cada job em cada máquina
    vector<vector<int>> schedule;     // sequencia de brinquedos de cada maquina
    vector<double> duration;          // tempo de termino de cada maquina
    vector<pair<int, int>> stageSeq;  // estagios (job, maquina) em ordem nao decrescente de startTime
};

void print(Solution s);
void calculateObjValue(Data *data, Solution *s);
bool checkSolution(Data *data, Solution *s);


#endif 