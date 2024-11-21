#include "export/mini-python.h"

#include <ctime>

int main(int argc, char**argv) {
    std::srand(std::time(nullptr));

    for (int i = 1; i < argc; ++i) {
        MiniPython::runFromFile(argv[i]);
    }
}
