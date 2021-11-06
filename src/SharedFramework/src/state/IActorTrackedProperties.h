#pragma once
#include <string>
#include <vector>

namespace DAF
{
    struct TrackedProperty
    {
        std::string name;
        float value;
    };

    /// <summary>
    /// Interface to a particular character's tracked property state representation
    /// This state governs the character's resultant dynamic appearance
    /// </summary>
    class IActorTrackedProperties
    {
    public:
        virtual ~IActorTrackedProperties() {}

        /// <summary>Get this character's current value for a particular tracked property</summary>
        /// <param name="propertyName">The property value we'd like to receive</param>
        /// <returns>The track property's current value</returns>
        virtual float GetTrackedPropertyValue(std::string propertyName) = 0;

        /// <summary> Set this character's tracked property  to the specified value</summary>
        /// <param name="propertyName">The tracked property we'd like to modify the value of</param>
        /// <param name="value">The value we'd like to set the specified property to</param>
        /// <returns>A list of all the trait IDs modified by changing the property value</returns>
        virtual std::vector<std::string> SetTrackedPropertyValue(std::string propertyName, float value) = 0;

        /// <summary>Get this character's current value for a particular tracked property</summary>
        /// <returns>A list of all the actors tracked property key-value pairs</returns>
        virtual std::vector<TrackedProperty> GetAllTrackedProperties() = 0;

        /// <summary>Get this character's current name within the system</summary>
        /// <returns>The character's name in the system</returns>
        virtual std::string GetActorName() = 0;

        /// <summary>Update this character's current name within the system</summary>
        /// <params name="newName">The new name to associate this character with</params>
        virtual void SetActorName(std::string newName) = 0;

    };
}

