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
    ('FloatZero', 'VAR(Float, 0)', 0.0),
    ('FloatOne', 'VAR(Float, 1)', 1.0),
    ('FloatTwo', 'VAR(Float, 2)', 2.0),
    ('FloatTwo2', 'VAR(Float, 2)', 2.0),
    ('Float2_5', 'VAR(Float, 2.5)', 2.5),
    ('StrEmpty', 'VAR(String, "")', ''),
    ('StrEmpty2', 'VAR(String, "")', ''),
    ('StrA', 'VAR(String, "some text")', 'some text'),
    ('StrA2', 'VAR(String, "some text")', 'some text'),
    ('StrB', 'VAR(String, "other text")', 'other text'),
)

# TODO: % is an operator for string formatting
# Does it mean that % operation is indeed defined for strings as formatting?

print(r"""
#include "variable/Variable.h"

#include "Test.h"

#define VAR(TYPE, VALUE) \
    std::static_pointer_cast<GenericVariable>(std::make_shared<TYPE ## Variable>(VALUE))

#define CHECK_VAR(_VAR, TYPE1, TYPE2, VALUE)                                 \
    MY_ASSERT(_VAR->get_type()  == VariableType::TYPE1);                     \
    if (_VAR->get_type()  == VariableType::TYPE1) {                          \
        auto converted = std::dynamic_pointer_cast<TYPE2 ## Variable>(_VAR); \
        MY_ASSERT(converted->get_value() == VALUE);                          \
    }

void test_variable_autogenerated() {
""")

for v in vars:
    print("    auto " + v[0] + " = " + v[1] + ";")

print("")
print("    // Equality")
print("")

for v1 in vars:
    for v2 in vars:
        not_equal = "" if v1[2] == v2[2] else "!"

        print("    MY_ASSERT(" + not_equal + v1[0] + "->equal(" + v2[0] + "));")

print("")
print("    // Less")
print("")

for v1 in vars:
    for v2 in vars:
        try:
            not_less = "" if v1[2] < v2[2] else "!"
            print("    MY_ASSERT(" + not_less + v1[0] + "->less(" + v2[0] + "));")
        except:
            print("    MUST_THROW(" + v1[0] + "->less(" + v2[0] + "));")

print("}")
