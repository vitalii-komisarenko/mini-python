#include "math.h"

#include <cmath>

namespace MiniPython {

double to_float(Variable x) {
    if (x->get_type() == VariableType::BOOL) {
        return x->to_bool() ? 1 : 0;
    }
    else if (x->get_type() == VariableType::INT) {
        return std::dynamic_pointer_cast<IntVariable>(x)->value;
    }
    else if (x->get_type() == VariableType::FLOAT) {
        return std::dynamic_pointer_cast<FloatVariable>(x)->value;
    }
    else {
        RAISE("TypeError", "Must be real number");
    }
}

Variable math::ceil(Variable x) {
    return NEW_INT(::ceil(to_float(x)));
}

Variable math::copysign(Variable x, Variable y) {
    float sign = to_float(y) >= 0 ? 1 : -1;
    return NEW_FLOAT(sign * ::fabs(to_float(x)));
}

Variable math::fabs(Variable x) {
    return NEW_INT(::fabs(to_float(x)));
}

Variable math::factorial(Variable x) {
    unsigned long long res = 1;
    for (size_t i = 0; i < to_float(x) + 1; ++i) {
        res *= i;
    }
    return NEW_INT(res);
}

Variable math::floor(Variable x) {
    return NEW_INT(::floor(to_float(x)));
}

Variable math::fsum(Variable x) {
    double res = 0;
    for (auto &x: std::dynamic_pointer_cast<IterableVariable>(x)->to_list()) {
        res += to_float(x);
    }
    return NEW_FLOAT(res);
}

Variable math::isfinite(Variable x) {
    return NEW_BOOL(!std::isinf(to_float(x)));
}

Variable math::isinf(Variable x) {
    return NEW_BOOL(std::isinf(to_float(x)));
}

Variable math::isnan(Variable x) {
    return NEW_BOOL(std::isnan(to_float(x)));
}

Variable math::isqrt(Variable x) {
    return NEW_INT(::floor(::sqrt(to_float(x))));
}

Variable math::trunc(Variable x) {
    return NEW_INT(::trunc(to_float(x)));
}

Variable math::cbrt(Variable x) {
    return NEW_FLOAT(::cbrt(to_float(x)));
}

Variable math::exp(Variable x) {
    return NEW_FLOAT(::exp(to_float(x)));
}

Variable math::exp2(Variable x) {
    return NEW_FLOAT(::exp2(to_float(x)));
}

Variable math::expm1(Variable x) {
    return NEW_FLOAT(::expm1(to_float(x)));
}

Variable math::log(Variable x) {
    return NEW_FLOAT(::log(to_float(x)));
}

Variable math::log1p(Variable x) {
    return NEW_FLOAT(::log1p(to_float(x)));
}

Variable math::log2(Variable x) {
    return NEW_FLOAT(::log2(to_float(x)));
}

Variable math::log10(Variable x) {
    return NEW_FLOAT(::log10(to_float(x)));
}

Variable math::pow(Variable x, Variable y) {
    return NEW_FLOAT(::pow(to_float(x), to_float(y)));
}

Variable math::sqrt(Variable x) {
    return NEW_FLOAT(::sqrt(to_float(x)));
}

Variable math::acos(Variable x) {
    return NEW_FLOAT(::acos(to_float(x)));
}

Variable math::asin(Variable x) {
    return NEW_FLOAT(::asin(to_float(x)));
}

Variable math::atan(Variable x) {
    return NEW_FLOAT(::atan(to_float(x)));
}

Variable math::atan2(Variable x, Variable y) {
    return NEW_FLOAT(::atan2(to_float(y), to_float(x)));
}

Variable math::cos(Variable x) {
    return NEW_FLOAT(::cos(to_float(x)));
}

static long double pi = 3.141592653589793238462643383279502884197;

Variable math::degrees(Variable x) {
    return NEW_FLOAT(to_float(x) * 180 / pi);
}

Variable math::radians(Variable x) {
    return NEW_FLOAT(to_float(x) * pi / 180);
}

Variable math::acosh(Variable x) {
    return NEW_FLOAT(::acosh(to_float(x)));
}

Variable math::asinh(Variable x) {
    return NEW_FLOAT(::asinh(to_float(x)));
}

Variable math::atanh(Variable x) {
    return NEW_FLOAT(::atanh(to_float(x)));
}

Variable math::cosh(Variable x) {
    return NEW_FLOAT(::cosh(to_float(x)));
}

Variable math::sinh(Variable x) {
    return NEW_FLOAT(::sinh(to_float(x)));
}

Variable math::tanh(Variable x) {
    return NEW_FLOAT(::tanh(to_float(x)));
}

} // namespace MiniPython
