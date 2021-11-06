#include "DefaultTransformations.h"

void DefaultTransformations::SetDefaultOfTrait(std::string traitID, std::shared_ptr<ITransformation> defaultTransformation)
{
    if (defaultTransformation == nullptr)
    {
        throw "Cannot set default transformation of " + traitID + " to null";
    }

    m_defaultTransformationsByTrait[traitID] = defaultTransformation;
}

std::shared_ptr<ITransformation> DefaultTransformations::GetDefaultOfTrait(std::string traitID)
{
    if (m_defaultTransformationsByTrait.count(traitID) == 1)
    {
        return m_defaultTransformationsByTrait[traitID];
    }
    else
    {
        return nullptr;
    }
}

const std::map<std::string, std::shared_ptr<ITransformation>>& DefaultTransformations::GetAllDefaults() const
{
    return m_defaultTransformationsByTrait;
}