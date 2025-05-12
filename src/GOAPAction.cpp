#include "GOAPAction.h"
#include <godot_cpp/core/class_db.hpp>

GOAPAction::GOAPAction() {}

void GOAPAction::_bind_methods()
{
    GDVIRTUAL_BIND(Start);
    GDVIRTUAL_BIND(Update, "deltaTime");
    GDVIRTUAL_BIND(IsDone);
    GDVIRTUAL_BIND(End);
}

void GOAPAction::Start()
{
    if (GDVIRTUAL_IS_OVERRIDDEN(Start))
    {
        GDVIRTUAL_CALL(Start);
    }
}

void GOAPAction::Update(float deltaTime)
{
    if (GDVIRTUAL_IS_OVERRIDDEN(Update))
    {
        GDVIRTUAL_CALL(Update, deltaTime);
    }
}

bool GOAPAction::IsDone()
{
    if (GDVIRTUAL_IS_OVERRIDDEN(IsDone))
    {
        bool isDone = true;
        GDVIRTUAL_CALL(IsDone, isDone);
        return isDone;
    }
    return true;
}

void GOAPAction::End()
{
    if (GDVIRTUAL_IS_OVERRIDDEN(End))
    {
        GDVIRTUAL_CALL(End);
    }
}