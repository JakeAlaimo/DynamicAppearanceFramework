#pragma once
#include "skse/PapyrusNativeFunctions.h"
#include "skse/GameReferences.h"
#include "ActorStateRegistry.h"
#include "TraitTransformationManager.h"

/// <summary>
/// Provides all of the functions that will be exposed to papyrus scripts when this plugin is loaded,
/// as well as injects the dependencies these functions will need to operate
/// </summary>
class PapyrusSandbox
{
#pragma region Helper Classes
public:
	struct SandboxDependencies
	{
		std::shared_ptr<DAF::TraitTransformationManager> transformationManager;
		std::shared_ptr<ActorStateRegistry> stateRegistry;
	};
#pragma endregion

public:
	/// <summary>Injects the sandbox dependencies for use</summary>
	/// <param name="dependencies">References to all the external components papyrus will need</param>
	static void ConfigureSandbox(SandboxDependencies &dependencies);

	/// <summary>
	/// Registers a specific actor with the framework, enabling dynamic adjustments to their appearance 
	/// </summary>
	/// <param name="base">Specifies that this corresponds to a global papyrus function</param>
	/// <param name="actor">The specific actor to opt-in to the framework</param>
	/// <returns>Whether the actor was successfully registered</returns>
	static bool RegisterActor(StaticFunctionTag* base, Actor* actor);

	/// <summary>
	/// Unregisters a specific actor from the framework, disabling dynamic adjustments to their appearance 
	/// </summary>
	/// <param name="base">Specifies that this corresponds to a global papyrus function</param>
	/// <param name="actor">The specific actor to opt-out of the framework</param>
	static void UnregisterActor(StaticFunctionTag* base, Actor* actor, bool revertAppearance);

	/// <summary>
	/// Retrieves a specific actor's current tracked property state value
	/// </summary>
	/// <param name="base">Specifies that this corresponds to a global papyrus function</param>
	/// <param name="actor">The specific actor whose state should be retrieved</param>
	/// <param name="propertyName">The specific tracked property to retrieve</param>
	static float GetTrackedProperty(StaticFunctionTag* base, Actor* actor, BSFixedString propertyName);

	/// <summary>
	/// Mutates a specific actor's tracked property state, adjusting their dynamic appearance 
	/// </summary>
	/// <param name="base">Specifies that this corresponds to a global papyrus function</param>
	/// <param name="actor">The specific actor whose state should be modified</param>
	/// <param name="propertyName">The specific tracked property to modify</param>
	/// <param name="propertyValue">The value to change the tracked property to</param>
	static void SetTrackedProperty(StaticFunctionTag* base, Actor* actor, BSFixedString propertyName, float propertyValue);

	/// <summary>Callback to register the papyrus functions so scripts can access them</summary>
	static bool RegisterFunctions(VMClassRegistry* registry);

private:

	// frustratingly, these member must be static, as there is no way to otherwise provide state
	// to the functions called by papyrus. This class structure is mainained so as to at least limit access
	// to the members, though the key limitation here is only one sandbox "exists"

	/// <summary>Injected reference to the configured transformation manager</summary>
	static std::shared_ptr<DAF::TraitTransformationManager> m_traitTransformationManager;

	/// <summary>Injected reference to the application actor state registry</summary>
	static std::shared_ptr<ActorStateRegistry> m_actorStateRegistry;

	// prevent contruction, so as to avoid the impression a proper "object" is used
	PapyrusSandbox();
};