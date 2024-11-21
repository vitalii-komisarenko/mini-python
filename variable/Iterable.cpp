#include "Variable.h"

namespace MiniPython {

bool IterableVariable::contains(const Variable &item_to_search) {
    for (auto &item: to_list()) {
        if (item->equal(item_to_search)) {
            return true;
        }
    }
    return false;
}

} // namespace MiniPython
