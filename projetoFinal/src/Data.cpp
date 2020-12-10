#include "Data.h"

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