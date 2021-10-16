#include "ActorTrackedProperties.h"

ActorTrackedProperties::ActorTrackedProperties(ITraitTransformationReader* transformationReader)
{
    
}

float ActorTrackedProperties::GetTrackedPropertyValue(std::string propertyName)
{
    if (Contains(propertyName))
    {
        return m_trackedProperties[propertyName];
    }
    return 0.0f;
}

std::vector<std::string> ActorTrackedProperties::SetTrackedPropertyValue(std::string propertyName, float value)
{
    if (Contains(propertyName))
    {
        m_trackedProperties[propertyName] = value;
    }

    return std::vector<std::string>();
}

std::vector<TrackedProperty> ActorTrackedProperties::GetAllTrackedProperties()
{
    std::vector<TrackedProperty> trackedProperties;

    for (std::pair<std::string, float> propertyPair : m_trackedProperties)
    {
        TrackedProperty property;
        property.name = propertyPair.first;
        property.value = propertyPair.second;

        trackedProperties.push_back(property);
    }

    return trackedProperties;
}


bool ActorTrackedProperties::Contains(std::string propertyName)
{
    return (m_trackedProperties.count(propertyName) == 1);
}
