#pragma once
#include <string>
#include <map>
#include <memory>
#include "ITransformation.h"
#include "../state/IActorTrackedProperties.h"

/// <summary>
/// Drives the blending between transformations of a visual trait according to an arbitrary controlling property value
/// </summary>
class TransformationCurve
{
#pragma region Helper Classes
public:

    struct Result 
    {
        std::shared_ptr<ITransformation> transformation;
        float weight = 1.0f;
    };

private:

    struct Key
    {
        float targetPropertyValue = 0.0f;
        std::shared_ptr<ITransformation> transformation;
        float weight = 1.0f;
    };

#pragma endregion

public:
    /// <param name="transformationType">The type of transformation this curve manages</param>
    /// <param name="controllingProperty">The tracked property this curve is driven by</param>
    TransformationCurve(TraitType transformationType, std::string controllingProperty);

    /// <summary>Add a transformation to the curve with the specified parameters</summary>
    /// <param name="transformation">The trait transformation to place on the curve</param>
    /// <param name="targetPropertyValue">The point on the curve to place the key</param>
    /// <param name="weight">The blend weight of the trait transformation</param>
    void AddKey(std::shared_ptr<ITransformation> transformation, float targetPropertyValue, float weight);

    /// <summary>Add a transformation to the curve with the specified parameters</summary>
    /// <param name="transformation">The trait transformation to place on the curve</param>
    /// <param name="targetPropertyValue">The point on the curve to place the key</param>
    void AddKey(std::shared_ptr<ITransformation> transformation, float targetPropertyValue);

    /// <summary>Interpolates between transformations on this curve according to the current property value</summary>
    /// <param name="properties">The current tracked property state to feed into the evaluation</param>
    /// <param name="defaultTransformation">The base, untransformed value of this trait</param>
    /// <returns>The interpolated transformation value, as well as controls for how to use it within a group</returns>
    Result Evaluate(IActorTrackedProperties &properties, std::shared_ptr<ITransformation> defaultTransformation);

    /// <summary>Return a read-only map of the keys in this curve</summary>
    const std::map<float, TransformationCurve::Key>& GetKeys() const;

private:
    /// <summary>The type of transformation this curve manages</summary>
    TraitType m_curveTransformationType = TraitType::None;

    /// <summary>The tracked property this curve is driven by</summary>
    std::string m_controllingProperty;

    /// <summary>The ordered transformation keys that make up this curve</summary>
    std::map<float, Key> m_sortedKeys;

    /// <summary>Whether to substitute the default transformation as an origin key</summary>
    bool m_useImplicitOrigin = true;


    /// <summary>Gets the pair of keys to blend between given the current position along the curve</summary>
    /// <param name="currentPropertyValue">The point along the curve to evaluate</param>
    /// <returns>The pair of keys the current property value sits between, ordered smaller to larger</returns>
    std::pair<Key*, Key*> GetBlendingKeysAt(float currentPropertyValue);

    /// <summary>Update the implied starting value to adjust from at this curve's origin</summary>
    /// <param name="defaultTransformation">This transformation's base value, without dynamic modification</param>
    void UpdateImplicitOrigin(std::shared_ptr<ITransformation> defaultTransformation);
};

