#include "WorldStateAsset.h"

void WorldStateEntryAsset::_bind_methods() {}

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