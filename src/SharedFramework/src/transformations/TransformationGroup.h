#pragma once
#include "../state/IActorTrackedProperties.h"
#include "ITransformation.h"
#include "TransformationCurve.h"

namespace DAF
{
    /// <summary>
    /// A structure that bundles all transformations of a particular visual trait
    /// Provides functions that return the result of combining the results of all transformations of the trait
    /// </summary>
    class TransformationGroup
    {
    public:
        /// <param name="transformationType">The type of transformation this group manages</param>
        TransformationGroup(TraitType transformationType);

        /// <summary>Get the result of applying all of the transformations for this group's trait</summary>
        /// <params name="properties">Reference to the tracked properties that govern the transformation result for this actor</params>
        /// <params name="defaultTransformation">The target actor's baseline trait value to adjust away from</params>
        /// <returns>The final combined value that should be fed back into this group's trait</returns>
        std::shared_ptr<ITransformation> Apply(IActorTrackedProperties& properties, std::shared_ptr<ITransformation> defaultTransformation);

        /// <summary>Add an additional transformation curve under the management of this transformation group</summary>
        /// <param name="curve">A curve maintaining the transformation of a trait governed by a single property</param>
        /// <param name="actorOverrideName">The actor this curve corresponds to, or "" if it applies to any actor</param>
        void AddTransformationCurve(std::shared_ptr<TransformationCurve> curve, std::string actorOverrideName);

    private:
        /// <summary>The type of transformation this group manages</summary>
        TraitType m_transformationType = TraitType::None;

        /// <summary>Collection of controlling properties mapped to transformation curves, indexed by actor name (for per-actor overrides)</summary>
        std::map<std::string, std::map<std::string, std::shared_ptr<TransformationCurve>>> m_transformationCurves;


        /// <summary>Evaluates all relevant curves under this group given this actor's properties and bundle their results in a list</summary>
        /// <params name="properties">Reference to the tracked properties that govern the transformation result for this actor</params>
        /// <params name="defaultTransformation">The target actor's baseline trait value to adjust away from</params>    /// 
        /// <returns>A list of the transformation results for each independent transformation curve</returns>
        std::vector<TransformationCurve::Result> EvaluateAllCurves(IActorTrackedProperties& properties, std::shared_ptr<ITransformation> defaultTransformation);
    };
}