#include "Solution.h"

void print(Solution s)
{
    for (int i = 0; i < s.schedule.size(); i++)
    {
        for (int j = 0; j < s.schedule[i].size(); j++)
        {
            cout << s.schedule[i][j] << " ";
        }
        cout << endl;
    }
}


void calculateObjValue(Data *data, Solution *s)
{
    vector<double> earliest(data->numJobs, 0); // o menor tempo no qual o job i pode começar
    s->duration = vector<double>(data->numMachines, 0);
    s->objValue = 0;

    int count = 0;
    int tamSeq = data->numJobs * data->numMachines;

    for (int count = 0; count < tamSeq; count++)
    {
        int machine = s->stageSeq[count].second, job = s->stageSeq[count].first;

        s->startTime[job][machine] = max(s->duration[machine], earliest[job]);
        s->duration[machine] = earliest[job] = s->startTime[job][machine] + data->processingTime[job][machine];

        s->objValue = max(s->objValue, s->duration[machine]);

    }
}


bool checkSolution(Data *data, Solution *s)
{
    // checando se uma maquina processa dois jobs ao mesmo tempo
    for (int k = 0; k < data->numMachines; k++)
    {
        for (int i = 0; i < data->numJobs; i++)
        {
            for (int j = 0; j < data->numJobs; j++)
            {
                if (j == i)
                    continue;
                if (!(s->startTime[j][k] - s->startTime[i][k] >= data->processingTime[i][k] || s->startTime[i][k] - s->startTime[j][k] >= data->processingTime[j][k]))
                {
                    return false;
                }
            }
        }
    }

    // checando se um job é processado por duas maquinas ao mesmo tempo
    for (int k = 0; k < data->numMachines; k++)
    {
        for (int l = 0; l < data->numMachines; l++)
        {
            if (k == l)
                continue;
            for (int i = 0; i < data->numJobs; i++)
            {
                if (!(s->startTime[i][k] - s->startTime[i][l] >= data->processingTime[i][l] || s->startTime[i][l] - s->startTime[i][k] >= data->processingTime[i][k]))
                {
                    return false;
                }
            }
        }
    }
    // valor obj
    double objValue = 0;
    for (int k = 0; k < data->numMachines; k++)
    {
        vector<pair<double, int>> v;
        for (int i = 0; i < data->numJobs; i++)
        {
            v.push_back({s->startTime[i][k], i});
        }
        sort(v.begin(), v.end());
        objValue = max(objValue, v.back().first + data->processingTime[v.back().second][k]);
    }
    if (objValue != s->objValue)
        return false;

    cout << "SOLUCAO OK!" << endl;

    return true;
}