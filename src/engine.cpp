#include "engine.h"

engine::engine(netlist* net)
:netPtr(net), N{0}, M{0}, cmp_count{0}, Vcount{0}, Icount{0}, Rcount{0}, Ccount{0}, Lcount{0}
{   
    cmp_count = netPtr->get_component_count();
    N = netPtr->get_number_of_nodes();
    M = netPtr->get_number_of_independent_voltage_sources();
    M += netPtr->getLcount();
    // cout << "Component Count = " << cmp_count << endl;
    // cout << "N = " << N << endl;
    // cout << "M = " << M << endl;
    
    for(int i = 0; i < N; i++){G[i] = new float[N];}
    for(int i = 0; i < N; i++){B[i] = new int[M];}
    for(int i = 0; i < M; i++){C[i] = new int[N];}
    for(int i = 0; i < M; i++){D[i] = new int[M];}
    for(int i = 0; i < N + M; i++){A[i] = new float[(N+M)*2];}
    
    initializeMatrix(G, N, N);
    initializeMatrix(B, N, M);
    initializeMatrix(C, M, N);
    initializeMatrix(D, M, M);
    initializeMatrix(A, N + M, N + M);
    initializeMatrix(I, N, 1);
    initializeMatrix(E, M, 1);
    initializeMatrix(X, N + M, 1);
    initializeMatrix(Z, N + M, 1);
}


engine::~engine(){
    netPtr = nullptr;
    deallocateMatrix(G, N);
    deallocateMatrix(B, N);
    deallocateMatrix(C, M);
    deallocateMatrix(D, M);
    deallocateMatrix(A, N + M);
    delete [] X;
    delete [] I;
    delete [] E;
    delete [] Z;

}
void engine::solve(){
    Build_A_matrix();
    Build_Z_matrix();
    // cout << "------------------------" << endl;
    // cout << "---------Gmatrix--------" << endl;
    // printMatrix(G, N, N);
    // cout << "---------Bmatrix--------" << endl;
    // printMatrix(B, N, M);
    // cout << "---------Cmatrix--------" << endl;
    // printMatrix(C, M, N);
    // cout << "---------Dmatrix--------" << endl;
    // printMatrix(D, M, M);
    // cout << "---------Imatrix--------" << endl;
    // printMatrix(I, N, 1);
    // cout << "---------Ematrix--------" << endl;
    // printMatrix(E, M, 1);
    // cout << "---------Zmatrix--------" << endl;
    // printMatrix(Z, N + M, 1);
    // cout << "---------Amatrix--------" << endl;
    // printMatrix(A, N + M, N + M);
    // cout << "---------Amatrix inverse--------" << endl;
    float **A_1 = new float *[N + M];
    for(int i = 0; i < N + M; i++){A_1[i] = new float[(N+M)*2];}
    MatrixInverse(A, A_1, N + M);
    // printMatrix(A_1, N + M, N + M);
    cout << "---------Xmatrix--------" << endl;
    float time = 0;
    // printMatrix(X, N + M, 1);
    while(time < netPtr->SimulationTime){
        for(int i = 0; i < N + M; i++) X[i] = 0;
        MatrixMultiplication(A_1, N + M, N + M, Z, N + M, 1, X);
        // time += netPtr->stepsize;
        ++time;
        // here we should use the obtained values to update the Z vector accounting for the capacitors/inductors
        update_Z_matrix();
        assertMatrix(X, N + M, 1);
        // printMatrix(X, N + M, 1);
    }
    assertMatrix(X, N + M, 1);
    printMatrix(X, N + M, 1);
    cout << "-----------------" << endl;
    return;
}

