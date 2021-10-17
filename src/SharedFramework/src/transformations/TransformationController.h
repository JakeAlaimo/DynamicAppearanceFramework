#pragma once
#include <string>
#include <vector>
#include <utility>
#include "ITransformation.h"
#include "../state/IActorTrackedProperties.h"

struct TransformationControllerOut {
    ITransformation* transformation;
    //additional blend configuration controls
};

struct TransformationTarget {
    float propertyValue;
    ITransformation* transformation;
};

class TransformationController
{
public:

    TransformationController(std::string controllingProperty);

    TransformationControllerOut Evaluate(IActorTrackedProperties* properties, ITransformation* defaultTransformation);

    void AddTransformationTarget(ITransformation* transformation, float targetPropertyValue);

private:

    std::pair<TransformationTarget, TransformationTarget> GetInterpolatingTransformationTargets(float currentPropertyValue);

    std::string m_controllingProperty;
    std::vector<TransformationTarget> m_sortedTransformationTargets;
};

