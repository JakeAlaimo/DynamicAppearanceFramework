#pragma once
#include "../SharedFramework/src/parse/IConfigurationParser.h"
#include "../SharedFramework/src/state/DefaultTransformations.h"
#include "MockTransformation.h"

using namespace DAF;

class MockConfigurationParser : public IConfigurationParser
{
public:

    std::shared_ptr<DefaultTransformations> defaultTransformations;

    MockConfigurationParser()
    {
        m_properties.emplace_back("Assaults");
        m_traitsByProperty["Assaults"].emplace_back("Eye Bags");
        m_traitsByProperty["Assaults"].emplace_back("Hunched Back");
        m_traitsByProperty["Assaults"].emplace_back("Jaw Width");

        defaultTransformations = std::make_shared<DefaultTransformations>();

        std::shared_ptr<MockTransformation> defaultTransformation = std::make_shared<MockTransformation>(0.5f);
        std::shared_ptr<MockTransformation> bigTransformation = std::make_shared<MockTransformation>(1.0f);
        std::shared_ptr<MockTransformation> smallTransformation = std::make_shared<MockTransformation>(0.0f);

        std::shared_ptr<TransformationGroup> jawWidthGroup = std::make_shared<TransformationGroup>(TraitType::Morph);
        m_groupsByTrait["Jaw Width"] = jawWidthGroup;
        defaultTransformations->SetDefaultOfTrait("Jaw Width", defaultTransformation);

        std::shared_ptr<TransformationGroup> eyeBagGroup = std::make_shared<TransformationGroup>(TraitType::Morph);
        m_groupsByTrait["Eye Bags"] = eyeBagGroup;
        defaultTransformations->SetDefaultOfTrait("Eye Bags", defaultTransformation);


        std::shared_ptr<TransformationCurve> assaultJawCurve = std::make_shared<TransformationCurve>(TraitType::Morph, "Assaults");
        assaultJawCurve->AddKey(bigTransformation, 10.0f);
        jawWidthGroup->AddTransformationCurve(assaultJawCurve, "");
        eyeBagGroup->AddTransformationCurve(assaultJawCurve, "");

        std::shared_ptr<TransformationCurve> noSleepCurve = std::make_shared<TransformationCurve>(TraitType::Morph, "Days Without Sleep");
        noSleepCurve->AddKey(smallTransformation, 6.0f);
        jawWidthGroup->AddTransformationCurve(noSleepCurve, "");
        eyeBagGroup->AddTransformationCurve(noSleepCurve, "");


        m_properties.emplace_back("Days Without Sleep");
        m_traitsByProperty["Days Without Sleep"].emplace_back("Eye Bags");


        m_properties.emplace_back("Magicka Percent Remaining");
        m_traitsByProperty["Magicka Percent Remaining"].emplace_back("Hair Saturation");

    }

    /// <summary>Provides transformation group indexed by the trait that group governs</summary>
    std::map<std::string, std::shared_ptr<TransformationGroup>> GetTransformationGroupsByTraitID() 
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

    void ClearGroups() 
    {
        m_groupsByTrait.clear();
    }

private:

    std::map<std::string, std::shared_ptr<TransformationGroup>> m_groupsByTrait;
    std::vector<std::string> m_properties;
    std::map<std::string, std::vector<std::string>> m_traitsByProperty;
};
