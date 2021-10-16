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

    /// <summary>
    /// Creates a TraitTransformationManager instance for a particular character
    /// </summary>
    /// <param name="actorTrackedProperties">A particular character's dynamic appearance state representation</param>
    /// <param name="transformationReader">A parser that has processed all of the trait transformation specifications this manager will use</param> 
    TraitTransformationManager(IActorTrackedProperties* actorTrackedProperties, ITraitTransformationReader* transformationReader);
    ~TraitTransformationManager();


    std::vector<ITransformation> ApplyAllTransformationGroups();
    std::vector<ITransformation> RevertAllTransformationGroups();

    ITransformation ApplyTransformationGroup(std::string traitID);


    void SetDefaultTransformationForTrait(std::string traitID, ITransformation defaultTransformation);






private:

    std::string m_actorName;
    IActorTrackedProperties* m_actorTrackedProperties;

};

