# RLC Circuit Solver

This C++ program is designed to solve RLC (Resistor-Inductor-Capacitor) circuits with any number of components. It includes implementations of the solver engine and netlist parser.


## Files

The project includes the following files located in the `src` folder:

- `engine.cpp`: Implementation of the circuit solver engine.
- `engine.h`: Header file for the circuit solver engine.
- `netlist.cpp`: Implementation of the netlist parser.
- `netlist.h`: Header file for the netlist parser.
- `matrixFunctions.cpp`: Implementation of the matrix Functions needed for the circuit engine.
- `main.cpp`: Main program file.
## Netlist Format

The netlist format follows a specific convention to represent electronic circuits. Each line in the netlist file represents a component or source in the circuit.

The format for each component/source line is as follows:

<Component_Type> <Node1> <Node2> <Value> <Initial_Condition>


Where:
- `<Component_Type>` can be:
  - `Vsrc`: Voltage source
  - `Isrc`: Current source
  - `R`: Resistor
  - `C`: Capacitor
  - `I`: Inductor

- `<Node1>` and `<Node2>` are the nodes connected by the component/source.

- `<Value>` is the value of the component (resistance, capacitance, inductance, or source value).

- `<Initial_Condition>` is the initial condition for the component/source, if applicable.

Example netlist:

0.1
1
R V1 V0 20 0
Vsrc V2 V1 32 0
R V2 V0 80 0
Vsrc V3 V0 20 0
R V2 V3 40 0
-1


Explanation:
- `0.1` represents the simulation time in seconds.
- `1` indicates the number of iterations.
- `R V2 V3 40 0`: Resistor connected between nodes 2 and 3 with a value of 40 and no initial condition.
- `Vsrc V3 V0 20 0`: Voltage source with a DC value of 20, connected between nodes 3 and 0, and no initial condition.
- `-1` indicates the end of the netlist.


## Engine
The engine employes Modified Nodal Analysis Method (MNA) to solve for the unknown node voltages and branch currents.  

In brief, the circuit is described with the equation: AX = Z

The AX=Z matrix equation is a fundamental representation used in circuit analysis and control systems. It consists of three main components:

A Matrix: The A matrix combines the coefficients of the unknown variables (represented by X) in the system of linear equations.
It is composed of four sub-matrices: G, B, C, and D.

G Matrix: Contains conductance terms for resistive elements.
B Matrix: Describes the relationship between branch currents and node voltages.
C Matrix: Relates voltage-controlled sources to node voltages.
D Matrix: Accounts for direct signal coupling between inputs and outputs.
X Vector: The X vector represents the vector of unknown variables in the system.
Z Vector: The Z vector represents the vector of source terms and boundary conditions applied to the circuit. It includes values representing independent sources and initial conditions.

Refer to this link for a more detailed explanation of the MNA method: https://cheever.domains.swarthmore.edu/Ref/mna/MNA2.html  

## Testcases
The project includes 13 test cases located in the `testcases` folder. These test cases cover various circuit configurations and scenarios to ensure the correctness of the solver.

## Output Format
Node voltages are listed, followed by the currents through the voltage sources.

## Sample Runs

### testcase 4

#### Circuit Schematic
![testcase4 circuit schematic](https://github.com/alhusseingamal/RLC-circuit-solver/blob/main/screenshots/testcase%204.PNG)
#### Output
![testcase4 solution](https://github.com/alhusseingamal/RLC-circuit-solver/blob/main/screenshots/testcase%204%20output.PNG)

### testcase 7
#### Circuit Schematic
![testcase7 circuit schematic](https://github.com/alhusseingamal/RLC-circuit-solver/blob/main/screenshots/testcase%207.png)
#### Output
![testcase7 solution](https://github.com/alhusseingamal/RLC-circuit-solver/blob/main/screenshots/testcase%207%20output.png)
At steady state, the capacitors act as open-circuit.  
The node voltages of nodes 1 and 2 are shown.

### testcase 9
#### Circuit Schematic with analytical solution
![testcase9 circuit schematic](https://github.com/alhusseingamal/RLC-circuit-solver/blob/main/screenshots/testcase%209%20with%20solution.PNG)
#### Output
![testcase9 solution](https://github.com/alhusseingamal/RLC-circuit-solver/blob/main/screenshots/testcase%209%20output.PNG)

### testcase 13
#### Circuit Schematic
![testcase13 circuit schematic](https://github.com/alhusseingamal/RLC-circuit-solver/blob/main/screenshots/testcase%2013.PNG)
#### Output
![testcase13 solution](https://github.com/alhusseingamal/RLC-circuit-solver/blob/main/screenshots/testcase%2013%20output.PNG)
The voltage at node 1 corresonds to the voltage across the voltage source = 120V  
At steady state, the inductor acts as a short circuit, hence:
    - node 2 is grounded and has a voltage  = 0V.
    - the magnitude of the total current is the current through the resistor = 120/15 = 8A as shown.
    - the current is -ve through the voltage source.
    - Since the inductor model includes a voltage source, the current through it is calculated as well. In this case it is +ve due to orientation.

## How to Run

To run the code:
1. Place your netlist in the `testcases` folder.
2. Open a terminal and navigate to the project directory.
3. Compile the source files using `g++ main.cpp -o main`.
4. Run the compiled executable using `./main`.

## License

This project is licensed under the MIT License.
