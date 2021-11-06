#include "pch.h"
#include "CppUnitTest.h"
#include "MockActorTrackedProperties.h"
#include "MockTransformation.h"
#include "../SharedFramework/src/transformations/TransformationCurve.h"
#include "../SharedFramework/src/transformations/TransformationGroup.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SharedFrameworkTest
{
	TEST_CLASS(TraitTransformationGroupTest)
	{
	public:
		TEST_METHOD(Constructor_Succeeds)
		{
			std::shared_ptr<MockTransformation> defaultTransformation = std::make_shared<MockTransformation>(0.0f);
			TransformationGroup group = TransformationGroup(TraitType::Morph);
		}

		TEST_METHOD(ApplyStandardValues_Succeeds)
		{
			MockActorTrackedProperties properties = MockActorTrackedProperties();
			std::shared_ptr<MockTransformation> defaultTransformation = std::make_shared<MockTransformation>(0.0f);

			TransformationGroup group = TransformationGroup(TraitType::Morph);
			std::shared_ptr<TransformationCurve> assaultsCurve = std::make_shared<TransformationCurve>(TraitType::Morph, "Assaults");
			std::shared_ptr<TransformationCurve> noSleepCurve = std::make_shared<TransformationCurve>(TraitType::Morph, "Days Without Sleep");

			std::shared_ptr<MockTransformation> transformation = std::make_shared<MockTransformation>(1.0f);

			assaultsCurve->AddKey(transformation, 10.0f); // mocked property 100% this key
			noSleepCurve->AddKey(transformation, 6.0f); // mocked property 50% to this key

			group.AddTransformationCurve(assaultsCurve, "");
			group.AddTransformationCurve(noSleepCurve, "");

			std::shared_ptr<ITransformation> resultTransformation = group.Apply(properties, defaultTransformation);

			float resultValue = *(float*)resultTransformation->GetTransformationData();

			// assaultCurve.Eval = 1, noSleepCurve.Eval = 0.5. (1 + 0.5) / 2 = 0.75
			Assert::AreEqual(0.75f, resultValue);
		}

		TEST_METHOD(ApplyWeightedValues_Succeeds)
		{
			MockActorTrackedProperties properties = MockActorTrackedProperties();
			std::shared_ptr<MockTransformation> defaultTransformation = std::make_shared<MockTransformation>(0.0f);

						TransformationGroup group = TransformationGroup(TraitType::Morph);
			std::shared_ptr<TransformationCurve> assaultsCurve = std::make_shared<TransformationCurve>(TraitType::Morph, "Assaults");
			std::shared_ptr<TransformationCurve> noSleepCurve = std::make_shared<TransformationCurve>(TraitType::Morph, "Days Without Sleep");

			std::shared_ptr<MockTransformation> transformation = std::make_shared<MockTransformation>(1.0f);

			assaultsCurve->AddKey(transformation, 10.0f, 2.0f); // mocked property 100% this key and weight
			noSleepCurve->AddKey(transformation, 6.0f, 10.0f); // mocked property 50% to this key and weight

			group.AddTransformationCurve(assaultsCurve, "");
			group.AddTransformationCurve(noSleepCurve, "");

			std::shared_ptr<ITransformation> resultTransformation = group.Apply(properties, defaultTransformation);

			float resultValue = *(float*)resultTransformation->GetTransformationData();

			// assaultCurve.Eval = 1, noSleepCurve.Eval = 0.5. (1 * 2 + 0.5 * 5.5) / 7.5 = 0.583333. Note the weighting
			Assert::AreEqual(4.75f / 7.5f, resultValue);
		}

		TEST_METHOD(ApplyNoCurvesReturnsDefault_Succeeds)
		{
			MockActorTrackedProperties properties = MockActorTrackedProperties();
			std::shared_ptr<MockTransformation> defaultTransformation = std::make_shared<MockTransformation>(10.0f);

			TransformationGroup group = TransformationGroup(TraitType::Morph);
			std::shared_ptr<ITransformation> resultTransformation = group.Apply(properties, defaultTransformation);

			float resultValue = *(float*)resultTransformation->GetTransformationData();

			// default == 10
			Assert::AreEqual(10.0f, resultValue);
		}

		TEST_METHOD(ApplyIgnoresCurvesOfWrongType_Succeeds)
		{
			MockActorTrackedProperties properties = MockActorTrackedProperties();
			std::shared_ptr<MockTransformation> defaultTransformation = std::make_shared<MockTransformation>(0.0f);

			TransformationGroup group = TransformationGroup(TraitType::Morph);
			std::shared_ptr<TransformationCurve> assaultsCurve = std::make_shared<TransformationCurve>(TraitType::Morph, "Assaults");
			std::shared_ptr<TransformationCurve> noSleepCurve = std::make_shared<TransformationCurve>(TraitType::None, "Days Without Sleep");

			std::shared_ptr<MockTransformation> transformation = std::make_shared<MockTransformation>(1.0f);

			assaultsCurve->AddKey(transformation, 10.0f); // mocked property 100% this key
			noSleepCurve->AddKey(transformation, 6.0f); // mocked property 50% to this key

			group.AddTransformationCurve(assaultsCurve, "");
			group.AddTransformationCurve(noSleepCurve, "");

			std::shared_ptr<ITransformation> resultTransformation = group.Apply(properties, defaultTransformation);

			float resultValue = *(float*)resultTransformation->GetTransformationData();

			// assaultCurve.Eval = 1, noSleepCurve.Eval = 0.5, but is ignored due to being wrong type
			Assert::AreEqual(1.0f, resultValue);
		}

		TEST_METHOD(ApplyUsesActorOverride_Succeeds)
		{
			MockActorTrackedProperties properties = MockActorTrackedProperties();
			std::shared_ptr<MockTransformation> defaultTransformation = std::make_shared<MockTransformation>(0.0f);

			TransformationGroup group = TransformationGroup(TraitType::Morph);
			std::shared_ptr<TransformationCurve> assaultsCurve = std::make_shared<TransformationCurve>(TraitType::Morph, "Assaults");
			std::shared_ptr<TransformationCurve> mockActorAssaultsCurve = std::make_shared<TransformationCurve>(TraitType::Morph, "Assaults");

			std::shared_ptr<MockTransformation> transformation = std::make_shared<MockTransformation>(1.0f);

			assaultsCurve->AddKey(transformation, 10.0f); // mocked property 100% this key
			mockActorAssaultsCurve->AddKey(transformation, 100.0f); // mocked property 10% to this key

			group.AddTransformationCurve(assaultsCurve, "");
			group.AddTransformationCurve(mockActorAssaultsCurve, "MockActor");

			std::shared_ptr<ITransformation> resultTransformation = group.Apply(properties, defaultTransformation);

			float resultValue = *(float*)resultTransformation->GetTransformationData();

			// assaultCurve.Eval = 1, mockActorAssaultsCurve.Eval = 0.1. Mock actor override value used in place
			Assert::AreEqual(0.1f, resultValue);
		}

		TEST_METHOD(ApplyNonOverridesSameForAnyActor_Succeeds)
		{
			MockActorTrackedProperties properties = MockActorTrackedProperties();
			std::shared_ptr<MockTransformation> defaultTransformation = std::make_shared<MockTransformation>(0.0f);

			TransformationGroup group = TransformationGroup(TraitType::Morph);
			std::shared_ptr<TransformationCurve> assaultsCurve = std::make_shared<TransformationCurve>(TraitType::Morph, "Assaults");

			std::shared_ptr<MockTransformation> transformation = std::make_shared<MockTransformation>(1.0f);

			assaultsCurve->AddKey(transformation, 10.0f); // mocked property 100% this key
			group.AddTransformationCurve(assaultsCurve, "");

			std::shared_ptr<ITransformation> resultTransformation = group.Apply(properties, defaultTransformation);
			float firstNameResultValue = *(float*)resultTransformation->GetTransformationData();

			properties.SetActorName("Different Name");

			resultTransformation = group.Apply(properties, defaultTransformation);
			float secondNameResultValue = *(float*)resultTransformation->GetTransformationData();

			Assert::AreEqual(firstNameResultValue, secondNameResultValue);
		}

		TEST_METHOD(ApplyDefaultNull_Fails)
		{
			MockActorTrackedProperties properties = MockActorTrackedProperties();
			std::shared_ptr<MockTransformation> defaultTransformation = nullptr;

			TransformationGroup group = TransformationGroup(TraitType::Morph);

			bool succeeded;
			try
			{
				std::shared_ptr<ITransformation> resultTransformation = group.Apply(properties, defaultTransformation);
				succeeded = true;
			}
			catch (...)
			{
				succeeded = false;
			}
			Assert::IsFalse(succeeded);
		}

		TEST_METHOD(ApplyDefaultWrongType_Fails)
		{
			MockActorTrackedProperties properties = MockActorTrackedProperties();
			std::shared_ptr<MockTransformation> defaultTransformation = std::make_shared<MockTransformation>(0.0f);
			defaultTransformation->ForceTraitType(TraitType::None); //purely a testing utility, transformations will have a static trait type

			TransformationGroup group = TransformationGroup(TraitType::Morph);

			bool succeeded;
			try
			{
				std::shared_ptr<ITransformation> resultTransformation = group.Apply(properties, defaultTransformation);
				succeeded = true;
			}
			catch (...)
			{
				succeeded = false;
			}
			Assert::IsFalse(succeeded);
		}
	};
}