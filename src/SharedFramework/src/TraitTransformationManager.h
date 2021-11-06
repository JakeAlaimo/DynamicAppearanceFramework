#pragma once
#include <string>
#include <vector>
#include "state/ActorState.h"
#include "parse/IConfigurationParser.h"

/// <summary>
/// Governs the dynamic manipulation of supported visual traits
/// </summary>
class TraitTransformationManager
{
public:
    /// <summary>Creates a TraitTransformationManager instance for the player's particular configuration</summary>
    /// <param name="configurationParser">A parser that has processed all of the trait transformation specifications this manager will use</param> 
    TraitTransformationManager(IConfigurationParser &configurationParser);

    /// <summary>Processes and blends the results of all transformation groups on a particular actor, returning a list of the results</summary>
    /// <param name="actorState">The target actor's full state representation within this system</param>
    /// <returns>A list of independent trait transformations that can be applied to the target actor to present the final dynamic trait state</returns>
    std::vector<std::shared_ptr<ITransformation>> ApplyAllTransformationGroups(ActorState &actorState);

    /// <summary>Removes the trait transformations for all transformation groups on a particular actor, returning a list of the results</summary>
    /// <param name="actorState">The target actor's full state representation within this system</param>
    /// <returns>A list of the trait transformations that represent the target actor's base, non-transformed state</returns>
    std::vector<std::shared_ptr<ITransformation>> RevertAllTransformationGroups(ActorState& actorState);

    /// <summary>Process and blends the results of a single transformation group on a particular actor, returning the final trait transformation</summary>
    /// <params name="traitID">String identifier for a specific dynamic trait</params>
    /// <param name="actorState">The target actor's full state representation within this system</param>
    /// <returns>A trait transformation that can be applied to present the final dynamic state for that actor's particular trait</returns>
    std::shared_ptr<ITransformation> ApplyTransformationGroup(std::string traitID, ActorState& actorState);

    /// <summary>Removes the trait transformations for a specific transformation group on a particular actor, returning the default trait transformation</summary>
    /// <params name="traitID">String identifier for a specific dynamic trait</params>
    /// <param name="actorState">The target actor's full state representation within this system</param>
    /// <returns>A trait transformation that can be applied to present the base, default state for that actor's particular trait</returns>
    std::shared_ptr<ITransformation> RevertTransformationGroup(std::string traitID, ActorState& actorState);

private:

    /// <summary>Mapping of trait IDs to the transformation group associated with them</summary>
    std::map<std::string, std::shared_ptr<TransformationGroup>> m_traitTransformations;
};

