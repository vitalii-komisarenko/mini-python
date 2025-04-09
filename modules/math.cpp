#include "Module.h"
#include "Instruction.h"

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

static Variable ceil(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_INT(::ceil(to_float(x)));
}

static Variable copysign(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    auto y = PARAM(1);
    float sign = to_float(y) >= 0 ? 1 : -1;
    return NEW_FLOAT(sign * ::fabs(to_float(x)));
}

static Variable fabs(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_INT(::fabs(to_float(x)));
}

static Variable factorial(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    unsigned long long res = 1;
    for (size_t i = 0; i < to_float(x) + 1; ++i) {
        res *= i;
    }
    return NEW_INT(res);
}

static Variable floor(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_INT(::floor(to_float(x)));
}

static Variable fsum(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    double res = 0;
    for (auto &x: std::dynamic_pointer_cast<IterableVariable>(x)->to_list()) {
        res += to_float(x);
    }
    return NEW_FLOAT(res);
}

static Variable isfinite(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_BOOL(!std::isinf(to_float(x)));
}

static Variable isinf(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_BOOL(std::isinf(to_float(x)));
}

static Variable isnan(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_BOOL(std::isnan(to_float(x)));
}

static Variable isqrt(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_INT(::floor(::sqrt(to_float(x))));
}

static Variable trunc(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_INT(::trunc(to_float(x)));
}

static Variable cbrt(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_FLOAT(::cbrt(to_float(x)));
}

static Variable exp(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_FLOAT(::exp(to_float(x)));
}

static Variable exp2(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_FLOAT(::exp2(to_float(x)));
}

static Variable expm1(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_FLOAT(::expm1(to_float(x)));
}

static Variable log(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_FLOAT(::log(to_float(x)));
}

static Variable log1p(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_FLOAT(::log1p(to_float(x)));
}

static Variable log2(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_FLOAT(::log2(to_float(x)));
}

static Variable log10(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_FLOAT(::log10(to_float(x)));
}

static Variable math_pow(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    auto y = PARAM(1);
    return NEW_FLOAT(::pow(to_float(x), to_float(y)));
}

static Variable sqrt(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_FLOAT(::sqrt(to_float(x)));
}

static Variable acos(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_FLOAT(::acos(to_float(x)));
}

static Variable asin(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_FLOAT(::asin(to_float(x)));
}

static Variable atan(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_FLOAT(::atan(to_float(x)));
}

static Variable atan2(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    auto y = PARAM(1);
    return NEW_FLOAT(::atan2(to_float(y), to_float(x)));
}

static Variable cos(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_FLOAT(::cos(to_float(x)));
}

static long double pi = 3.141592653589793238462643383279502884197;

static Variable degrees(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_FLOAT(to_float(x) * 180 / pi);
}

static Variable radians(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_FLOAT(to_float(x) * pi / 180);
}

static Variable acosh(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_FLOAT(::acosh(to_float(x)));
}

static Variable asinh(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_FLOAT(::asinh(to_float(x)));
}

static Variable atanh(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_FLOAT(::atanh(to_float(x)));
}

static Variable cosh(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_FLOAT(::cosh(to_float(x)));
}

static Variable sinh(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_FLOAT(::sinh(to_float(x)));
}

static Variable tanh(const InstructionParams &params, Scope *scope) {
    auto x = PARAM(0);
    return NEW_FLOAT(::tanh(to_float(x)));
}

math::math() {
    SET_FUNCTION("ceil", ceil);
    SET_FUNCTION("copysign", copysign);
    SET_FUNCTION("fabs", fabs);
    SET_FUNCTION("factorial", factorial);
    SET_FUNCTION("floor", floor);
    SET_FUNCTION("fsum", fsum);
    SET_FUNCTION("isfinite", isfinite);
    SET_FUNCTION("isinf", isinf);
    SET_FUNCTION("isnan", isnan);
    SET_FUNCTION("isqrt", isqrt);
    SET_FUNCTION("trunc", trunc);
    SET_FUNCTION("cbrt", cbrt);
    SET_FUNCTION("exp", exp);
    SET_FUNCTION("exp2", exp2);
    SET_FUNCTION("expm1", expm1);
    SET_FUNCTION("log", log);
    SET_FUNCTION("log1p", log1p);
    SET_FUNCTION("log2", log2);
    SET_FUNCTION("log10", log10);
    SET_FUNCTION("pow", math_pow);
    SET_FUNCTION("sqrt", sqrt);
    SET_FUNCTION("acos", acos);
    SET_FUNCTION("asin", asin);
    SET_FUNCTION("atan", atan);
    SET_FUNCTION("atan2", atan2);
    SET_FUNCTION("cos", cos);
    SET_FUNCTION("degrees", degrees);
    SET_FUNCTION("radians", radians);
    SET_FUNCTION("acosh", acosh);
    SET_FUNCTION("asinh", asinh);
    SET_FUNCTION("atanh", atanh);
    SET_FUNCTION("cosh", cosh);
    SET_FUNCTION("sinh", sinh);
    SET_FUNCTION("tanh", tanh);

    set_attr("pi", NEW_FLOAT(pi));
    set_attr("e", NEW_FLOAT(2.718281828459045));
}

} // namespace MiniPython
