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
    if (m_statesByActor.count(actor) == 0)
    {
        // create new state objects for the actor
        std::shared_ptr<DAF::ActorTrackedProperties> newProperties = std::make_shared<DAF::ActorTrackedProperties>(*m_configurationParser);
        std::shared_ptr<DAF::DefaultTransformations> newDefaults = std::make_shared<DAF::DefaultTransformations>();

        // TODO: populate defaults

        // bundle the actor state and add to the registry
        std::shared_ptr<DAF::ActorState> thisActorState = std::make_shared<DAF::ActorState>(newProperties, newDefaults);
        m_statesByActor[actor] = thisActorState;
        return true;
    }
    else
    {
        return false; // actor already registered
    }
}

void ActorStateRegistry::UnregisterActor(Actor* actor)
{
    if (m_statesByActor.count(actor) == 1)
    {
        m_statesByActor.erase(actor);
    }
}

std::shared_ptr<DAF::ActorState> ActorStateRegistry::GetActorState(Actor* actor)
{
    if (m_statesByActor.count(actor) == 1)
    {
        return m_statesByActor[actor];
    }

    return nullptr;
}