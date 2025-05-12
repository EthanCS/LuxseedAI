#include "GOAPPlanner.h"
#include "GOAPActionPlan.h"
#include <godot_cpp/core/class_db.hpp>

GOAPPlanner::GOAPPlanner() {}

void GOAPPlanner::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("GetDebugName"), &GOAPPlanner::GetDebugName);
    ClassDB::bind_method(D_METHOD("SetDebugName", "name"), &GOAPPlanner::SetDebugName);
    ClassDB::add_property("GOAPPlanner", PropertyInfo(Variant::STRING, "debugName"), "SetDebugName", "GetDebugName");

    ClassDB::bind_method(D_METHOD("GetWorldState"), &GOAPPlanner::GetWorldState);
    ClassDB::bind_method(D_METHOD("SetWorldState", "worldState"), &GOAPPlanner::SetWorldState);

    ClassDB::bind_method(D_METHOD("GetNumActions"), &GOAPPlanner::GetNumActions);
    // ClassDB::bind_method(D_METHOD("AddAction", "name", "action"), &GOAPPlanner::AddAction);

    ClassDB::bind_method(D_METHOD("GetNumGoals"), &GOAPPlanner::GetNumGoals);
    // ClassDB::bind_method(D_METHOD("AddGoal", "name", "priority"), &GOAPPlanner::AddGoal);
}

bool GOAPPlanner::CheckIfPlanSuccess(const Goal &InGoal, const WorldState *Ws, class GOAPActionPlan *OutPlan,
                                     WorldState *TestSnapshot)
{
    return false;
}