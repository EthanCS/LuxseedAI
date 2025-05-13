#include "GOAPEffect.h"
#include "WorldState.h"
#include <godot_cpp/core/class_db.hpp>

GOAPEffect::GOAPEffect() {}

void GOAPEffect::_bind_methods() { GDVIRTUAL_BIND(_take_effect, "ws"); }

void GOAPEffect::TakeEffect(WorldState *ws)
{
    if (GDVIRTUAL_IS_OVERRIDDEN(_take_effect))
    {
        GDVIRTUAL_CALL(_take_effect, ws);
    }
}