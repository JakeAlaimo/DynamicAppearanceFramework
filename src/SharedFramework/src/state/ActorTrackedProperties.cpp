#include "ActorTrackedProperties.h"

ActorTrackedProperties::ActorTrackedProperties(IConfigurationParser &configurationParser)
{
    //populate the tracked property map
    for (std::string propertyName : configurationParser.GetTrackedProperties())
    {
        m_trackedProperties[propertyName] = 0.0f;
    }

    //store property -> governed traits so we can determine traits modified by updating property values
    m_managedTraitIDsByTrackedProperty = configurationParser.GetManagedTraitIDsByTrackedProperty();
}

ActorTrackedProperties::ActorTrackedProperties(IConfigurationParser &configurationParser, std::map<std::string, float> startingPropertyValues)
{
    //populate the tracked property map
    for (std::string propertyName : configurationParser.GetTrackedProperties())
    {
        float propertyValue = 0.0f;

        //use saved value if provided
        if (startingPropertyValues.count(propertyName) == 1)
        {
            propertyValue = startingPropertyValues[propertyName];
        }

        m_trackedProperties[propertyName] = propertyValue;
    }

    //store property -> governed traits so we can determine traits modified by updating property values
    m_managedTraitIDsByTrackedProperty = configurationParser.GetManagedTraitIDsByTrackedProperty();
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
        return m_managedTraitIDsByTrackedProperty[propertyName];
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

std::string ActorTrackedProperties::GetActorName() 
{
    return actorName;
}

void ActorTrackedProperties::SetActorName(std::string newName) 
{
    actorName = newName;
}

bool ActorTrackedProperties::Contains(std::string propertyName)
{
    return (m_trackedProperties.count(propertyName) == 1);
}
