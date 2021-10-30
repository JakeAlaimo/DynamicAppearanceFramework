#include "pch.h"
#include "CppUnitTest.h"
#include "MockActorTrackedProperties.h"
#include "MockTransformation.h"
#include "../SharedFramework/src/transformations/TransformationCurve.h"
#include "../SharedFramework/src/transformations/TransformationGroup.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SharedFrameworkTest
{
	TEST_CLASS(TraitTransformatioGroupTest)
	{
	public:
		TEST_METHOD(Constructor_Succeeds)
		{
			std::shared_ptr<MockTransformation> defaultTransformation = std::make_shared<MockTransformation>(0.0f);
			TransformationGroup group = TransformationGroup(TraitType::Morph, defaultTransformation);
		}

		TEST_METHOD(ConstructorNoDefault_Fails)
		{
			bool succeeded;
			try
			{
				TransformationGroup group = TransformationGroup(TraitType::Morph, nullptr);
				succeeded = true;
			}
			catch (...)
			{
				succeeded = false;
			}
			Assert::IsFalse(succeeded);
		}

		TEST_METHOD(ConstructorDefaultWrongType_Fails)
		{
			bool succeeded;
			try
			{
				std::shared_ptr<MockTransformation> defaultTransformation = std::make_shared<MockTransformation>(0.0f);
				TransformationGroup group = TransformationGroup(TraitType::None, defaultTransformation);
				succeeded = true;
			}
			catch (...)
			{
				succeeded = false;
			}
			Assert::IsFalse(succeeded);
		}

		TEST_METHOD(ApplyStandardValues_Succeeds)
		{
			MockActorTrackedProperties properties = MockActorTrackedProperties();
			std::shared_ptr<MockTransformation> defaultTransformation = std::make_shared<MockTransformation>(0.0f);

			TransformationGroup group = TransformationGroup(TraitType::Morph, defaultTransformation);
			TransformationCurve assaultsCurve = TransformationCurve(TraitType::Morph, "Assaults");
			TransformationCurve noSleepCurve = TransformationCurve(TraitType::Morph, "Days Without Sleep");

			std::shared_ptr<MockTransformation> transformation = std::make_shared<MockTransformation>(1.0f);

			assaultsCurve.AddKey(transformation, 10.0f); // mocked property 100% this key
			noSleepCurve.AddKey(transformation, 6.0f); // mocked property 50% to this key

			group.AddTransformationCurve(assaultsCurve, "");
			group.AddTransformationCurve(noSleepCurve, "");

			std::shared_ptr<ITransformation> resultTransformation = group.Apply(properties);

			float resultValue = *(float*)resultTransformation->GetTransformationData();

			// assaultCurve.Eval = 1, noSleepCurve.Eval = 0.5. (1 + 0.5) / 2 = 0.75
			Assert::AreEqual(0.75f, resultValue);
		}

		TEST_METHOD(ApplyWeightedValues_Succeeds)
		{
			MockActorTrackedProperties properties = MockActorTrackedProperties();
			std::shared_ptr<MockTransformation> defaultTransformation = std::make_shared<MockTransformation>(0.0f);

			TransformationGroup group = TransformationGroup(TraitType::Morph, defaultTransformation);
			TransformationCurve assaultsCurve = TransformationCurve(TraitType::Morph, "Assaults");
			TransformationCurve noSleepCurve = TransformationCurve(TraitType::Morph, "Days Without Sleep");

			std::shared_ptr<MockTransformation> transformation = std::make_shared<MockTransformation>(1.0f);

			assaultsCurve.AddKey(transformation, 10.0f, 2.0f); // mocked property 100% this key and weight
			noSleepCurve.AddKey(transformation, 6.0f, 10.0f); // mocked property 50% to this key and weight

			group.AddTransformationCurve(assaultsCurve, "");
			group.AddTransformationCurve(noSleepCurve, "");

			std::shared_ptr<ITransformation> resultTransformation = group.Apply(properties);

			float resultValue = *(float*)resultTransformation->GetTransformationData();

			// assaultCurve.Eval = 1, noSleepCurve.Eval = 0.5. (1 * 2 + 0.5 * 5.5) / 7.5 = 0.583333. Note the weighting
			Assert::AreEqual(4.75f / 7.5f, resultValue);
		}

		TEST_METHOD(ApplyNoCurvesReturnsDefault_Succeeds)
		{
			MockActorTrackedProperties properties = MockActorTrackedProperties();
			std::shared_ptr<MockTransformation> defaultTransformation = std::make_shared<MockTransformation>(10.0f);

			TransformationGroup group = TransformationGroup(TraitType::Morph, defaultTransformation);
			std::shared_ptr<ITransformation> resultTransformation = group.Apply(properties);

			float resultValue = *(float*)resultTransformation->GetTransformationData();

			// default == 10
			Assert::AreEqual(10.0f, resultValue);
		}

		TEST_METHOD(ApplyIgnoresCurvesOfWrongType_Succeeds)
		{
			MockActorTrackedProperties properties = MockActorTrackedProperties();
			std::shared_ptr<MockTransformation> defaultTransformation = std::make_shared<MockTransformation>(0.0f);

			TransformationGroup group = TransformationGroup(TraitType::Morph, defaultTransformation);
			TransformationCurve assaultsCurve = TransformationCurve(TraitType::Morph, "Assaults");
			TransformationCurve noSleepCurve = TransformationCurve(TraitType::None, "Days Without Sleep");

			std::shared_ptr<MockTransformation> transformation = std::make_shared<MockTransformation>(1.0f);

			assaultsCurve.AddKey(transformation, 10.0f); // mocked property 100% this key
			noSleepCurve.AddKey(transformation, 6.0f); // mocked property 50% to this key

			group.AddTransformationCurve(assaultsCurve, "");
			group.AddTransformationCurve(noSleepCurve, "");

			std::shared_ptr<ITransformation> resultTransformation = group.Apply(properties);

			float resultValue = *(float*)resultTransformation->GetTransformationData();

			// assaultCurve.Eval = 1, noSleepCurve.Eval = 0.5, but is ignored due to being wrong type
			Assert::AreEqual(1.0f, resultValue);
		}

		TEST_METHOD(ApplyUsesActorOverride_Succeeds)
		{
			MockActorTrackedProperties properties = MockActorTrackedProperties();
			std::shared_ptr<MockTransformation> defaultTransformation = std::make_shared<MockTransformation>(0.0f);

			TransformationGroup group = TransformationGroup(TraitType::Morph, defaultTransformation);
			TransformationCurve assaultsCurve = TransformationCurve(TraitType::Morph, "Assaults");
			TransformationCurve mockActorAssaultsCurve = TransformationCurve(TraitType::Morph, "Assaults");

			std::shared_ptr<MockTransformation> transformation = std::make_shared<MockTransformation>(1.0f);

			assaultsCurve.AddKey(transformation, 10.0f); // mocked property 100% this key
			mockActorAssaultsCurve.AddKey(transformation, 100.0f); // mocked property 10% to this key

			group.AddTransformationCurve(assaultsCurve, "");
			group.AddTransformationCurve(mockActorAssaultsCurve, "MockActor");

			std::shared_ptr<ITransformation> resultTransformation = group.Apply(properties);

			float resultValue = *(float*)resultTransformation->GetTransformationData();

			// assaultCurve.Eval = 1, mockActorAssaultsCurve.Eval = 0.1. Mock actor override value used in place
			Assert::AreEqual(0.1f, resultValue);
		}

		TEST_METHOD(ApplyNonOverridesSameForAnyActor_Succeeds)
		{
			MockActorTrackedProperties properties = MockActorTrackedProperties();
			std::shared_ptr<MockTransformation> defaultTransformation = std::make_shared<MockTransformation>(0.0f);

			TransformationGroup group = TransformationGroup(TraitType::Morph, defaultTransformation);
			TransformationCurve assaultsCurve = TransformationCurve(TraitType::Morph, "Assaults");

			std::shared_ptr<MockTransformation> transformation = std::make_shared<MockTransformation>(1.0f);

			assaultsCurve.AddKey(transformation, 10.0f); // mocked property 100% this key
			group.AddTransformationCurve(assaultsCurve, "");

			std::shared_ptr<ITransformation> resultTransformation = group.Apply(properties);
			float firstNameResultValue = *(float*)resultTransformation->GetTransformationData();

			properties.SetActorName("Different Name");

			resultTransformation = group.Apply(properties);
			float secondNameResultValue = *(float*)resultTransformation->GetTransformationData();

			Assert::AreEqual(firstNameResultValue, secondNameResultValue);
		}

		TEST_METHOD(RevertReturnsDefault_Succeeds)
		{
			std::shared_ptr<MockTransformation> defaultTransformation = std::make_shared<MockTransformation>(10.0f);
			std::shared_ptr<MockTransformation> defaultTransformation2 = std::make_shared<MockTransformation>(15.0f);

			TransformationGroup group = TransformationGroup(TraitType::Morph, defaultTransformation);

			float resultValue = *(float*)group.Revert()->GetTransformationData();
			Assert::AreEqual(10.0f, resultValue);

			group.SetDefaultTransformation(defaultTransformation2);

			resultValue = *(float*)group.Revert()->GetTransformationData();
			Assert::AreEqual(15.0f, resultValue);
		}

		TEST_METHOD(SetDefaultToNull_Fails)
		{
			std::shared_ptr<MockTransformation> defaultTransformation = std::make_shared<MockTransformation>(0.0f);
			TransformationGroup group = TransformationGroup(TraitType::Morph, defaultTransformation);

			bool succeeded;
			try
			{
				group.SetDefaultTransformation(nullptr);
				succeeded = true;
			}
			catch (...)
			{
				succeeded = false;
			}
			Assert::IsFalse(succeeded);
		}

		TEST_METHOD(SetDefaultToWrongType_Fails)
		{
			std::shared_ptr<MockTransformation> defaultTransformation = std::make_shared<MockTransformation>(0.0f);
			TransformationGroup group = TransformationGroup(TraitType::Morph, defaultTransformation);

			std::shared_ptr<MockTransformation> newDefaultTransformation = std::make_shared<MockTransformation>(0.0f);
			newDefaultTransformation->ForceTraitType(TraitType::None); //purely a testing utility, transformations will have a static trait type

			bool succeeded;
			try
			{
				group.SetDefaultTransformation(newDefaultTransformation);
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