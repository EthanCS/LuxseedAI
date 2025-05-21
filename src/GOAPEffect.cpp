#include "GOAPEffect.h"
#include "WorldState.h"
#include <godot_cpp/core/class_db.hpp>

GOAPEffect::GOAPEffect() {}

void GOAPEffect::_bind_methods() { GDVIRTUAL_BIND(_apply, "ws"); }

void GOAPEffect::apply(WorldState *ws)
{
    if (GDVIRTUAL_IS_OVERRIDDEN(_apply))
    {
        GDVIRTUAL_CALL(_apply, ws);
    }
}