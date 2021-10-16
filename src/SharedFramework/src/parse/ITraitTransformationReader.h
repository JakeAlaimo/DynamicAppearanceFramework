#pragma once
#include <map>
#include <vector>
#include <string>
#include "../transformations/ITransformationGroup.h"

/// <summary>
/// Interface to a utility that reads in trait transformation specification files and provides 
/// the object representations of those transformations
/// </summary>
class ITraitTransformationReader
{
    virtual std::map<std::string, ITransformationGroup> GetTransformationGroupsByTraitID() = 0;

    virtual std::vector<std::string> GetTrackedProperties() = 0;
    virtual std::map<std::string, std::vector<std::string>> GetTrackedPropertiesByTraitID() = 0;
};

