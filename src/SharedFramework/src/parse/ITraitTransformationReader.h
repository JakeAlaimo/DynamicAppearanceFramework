#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "../transformations/ITransformationGroup.h"

/// <summary>
/// Interface to a utility that reads in trait transformation specification files and provides 
/// the object representations of those transformations
/// </summary>
class ITraitTransformationReader
{
public:
    /// <summary>Provides transformation group indexed by the trait that group governs</summary>
    virtual std::map<std::string, std::shared_ptr<ITransformationGroup>> GetTransformationGroupsByTraitID() = 0;

    /// <summary>Gets a list of all the tracked properties that govern traits</summary>
    virtual std::vector<std::string> GetTrackedProperties() = 0;

    /// <summary>Provides a map of tracked properties to the traits they govern</summary>
    virtual std::map<std::string, std::vector<std::string>> GetManagedTraitIDsByTrackedProperty() = 0;
};

