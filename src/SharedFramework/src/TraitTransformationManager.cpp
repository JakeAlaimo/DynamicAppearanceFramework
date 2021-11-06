#include "TraitTransformationManager.h"

TraitTransformationManager::TraitTransformationManager(IConfigurationParser &configurationParser)
{
    m_traitTransformations = configurationParser.GetTransformationGroupsByTraitID();
}

std::vector<std::shared_ptr<ITransformation>> TraitTransformationManager::ApplyAllTransformationGroups(ActorState& actorState)
{
    std::vector<std::shared_ptr<ITransformation>> finalTransformations = std::vector<std::shared_ptr<ITransformation>>();

    for (auto transformationMapping : m_traitTransformations)
    {
        std::shared_ptr<ITransformation> groupResult = ApplyTransformationGroup(transformationMapping.first, actorState);
        finalTransformations.push_back(groupResult);
    }    

    return finalTransformations;
}

std::vector<std::shared_ptr<ITransformation>> TraitTransformationManager::RevertAllTransformationGroups(ActorState& actorState)
{
    std::vector<std::shared_ptr<ITransformation>> finalTransformations = std::vector<std::shared_ptr<ITransformation>>();

    for (auto transformationMapping : m_traitTransformations)
    {
        std::shared_ptr<ITransformation> groupResult = RevertTransformationGroup(transformationMapping.first, actorState);
        finalTransformations.push_back(groupResult);
    }    

    return finalTransformations;
}

std::shared_ptr<ITransformation> TraitTransformationManager::ApplyTransformationGroup(std::string traitID, ActorState& actorState)
{
    if (m_traitTransformations.count(traitID) == 1 && m_traitTransformations[traitID] != nullptr
        && actorState.trackedProperties != nullptr && actorState.defaultTransformations->GetDefaultOfTrait(traitID) != nullptr)
    {
        return m_traitTransformations[traitID]->Apply(*actorState.trackedProperties, actorState.defaultTransformations->GetDefaultOfTrait(traitID));
    }
    return nullptr;
}

std::shared_ptr<ITransformation> TraitTransformationManager::RevertTransformationGroup(std::string traitID, ActorState& actorState)
{
    return actorState.defaultTransformations->GetDefaultOfTrait(traitID);
}