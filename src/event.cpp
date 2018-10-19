#include "event.h"

bool eCmp::operator()(const ePtr& e1, const ePtr& e2) const {
    return e1->name < e2->name;
};
