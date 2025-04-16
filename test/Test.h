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

#define MY_ASSERT_EQUAL(X, Y)                                                                                      \
    try {                                                                                                          \
        if ((X) != (Y)) {                                                                                          \
            std::cerr << __FILE__ << ": " << __LINE__ << ": " << #X << " != " #Y << "\n";                          \
            std::cerr << "  Left side = " << (X) << "\n";                                                          \
            std::cerr << "  Right side = " << (Y) << "\n";                                                         \
        }                                                                                                          \
    }                                                                                                              \
    catch(std::exception& ex) {                                                                                    \
        std::cerr << __FILE__ << ": " << __LINE__ << ": " << #X << " failed with exception " << ex.what() << "\n"; \
    }

using namespace MiniPython;
