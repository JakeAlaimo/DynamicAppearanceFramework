#include "TransformationGroup.h"

TransformationGroup::TransformationGroup(TraitType transformationType, std::shared_ptr<ITransformation> defaultTransformation)
{
    m_transformationType = transformationType;
    SetDefaultTransformation(defaultTransformation);
}

std::shared_ptr<ITransformation> TransformationGroup::Apply(IActorTrackedProperties& properties)
{
    std::vector<TransformationCurve::Result> curveResults = EvaluateAllCurves(properties);

    if (curveResults.size() == 0)
    {
        return m_defaultTransformation;
    }

    // parse the curve results into the proper format
    std::vector<std::pair<ITransformation&, float>> weightedTransforms;
    for (TransformationCurve::Result result : curveResults)
    {
        weightedTransforms.push_back(std::pair<ITransformation&, float>(*result.transformation, result.weight));
    }

    // WeightedAverage should be stateless, it should be irrelevant which transformation actually calls it (unless explicitly needed)
    return curveResults[0].transformation->WeightedAverage(weightedTransforms);
}

std::shared_ptr<ITransformation> TransformationGroup::Revert()
{
    return m_defaultTransformation;
}

void TransformationGroup::SetDefaultTransformation(std::shared_ptr<ITransformation> defaultTransformation)
{
    if (defaultTransformation == nullptr || m_transformationType != defaultTransformation->GetTraitType())
    {
        throw "Default transformation not valid for TransformationGroup";
    }

    m_defaultTransformation = defaultTransformation;
}

void TransformationGroup::AddTransformationCurve(std::shared_ptr<TransformationCurve> curve, std::string actorOverrideName)
{
    if (curve->GetType() != m_transformationType)
    {
        return;
    }

    // add to the list of curves, indexed by property and then actor name
    // note: repeat entries will be replaced
    std::string controllingProperty = curve->GetControllingProperty();
    m_transformationCurves[controllingProperty][actorOverrideName] = curve;
}

std::vector<TransformationCurve::Result> TransformationGroup::EvaluateAllCurves(IActorTrackedProperties& properties)
{
    std::string targetActor = properties.GetActorName();
    std::vector<TransformationCurve::Result> curveResults;

    for (auto curveGrouping : m_transformationCurves)
    {
        std::string propertyName = curveGrouping.first;
        std::map<std::string, std::shared_ptr<TransformationCurve>> perActorCurveMapping = curveGrouping.second;
        TransformationCurve::Result result;

        if (perActorCurveMapping.count(targetActor) == 1) // if actor specific override specified for this actor, use that
        {
            result = perActorCurveMapping[targetActor]->Evaluate(properties, m_defaultTransformation);
        }
        else // otherwise, use the general curve
        {
            result = perActorCurveMapping[""]->Evaluate(properties, m_defaultTransformation);
        }

        curveResults.push_back(result);
    }

    return curveResults;
}
