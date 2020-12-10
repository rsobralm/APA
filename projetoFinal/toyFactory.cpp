#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>

using namespace std;

struct Data
{
    int numJobs, numMachines;
    vector<vector<double>> processingTime;
};

struct Solution
{
    double objValue;
    vector<vector<double>> startTime; //inicio de cada job em cada máquina
    vector<vector<int>> schedule;     // sequencia de brinquedos de cada maquina
    vector<double> duration;          // tempo de termino de cada maquina
    vector<pair<int, int>> stageSeq;  // estagios (job, maquina) em ordem nao decrescente de startTime
};

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

        s->schedule[machine].push_back(job);
    }
}

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
        int j = rand() % availableJobs.size(), i = rand() % availableJobs[j].second.size();
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

    return improved;
}

void readData(int argc, char **argv, Data *data)
{

    if (argc < 2)
    {
        cout << "\nFaltando parametros\n";
        cout << " ./exec [Instancia] " << endl;
        exit(1);
    }

    if (argc > 2)
    {
        cout << "\nMuitos parametros\n";
        cout << " ./exec [Instancia] " << endl;
        exit(1);
    }

    string arquivo, ewt;

    char *instancia;
    instancia = argv[1];

    ifstream in(instancia, ios::in);

    if (!in)
    {
        cout << "arquivo nao pode ser aberto\n";
        exit(1);
    }

    while (arquivo.compare("maquinas") != 0)
    {
        in >> arquivo;
    }
    if ((arquivo.compare("maquinas") == 0))
        in >> data->numMachines;

    while (arquivo.compare("brinquedos") != 0)
    {
        in >> arquivo;
    }
    if ((arquivo.compare("brinquedos") == 0))
        in >> data->numJobs;

    while (arquivo.compare("BxM") != 0)
    {
        in >> arquivo;
    }

    data->processingTime.assign(data->numJobs, vector<double>(data->numMachines));

    for (int i = 0; i < data->numJobs; i++)
    {
        for (int j = 0; j < data->numMachines; j++)
        {
            in >> data->processingTime[i][j];
        }
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