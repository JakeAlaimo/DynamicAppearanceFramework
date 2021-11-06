#pragma once
#include "IActorTrackedProperties.h"
#include "DefaultTransformations.h"

/// <summary>
/// Pairs all of the per-actor state information required to manager an actor's transformations
/// </summary>
struct ActorState 
{
    ActorState(std::shared_ptr<IActorTrackedProperties> properties, std::shared_ptr<DefaultTransformations> defaults)
    {
        trackedProperties = properties;
        defaultTransformations = defaults;
    }

    std::shared_ptr<IActorTrackedProperties> trackedProperties;
    std::shared_ptr<DefaultTransformations> defaultTransformations;
};