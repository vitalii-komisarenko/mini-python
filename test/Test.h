#include <iostream>

#define MY_ASSERT(X)                                                                                               \
    try {                                                                                                          \
        if (!(X)) {                                                                                                \
            std::cerr << __FILE__ << ": " << __LINE__ << ": " << #X << " failed\n";                                \
        }                                                                                                          \
    }                                                                                                              \
    catch(std::exception& ex) {                                                                                    \
        std::cerr << __FILE__ << ": " << __LINE__ << ": " << #X << " failed with exception " << ex.what() << "\n"; \
    }

using namespace MiniPython;
