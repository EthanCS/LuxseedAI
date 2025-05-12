#ifndef GOAP_ACTION_H
#define GOAP_ACTION_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class GOAPAction : public Object
{
    GDCLASS(GOAPAction, Object)

public:
    GOAPAction();

    virtual void Start();
    virtual void Update(float deltaTime);
    virtual bool IsDone();
    virtual void End();

    GDVIRTUAL0(Start);
    GDVIRTUAL1(Update, float);
    GDVIRTUAL0R(bool, IsDone);
    GDVIRTUAL0(End);

protected:
    static void _bind_methods();
};

#endif