#ifndef GOAP_CONDITION_H
#define GOAP_CONDITION_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class GOAPCondition : public Object
{
    GDCLASS(GOAPCondition, Object)

public:
    GOAPCondition();

    bool is_pass(const class WorldState *ws);
    GDVIRTUAL1R(bool, _is_pass, const godot::Object *);

protected:
    static void _bind_methods();
};

#endif