#ifndef GOAP_EFFECT_H
#define GOAP_EFFECT_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class GOAPEffect : public Object
{
    GDCLASS(GOAPEffect, Object)

public:
    GOAPEffect();

    void apply(class WorldState *ws);
    GDVIRTUAL1(_apply, godot::Object *);

protected:
    static void _bind_methods();
};

#endif