#include "Variable.h"

#include <map>

namespace MiniPython {

const std::map<std::string, int> typecode_sizes = {
    {"b", 1},
    {"B", 1},
    {"u", 2},
    {"w", 4},
    {"h", 2},
    {"H", 2},
    {"i", 2},
    {"I", 2},
    {"l", 4},
    {"L", 4},
    {"q", 8},
    {"Q", 8},
    {"f", 4},
    {"d", 8},
};

ArrayVariable::ArrayVariable(Variable typecode, ListType _list)
    : ListVariable(_list)
{
    if (!typecode_sizes.contains(typecode->to_str())) {
        RAISE("ValueError", "bad typecode (must be b, B, u, h, H, i, I, l, L, q, Q, f or d)");
    }

    set_attr("typecode", typecode);
    set_attr("itemsize", NEW_INT(typecode_sizes.at(typecode->to_str())));
}

std::string ArrayVariable::to_str() {
    if (list.size()) {
        return std::string("array('") + get_attr("typecode")->to_str() + "', " + ListVariable::to_str() + ")";
    }
    else {
        return std::string("array('") + get_attr("typecode")->to_str() + "')";
    }
}

} // namespace MiniPython
