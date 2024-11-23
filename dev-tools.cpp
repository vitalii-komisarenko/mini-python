/**
 @brief Developer tools
 
 Utility functions to facilitate development. (E.g. to visualize some internal layout)
 */

#include <cstring>
#include <iostream>
#include "src/Instruction.h"

using namespace MiniPython;

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "At least one parameter required.\n"
                  << "Possible values:\n"
                  << "    instr - supply a line from command-line and visualize how it is parsed internally\n";
        exit(1);
    }

    if (!strcmp(argv[1], "instr")) {
        std::string line;
        while (std::cin >> line) {
            std::cout <<  Instruction::fromTokenList(tokenizeLine(line)).debug_string() << "\n";
        }
    }
}