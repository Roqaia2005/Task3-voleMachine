#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
using namespace std;
class Memory
{
private:
 int size;
vector<int> data;
public: //constructor set size for memory size and initial values zeros
    Memory(int memorySize) : size(memorySize), data(memorySize,0) {
    }
// function to read address from memory
    int read(int address) {
        if (address >= 0 && address < size) {
            return data[address];
          } else {
            cout << " invalid memory address " << address << endl;}
        return 0;
    }
//  function to put value in some address at the memory
    void write(int address, int value) {
        if (address >= 0 && address < size) {
            data[address] = value;
        } else {
            cout << " invalid memory address " << address << endl;
            
        }
    }
};
//Central Processing unit class which have reference from memory and arithmitic operations and Registers
class CPU {
private:
    Memory &memory;
    vector<int> registers;
    int pc; // program counter which indicate to the current address

public:
    CPU(Memory &mem) : memory(mem), registers(16, 0), pc(0) {} //constructor
//function to fetch instruction from memory
    int fetch() {
        int instruction = memory.read(pc);//read instruction from memory
        return instruction;
    }
//function to decode instruction using bitmasking operators
    void decode(int instruction, int &opcode, int &operand1, int &operand2) {
        opcode = (instruction >> 12) & 0xF; //shift right 12 bits to isolate the first 4 bits for op-code
        operand1 = (instruction >> 8) & 0xF;//shift right 8 bits to isolate the second 4 bits for operand 1
        operand2 = instruction & 0xFF;//just make and operation with 0xFF which is 11111111 to isolate the last 8 bits which is operand2 
    }
    void execute(int opcode, int operand1, int operand2) {
        switch (opcode) {
        case 0x1: // LOAD
            registers[operand1] = memory.read(operand2);
            break;

        case 0x2:
            
        // LOAD (immediate)
            registers[operand1] = operand2;
            break;

        case 0x3: // STORE
            if (operand1 == 0) { // Special case for screen
                // Implement screen output or some other operation
                cout << "Screen output: " << registers[operand2] << endl;
            } else {
                memory.write(operand2, registers[operand1]);
            }
            break;

        case 0x4: // MOVE
            registers[operand2] = registers[operand1];
            break;

        case 0x5: // ADD (two's complement)
            registers[operand1] = registers[operand2] + registers[operand1];
            break;

        case 0xB: // JUMP
            if (registers[operand1] == registers[0]) {
                pc = operand2;
            } else {
                pc++;
            }
            break;

        case 0xC: // HALT
            cout << "HALT instruction encountered. Ending execution." << endl;
            break;

        default:
            cout << "Unsupported opcode: " << hex << opcode << "Ending execution." << endl;//print this message if the opcode doesnot supported in program 
            pc = 256; // Set pc to memorySize to exit the loop (pc reached the end of the memory)
            break;
        }
    }

//function to run the program
    void run() {
        int opcode, operand1, operand2;
        while (opcode!=0xC) { 
            int instruction = fetch();
            decode(instruction, opcode, operand1, operand2);
            execute(opcode, operand1, operand2);
            pc++;
        }
    }
//getter method to get registers 
    const vector<int> &getRegisters() const {
        return registers;
    }

 };
//Try the program in main
int main() {
    int memorySize = 256;
    Memory memory(memorySize);
    CPU cpu(memory); 

    // Menu loop
    while (true) {
        cout << "Menu:\n";
        cout << "1. Load a program and run\n";
        cout << "2. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            {
                // Load program from file
                string filename;
                cout << "Enter the name of the program file: ";
                cin >> filename;

                ifstream inputFile(filename);
                if (!inputFile) {
                    cout << "Unable to open program file." << endl;
                    continue; // Return to the menu
                }

                int address = 0;
                int value;
                while (inputFile >> hex >> value) { //read instructions from file as hex
                    memory.write(address, value);  //load instructions to the memory
                    address++;
                }

                // Close the file
                inputFile.close();
                cout << "Program loaded successfully." << endl;
                cpu.run(); //run the program
                
                // Print register values after execution
                const vector<int> &registers = cpu.getRegisters();
                for (int i = 0; i < registers.size(); ++i) {
                    cout << "Register " << i << ": " << registers[i] << endl;
                }
                
            }
            break;
        case 2:
            cout << "Exiting the program." << endl;
            return 0;

        default:
            cout << "Invalid choice. Please enter a valid option." << endl;
            break;
        }
        
    }

    return 0;
}
