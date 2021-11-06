#pragma once
#include <string>
#include <map>
#include <memory>
#include "../transformations/ITransformation.h"

/// <summary>
/// Manages the state of an actor's default transformations for every trait. 
/// These defaults are the baseline that is adjusted from via dynamic adjustment
/// </summary>
class DefaultTransformations 
{
public:

    /// <summary>Given a trait identifier, sets the actor's default transformation for that trait</summary>
    /// <param name="traitID">String identifier to a specific dynamic trait</param>
    /// <param name="defaultTransformation">The baseline transformation value to dynamically adjust from for this actor's trait</param>
    void SetDefaultOfTrait(std::string traitID, std::shared_ptr<ITransformation> defaultTransformation);

    /// <summary>Given a trait identifier, returns the actor's default transformation for that trait</summary>
    /// <param name="traitID">String identifier to a specific dynamic trait</param>
    /// <returns>The baseline transformation value to dynamically adjust from for this actor's trait</returns>
    std::shared_ptr<ITransformation> GetDefaultOfTrait(std::string traitID);

    /// <summary>Returns a read-only mapping of all trait IDs to their default transformations</summary>
    const std::map<std::string, std::shared_ptr<ITransformation>>& GetAllDefaults() const;

private:

    /// <summary>Internal mapping of traitIDs to their baseline transformation state</summary>
    std::map<std::string, std::shared_ptr<ITransformation>> m_defaultTransformationsByTrait;
};