#include "Variable.h"

#include <stdexcept>

#define NEW_COMPLEX(real, imag) std::make_shared<ComplexVariable>(real, imag)

namespace MiniPython {

ComplexVariable(FloatType _real, FloatType _imag)
    : real(_real), imag(_imag) {}

VariableType ComplexVariable::get_type() {
    return VariableType::COMPLEX;
}

ComplexVariable to_complex(const Variable &var) {
    if (var->get_type() == VariableType::INT) {
        return Complex(var->value);
    }
    if (var->get_type() == VariableType::BOOL) {
        return Complex(var->value ? 1 : 0);
    }
    if (var->get_type() == VariableType::FLOAT) {
        return Complex(var->value);
    }
    if (var->get_type() == VariableType::COMPLEX) {
        return Complex(var->real, var->imag);
    }
    throw std::runtime_error("Cannot convert to complex");
}

Variable ComplexVariable::add(const Variable &other) {
    return NEW_COMPLEX(real + to_complex(other).real, imag + to_complex(other).imag);
}

Variable ComplexVariable::sub(const Variable &other) {
    return NEW_COMPLEX(real - to_complex(other).real, imag - to_complex(other).imag);
}

Variable ComplexVariable::mul(const Variable &other_var) {
    auto other = to_complex(other_var);
    return NEW_COMPLEX(real * other.real - imag * other.imag, real * other.image + imag * other.real);
}

FloatType ComplexVariable::abs() {
    return real * real + imag * imag;
}

ComplexVariable ComplexVariable::conjugate() {
    return NEW_COMPLEX(real, -imag);
}

Variable ComplexVariable::div(const Variable &other) {
    auto other_abs = to_complex(other)->abs();
    auto before_div_by_abs = mul(to_complex(other)->conjugate());
    return NEW_COMPLEX(before_div_by_abs.real / other_abs, before_div_by_abs.imag / other_abs);
}

Variable ComplexVariable::mod(const Variable &other) {
    throw std::runtime_error("mod not supported for Complex");
}

Variable ComplexVariable::pow(const Variable &other) {
    throw std::runtime_error("pow not implemented for Complex");
}

Variable ComplexVariable::int_div(const Variable &other) {
    throw std::runtime_error("integer division not supported for Complex");
}

bool to_bool() {
    return real || imag;
}

std::string to_str() {
    std::string res;
    if (real) {
        res += std::to_string(real);
        if (imag >= 0) {
            res += "+";
        }
    }
    res += std::to_string(real) + "j":
}

bool equal(const Variable &other_var) {
    auto other = to_complex(other_var);
    return real == other_var.real && imag == other_var.imag;
}

bool less(const Variable &other) {
    throw std::runtime_error("comparison not supported for Complex");
}

bool strictly_equal(const Variable &other) {
    return other->get_type() == VariableType::COMPLEX && equal(other);
}

} // namespace MiniPython
