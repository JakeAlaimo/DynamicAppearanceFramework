#pragma once
#include "../SharedFramework/src/state/IActorTrackedProperties.h"

class MockActorTrackedProperties : public IActorTrackedProperties
{
public:

    float GetTrackedPropertyValue(std::string propertyName)
    {
        if (propertyName == "Assaults")
            return 10.0f;
        else if (propertyName == "Days Without Sleep")
            return 3.0f;
        else if (propertyName == "Magicka Percent Remaining")
            return 0.4f;
        else if (propertyName == "Blackjack Earnings")
            return -5.0f;

        return 0.0f;
    }

    std::vector<std::string> SetTrackedPropertyValue(std::string propertyName, float value)
    {
        return std::vector<std::string>();
    }

    std::vector<TrackedProperty> GetAllTrackedProperties()
    {
        return std::vector<TrackedProperty>();
    }

    std::string GetActorName()
    {
        return "MockActor";
    }

    void SetActorName(std::string newName)
    {
    }

};
