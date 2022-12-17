// simulator.cpp
/*
Contributors: 
Brian P., Nick M., Vince V., Jonathan B.
Details are located in the final.doc.txt

*/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <map>
#include <vector>
#include <string>
using namespace std;
int bin_to_dec(string bin);
int main()
{   
    ifstream fin("final.binary.txt");             // the file with binary code (0s and 1s)
    if(fin.fail())                                // make sure file opened okay
    {
        cout << "File open failed\n";
        exit(0);
    }
    string instruction;                          // hold the entire binary instruction
    string opcode;                               // hold the binary opcode (4 bits)
    string plate;                                // placeholder, acts as AC
    string lmh;                                  // loading memory holder,used for loop activation
    int b = 0;                                   // iteration holder, used for loop activation
    
    map<string, int> reg_map;                    // a map from strings to ints used for registers
    map<string, vector<int> > ary_map;           // a map from strings to vector<int> used for arrays
    vector<string> memory;                       // load instructions in vector to represent memory
    
    reg_map["000"] = 0;                     //r1 register set to zero              
    reg_map["001"] = 0;                     //r2 register set to zero
    reg_map["010"] = 0;                     //r3 register set to zero
    reg_map["011"] = 0;                     //r4 register set to zero
    reg_map["100"] = 0;                     //r5 register set to zero
    reg_map["101"] = 0;                     //r6 register set to zero
    reg_map["110"] = 0;                     //r7 register set to zero
    reg_map["111"] = 0;                     //r8 register set to zero 
    while(fin >> instruction)               // read in the entire line of instruction (13 bits)
    {         
        memory.push_back(instruction);      // place it into our memory vector
    }
    fin.close();                            // program is loaded.
    for(unsigned i = 0; i < memory.size(); i++)     // for loop to iterate through vector of instructions
    {   
        instruction = memory.at(i);                 // retrieve the current instruction
        opcode = instruction.substr(0,4);           // take the first 4 bits of the instructionor PUT
        if(opcode == "0000") // HALT
        {
            cout << "Program has finished running.\n";           
        }
        else if(opcode == "0001") // Input
        {   

            cin >> plate;                            // Take input
            int p = stoi(plate);                     // Make str -> int
            plate = to_string(p);                    // Put into "AC"
        }
        else if(opcode == "0011") //PUT
        {
            string n = instruction.substr(4,6);     // read the 6 bits related to numerical value
            int val = bin_to_dec(n);                // convert binary to decimal
            string regis = instruction.substr(10,3);    // find the 3 bits related to the register
            reg_map[regis] = val;                       // use those 3 bits as location to store value
        }
        else if(opcode == "0100") //Skipcond // instruction for skipping
        {
            
            string n = instruction.substr(8,2);         // read the 2 bits related to numerical value
            int val = bin_to_dec(n);                    // turns instruction to value (1, 2 or 3)
            string regis = instruction.substr(10,3);    // Takes register
            if (val == 1 && reg_map[regis] > 0){        // Skipcond 1: if num > 0
                i = i + 1;                  // Iteratures forward once
                instruction = memory.at(i); // Changes instruction place
            }
            else if(val == 2 && reg_map[regis] == 0){   // Skipcond 2: if num == 0
                i = i + 1;                  // Iteratures forward once
                instruction = memory.at(i); // Changes instruction place
            }
            else if(val == 3 && reg_map[regis] < 0){    // Skipcond 3: if num < 0
                i = i + 1;                  // Iteratures forward once
                instruction = memory.at(i); // Changes instruction place
            }
        }
        else if(opcode == "0101") //SUM                 // SUM instruction is followed by three registers
        {   
            string regisA = instruction.substr(4,3);    // bits 4-6 are the first register
            string regisB = instruction.substr(7,3);    // bits 7-9 are the second register
            string regisC = instruction.substr(10,3);   // bits 10-12 are the last register
            
            int sum_result = reg_map[regisA] + reg_map[regisB]; // add the first two registers
            reg_map[regisC] = sum_result;                       // place the result in the third register
        }
        else if(opcode == "0110") // Store
        {  
            string regis = instruction.substr(10,3);    // takes register
            reg_map[regis] = stoi(plate);               // finds register and stores it into "AC"
        }
        else if(opcode == "0111") // Subt
        {  
            string regisA = instruction.substr(4,3);    // bits 4-6 are the first register
            string regisB = instruction.substr(7,3);    // bits 7-9 are the second register
            string regisC = instruction.substr(10,3);   // bits 10-12 are the last register
            
            int sum_result = reg_map[regisA] - reg_map[regisB]; // add the first two registers
            reg_map[regisC] = sum_result;             // finds register and stores it into "AC"
        }
        else if(opcode == "1010") // Mult
        {
            string regisA = instruction.substr(4,3);    // bits 4-6 are the first register
            string regisB = instruction.substr(7,3);    // bits 7-9 are the second register
            string regisC = instruction.substr(10,3);   // bits 10-12 are the last register
            

            // reg_map[regisA] = 3; reg_map[regisB] = 7;
            int sum_result = reg_map[regisA] * reg_map[regisB]; // add the first two registers
            reg_map[regisC] = sum_result;             // finds register and stores it into "AC"
            // cout << regisA << " " << regisB << endl;
            // cout << reg_map[regisA] << " " << reg_map[regisB] << endl;
            // cout 
        }
        else if(opcode == "1100") // Clear
        {  
            reg_map["000"] = 0;                     //r1 register set to zero              
            reg_map["001"] = 0;                     //r2 register set to zero
            reg_map["010"] = 0;                     //r3 register set to zero
            reg_map["011"] = 0;                     //r4 register set to zero
            reg_map["100"] = 0;                     //r5 register set to zero
            reg_map["101"] = 0;                     //r6 register set to zero
            reg_map["110"] = 0;                     //r7 register set to zero
            reg_map["111"] = 0;                     //r8 register set to zero
        }
        else if(opcode == "1101"){          //Jump
            string regis = instruction.substr(10,3); // Takes which register to count from
            instruction = lmh;                       // Loads saved loop instruction
            i = b;                                   // Loads saved count 
            if(reg_map[regis] >= 1) // Iterates down, commenses iteration
            {
                reg_map[regis]--;
                
            }
        }
        else if(opcode == "1110") //LOOP // instruction for looping
        {
            string regis = instruction.substr(10,3);    // Takes register to iterate from
            b = i;                                      // Loads current count into b, serves as a placeholder to jump back to
            lmh = memory.at(i);                         // Loads current instruction into lmh, serves as placeholder to jump to
            if(reg_map[regis] > 1)                      // Count down, used to iterate, mimick looping
            {
                reg_map[regis]--;
            }
        }
        else if(opcode == "1111") //OUT                 // print to screen
        {
            string regis = instruction.substr(10,3);     // find which register to print
            cout << reg_map[regis] << endl;
        }
        else                                            // Opcode not found
        {
            cout << opcode << endl;
            cout << "ERROR. OPCODE NOT FOUND. PROGRAM WILL TERMINATE.\n";
            exit(2);
        }
    }
    return 0;
}
int bin_to_dec(string bin) 
{
    int num = 0;
    for (int i = 0; i < bin.length(); i++) 
        if (bin[i] == '1')
            num += pow(2, bin.length() - 1 - i);
    
    return num;
}
// template<typename K, typename V>
// void print_map(map<string, vector<int> > const &m)
// {
//     for (auto it = m.cbegin(); it != m.cend(); ++it) {
//         cout << "{" << (*it).first << ": " << endl;
//         cout << (*it).second << endl;
//     }
// };