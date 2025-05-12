#include "GOAPActionPlan.h"
#include <godot_cpp/core/class_db.hpp>

GOAPActionPlan::GOAPActionPlan() {}

void GOAPActionPlan::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("IsFinished"), &GOAPActionPlan::IsFinished);
    ClassDB::bind_method(D_METHOD("IsEmpty"), &GOAPActionPlan::IsEmpty);
    ClassDB::bind_method(D_METHOD("Length"), &GOAPActionPlan::Length);

    ClassDB::bind_method(D_METHOD("Reset"), &GOAPActionPlan::Reset);
}

void GOAPActionPlan::Reset()
{
    goalIndex = -1;
    actionSequences.clear();
    curActionIndex = -1;
    bNeedStartAction = true;
    bFinished = false;
}