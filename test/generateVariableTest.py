vars = (
    # 1. C++ name
    # 2. C++ init
    # 3. Python value
    ('None', 'std::static_pointer_cast<GenericVariable>(std::make_shared<NoneVariable>())', None),
    ('None2', 'std::static_pointer_cast<GenericVariable>(std::make_shared<NoneVariable>())', None),
    ('False', 'VAR(Bool, false)', False),
    ('False2', 'VAR(Bool, false)', False),
    ('True', 'VAR(Bool, true)', True),
    ('True2', 'VAR(Bool, true)', True),
    ('IntZero', 'VAR(Int, 0)', 0),
    ('IntOne', 'VAR(Int, 1)', 1),
    ('IntTwo', 'VAR(Int, 2)', 2),
    ('IntTwo2', 'VAR(Int, 2)', 2),
    ('Int8', 'VAR(Int, 8)', 8),
    ('IntMinusOne', 'VAR(Int, -1)', -1),
    ('IntMinusOne2', 'VAR(Int, -1)', -1),
    ('IntMinus12', 'VAR(Int, -12)', -12),
    ('IntMinus73', 'VAR(Int, -73)', -73),
    ('FloatMinusOne', 'VAR(Float, -1)', -1.0),
    ('FloatZero', 'VAR(Float, 0)', 0.0),
    ('FloatOne', 'VAR(Float, 1)', 1.0),
    ('FloatTwo', 'VAR(Float, 2)', 2.0),
    ('FloatTwo2', 'VAR(Float, 2)', 2.0),
    ('Float2_5', 'VAR(Float, 2.5)', 2.5),
    ('Float_23_74', 'VAR(Float, -23.74)', -23.74),
    ('StrEmpty', 'VAR(String, "")', ''),
    ('StrEmpty2', 'VAR(String, "")', ''),
    ('StrA', 'VAR(String, "some text")', 'some text'),
    ('StrA2', 'VAR(String, "some text")', 'some text'),
    ('StrB', 'VAR(String, "other text")', 'other text'),
    ('ListEmpty', 'VAR(List, ListVariable::ListType())', []),
    ('ListEmpty2', 'VAR(List, ListVariable::ListType())', []),
)

# TODO: % is an operator for string formatting
# Does it mean that % operation is indeed defined for strings as formatting?

header = r"""
#include "variable/Variable.h"

#include "Test.h"

#define VAR(TYPE, VALUE) \
    std::static_pointer_cast<GenericVariable>(std::make_shared<TYPE ## Variable>(VALUE))

enum class OpReturningVar {
    add,
    sub,
    mul,
    div,
    int_div,
    pow,
    mod,
};

enum class AssertResult {
    PASS,
    FAIL,
    EXCEPTION,
};

static std::ostream& operator<< (std::ostream& os, AssertResult res) {
    switch (res) {
    case AssertResult::PASS:       os << "PASS";      break;
    case AssertResult::FAIL:       os << "FAIL";      break;
    case AssertResult::EXCEPTION:  os << "EXCEPTION"; break;
    default:                       os << "???";
    }
    return os;
}

static Variable doOpReturningVar(Variable arg1, Variable arg2, OpReturningVar op) {
    switch(op) {
    case OpReturningVar::add: return arg1->add(arg2);
    case OpReturningVar::sub: return arg1->sub(arg2);
    case OpReturningVar::mul: return arg1->mul(arg2);
    case OpReturningVar::div: return arg1->div(arg2);
    case OpReturningVar::int_div: return arg1->int_div(arg2);
    case OpReturningVar::mod: return arg1->mod(arg2);
    case OpReturningVar::pow: return arg1->pow(arg2);
    default: throw std::runtime_error("Bad op");
    }
}

static void doOpReturningVarAndCompareResult(Variable arg1, Variable arg2, OpReturningVar op, Variable expected_res, AssertResult a_res, const char *descr) {
    try {
        auto res_var = doOpReturningVar(arg1, arg2, op);
        auto assertion_result = res_var->strictly_equal(expected_res) ? AssertResult::PASS : AssertResult::FAIL;
        if (assertion_result != a_res) {
            std::cerr << descr << " returned " << assertion_result << " instead of " << a_res << "\n";
        }
    }
    catch(...) {
        if (a_res != AssertResult::EXCEPTION) {
            std::cerr << descr << " returned EXCEPTION instead of " << a_res << "\n";
        }
    }
}

enum class OpReturningBool {
    equal,
    less,
};

static bool doOpReturningBool(Variable arg1, Variable arg2, OpReturningBool op) {
    switch(op) {
    case OpReturningBool::equal: return arg1->equal(arg2);
    case OpReturningBool::less: return arg1->less(arg2);
    default: throw std::runtime_error("Bad op");
    }
}

static void doOpReturningBoolAndCompareResult(Variable arg1, Variable arg2, OpReturningBool op, bool expected_res, AssertResult a_res, const char *descr) {
    try {
        auto assertion_result = doOpReturningBool(arg1, arg2, op) ? AssertResult::PASS : AssertResult::FAIL;
        if (assertion_result != a_res) {
            std::cerr << descr << " returned " << assertion_result << " instead of " << a_res << "\n";
        }
    }
    catch(...) {
        if (a_res != AssertResult::EXCEPTION) {
            std::cerr << descr << " returned EXCEPTION instead of " << a_res << "\n";
        }
    }
}

"""

