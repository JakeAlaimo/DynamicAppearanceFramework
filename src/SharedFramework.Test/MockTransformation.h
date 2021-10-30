#pragma once
#include "../SharedFramework/src/transformations/ITransformation.h"

class MockTransformation : public ITransformation
{
public:

    MockTransformation(float transformValue)
    {
        value = transformValue;
    }

    /// <summary>Returns the type of transformation this result represents (and thus how the data should be interpreted)</summary>
    TraitType GetTraitType()
    {
        return traitType;
    }

    void ForceTraitType(TraitType newType)
    {
        traitType = newType;
    }

    /// <summary>Returns the data needed to apply this transformation</summary>
    void* GetTransformationData()
    {
        return &value;
    }

    /// <summary>Returns the linear blend from this to the other transformation, at the percent specified<summary>
    std::shared_ptr<ITransformation> LerpTo(ITransformation &other, float lerpVal)
    {
        if (GetTraitType() == other.GetTraitType())
        {
            float otherTransformValue = *reinterpret_cast<float *>(other.GetTransformationData());

            float diff = (otherTransformValue - value);
            float lerpedValue = value + (diff * lerpVal);

            return std::make_shared<MockTransformation>(lerpedValue);
        }

        return std::make_shared<MockTransformation>(value);
    }


    virtual std::shared_ptr<ITransformation> WeightedAverage(std::vector<std::pair<ITransformation&, float>> weightedTransformations)
    {
        float weightedValueSum = 0.0f;
        float totalWeight = 0.0f;

        for (auto pair : weightedTransformations) 
        {
            ITransformation& transformation = pair.first;
            float weight = pair.second;

            if (transformation.GetTraitType() == GetTraitType())
            {
                float transformValue = *reinterpret_cast<float*>(transformation.GetTransformationData());
                weightedValueSum += transformValue * weight;
            }

            totalWeight += weight;
        }
        
        return std::make_shared<MockTransformation>(weightedValueSum / totalWeight);
    }

private:

    float value;
    TraitType traitType = TraitType::Morph;
};
