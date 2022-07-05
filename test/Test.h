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
            std::cerr << "  Left side = " << X << "\n";                                                            \
            std::cerr << "  Right side = " << Y << "\n";                                                           \
        }                                                                                                          \
    }                                                                                                              \
    catch(std::exception& ex) {                                                                                    \
        std::cerr << __FILE__ << ": " << __LINE__ << ": " << #X << " failed with exception " << ex.what() << "\n"; \
    }

#define MUST_THROW(X)                                                                                              \
    try {                                                                                                          \
        X;                                                                                                         \
        std::cerr << __FILE__ << ": " << __LINE__ << ": " << #X << " should have thrown an exception\n";           \
    }                                                                                                              \
    catch(...) {                                                                                                   \
    }

#define CHECK_VAR(_VAR, TYPE1, TYPE2, VALUE)                                                                       \
    try {                                                                                                          \
        MY_ASSERT(_VAR->get_type() == VariableType::TYPE1);                                                        \
        if (_VAR->get_type()  == VariableType::TYPE1) {                                                            \
            auto converted = std::dynamic_pointer_cast<TYPE2 ## Variable>(_VAR);                                   \
            MY_ASSERT_EQUAL(std::dynamic_pointer_cast<TYPE2 ## Variable>(_VAR)->get_value(), VALUE);               \
        }                                                                                                          \
    }                                                                                                              \
    catch (std::exception &ex) {                                                                                   \
        std::cerr << __FILE__ << ": " << __LINE__ << ": " << "Exception " << ex.what() << "\n";                    \
    }


using namespace MiniPython;
