#include "PapyrusSandbox.h"
#include "skse/PapyrusActorBase.h"
#include "skse/PapyrusActor.h"
#include "skse/GameObjects.h"

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
	}

	return registered;
}

void PapyrusSandbox::UnregisterActor(StaticFunctionTag* base, Actor* actor, bool revertAppearance)
{
	if (actor == nullptr || m_actorStateRegistry->GetActorState(actor) == nullptr)
		return;

	if (revertAppearance)
	{
		std::vector<std::shared_ptr<DAF::ITransformation>> revertedTransformations =
			m_traitTransformationManager->RevertAllTransformationGroups(*m_actorStateRegistry->GetActorState(actor));

		//TODO: apply reverted traits here
	}

	m_actorStateRegistry->UnregisterActor(actor);
}

void PapyrusSandbox::SetTrackedProperty(StaticFunctionTag* base, Actor* actor, BSFixedString propertyName, float propertyValue)
{
	std::shared_ptr<DAF::ActorState> state = m_actorStateRegistry->GetActorState(actor);

	if (state == nullptr)
		return;

	// set new tracked property state and adjust the impacted visual traits
	std::vector<std::string> modifiedTraits = state->trackedProperties->SetTrackedPropertyValue(propertyName.data, propertyValue);
	for (std::string trait : modifiedTraits)
	{
		std::shared_ptr<DAF::ITransformation> traitTransformation = m_traitTransformationManager->ApplyTransformationGroup(trait, *state);
		//TODO: apply trait here ~
	}

	//dummy test code below - remove when able
	papyrusActorBase::SetFaceMorph((TESNPC *)actor->baseForm, propertyValue, 4);
	//papyrusActor::QueueNiNodeUpdate(actor);

	_MESSAGE(propertyName.data);
}

bool PapyrusSandbox::RegisterFunctions(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, bool, Actor*>("RegisterActor", "DynamicAppearanceFramework", RegisterActor, registry));
	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, Actor*, bool>("UnregisterActor", "DynamicAppearanceFramework", UnregisterActor, registry));
	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, Actor*, BSFixedString, float>("SetTrackedProperty", "DynamicAppearanceFramework", SetTrackedProperty, registry));
	
	registry->SetFunctionFlags("DynamicAppearanceFramework", "RegisterActor", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("DynamicAppearanceFramework", "UnregisterActor", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("DynamicAppearanceFramework", "SetTrackedProperty", VMClassRegistry::kFunctionFlag_NoWait);

	return true;
}

