#include <iostream>
#include <chrono>
#include <limits>
#include <ilcplex/ilocplex.h>

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

    {
        int makespan = 300;
        IloEnv env;
        IloModel model(env);

        IloArray<IloArray<IloBoolVarArray>> x(env, data.numJobs);
        for (int i = 0; i < data.numJobs; i++)
        {
            x[i] = IloArray<IloBoolVarArray>(env, data.numMachines);
            for (int j = 0; j < data.numMachines; j++)
            {
                x[i][j] = IloBoolVarArray(env, makespan);
                for (int k = 0; k < makespan; k++)
                {
                    model.add(x[i][j][k]);
                }
            }
        }
        IloIntVar Cmax(env, 0, IloInfinity);
        model.add(Cmax);

        IloExpr sumObj(env);
        sumObj += Cmax;

        model.add(IloMinimize(env, sumObj));

        for (int i = 0; i < data.numJobs; i++)
        {
            for (int j = 0; j < data.numMachines; j++)
            {
                IloExpr sum(env);
                for (int k = 0; k < makespan; k++)
                {
                    sum += x[i][j][k];
                }
                model.add(sum == 1);
            }
        }

        for (int i = 0; i < data.numJobs; i++)
        {
            for (int j = 0; j < data.numMachines; j++)
            {
                IloExpr sum(env);
                for (int k = 0; k < makespan; k++)
                {
                    sum += (k + data.processingTime[i][j]) * x[i][j][k];
                }
                model.add(sum <= Cmax);
            }
        }

        for (int j = 0; j < data.numMachines; j++)
        {
            for (int k = 0; k < makespan; k++)
            {
                IloExpr sum(env);
                for (int i = 0; i < data.numJobs; i++)
                {
                    for (int l = max( 0, k - (int) data.processingTime[i][j] + 1); l <= k; l++)
                    {
                        sum += x[i][j][l];
                    }
                }
                model.add(sum <= 1);
            }
        }

        // for(int i = 0; i < data.numJobs; i++)
        // {
        //     for(int )
        // }
        IloCplex FABRICA(model);

        FABRICA.solve();

        cout << "Valor CPLEX: " << FABRICA.getObjValue() << "\n\n\n";

        // for(int i = 0; i < )
    }

    construction2(&data, &s);
    int constructionSol = s.objValue;

    auto end = chrono::system_clock::now();
    chrono::duration<double> elapsed = end - start;
    timeConstruction = elapsed.count();

    // checkSolution(&data, &s);

    int k = 1;
    while (movSwap(&data, &s))
        ;
    end = chrono::system_clock::now();
    elapsed = end - start;
    timeNeighborhood = elapsed.count();

    //latex
    cout << "instancia & " << constructionSol << " & " << timeConstruction << " & " << s.objValue << " & " << timeNeighborhood << " \\" << endl;

    return 0;
}
