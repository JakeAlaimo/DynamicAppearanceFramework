#include "MockActorTrackedProperties.h"

MockActorTrackedProperties::MockActorTrackedProperties()
{
    m_trackedProperties["Murders"] = 50.0f;
    m_trackedProperties["Days Without Sleep"] = 3.0f;
    m_trackedProperties["Age"] = 27.25f;
    m_trackedProperties["One Handed Skill"] = 75.0f;
    m_trackedProperties["Pockets Picked"] = 15.0f;
}

float MockActorTrackedProperties::GetTrackedPropertyValue(std::string propertyName)
{
    if (m_trackedProperties.count(propertyName) == 1)
    {
        return m_trackedProperties[propertyName];
    }

    return 0.0f;
}

std::vector<std::string> MockActorTrackedProperties::SetTrackedPropertyValue(std::string propertyName, float value)
{
    if (m_trackedProperties.count(propertyName) == 1)
    {
        m_trackedProperties[propertyName] = value;
    }

    return std::vector<std::string>();
}

std::vector<TrackedProperty> MockActorTrackedProperties::GetAllTrackedProperties()
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
