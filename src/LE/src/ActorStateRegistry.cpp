#include "ActorStateRegistry.h"
#include "state/ActorTrackedProperties.h"

ActorStateRegistry::ActorStateRegistry(std::shared_ptr<DAF::IConfigurationParser> configurationParser)
{
    if (configurationParser == nullptr)
    {
        throw "ActorStateRegistry requires non-null configuration parser on construction";
    }

    m_configurationParser = configurationParser;
}

bool ActorStateRegistry::RegisterActor(Actor* actor) 
{
    if (m_statesByActorID.count(actor->formID) == 0)
    {
        // create new state objects for the actor
        std::shared_ptr<DAF::ActorTrackedProperties> newProperties = std::make_shared<DAF::ActorTrackedProperties>(*m_configurationParser);
        std::shared_ptr<DAF::DefaultTransformations> newDefaults = std::make_shared<DAF::DefaultTransformations>();

        // TODO: populate defaults

        // bundle the actor state and add to the registry
        std::shared_ptr<DAF::ActorState> thisActorState = std::make_shared<DAF::ActorState>(newProperties, newDefaults);
        m_statesByActorID[actor->formID] = thisActorState;
        return true;
    }
    else
    {
        return false; // actor already registered
    }
}

void ActorStateRegistry::UnregisterActor(Actor* actor)
{
    if (m_statesByActorID.count(actor->formID) == 1)
    {
        m_statesByActorID.erase(actor->formID);
    }
}

std::shared_ptr<DAF::ActorState> ActorStateRegistry::GetActorState(Actor* actor)
{
    if (m_statesByActorID.count(actor->formID) == 1)
    {
        return m_statesByActorID[actor->formID];
    }

    return nullptr;
}