#include "GOAPCondition.h"
#include "WorldState.h"
#include <godot_cpp/core/class_db.hpp>

GOAPCondition::GOAPCondition() {}

void GOAPCondition::_bind_methods() { GDVIRTUAL_BIND(_is_pass, "ws"); }

bool GOAPCondition::is_pass(const WorldState *ws)
{
    if (GDVIRTUAL_IS_OVERRIDDEN(_is_pass))
    {
        bool isPass = true;
        GDVIRTUAL_CALL(_is_pass, ws, isPass);
        return isPass;
    }
    return false;
}