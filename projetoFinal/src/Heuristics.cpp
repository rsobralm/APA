#include "Heuristics.h"

void construction(Data *data, Solution *s)
{
    s->objValue = 0;
    s->startTime = vector<vector<double>>(data->numJobs, vector<double>(data->numMachines, -1));
    s->schedule = vector<vector<int>>(data->numMachines);
    s->duration = vector<double>(data->numMachines, 0);
    s->stageSeq = vector<pair<int, int>>(data->numJobs * data->numMachines);

    vector<pair<int, vector<int>>> availableJobs(data->numMachines, {0, vector<int>(data->numJobs)});

    for (int j = 0; j < data->numMachines; j++)
    {
        availableJobs[j].first = j;
        for (int i = 0; i < data->numJobs; i++)
            availableJobs[j].second[i] = i;
    }

    vector<double> earliest(data->numJobs, 0); // o menor tempo no qual o job i pode começar

    int count = 0;
    while (!availableJobs.empty())
    {
        int best_i, best_j;
        double bestObjValue = numeric_limits<double>::infinity();
        for (int j = 0; j < availableJobs.size(); j++)
        {
            for (int i = 0; i < availableJobs[j].second.size(); i++)
            {
                int machine = availableJobs[j].first, job = availableJobs[j].second[i];
                double startTime = max(s->duration[machine], earliest[job]);
                double duration = startTime + data->processingTime[job][machine];
                double objValue = max(s->objValue, duration);

                if (objValue < bestObjValue)
                {
                    best_i = i;
                    best_j = j;
                    bestObjValue = objValue;
                }
            }
        }

        int i = best_i, j = best_j;
        int machine = availableJobs[j].first, job = availableJobs[j].second[i];
        s->startTime[job][machine] = max(s->duration[machine], earliest[job]);
        s->duration[machine] = earliest[job] = s->startTime[job][machine] + data->processingTime[job][machine];

        s->objValue = max(s->objValue, s->duration[machine]);

        s->schedule[machine].push_back(job);

        swap(availableJobs[j].second[i], availableJobs[j].second.back());
        availableJobs[j].second.pop_back();
        if (availableJobs[j].second.empty())
        {
            availableJobs[j].second.swap(availableJobs.back().second);
            swap(availableJobs[j].first, availableJobs.back().first);
            availableJobs.pop_back();
        }

        s->stageSeq[count] = {job, machine};
        count++;
    }
}

bool movSwap(Data *data, Solution *s)
{
    int numStages = data->numJobs * data->numMachines;
    double bestObj = s->objValue;
    int best_i, best_j;
    bool improved = false;

    for (int i = 0; i < numStages; i++)
    {
        for (int j = i + 1; j < numStages; j++)
        {
            swap(s->stageSeq[i], s->stageSeq[j]);
            calculateObjValue(data, s);
            if (s->objValue < bestObj)
            {
                best_i = i;
                best_j = j;
                bestObj = s->objValue;
                improved = true;
            }
            swap(s->stageSeq[i], s->stageSeq[j]);
        }
    }

    if (improved)
        swap(s->stageSeq[best_i], s->stageSeq[best_j]);

    calculateObjValue(data, s);
    // cout << "Checando... \n";
    // checkSolution(data, s);

    return improved;
}