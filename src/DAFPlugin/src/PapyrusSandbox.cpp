#include "PapyrusSandbox.h"
#include "PapyrusActorBase.h"
#include "PapyrusActor.h"
#include "GameObjects.h"

//TODO: remove this
#include "skse/GameData.h"
#include "transformations/MorphTransformation.h"
#include "chargen/MorphHandler.h"
#include "chargen/PapyrusCharGen.h"

extern MorphHandler g_morphHandler;


std::shared_ptr<DAF::TraitTransformationManager> PapyrusSandbox::m_traitTransformationManager;
std::shared_ptr<ActorStateRegistry> PapyrusSandbox::m_actorStateRegistry;

void PapyrusSandbox::ConfigureSandbox(PapyrusSandbox::SandboxDependencies &dependencies)
{
	if (dependencies.transformationManager == nullptr || dependencies.stateRegistry == nullptr)
	{
		throw "Papyrus Sandbox dependencies must not be null";
	}

	m_traitTransformationManager = dependencies.transformationManager;
	m_actorStateRegistry = dependencies.stateRegistry;
}

bool PapyrusSandbox::RegisterActor(StaticFunctionTag* base, Actor* actor)
{
	if (actor == nullptr)
		return false;

	bool registered = m_actorStateRegistry->RegisterActor(actor);

	if (registered) // apply initial transformation groups on registration
	{
		std::vector<std::shared_ptr<DAF::ITransformation>> traitTransformations = 
			m_traitTransformationManager->ApplyAllTransformationGroups(*m_actorStateRegistry->GetActorState(actor));

		//TODO: apply traits here ~
		_MESSAGE("Registered: %s\n", CALL_MEMBER_FN(actor, GetReferenceName)());
	}

	return registered;
}

void PapyrusSandbox::UnregisterActor(StaticFunctionTag* base, Actor* actor, bool revertAppearance)
{
	(*g_thePlayer)->UpdateSkinColor();
	UpdatePlayerTints();

	if (actor == nullptr || m_actorStateRegistry->GetActorState(actor) == nullptr)
		return;

	_MESSAGE("Unregistered: %s\n", CALL_MEMBER_FN(actor, GetReferenceName)());

	if (revertAppearance)
	{
		std::vector<std::shared_ptr<DAF::ITransformation>> revertedTransformations =
			m_traitTransformationManager->RevertAllTransformationGroups(*m_actorStateRegistry->GetActorState(actor));

		//TODO: apply reverted traits here
	}

	m_actorStateRegistry->UnregisterActor(actor);
}

float PapyrusSandbox::GetTrackedProperty(StaticFunctionTag* base, Actor* actor, BSFixedString propertyName)
{
	std::shared_ptr<DAF::ActorState> state = m_actorStateRegistry->GetActorState(actor);
	if (state == nullptr)
		return 0.0f;
	return state->trackedProperties->GetTrackedPropertyValue(propertyName.data);
}

void PapyrusSandbox::SetTrackedProperty(StaticFunctionTag* base, Actor* actor, BSFixedString propertyName, float propertyValue)
{
	std::shared_ptr<DAF::ActorState> state = m_actorStateRegistry->GetActorState(actor);

	if (state == nullptr)
		return;

	// player may have changed names, update internal representation so that overrides work correctly
	state->trackedProperties->SetActorName(CALL_MEMBER_FN(actor, GetReferenceName)());

	_MESSAGE("SetTrackedProperty: %s, %s", CALL_MEMBER_FN(actor, GetReferenceName)(), propertyName);

	// set new tracked property state and adjust the impacted visual traits
	std::vector<std::string> modifiedTraits = state->trackedProperties->SetTrackedPropertyValue(propertyName.data, propertyValue);
	for (std::string trait : modifiedTraits)
	{
		std::shared_ptr<DAF::ITransformation> traitTransformation = m_traitTransformationManager->ApplyTransformationGroup(trait, *state);
		//TODO: apply trait here ~
		DAF::MorphTransformation::Data data = *reinterpret_cast<DAF::MorphTransformation::Data*>(traitTransformation->GetTransformationData());
		
		PresetDataPtr dataPtr = g_morphHandler.ReadCurrentStateAsPreset(actor);
		for (size_t i = 0; i < dataPtr->morphs.size(); i++)
		{
			dataPtr->morphs[i] = data.value;

		}
		g_morphHandler.AssignPreset(dynamic_cast<TESNPC*>(actor->baseForm), dataPtr);
		g_morphHandler.ApplyPresetData(actor, dataPtr, true, MorphHandler::ApplyTypes::kPresetApplyAll);

		//ApplyPreset(actor, actor->race, dynamic_cast<TESNPC*>(actor->baseForm), , MorphHandler::ApplyTypes::kPresetApplyAll);
		for (size_t i = 0; i < dataPtr->headParts.size(); i++)
		{
			//g_morphHandler.ApplyPreset(dynamic_cast<TESNPC*>(actor->baseForm), actor->GetFaceGenNiNode(), dataPtr->headParts[i]);
		}

		//temp delete

		_MESSAGE("%s -> %f", trait.data(), data.value);
	}

	_MESSAGE("New Property Value: %f", state->trackedProperties->GetTrackedPropertyValue(propertyName.data));
	_MESSAGE("");
}

bool PapyrusSandbox::RegisterFunctions(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, bool, Actor*>("RegisterActor", "DynamicAppearanceFramework", RegisterActor, registry));
	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, Actor*, bool>("UnregisterActor", "DynamicAppearanceFramework", UnregisterActor, registry));
	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, float, Actor*, BSFixedString>("GetTrackedProperty", "DynamicAppearanceFramework", GetTrackedProperty, registry));
	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, Actor*, BSFixedString, float>("SetTrackedProperty", "DynamicAppearanceFramework", SetTrackedProperty, registry));
	
	registry->SetFunctionFlags("DynamicAppearanceFramework", "RegisterActor", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("DynamicAppearanceFramework", "UnregisterActor", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("DynamicAppearanceFramework", "GetTrackedProperty", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("DynamicAppearanceFramework", "SetTrackedProperty", VMClassRegistry::kFunctionFlag_NoWait);

	return true;
}

