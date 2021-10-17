#pragma once
#include <string>
#include <vector>
#include <utility>
#include <memory>
#include "ITransformation.h"
#include "../state/IActorTrackedProperties.h"

struct TransformationControllerOut {
    std::shared_ptr<ITransformation> transformation;
    //additional blend configuration controls
};

struct TransformationTarget {
    float propertyValue;
    std::shared_ptr<ITransformation> transformation;
};

class TransformationController
{
public:

    TransformationController(std::string controllingProperty);

    TransformationControllerOut Evaluate(IActorTrackedProperties &properties, ITransformation &defaultTransformation);

    void AddTransformationTarget(std::shared_ptr<ITransformation> transformation, float targetPropertyValue);

private:

    std::pair<TransformationTarget, TransformationTarget> GetInterpolatingTransformationTargets(float currentPropertyValue);

    std::string m_controllingProperty;
    std::vector<TransformationTarget> m_sortedTransformationTargets;
};

