#include "TransformationCurve.h"

TransformationCurve::TransformationCurve(TraitType transformationType, std::string controllingProperty)
{
    m_curveTransformationType = transformationType;
    m_controllingProperty = controllingProperty;

    //add special-case origin key
    m_sortedKeys[0.0f] = TransformationCurve::Key();
    m_sortedKeys[0.0f].targetPropertyValue = 0.0f;
}

void TransformationCurve::AddKey(std::shared_ptr<ITransformation> transformation, float targetPropertyValue, float weight)
{
    AddKey(transformation, targetPropertyValue);

    if (m_sortedKeys.count(targetPropertyValue) == 1)
    {
        m_sortedKeys[targetPropertyValue].weight = weight;
    }
}

void TransformationCurve::AddKey(std::shared_ptr<ITransformation> transformation, float targetPropertyValue)
{
    if (transformation == nullptr || transformation->GetTraitType() != m_curveTransformationType)
        return;

    //if key already present, update values - else add new key
    if (m_sortedKeys.count(targetPropertyValue) == 1) 
    {
        m_sortedKeys[targetPropertyValue].targetPropertyValue = targetPropertyValue;
        m_sortedKeys[targetPropertyValue].transformation = transformation;
    }
    else
    {
        TransformationCurve::Key newKey = TransformationCurve::Key();
        newKey.transformation = transformation;
        newKey.targetPropertyValue = targetPropertyValue;

        m_sortedKeys[targetPropertyValue] = newKey;
    }

    if (targetPropertyValue == 0)
    {
        m_useImplicitOrigin = false;
    }
}

TransformationCurve::Result TransformationCurve::Evaluate(IActorTrackedProperties& properties, std::shared_ptr<ITransformation> defaultTransformation)
{
    float propertyValue = properties.GetTrackedPropertyValue(m_controllingProperty);
    std::pair<TransformationCurve::Key*, TransformationCurve::Key*> blendingKeys = GetBlendingKeysAt(propertyValue);
    UpdateImplicitOrigin(defaultTransformation);

    std::shared_ptr<ITransformation> lerpedTranformation;
    float lerpedWeight = 1.0f;

    if (blendingKeys.first != nullptr && blendingKeys.second != nullptr) //we have not extended beyond an extreme key
    {
        //get relative position on curve - only linear interpolation supported currently
        float lerpBetweenKeys = (propertyValue - blendingKeys.first->targetPropertyValue) / (blendingKeys.second->targetPropertyValue - blendingKeys.first->targetPropertyValue);

        //lerp the transformation values
        lerpedTranformation = blendingKeys.first->transformation->LerpTo(*blendingKeys.second->transformation, lerpBetweenKeys);
        lerpedWeight = blendingKeys.first->weight + (blendingKeys.second->weight - blendingKeys.first->weight) * lerpBetweenKeys;
    }
    else //we have extended beyond an extreme key
    {
        lerpedTranformation = (blendingKeys.first != nullptr) ? blendingKeys.first->transformation : blendingKeys.second->transformation;
        lerpedWeight = (blendingKeys.first != nullptr) ? blendingKeys.first->weight : blendingKeys.second->weight;
    }

    TransformationCurve::Result result = TransformationCurve::Result();
    result.transformation = lerpedTranformation;
    result.weight = lerpedWeight;

    return result;
}

const std::map<float, TransformationCurve::Key>& TransformationCurve::GetKeys() const
{
    return m_sortedKeys;
}

std::string TransformationCurve::GetControllingProperty()
{
    return m_controllingProperty;
}

TraitType TransformationCurve::GetType()
{
    return m_curveTransformationType;
}

std::pair<TransformationCurve::Key*, TransformationCurve::Key*> TransformationCurve::GetBlendingKeysAt(float currentPropertyValue)
{
    TransformationCurve::Key* lowKey = nullptr;
    TransformationCurve::Key* highKey = nullptr;

    for (std::pair<float, TransformationCurve::Key> keyMapping : m_sortedKeys)
    {
        TransformationCurve::Key key = keyMapping.second;

        if (currentPropertyValue < key.targetPropertyValue)
        {
            highKey = &m_sortedKeys[keyMapping.first];
            break;
        }
        lowKey = &m_sortedKeys[keyMapping.first];
    }

    return std::pair<TransformationCurve::Key*, TransformationCurve::Key*>(lowKey, highKey);
}

void TransformationCurve::UpdateImplicitOrigin(std::shared_ptr<ITransformation> defaultTransformation)
{
    if (m_useImplicitOrigin)
    {
        if (defaultTransformation->GetTraitType() != m_curveTransformationType)
            throw - 1;

        m_sortedKeys[0.0f].transformation = defaultTransformation;
    }
}