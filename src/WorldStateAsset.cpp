#include "WorldStateAsset.h"
#include "WorldState.h"

//////////////////////////////////////////////////////////////////////////////////////
// WorldStateEntryAsset
//////////////////////////////////////////////////////////////////////////////////////
WorldStateEntryAsset::WorldStateEntryAsset() : supported_type(SupportedType::INT), name(""), value(0) {}

WorldStateEntryAsset::SupportedType WorldStateEntryAsset::get_supported_type() const { return supported_type; }
void WorldStateEntryAsset::set_supported_type(SupportedType p_type)
{
    if (supported_type == p_type)
        return;

    supported_type = p_type;
    reset_value();

    notify_property_list_changed();
    emit_changed();
}

void WorldStateEntryAsset::reset_value()
{
    switch (supported_type)
    {
    case INT:
        set_value(0);
        break;
    case FLOAT:
        set_value(0.0f);
        break;
    case BOOL:
        set_value(false);
        break;
    }
}

const Variant &WorldStateEntryAsset::get_value() const { return value; }
void WorldStateEntryAsset::set_value(const Variant &p_value)
{
    value = p_value;
    emit_changed();
}

void WorldStateEntryAsset::_bind_methods()
{
    BIND_ENUM_CONSTANT(INT);
    BIND_ENUM_CONSTANT(FLOAT);
    BIND_ENUM_CONSTANT(BOOL);

    ClassDB::bind_method(D_METHOD("reset_value"), &WorldStateEntryAsset::reset_value);

    ClassDB::bind_method(D_METHOD("set_name", "name"), &WorldStateEntryAsset::set_name);
    ClassDB::bind_method(D_METHOD("get_name"), &WorldStateEntryAsset::get_name);

    ClassDB::bind_method(D_METHOD("set_supported_type", "type"), &WorldStateEntryAsset::set_supported_type);
    ClassDB::bind_method(D_METHOD("get_supported_type"), &WorldStateEntryAsset::get_supported_type);

    ClassDB::bind_method(D_METHOD("get_value"), &WorldStateEntryAsset::get_value);
    ClassDB::bind_method(D_METHOD("set_value", "value"), &WorldStateEntryAsset::set_value);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
}

void WorldStateEntryAsset::_get_property_list(List<PropertyInfo> *p_list) const
{
    p_list->push_back(
        PropertyInfo(Variant::INT, "supported_type", PROPERTY_HINT_ENUM, "INT,FLOAT,BOOL", PROPERTY_USAGE_DEFAULT));

    Variant::Type type = Variant::Type::NIL;
    switch (supported_type)
    {
    case INT:
        type = Variant::INT;
        break;
    case FLOAT:
        type = Variant::FLOAT;
        break;
    case BOOL:
        type = Variant::BOOL;
        break;
    }

    p_list->push_back(PropertyInfo(type, "value", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT));
}

bool WorldStateEntryAsset::_get(const StringName &p_path, Variant &r_ret) const
{
    String path = p_path;
    if (path == "supported_type")
    {
        r_ret = get_supported_type();
    }
    else if (path == "value")
    {
        r_ret = get_value();
    }
    else
    {
        return false;
    }
    return true;
}

bool WorldStateEntryAsset::_set(const StringName &p_path, const Variant &p_value)
{
    String path = p_path;
    if (path == "supported_type")
    {
        set_supported_type((SupportedType)(unsigned int)p_value);
    }
    else if (path == "value")
    {
        set_value(p_value);
    }
    else
    {
        return false;
    }

    return true;
}

const String &WorldStateEntryAsset::get_name() const { return name; }
void WorldStateEntryAsset::set_name(const String &p_name) { name = p_name; }

//////////////////////////////////////////////////////////////////////////////////////
// WorldStateAsset
//////////////////////////////////////////////////////////////////////////////////////
WorldStateAsset::WorldStateAsset() {}

void WorldStateAsset::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_entries"), &WorldStateAsset::get_entries);
    ClassDB::bind_method(D_METHOD("set_entries", "entries"), &WorldStateAsset::set_entries);

    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "entries", PROPERTY_HINT_TYPE_STRING,
                              String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) +
                                  ":WorldStateEntryAsset"),
                 "set_entries", "get_entries");
}

WorldState *WorldStateAsset::create_runtime_world_state() const
{
    WorldState *ws = memnew(WorldState);
    for (int i = 0; i < entries.size(); i++)
    {
        if (entries[i].get_type() == Variant::NIL)
            continue;

        Ref<WorldStateEntryAsset> entry = entries[i];
        ws->add_variable(entry->get_name(), entry->get_value());
    }
    return ws;
}