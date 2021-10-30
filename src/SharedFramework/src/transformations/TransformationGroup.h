#pragma once
#include "../state/IActorTrackedProperties.h"
#include "ITransformation.h"
#include "TransformationCurve.h"

/// <summary>
/// A structure that bundles all transformations of a particular visual trait
/// Provides functions that return the result of combining the results of all transformations of the trait
/// </summary>
class TransformationGroup 
{
public:
    /// <param name="transformationType">The type of transformation this group manages</param>
    /// <param name="defaultTransformation">The new default value to use for this trait. The value without transformations</param>
    TransformationGroup(TraitType transformationType, std::shared_ptr<ITransformation> defaultTransformation);

    /// <summary>Get the result of applying all of the transformations for this group's trait</summary>
    /// <params name="properties">Reference to the tracked properties that govern the transformation result for this actor</params>
    /// <returns>The final combined value that should be fed back into this group's trait</returns>
    std::shared_ptr<ITransformation> Apply(IActorTrackedProperties &properties);

    /// <summary>
    /// Get the result of removing all of this group's transformations from a trait
    /// </summary>
    /// <returns>The default, non-transformed base value for this group's trait</returns>
    std::shared_ptr<ITransformation> Revert();

    /// <summary>
    /// Set the base trait value this transformation group modifies
    /// </summary>
    /// <param name="defaultTransformation">The new default value to use for this trait. The value without transformations</param>
    void SetDefaultTransformation(std::shared_ptr<ITransformation> defaultTransformation);

    /// <summary>Add an additional transformation curve under the management of this transformation group</summary>
    /// <param name="curve">A curve maintaining the transformation of a trait governed by a single property</param>
    /// <param name="actorOverrideName">The actor this curve corresponds to, or "" if it applies to any actor</param>
    void AddTransformationCurve(std::shared_ptr<TransformationCurve> curve, std::string actorOverrideName);

private:
    /// <summary>The type of transformation this group manages</summary>
    TraitType m_transformationType = TraitType::None;

    /// <summary>Collection of controlling properties mapped to transformation curves, indexed by actor name (for per-actor overrides)</summary>
    std::map<std::string, std::map<std::string, std::shared_ptr<TransformationCurve>>> m_transformationCurves;

    /// <summary>The base value of the trait this group modifies</summary>
    std::shared_ptr<ITransformation> m_defaultTransformation;


    /// <summary>Evaluates all relevant curves under this group given this actor's properties and bundle their results in a list</summary>
    /// <params name="properties">Reference to the tracked properties that govern the transformation result for this actor</params>
    /// <returns>A list of the transformation results for each independent transformation curve</returns>
    std::vector<TransformationCurve::Result> EvaluateAllCurves(IActorTrackedProperties& properties);
};