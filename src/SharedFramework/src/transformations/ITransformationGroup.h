#pragma once
#include "../state/IActorTrackedProperties.h"
#include "ITransformation.h"

/// <summary>
/// Interface of a structure that bundles all transformations of a particular visual trait
/// Provides functions that return the result of combining the results of all transformations of the trait
/// </summary>
class ITransformationGroup 
{
public:
    /// <summary>Get the result of applying all of the transformations for this group's trait</summary>
    /// <params name="properties">Reference to the tracked properties that govern the transformation result for this actor</params>
    /// <returns>The final combined value that should be fed back into this group's trait</returns>
    virtual std::shared_ptr<ITransformation> Apply(IActorTrackedProperties &properties) = 0;

    /// <summary>
    /// Get the result of removing all of this group's transformations from a trait
    /// </summary>
    /// <returns>The default, non-transformed base value for this group's trait</returns>
    virtual std::shared_ptr<ITransformation> Revert() = 0;

    /// <summary>
    /// Set the base trait value this transformation group modifies
    /// </summary>
    /// <param name="defaultValue">The new default value to use for this trait. The value without transformations</param>
    virtual void SetDefaultValue(std::shared_ptr<ITransformation> defaultValue) = 0;
};