#pragma once
#include "../SharedFramework/src/parse/ITraitTransformationReader.h"

class MockTraitTransformationReader : public ITraitTransformationReader
{
public:

    MockTraitTransformationReader()
    {
        m_properties.emplace_back("Assaults");
        m_traitsByProperty["Assaults"].emplace_back("Eye Bags");
        m_traitsByProperty["Assaults"].emplace_back("Hunched Back");


        m_properties.emplace_back("Days Without Sleep");
        m_traitsByProperty["Days Without Sleep"].emplace_back("Eye Bags");


        m_properties.emplace_back("Magicka Percent Remaining");
        m_traitsByProperty["Magicka Percent Remaining"].emplace_back("Hair Saturation");

    }

    /// <summary>Provides transformation group indexed by the trait that group governs</summary>
    std::map<std::string, std::shared_ptr<ITransformationGroup>> GetTransformationGroupsByTraitID() 
    {
        return m_groupsByTrait;
    }

    /// <summary>Gets a list of all the tracked properties that govern traits</summary>
    std::vector<std::string> GetTrackedProperties()
    {
        return m_properties;
    }

    /// <summary>Provides a map of tracked properties to the traits they govern</summary>
    std::map<std::string, std::vector<std::string>> GetManagedTraitIDsByTrackedProperty()
    {
        return m_traitsByProperty;
    }

private:

    std::map<std::string, std::shared_ptr<ITransformationGroup>> m_groupsByTrait;
    std::vector<std::string> m_properties;
    std::map<std::string, std::vector<std::string>> m_traitsByProperty;
};
