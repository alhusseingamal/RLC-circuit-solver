#include <iostream>
#include "netlist.cpp"
#include "engine.cpp"
using namespace std;

int main(){
    string testcaseFileName;
    cout << "Enter the testcase file name (with no extension): ";
    cin >> testcaseFileName;
    string filename = "../testcases/" + testcaseFileName + ".txt";
    netlist net(filename);
    engine myengine(&net);
    myengine.solve();
    return 0;
}