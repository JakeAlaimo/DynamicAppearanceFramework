#include "MorphTransformation.h"

namespace DAF
{
    MorphTransformation::MorphTransformation(int morphIndex, float morphValue)
    {
        if (morphIndex < 0)
            throw "Invalid morph index for transformation";

        m_morphData.index = morphIndex;
        m_morphData.value = morphValue;
    }

    TraitType MorphTransformation::GetTraitType()
    {
        return TraitType::Morph;
    }

    void* MorphTransformation::GetTransformationData()
    {      
        return &m_morphData;
    }

    std::shared_ptr<ITransformation> MorphTransformation::LerpTo(ITransformation& other, float lerpVal)
    {
        if (GetTraitType() == other.GetTraitType())
        {
            Data otherMorphData = *reinterpret_cast<Data*>(other.GetTransformationData());

            if (m_morphData.index != otherMorphData.index) // different traits being morphed
                return std::make_shared<MorphTransformation>(m_morphData.index, m_morphData.value);

            float diff = (otherMorphData.value - m_morphData.value);
            float lerpedValue = m_morphData.value + (diff * lerpVal);

            return std::make_shared<MorphTransformation>(m_morphData.index, lerpedValue);
        }

        return std::make_shared<MorphTransformation>(m_morphData.index, m_morphData.value);
    }

    std::shared_ptr<ITransformation> MorphTransformation::WeightedAverage(std::vector<std::pair<ITransformation&, float>> weightedTransformations)
    {
        float weightedValueSum = 0.0f;
        float totalWeight = 0.0f;

        for (auto pair : weightedTransformations)
        {
            ITransformation& transformation = pair.first;
            float weight = pair.second;

            if (transformation.GetTraitType() == GetTraitType())
            {
                Data morphData = *reinterpret_cast<Data*>(transformation.GetTransformationData());

                if (morphData.index == m_morphData.index)
                {
                    weightedValueSum += morphData.value * weight;
                }
            }

            totalWeight += weight;
        }

        return std::make_shared<MorphTransformation>(m_morphData.index, weightedValueSum / totalWeight);
    }
}