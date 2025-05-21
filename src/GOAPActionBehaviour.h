#ifndef GOAP_ACTION_BEHAVIOUR_H
#define GOAP_ACTION_BEHAVIOUR_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class GOAPActionBehaviour : public Object
{
    GDCLASS(GOAPActionBehaviour, Object)

public:
    GOAPActionBehaviour();

    void start();
    void update(float deltaTime);
    bool is_done();
    void end();

    GDVIRTUAL0(_start);
    GDVIRTUAL1(_update, float);
    GDVIRTUAL0R(bool, _is_done);
    GDVIRTUAL0(_end);

protected:
    static void _bind_methods();
};

#endif