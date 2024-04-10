/*
N: Number of nodes
M: Number of independent voltage sources
*/

#ifndef ENGINE_H
#define ENGINE_H
#include <iostream>
#include <string>
#include "netlist.h"
#include "matrixFunctions.cpp"
using namespace std;

class engine{

    // Netlist-related Variables
    int cmp_count;
    int Vcount, Icount, Rcount, Ccount, Lcount;
    netlist *netPtr = nullptr;

    // Modified Nodal Analysis Parameters
    int N, M; // N is the number of nodes, and M is the number of independent voltage sources + the number of inductors
    float **G = new float *[N];
    int **B = new int *[N];
    int **C = new int *[M];
    int **D = new int *[M];
    float *X = new float (N + M);
    float *I = new float (N);
    float *E = new float (M);
    float *Z = new float (N + M);
    float **A = new float *[N + M];
    // float *A; 

public:
    engine(netlist*);
    ~engine();
    void solve();

    void Build_A_matrix();
    void Build_G_matrix(int, int, float, float);
    void Build_B_matrix(int, int, int);
    void Build_C_matrix();
    void Build_D_matrix();
    void Build_I_matrix(int, int, float);
    void Build_E_matrix(int, float);
    void Build_Z_matrix();

    void update_Z_matrix();
    void Cstamp(int, int, float, float);
    void Lstamp(int, int, int, float, float);
};

#endif // ENGINE_H