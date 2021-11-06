#pragma once
#include "skse/GameObjects.h"
#include "state/ActorState.h"
#include "parse/IConfigurationParser.h"

/// <summary>
/// Maps specific actors to their state representations within the framework
/// </summary>
class ActorStateRegistry
{
public:
    /// <summary>Constructs a new actor state registry</summary>
    /// <param name="configurationParser">Reference to the framework configuration specified</param>
    ActorStateRegistry(std::shared_ptr<DAF::IConfigurationParser> configurationParser);

    //TODO: Handle serialization reconstructing the registry (so tracked properties carry between saves etc)
    /// <summary>Reconstructs the actor state registry given some serialized data</summary>
    /// <param name="configurationParser">Reference to the framework configuration specified</param>
    //ActorStateRegistry(std::shared_ptr<IConfigurationParser> configurationParser, ?);

    /// <summary>Add the specified actor to the registry, opting them into dynamic transformations</summary>
    /// <param name="actor">The actor to register</param>
    /// <returns>Whether the actor was successfully registered</returns>
    bool RegisterActor(Actor* actor);

    //TODO: Handle serialization of complete actor state - probably custom format
    /// <summary>Add the specified actor to the registry with the specified starting values, opting them into dynamic transformations</summary>
    /// <param name="actor">The actor to register</param>
    /// <returns>Whether the actor was successfully registered</returns>
    //bool RegisterActor(Actor* actor, std::map<std::string, float> startingPropertyValues);

    /// <summary>Remove the specified actor from the registry, opting them out of dynamic transformations</summary>
    /// <param name="actor">The actor to unregister</param>
    void UnregisterActor(Actor* actor);

    /// <summary>Gets the given actor's state representation, if available</summary>
    /// <param name="actor">The actor to retrieve the state of</param>
    /// <returns>The target actor's DAF state representation, if valid</returns>
    std::shared_ptr<DAF::ActorState> GetActorState(Actor* actor);

    //TODO: serialize registry data function

private:

    /// <summary>Internal mapping of actor to their state representation</summary>
    std::map<Actor*, std::shared_ptr<DAF::ActorState>> m_statesByActor;

    /// <summary>Internal reference to the framework configuration. Needed to inform new actor state creation</summary>
    std::shared_ptr<DAF::IConfigurationParser> m_configurationParser;
};



