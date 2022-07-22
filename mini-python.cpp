#include "export/mini-python.h"

int main(int argc, char**argv) {
    for (int i = 1; i < argc; ++i) {
        MiniPython::runFromFile(argv[i]);
    }
}