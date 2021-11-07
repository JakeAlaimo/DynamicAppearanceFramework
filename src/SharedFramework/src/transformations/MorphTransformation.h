#pragma once
#include "ITransformation.h"

namespace DAF
{
    /// <summary>
    /// A transformation implementation specifying how actor morph transformations should be laid out and managed
    /// </summary>
    class MorphTransformation : public ITransformation
    {
    #pragma region Helper Classes
    public:
        /// <summary>The underlying POD structure of a morph transformation</summary>
        struct Data
        {
            /// <summary>Identifies which morph this transformation corresponds to</summary>
            int index; 

            /// <summary>Identifies the value the morph should be set to - vanilla interval [0 - 1.0]</summary>
            float value;
        };
    #pragma endregion

    public:
        MorphTransformation(int morphIndex, float morphValue);

        /// <summary>Returns the type of transformation this result represents (and thus how the data should be interpreted)</summary>
        virtual TraitType GetTraitType();

        /// <summary>Returns the data needed to apply this transformation</summary>
        virtual void* GetTransformationData();

        /// <summary>Returns the linear blend from this to the other transformation, at the percent specified<summary>
        virtual std::shared_ptr<ITransformation> LerpTo(ITransformation& other, float lerpVal);

        /// <summary>Returns the blend of all transformations in the list given their weighting factors<summary>
        virtual std::shared_ptr<ITransformation> WeightedAverage(std::vector<std::pair<ITransformation&, float>> weightedTransformations);

    private:
        /// <summary>This transformation's internal data representation</summary>
        Data m_morphData;
    };
}
