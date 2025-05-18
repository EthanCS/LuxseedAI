#include "WorldStateAsset.h"

//////////////////////////////////////////////////////////////////////////////////////
// WorldStateEntryAsset
//////////////////////////////////////////////////////////////////////////////////////
WorldStateEntryAsset::WorldStateEntryAsset() : supportedType(SupportedType::Int), name(""), value(0) {}

WorldStateEntryAsset::SupportedType WorldStateEntryAsset::get_supported_type() const { return supportedType; }
void WorldStateEntryAsset::set_supported_type(SupportedType p_type)
{
    if (supportedType == p_type)
        return;

    supportedType = p_type;
    reset_value();

    notify_property_list_changed();
    emit_changed();
}

void WorldStateEntryAsset::reset_value()
{
    switch (supportedType)
    {
    case Int:
        set_value(0);
        break;
    case Float:
        set_value(0.0f);
        break;
    case Bool:
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
    BIND_ENUM_CONSTANT(Int);
    BIND_ENUM_CONSTANT(Float);
    BIND_ENUM_CONSTANT(Bool);

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
        PropertyInfo(Variant::INT, "supportedType", PROPERTY_HINT_ENUM, "Int,Float,Bool", PROPERTY_USAGE_DEFAULT));

    Variant::Type type = Variant::Type::NIL;
    switch (supportedType)
    {
    case Int:
        type = Variant::INT;
        break;
    case Float:
        type = Variant::FLOAT;
        break;
    case Bool:
        type = Variant::BOOL;
        break;
    }

    p_list->push_back(PropertyInfo(type, "value", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT));
}

bool WorldStateEntryAsset::_get(const StringName &p_path, Variant &r_ret) const
{
    String path = p_path;
    if (path == "supportedType")
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
    if (path == "supportedType")
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