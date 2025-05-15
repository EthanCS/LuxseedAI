#include "WorldStateAsset.h"

void WorldStateEntryAsset::_bind_methods()
{
    BIND_ENUM_CONSTANT(Int);
    BIND_ENUM_CONSTANT(Float);
    BIND_ENUM_CONSTANT(Bool);

    ClassDB::bind_method(D_METHOD("set_name", "name"), &WorldStateEntryAsset::set_name);
    ClassDB::bind_method(D_METHOD("get_name"), &WorldStateEntryAsset::get_name);
    ClassDB::bind_method(D_METHOD("set_supported_type", "type"), &WorldStateEntryAsset::set_supported_type);
    ClassDB::bind_method(D_METHOD("get_supported_type"), &WorldStateEntryAsset::get_supported_type);

    ClassDB::bind_method(D_METHOD("get_buffer_data"), &WorldStateEntryAsset::get_buffer_data);
    ClassDB::bind_method(D_METHOD("set_buffer_data", "data"), &WorldStateEntryAsset::set_buffer_data);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "supported_type", PROPERTY_HINT_ENUM, "Int,Float,Bool"),
                 "set_supported_type", "get_supported_type");
    ADD_PROPERTY(
        PropertyInfo(Variant::PACKED_BYTE_ARRAY, "buffer_data", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_STORAGE),
        "set_buffer_data", "get_buffer_data");
}

void WorldStateEntryAsset::_get_property_list(List<PropertyInfo> *p_list) const
{
    Variant::Type type;
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
    default:
        return;
    }
    p_list->push_back(PropertyInfo(type, "value"));
}

bool WorldStateEntryAsset::_get(const StringName &p_name, Variant &r_ret) const
{
    if (p_name == StringName("value"))
    {
        switch (supportedType)
        {
        case Int:
            r_ret = *reinterpret_cast<const int32_t *>(buffer);
            return true;
        case Float:
            r_ret = *reinterpret_cast<const float *>(buffer);
            return true;
        case Bool:
            r_ret = *reinterpret_cast<const bool *>(buffer);
            return true;
        }
    }
    return false;
}

bool WorldStateEntryAsset::_set(const StringName &p_name, const Variant &p_value)
{
    if (p_name == StringName("value"))
    {
        switch (supportedType)
        {
        case Int:
            *reinterpret_cast<int32_t *>(buffer) = p_value;
            return true;
        case Float:
            *reinterpret_cast<float *>(buffer) = p_value;
            return true;
        case Bool:
            *reinterpret_cast<bool *>(buffer) = p_value;
            return true;
        }
    }
    return false;
}

PackedByteArray WorldStateEntryAsset::get_buffer_data() const
{
    PackedByteArray arr;
    arr.resize(kMaxBufferSize);
    memcpy((void *)arr.ptrw(), buffer, kMaxBufferSize);
    return arr;
}

void WorldStateEntryAsset::set_buffer_data(const PackedByteArray &p_data)
{
    memset(buffer, 0, kMaxBufferSize); // 清空缓冲区
    if (p_data.size() > 0)
    {
        memcpy(buffer, p_data.ptr(), MIN(p_data.size(), kMaxBufferSize));
    }
}

String WorldStateEntryAsset::get_name() const { return name; }
void WorldStateEntryAsset::set_name(const String &p_name) { name = p_name; }

WorldStateEntryAsset::SupportedType WorldStateEntryAsset::get_supported_type() const { return supportedType; }
void WorldStateEntryAsset::set_supported_type(SupportedType p_type)
{
    supportedType = p_type;
    memset(buffer, 0, kMaxBufferSize);
}

void WorldStateAsset::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_data", "data"), &WorldStateAsset::set_data);
    ClassDB::bind_method(D_METHOD("get_data"), &WorldStateAsset::get_data);
    ClassDB::bind_method(D_METHOD("set_value", "value"), &WorldStateAsset::set_value);
    ClassDB::bind_method(D_METHOD("get_value"), &WorldStateAsset::get_value);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "data"), "set_data", "get_data");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "value"), "set_value", "get_value");
}

void WorldStateAsset::set_data(const String &p_data) { data = p_data; }
String WorldStateAsset::get_data() const { return data; }

void WorldStateAsset::set_value(int p_value) { value = p_value; }
int WorldStateAsset::get_value() const { return value; }