#ifndef NETLIST_H
#define NETLIST_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <vector>
using namespace std;

#define NUMBER_OF_COMPONENTS 20
#define NUMBER_OF_PARAMETERS 5

#define COMPONENT_DEFINITION_INDEX 0 // This represents the definition of the component: its type and name, ex: Vsrc ==> (Voltage source called src)
// This is an index within the array
#define COMPONENT_LETTER_INDEX 0 // This is the letter identifier of the component within the component name; Vsrc; This is the position of the "V"
// This is an index within the string defining the component
#define COMPONENT_NODE_1_INDEX 1
#define COMPONENT_NODE_2_INDEX 2
#define COMPONENT_VALUE_INDEX 3
#define COMPONENT_INITIAL_CONDITION_INDEX 4


class netlist
{
    /// @brief 
    string netlist_name;
    ifstream inFile;
    istringstream iss;


    // Schematic and Component Variables
    int component_count;
    int Vcount, Icount, Rcount, Ccount, Lcount;
    set<string>Nodes;
    int number_of_nodes;

public:
 // Simulation Variables
    float stepsize;
    float SimulationTime;
    
    string components[NUMBER_OF_COMPONENTS][NUMBER_OF_PARAMETERS]; 
    vector<string>nodes_array;
    vector<string>voltage_sources_array;
    vector<string>inductors_array;
    netlist(string);
    ~netlist();
    void read_File_Name();
    void openFile();
    void closeFile();
    void processFile();
    void extractComponent(string&, int);
    void extractNodes_and_Sources();
    
    void Assign_IDs_to_Nodes();
    void Assign_IDs_to_Voltage_Sources();
    void Assign_IDs_to_Current_Sources();
    
    int get_Node_ID(string);
    int get_Voltage_Source_ID(string);
    int get_Inductor_ID(string);
    int get_component_count();
    int get_number_of_nodes();
    int get_number_of_independent_voltage_sources();


    int getVcount();
    int getIcount();
    int getRcount();
    int getCcount();
    int getLcount();

};

#endif // NETLIST_H
