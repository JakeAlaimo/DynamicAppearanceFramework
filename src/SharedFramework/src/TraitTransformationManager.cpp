#include "TraitTransformationManager.h"

TraitTransformationManager::TraitTransformationManager(std::shared_ptr<IActorTrackedProperties> actorTrackedProperties, ITraitTransformationReader &transformationReader) 
{
    if (actorTrackedProperties == nullptr)
    {
        throw "Cannot create transformation manager without valid actor properties";
    }

    m_actorTrackedProperties = actorTrackedProperties;
    m_traitTransformations = transformationReader.GetTransformationGroupsByTraitID();
}

std::vector<std::shared_ptr<ITransformation>> TraitTransformationManager::ApplyAllTransformationGroups()
{
    std::vector<std::shared_ptr<ITransformation>> finalTransformations = std::vector<std::shared_ptr<ITransformation>>();

    if (m_actorTrackedProperties != nullptr)
    {
        for (auto transformationMapping : m_traitTransformations)
        {
            std::shared_ptr<ITransformation> groupResult = ApplyTransformationGroup(transformationMapping.first);
            finalTransformations.push_back(groupResult);
        }
    }

    return finalTransformations;
}

std::vector<std::shared_ptr<ITransformation>> TraitTransformationManager::RevertAllTransformationGroups() 
{
    std::vector<std::shared_ptr<ITransformation>> finalTransformations = std::vector<std::shared_ptr<ITransformation>>();

    for (auto transformationMapping : m_traitTransformations)
    {
        std::shared_ptr<ITransformation> groupResult = RevertTransformationGroup(transformationMapping.first);
        finalTransformations.push_back(groupResult);
    }    

    return finalTransformations;
}

std::shared_ptr<ITransformation> TraitTransformationManager::ApplyTransformationGroup(std::string traitID)
{
    if (m_traitTransformations.count(traitID) == 1 && m_traitTransformations[traitID] != nullptr
        && m_actorTrackedProperties != nullptr)
    {
        return m_traitTransformations[traitID]->Apply(*m_actorTrackedProperties);
    }
    return nullptr;
}

std::shared_ptr<ITransformation> TraitTransformationManager::RevertTransformationGroup(std::string traitID)
{
    if (m_traitTransformations.count(traitID) == 1 && m_traitTransformations[traitID] != nullptr)
    {
        return m_traitTransformations[traitID]->Revert();
    }
    return nullptr;
}

void TraitTransformationManager::SetDefaultTransformationForTrait(std::string traitID, std::shared_ptr<ITransformation> defaultTransformation)
{
    if (defaultTransformation == nullptr || m_traitTransformations.count(traitID) == 0 || m_traitTransformations[traitID] == nullptr)
    {
        throw "Cannot set default transformation";
    }

    m_traitTransformations[traitID]->SetDefaultTransformation(defaultTransformation);
}