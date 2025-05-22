#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "GOAPActionBehaviour.h"
#include "GOAPActionPlan.h"
#include "GOAPAsset.h"
#include "GOAPCondition.h"
#include "GOAPEffect.h"
#include "GOAPPlanner.h"

#include "WorldState.h"
#include "WorldStateAsset.h"

#ifdef TOOLS_ENABLED
#include "editor/GOAPEditorPlugin.h"
#endif

using namespace godot;

void initialize_luxseedai_module(ModuleInitializationLevel p_level)
{
    if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
    {
        GDREGISTER_RUNTIME_CLASS(WorldState);
        GDREGISTER_CLASS(WorldStateAsset);
        GDREGISTER_CLASS(WorldStateEntryAsset);

        GDREGISTER_CLASS(GOAPAsset);
        GDREGISTER_CLASS(GOAPGoalAsset);
        GDREGISTER_CLASS(GOAPActionAsset);
        GDREGISTER_RUNTIME_CLASS(GOAPActionBehaviour);
        GDREGISTER_RUNTIME_CLASS(GOAPActionPlan);
        GDREGISTER_RUNTIME_CLASS(GOAPPlanner);
        GDREGISTER_RUNTIME_CLASS(GOAPCondition);
        GDREGISTER_RUNTIME_CLASS(GOAPEffect);
    }
#ifdef TOOLS_ENABLED
    else if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR)
    {
        GDREGISTER_INTERNAL_CLASS(GOAPGraphEditor);
        GDREGISTER_INTERNAL_CLASS(GOAPEditorPlugin);
        EditorPlugins::add_by_type<GOAPEditorPlugin>();
    }
#endif
}

void uninitialize_luxseedai_module(ModuleInitializationLevel p_level)
{
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE && p_level != MODULE_INITIALIZATION_LEVEL_EDITOR)
    {
        return;
    }
}

extern "C"
{
    // Initialization.
    GDExtensionBool GDE_EXPORT luxseedai_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address,
                                                      const GDExtensionClassLibraryPtr p_library,
                                                      GDExtensionInitialization *r_initialization)
    {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_luxseedai_module);
        init_obj.register_terminator(uninitialize_luxseedai_module);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}