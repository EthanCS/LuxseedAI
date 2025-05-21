#include "GOAPActionBehaviour.h"
#include <godot_cpp/core/class_db.hpp>

GOAPActionBehaviour::GOAPActionBehaviour() {}

void GOAPActionBehaviour::_bind_methods()
{
    GDVIRTUAL_BIND(_start);
    GDVIRTUAL_BIND(_update, "deltaTime");
    GDVIRTUAL_BIND(_is_done);
    GDVIRTUAL_BIND(_end);
}

void GOAPActionBehaviour::start()
{
    if (GDVIRTUAL_IS_OVERRIDDEN(_start))
    {
        GDVIRTUAL_CALL(_start);
    }
}

void GOAPActionBehaviour::update(float deltaTime)
{
    if (GDVIRTUAL_IS_OVERRIDDEN(_update))
    {
        GDVIRTUAL_CALL(_update, deltaTime);
    }
}

bool GOAPActionBehaviour::is_done()
{
    if (GDVIRTUAL_IS_OVERRIDDEN(_is_done))
    {
        bool isDone = true;
        GDVIRTUAL_CALL(_is_done, isDone);
        return isDone;
    }
    return true;
}

void GOAPActionBehaviour::end()
{
    if (GDVIRTUAL_IS_OVERRIDDEN(_end))
    {
        GDVIRTUAL_CALL(_end);
    }
}