/* 

    This is responsible for updating the Z matrix after each iteration to account for the changes due to the caps and inductors
    This is done in three steps:
        1) Zero out the Z matrix; this is done to avoid accumulating repeating values as we iterate
        -> i.e. We need to account for the independent sources only one time and not every time we iterate
        2) re-assign to the independent sources -- This is the initial value of Z
        -> Z[i] = I[i] for i in range[0,N) (i.e the N values of the independent current sources)
        -> and Z[i] = E[i] for i in range [N, N + M] (i.e the M values for the independent voltage sources)
        3) add to the Z matrix the values incurred from the capacitance(s) and inductor(s) from the last iteration
        -> We loop over all caps and inductors and add/subtract their values from the last iteration
        -> Those values are found in the X-matrix
        -> More on this in the documentation/slides
*/
void engine::update_Z_matrix(){ 
    for(int i = 0; i < N; i++){ // we need to reset the Z matrix
        // This line achieves both steps 1) and 2)
        Z[i] = I[i]; 
    }
    for(int i = N; i < N + M; i++){ // we need to reset the Z matrix
        // This line achieves both steps 1) and 2)
        Z[i] = E[i - N];
    }
    // step 3)
    // identify all capacitors and its two nodes
    // for each capacitor, at the +ve node Z[node1] = C/h * X[node1]
    // for each capacitor, at the -ve node Z[node2] = -1 * C/h * X[node2]
    for(int i = 0; i < netPtr->get_component_count(); i++){
        int node1 = netPtr->get_Node_ID(netPtr->components[i][COMPONENT_NODE_1_INDEX]);
        int node2 = netPtr->get_Node_ID(netPtr->components[i][COMPONENT_NODE_2_INDEX]);
        --node1, --node2; // as we do this decrements, the ground node will have the value -1
        float val = stof(netPtr->components[i][COMPONENT_VALUE_INDEX]);
        float init_condition = stof(netPtr->components[i][COMPONENT_INITIAL_CONDITION_INDEX]);
        string component = netPtr->components[i][COMPONENT_DEFINITION_INDEX].substr(COMPONENT_LETTER_INDEX,1);
        if(component == "C"){
            if(node1 > -1){
                float Vcapacitor = X[node1] - 0; // the - 0 here represents if node2 is the ground
                if(node2 > -1)
                    Vcapacitor = (X[node1] - X[node2]);
                Z[node1] += (val/netPtr->stepsize) * Vcapacitor;
            }
            if(node2 > -1){
                float Vcapacitor = 0 - X[node2]; // the 0 - here represents if node1 is the ground
                if(node1 > -1)
                    Vcapacitor = (X[node1] - X[node2]);
                Z[node2] += -1 * (val/netPtr->stepsize) * Vcapacitor;
            }
        }
        if(component == "L"){
            // if(node1 > -1){
                string component_name = netPtr->components[i][COMPONENT_DEFINITION_INDEX].substr(COMPONENT_LETTER_INDEX+1);
                int id = netPtr->get_Inductor_ID(component_name);
                float Iinductor = X[N + id]; 
                // cout << "inductor id = " << id << endl;
                // N to account for the number of nodes
                // then id represents the id of the inductor (amongst other inductors, if any, and the voltage sources)
                Z[N + id] += -1 *  (val/netPtr->stepsize) * Iinductor;
            // }
        }
    }
    return;
}


