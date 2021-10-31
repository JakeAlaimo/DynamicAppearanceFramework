#pragma once
#include <string>
#include <vector>
#include "state/IActorTrackedProperties.h"
#include "parse/ITraitTransformationReader.h"
#include "transformations/ITransformation.h"

/// <summary>
/// Governs the dynamic manipulation of supported visual traits for a single character
/// </summary>
class TraitTransformationManager
{
public:

    /// <summary>Creates a TraitTransformationManager instance for a particular character</summary>
    /// <param name="actorTrackedProperties">A particular character's dynamic appearance state representation</param>
    /// <param name="transformationReader">A parser that has processed all of the trait transformation specifications this manager will use</param> 
    TraitTransformationManager(std::shared_ptr<IActorTrackedProperties> actorTrackedProperties, ITraitTransformationReader &transformationReader);

    /// <summary>Process and blends the results of all transformation groups, returning a list of the results</summary>
    /// <returns>A list of independent trait transformations that can be applied to present the final dynamic trait state</returns>
    std::vector<std::shared_ptr<ITransformation>> ApplyAllTransformationGroups();

    /// <summary>Removes the trait transformations for all transformation groups, returning a list of the results</summary>
    /// <returns>A list of the trait transformations that represent the actor's base, non-transformed state</returns>
    std::vector<std::shared_ptr<ITransformation>> RevertAllTransformationGroups();

    /// <summary>Process and blends the results of a single transformation group, returning the final trait transformation</summary>
    /// <params name="traitID">String identifier for a specific dynamic trait</params>
    /// <returns>A trait transformation that can be applied to present the final dynamic state for that particular trait</returns>
    std::shared_ptr<ITransformation> ApplyTransformationGroup(std::string traitID);

    /// <summary>Removes the trait transformations for a specific transformation groups, returning the default trait transformation</summary>
    /// <params name="traitID">String identifier for a specific dynamic trait</params>
    /// <returns>A trait transformation that can be applied to present the base, default state for that particular trait</returns>
    std::shared_ptr<ITransformation> RevertTransformationGroup(std::string traitID);

    /// <summary>Process and blends the results of a single transformation group, returning the final trait transformation</summary>
    /// <params name="traitID">String identifier for a specific dynamic trait</params>
    /// <params name="defaultTransformation">The new base, untransformed value for the trait</params>
    void SetDefaultTransformationForTrait(std::string traitID, std::shared_ptr<ITransformation> defaultTransformation);

private:

    /// <summary>This character's dynamic appearance state representation</summary>
    std::shared_ptr<IActorTrackedProperties> m_actorTrackedProperties = nullptr;

    /// <summary>Mapping of trait IDs to the transformation group associated with them</summary>
    std::map<std::string, std::shared_ptr<TransformationGroup>> m_traitTransformations;
};

