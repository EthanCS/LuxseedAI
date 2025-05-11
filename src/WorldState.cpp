#include "WorldState.h"
#include "godot_cpp/core/print_string.hpp"
#include <godot_cpp/core/class_db.hpp>

WorldState::WorldState() : bSnapshot(false), mainWorldState(nullptr) {}

void WorldState::SnapshotFrom(const WorldState *src)
{
    if (src == nullptr)
    {
        godot::print_error("src world state is null");
        return;
    }

    data = src->data;
    bSnapshot = true;
    mainWorldState = src->bSnapshot ? src->mainWorldState : src;
}

void WorldState::CopyDataFrom(const WorldState *src)
{
    if (src == nullptr)
        return;

    if (data.size() == src->data.size())
    {
        data = src->data;
    }
}

bool WorldState::IsValid() const noexcept
{
    if (bSnapshot && mainWorldState == nullptr)
        return false;

    return data.size() == GetMetas().size() == GetKeyIndexMapping().size();
}

int WorldState::GetKeyID(const String &name) const
{
    for (const auto &m : GetMetas())
    {
        if (m.Name == name)
        {
            return m.KeyID;
        }
    }
    return -1;
}

int WorldState::GetVariableCount() const noexcept { return data.size(); }

int WorldState::HasVariable(int KeyID) const
{
    const auto &k = GetKeyIndexMapping();
    return k.find(KeyID) == k.end();
}

Variant WorldState::GetVariable(int KeyID) noexcept
{
    int index = GetKeyIndexMapping().at(KeyID);
    return data[index];
}

void WorldState::SetVariable(int KeyID, const Variant &Data) noexcept
{
    int index = GetKeyIndexMapping().at(KeyID);
    data[index] = Data;
}

void WorldState::AddVariable(int KeyID, const String &Name, const Variant &Data)
{
    if (bSnapshot)
        return;

    Meta m = {};
    m.Location = data.size();
    m.KeyID = KeyID;
    m.Name = Name;

    data.push_back(Data);
    metas.push_back(m);
    keyIndexMapping.insert(std::pair<int, int>(KeyID, (int)(metas.size() - 1)));
}

void WorldState::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("SnapshotFrom"), &WorldState::SnapshotFrom);
    ClassDB::bind_method(D_METHOD("CopyDataFrom", "src"), &WorldState::CopyDataFrom);
    ClassDB::bind_method(D_METHOD("IsValid"), &WorldState::IsValid);
    ClassDB::bind_method(D_METHOD("GetKeyID", "Name"), &WorldState::GetKeyID);
    ClassDB::bind_method(D_METHOD("GetVariableCount"), &WorldState::GetVariableCount);
    ClassDB::bind_method(D_METHOD("HasVariable", "KeyID"), &WorldState::HasVariable);
    ClassDB::bind_method(D_METHOD("GetVariable", "KeyID"), &WorldState::GetVariable);
    ClassDB::bind_method(D_METHOD("SetVariable", "KeyID", "Data"), &WorldState::SetVariable);
    ClassDB::bind_method(D_METHOD("AddVariable", "KeyID", "Name", "Data"), &WorldState::AddVariable);
}