void engine::Cstamp(int node1, int node2, float capacitance, float init_condition){
    Build_G_matrix(node1, node2, netPtr->stepsize/capacitance, init_condition); // what we need to get there is C/h, and since we get the reciprocal inside the function Build_G_matrix(), so we pass here h/C
    // we need to account for the initial conditions by modifying the X (or Z i am not 100% sure) matrix from the start with the inital value of the voltage on the Cap
    // for(int i = 0; i < ; i++){
    //     break;
    // }
}
void engine::Lstamp(int node1, int node2, int id, float inductance, float init_condition){
    Build_B_matrix(node1, node2, id);
    D[id][id] += -1 * inductance / netPtr->stepsize;
}
// void engine::Build_D_matrix(int node1, int node2, int id, float inductance){
//     D[id][id] = L
// }
void engine::Build_A_matrix(){
    for(int i = 0; i < cmp_count; i++){
        int node1 = netPtr->get_Node_ID(netPtr->components[i][COMPONENT_NODE_1_INDEX]);
        int node2 = netPtr->get_Node_ID(netPtr->components[i][COMPONENT_NODE_2_INDEX]);
        --node1, --node2;
        float val = stof(netPtr->components[i][COMPONENT_VALUE_INDEX]);
        float init_condition = stof(netPtr->components[i][COMPONENT_INITIAL_CONDITION_INDEX]);
        string component = netPtr->components[i][COMPONENT_DEFINITION_INDEX].substr(COMPONENT_LETTER_INDEX,1);
        if(component == "R"){ // Resistor
            Build_G_matrix(node1, node2, val, init_condition);
        }else if(component == "C"){
            Cstamp(node1, node2, val, init_condition);
        }else if(component == "L"){
            string component_name = netPtr->components[i][COMPONENT_DEFINITION_INDEX].substr(COMPONENT_LETTER_INDEX+1);
            int id = netPtr->get_Inductor_ID(component_name);
            Lstamp(node1, node2, id, val, init_condition);
        }
        else if(component == "V"){
            string component_name = netPtr->components[i][COMPONENT_DEFINITION_INDEX].substr(COMPONENT_LETTER_INDEX+1);
            int Vsrc_id = netPtr->get_Voltage_Source_ID(component_name);
            Build_B_matrix(node1, node2, Vsrc_id);
            Build_E_matrix(Vsrc_id, val);
        }
        else if(component == "I"){
            Build_I_matrix(node1, node2, val);
        }
        else{ // Handle dependent sources
           // 
        }
    }
    Build_C_matrix();
    // This is the actual builiding of the A matrix by combining the matrices G, B, C and D
    // combine G
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            A[i][j] = G[i][j];
        }
    }
    // combine B
    for(int i = 0; i < N; i++){
        for(int j = N; j < N + M; j++){
            A[i][j] = float(B[i][j-N]);
        }
    }
    // combine C
    for(int i = N; i < N + M; i++){
        for(int j = 0; j < N; j++){
            A[i][j] = float(C[(i-N)][j]);
        }
    }
    // combine D
    for(int i = N; i < N + M; i++){
        for(int j = N; j < N + M; j++){
            A[i][j] = float(D[i-N][j-N]);
        }
    }
}



void engine::Build_C_matrix(){ // It is the transpose of the B matrix
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            C[j][i] = B[i][j];   
        }
    }
}
void engine::Build_B_matrix(int node1, int node2, int id){
    if(node1 == node2) return;
    if(node1 > -1)
        B[node1][id] = 1;
    if(node2 > -1)
        B[node2][id] = -1;
}
void engine::Build_I_matrix(int node1, int node2, float val){
    if(node1 == node2) return;
    if(node1 > -1) I[node1] += val;
    if(node2 > -1) I[node2] += -1 * val;
}
void engine::Build_E_matrix(int Vsrc_id, float val){
    E[Vsrc_id] = val;
}
void engine::Build_Z_matrix(){
    for(int i = 0; i < N; i++) Z[i] = I[i];
    for(int i = N; i < N+M; i++) Z[i] = E[i-N];
}
void engine::Build_G_matrix(int node1, int node2, float resistance, float init_condition){
    float Conductance = 1/float(resistance);
    // cout << "Conductance = " << Conductance << endl;
    if(node1 == node2) return;    
    if(node1 > -1){
        G[node1][node1] += Conductance;
        if(node2 > -1){
            G[node2][node2] += Conductance;
            G[node1][node2] += -1*Conductance;
            G[node2][node1] += -1*Conductance;

        }
    }else if(node2 > -1){
        G[node2][node2] += Conductance;
        if(node2 > -1){
            G[node1][node1] += Conductance;
            G[node2][node1] += -1*Conductance;
            G[node1][node2] += -1*Conductance;
        }
    }
   return;
}