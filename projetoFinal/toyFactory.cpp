#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <map>

using namespace std;

struct Data
{
    int numJobs, numMachines;
    vector<vector<double>> processingTime;
};

struct Stage
{
    int job, machine;
    double startTime, endTime;
};

struct Solution
{
    double objValue;
    vector<vector<double>> startTime;
    vector<vector<int>> schedule; // sequencia de brinquedos de cada maquina
    vector<double> duration;      // tempo de termino de cada maquina
};

void print(Solution s)
{

}

void construction(Data *data, Solution *s)
{
    s->objValue = 0;
    s->startTime = vector<vector<double>>(data->numJobs, vector<double>(data->numMachines, -1));
    s->schedule = vector<vector<int>>(data->numMachines, vector<int>(data->numJobs, -1));
    s->duration = vector<double>(data->numMachines, 0);

    vector<pair<int, vector<int>>> availableJobs(data->numMachines, {0, vector<int>(data->numJobs)});

    for (int j = 0; j < data->numMachines; j++)
    {
        availableJobs[j].first = j;
        for (int i = 0; i < data->numJobs; i++)
            availableJobs[j].second[i] = i;
    }

    vector<double> earliest(data->numJobs, 0); // o menor tempo no qual o job i pode começar

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
    }
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

    return 0;
}