
#include "netlist.h"

using namespace std;

netlist::netlist(string netlist_file_name):
Vcount{0}, Icount{0}, Rcount{0}, Ccount{0}, Lcount{0}, SimulationTime{0}, stepsize{0}, component_count{0}, number_of_nodes{0}
{
    netlist_name = netlist_file_name;
    processFile();
    extractNodes_and_Sources();
    Assign_IDs_to_Nodes();
}
int netlist::get_component_count(){return component_count;}
int netlist::getVcount(){return Vcount;}
int netlist::getIcount(){return Icount;}
int netlist::getRcount(){return Rcount;}
int netlist::getCcount(){return Ccount;}
int netlist::getLcount(){return Lcount;}
void netlist::processFile(){
    openFile();
    string line;
    getline(inFile, line);
    stepsize = stof(line);
    getline(inFile, line);
    SimulationTime = stof(line);
    while(!inFile.eof()){
        getline(inFile, line); // read a line from the netlist
        if(line[0] == '.'){
            // other processing
            // maybe some function like: extractAnalyses()            
            continue; // for now
        }
        if(line[0] == '-'){ // since currently our file ends by a "-1"
            break;
        }
        else{
            extractComponent(line, component_count); // the component_count here represents its index = actual component count - 1
            ++component_count;
        }
    }
    // cout << component_count << endl;
    // for(int i = 0; i < component_count; i++){
    //     for(int j = 0; j < NUMBER_OF_PARAMETERS; j++){
    //         cout << components[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    closeFile();
}
// For the time being, we will assume each component has only five parameters: name node+ node- Val init_Val
static int Vsrc_id = 0;
static int Isrc_id = 0;
static int L_id = 0;
void netlist::extractComponent(string& line, int comp_number){
    iss.clear();
    iss.str(line);
    string word;
    int parameter_number = 0;
    while(iss.good()){
        iss >> word;
        if(word == "Vsrc"){
            word = "V";
            word+=char(Vsrc_id++);
        }
        else if(word == "Isrc"){
            word = "I";
            word+=char(Isrc_id++);
        }
        else if(word == "I"){
            word = "L";
            word+=char(L_id++);
        }
        components[comp_number][parameter_number] = word;
        ++parameter_number;
    }
}

void netlist::extractNodes_and_Sources(){
    for(int i = 0; i < component_count; i++){
        if(components[i][COMPONENT_DEFINITION_INDEX].substr(COMPONENT_LETTER_INDEX,1) == "V"){
            voltage_sources_array.push_back(components[i][COMPONENT_DEFINITION_INDEX].substr(COMPONENT_LETTER_INDEX+1));
            ++Vcount;
        }
        else if(components[i][COMPONENT_DEFINITION_INDEX].substr(COMPONENT_LETTER_INDEX,1) == "C"){
            ++Ccount;
        }
        else if(components[i][COMPONENT_DEFINITION_INDEX].substr(COMPONENT_LETTER_INDEX,1) == "L"){
            inductors_array.push_back(components[i][COMPONENT_DEFINITION_INDEX].substr(COMPONENT_LETTER_INDEX+1));
            ++Lcount;
        }
        /*
        We will insert all the nodes in a set. The resulting set will have all the nodes in the circuit including, as it should be, the 
        ground node, which we don't need
        */
        Nodes.insert(components[i][COMPONENT_NODE_1_INDEX]);
        Nodes.insert(components[i][COMPONENT_NODE_2_INDEX]);
    }
}


void netlist::Assign_IDs_to_Nodes(){
    number_of_nodes = Nodes.size(); // including the ground node
    // Since we are using a set, and node the ground node is reserved the names "0" or"V0", so it will end up at index 0 of the nodes_array
    set<string>::iterator itr;
    while(!Nodes.empty()){
        itr = Nodes.begin();
        nodes_array.push_back(*itr);
        Nodes.erase(itr);
    }
    // for(int i = 0; i < number_of_nodes; i++)
    //     cout << nodes_array[i] << " ";
    // cout << endl;
}

int netlist::get_Node_ID(string nodename){
    for(int i = 0; i < number_of_nodes; i++){
        if(nodes_array[i] == nodename)
            return i;
    }
    return -1;
}

// The voltage source IDs are just their index
int netlist::get_Voltage_Source_ID(string voltage_source_name){
    for(int i = 0; i < Vcount; i++){
        if(voltage_sources_array[i] == voltage_source_name)
            return i;
    }
    return -1;
}

int netlist::get_Inductor_ID(string inductor_name){
    for(int i = 0; i < Lcount; i++){
        if(inductors_array[i] == inductor_name)
            return Vcount + i; // since both will be treated the same, we assume the inductor's ID is Vcount + the inductor's index
    }
    return -1;
}

void netlist::openFile(){
    inFile.open(netlist_name);
    if(inFile.is_open()){
        // cout << "File opened successfully!\n";
    }else{
        cout << "Failed to open the file!\n";
        exit(-1);
    }
}
void netlist::closeFile(){
    inFile.close();
    // cout << "File closed successfully!\n";
}
netlist::~netlist()
{
}
int netlist::get_number_of_nodes(){
    return number_of_nodes - 1; // the -1 exists to un-account for the ground node
}
int netlist::get_number_of_independent_voltage_sources(){
    return Vcount;
}