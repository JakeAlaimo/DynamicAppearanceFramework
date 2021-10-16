#pragma once
#include <map>
#include "IActorTrackedProperties.h"
#include "../parse/ITraitTransformationReader.h"

/// <summary>
/// Standard concrete implementation for a particular character's tracked property state representation
/// This state governs the character's resultant dynamic appearance
/// </summary>
class ActorTrackedProperties: public IActorTrackedProperties
{
public:

    ActorTrackedProperties(ITraitTransformationReader* transformationReader);

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

    /// <summary>Returns whether the given property is being tracked by this object</summary>
    /// <param name="propertyName">The property to check if it is being tracking</param>
    /// <returns>True if the given property name is tracked</returns>
    bool Contains(std::string propertyName);

private:

    ///<summary>Internal mapping of tracked property names to property values</summary>
    std::map<std::string, float> m_trackedProperties;
};

