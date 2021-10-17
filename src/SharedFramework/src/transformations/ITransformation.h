#pragma once
#include "../TraitType.h"
#include <vector>
#include <memory>

/// <summary>
/// Interface to a bundle of data that can be acted on and applied to perform the transformation of some trait
/// </summary>
class ITransformation
{
public:
    /// <summary>Returns the type of transformation this result represents (and thus how the data should be interpreted)</summary>
    virtual TraitType GetTraitType() = 0;

    /// <summary>Returns the data needed to apply this transformation</summary>
    virtual void* GetTransformationData() = 0;

    /// <summary>Returns the linear blend from this to the other transformation, at the percent specified<summary>
    virtual std::shared_ptr<ITransformation> LerpTo(ITransformation &other, float lerpVal) = 0;
};