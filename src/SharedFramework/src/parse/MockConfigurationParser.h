#pragma once
#include "IConfigurationParser.h"
#include "../transformations/MorphTransformation.h"
#include "../state/DefaultTransformations.h"

namespace DAF
{
    class MockConfigurationParser : public IConfigurationParser
    {
    public:

        MockConfigurationParser()
        {
            // define mocked properties
            m_properties.emplace_back("Property 1");
            m_properties.emplace_back("Property 2");
            m_properties.emplace_back("Property 3");

            // define transformations "read" in
            std::shared_ptr<MorphTransformation> maxTranformationTrait1 = std::make_shared<MorphTransformation>(0, 1.0f);
            std::shared_ptr<MorphTransformation> maxTranformationTrait2 = std::make_shared<MorphTransformation>(1, 1.0f);

            // "configure" each trait individually

            // trait 1
            m_groupsByTrait["Trait 1"] = std::make_shared<TransformationGroup>(TraitType::Morph);

            std::shared_ptr<TransformationCurve> trait1Property1Curve = std::make_shared<TransformationCurve>(TraitType::Morph, "Property 1");
            trait1Property1Curve->AddKey(maxTranformationTrait1, 10.0f);
            std::shared_ptr<TransformationCurve> trait1Property1OverrideCurve = std::make_shared<TransformationCurve>(TraitType::Morph, "Property 1");
            trait1Property1OverrideCurve->AddKey(maxTranformationTrait1, 5.0f);
            std::shared_ptr<TransformationCurve> trait1Property2Curve = std::make_shared<TransformationCurve>(TraitType::Morph, "Property 2");
            trait1Property2Curve->AddKey(maxTranformationTrait1, 20.0f);

            m_groupsByTrait["Trait 1"]->AddTransformationCurve(trait1Property1Curve, "");
            m_groupsByTrait["Trait 1"]->AddTransformationCurve(trait1Property1OverrideCurve, "Override");
            m_groupsByTrait["Trait 1"]->AddTransformationCurve(trait1Property2Curve, "");
            
            m_traitsByProperty["Property 1"].emplace_back("Trait 1"); // explicate the properties that govern trait 1
            m_traitsByProperty["Property 2"].emplace_back("Trait 1");

            // trait 2
            m_groupsByTrait["Trait 2"] = std::make_shared<TransformationGroup>(TraitType::Morph);

            std::shared_ptr<TransformationCurve> trait2Property2Curve = std::make_shared<TransformationCurve>(TraitType::Morph, "Property 2");
            trait2Property2Curve->AddKey(maxTranformationTrait2, 10.0f);

            m_groupsByTrait["Trait 2"]->AddTransformationCurve(trait2Property2Curve, "");

            m_traitsByProperty["Property 2"].emplace_back("Trait 2"); ; // explicate the properties that govern trait 2
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

    private:

        std::map<std::string, std::shared_ptr<TransformationGroup>> m_groupsByTrait;
        std::vector<std::string> m_properties;
        std::map<std::string, std::vector<std::string>> m_traitsByProperty;
    };
}