for v in vars:
    header += "static auto " + v[0] + " = " + v[1] + ";\n"


def generate_for_arithmetic_operation(operation, cpp_name, fh):
    global vars

    for v1 in vars:
        for v2 in vars:
            assert_result = "PASS"
            _type = "None"
            res = ""
            try:
                res = eval('v1[2] ' + operation + 'v2[2]')

            except:
                assert_result = "EXCEPTION"

            _type = str(type(res)).replace("<class '", "").replace("'>", "")
            res_str = ""

            if _type == "None":
                res_str = "None"
            if _type == "str":
                _type = "string"
                res = '"' + res + '"'
            elif _type == "complex":
                # Complex numbers not implemented
                continue
            elif _type == 'list':
                # Only empty lists supported in tests
                res_str = 'VAR(List, ListVariable::ListType())'

            if not res_str:
                res_str = "VAR(" + _type.capitalize() + ", " + str(res) + ")"

            descr = '"' + v1[0] + '->' + cpp_name + '(' + v2[0] + ')"'
            print("    doOpReturningVarAndCompareResult(" + v1[0] + ", " + v2[0] + ", OpReturningVar::" + cpp_name + ", " + res_str + ", AssertResult::" + assert_result + ", " + descr + ");", file=fh)


ops = (
    ("+", "add"),
    ("-", "sub"),
    ("*", "mul"),
    ("/", "div"),
    ("//", "int_div"),
    ("**", "pow"),
    ("%", "mod"),
)

funcs = []

for o in ops:
    funcs.append("test_variable_" + o[1])

    with open("Autogenerated/test_variable_" + o[1] + ".cpp", "w") as f:
        print(header, file=f)
        print("void test_variable_" + o[1] + "() {", file=f)
        generate_for_arithmetic_operation(o[0], o[1], f)
        print("}", file=f)

ops2 = (
    ("==", "equal"),
    ("<", "less"),
)

for o in ops2:
    funcs.append("test_variable_" + o[1])

    with open("Autogenerated/test_variable_" + o[1] + ".cpp", "w") as f:
        print(header, file=f)
        print("void test_variable_" + o[1] + "() {", file=f)

        for v1 in vars:
            for v2 in vars:
                assert_result = "EXCEPTION"
                res = "false"
                try:
                    if eval('v1[2] ' + o[0] + 'v2[2]') == True:
                        assert_result = "PASS"
                        res = "true"
                    else:
                        assert_result =  "FAIL"
                except:
                    pass

                descr = '"' + v1[0] + '->' + o[1] + '(' + v2[0] + ')"'
                print("    doOpReturningBoolAndCompareResult(" + v1[0] + ", " + v2[0] + ", OpReturningBool::" + o[1] + ", " + res + ", AssertResult::" + assert_result + ", " + descr + ");", file=f)

        print("}", file=f)

with open("Autogenerated/test_variable_to_bool.cpp", "w") as f:
    print(header, file=f)
    print("void test_variable_to_bool () {", file=f)

    for v in vars:
        isNot = "" if v[2] else "!"
        print("    MY_ASSERT(" + isNot + v[0] + "->to_bool());", file=f)

    print("}", file=f)

    funcs.append('test_variable_to_bool')

with open("Autogenerated/test_variable_to_str.cpp", "w") as f:
    print(header, file=f)
    print("void test_variable_to_str () {", file=f)

    for v in vars:
        print(f'    MY_ASSERT_EQUAL({v[0]}->to_str(), "{str(v[2])}");', file=f)

    print("}", file=f)

    funcs.append('test_variable_to_str')

with open("Autogenerated/test_variable.cpp", "w") as f:
    for func in funcs:
        print("extern void " + func + "();", file=f)

    print("void test_variable() {", file=f)

    for func in funcs:
        print("    " + func + "();", file=f)

    print("}", file=f)
