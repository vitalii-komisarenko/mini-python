#pragma once

#include "../variable/Variable.h"

namespace MiniPython {

class math {
public:
    Variable ceil(Variable x);
    Variable copysign(Variable x, Variable y);
    Variable fabs(Variable x);
    Variable factorial(Variable x);
    Variable floor(Variable x);
    Variable fsum(Variable x);
    Variable isfinite(Variable x);
    Variable isinf(Variable x);
    Variable isnan(Variable x);
    Variable isqrt(Variable x);
    Variable trunc(Variable x);
    Variable cbrt(Variable x);
    Variable exp(Variable x);
    Variable exp2(Variable x);
    Variable expm1(Variable x);
    Variable log(Variable x);
    Variable log1p(Variable x);
    Variable log2(Variable x);
    Variable log10(Variable x);
    Variable pow(Variable x, Variable y);
    Variable sqrt(Variable x);
    Variable acos(Variable x);
    Variable asin(Variable x);
    Variable atan(Variable x);
    Variable atan2(Variable x, Variable y);
    Variable cos(Variable x);
    Variable degrees(Variable x);
    Variable radians(Variable x);
    Variable acosh(Variable x);
    Variable asinh(Variable x);
    Variable atanh(Variable x);
    Variable cosh(Variable x);
    Variable sinh(Variable x);
    Variable tanh(Variable x);
};

} // namespace MiniPython
