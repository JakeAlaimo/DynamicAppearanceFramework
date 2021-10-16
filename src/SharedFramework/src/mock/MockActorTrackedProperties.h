#pragma once
#include "../state/IActorTrackedProperties.h"
#include <map>

class MockActorTrackedProperties : public IActorTrackedProperties
{
public:

    MockActorTrackedProperties();

    /// <summary>Get this character's current value for a particular tracked property</summary>
    /// <param name="propertyName">The property value we'd like to receive</param>
    /// <returns>The track property's current value</returns>
    float GetTrackedPropertyValue(std::string propertyName);

    /// <summary> Set this character's tracked property  to the specified value</summary>
    /// <param name="propertyName">The tracked property we'd like to modify the value of</param>
    /// <param name="value">The value we'd like to set the specified property to</param>
    /// <returns>A list of all the trait IDs modified by changing the property value</returns>
    std::vector<std::string> SetTrackedPropertyValue(std::string propertyName, float value) = 0;

    /// <summary>Get this character's current value for a particular tracked property</summary>
    /// <returns>A list of all the actors tracked property key-value pairs</returns>
    std::vector<TrackedProperty> GetAllTrackedProperties();

private:

    std::map<std::string, float> m_trackedProperties;
};

