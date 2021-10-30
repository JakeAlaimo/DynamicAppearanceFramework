#include "pch.h"
#include "CppUnitTest.h"
#include "MockActorTrackedProperties.h"
#include "../SharedFramework/src/transformations/TransformationCurve.h"

#include "MockTransformation.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SharedFrameworkTest
{
	TEST_CLASS(TransformationCurveTest)
	{
	public:
		TEST_METHOD(Constructor_Succeeds)
		{
			TransformationCurve curve = TransformationCurve(TraitType::Morph, "Assaults");
		}

		TEST_METHOD(GetControllingProperty_Succeeds)
		{
			TransformationCurve curve = TransformationCurve(TraitType::Morph, "Assaults");
			Assert::AreEqual((std::string)"Assaults", curve.GetControllingProperty());
		}

		TEST_METHOD(GetType_Succeeds)
		{
			TransformationCurve curve = TransformationCurve(TraitType::Morph, "Assaults");
			Assert::AreEqual((int)TraitType::Morph, (int)curve.GetType());
		}

		TEST_METHOD(AddingValidKeys_Succeeds)
		{
			TransformationCurve curve = TransformationCurve(TraitType::Morph, "Assaults");

			std::shared_ptr<MockTransformation> smallTransform = std::make_shared<MockTransformation>(0.0f);
			std::shared_ptr<MockTransformation> bigTransform = std::make_shared<MockTransformation>(1.0f);

			curve.AddKey(smallTransform, 0.0f);
			curve.AddKey(bigTransform, 20.0f);
			curve.AddKey(bigTransform, -20.0f);

			//explicit origin + 2 other valid keys
			Assert::AreEqual((size_t)3, curve.GetKeys().size());

			float keyData = *(reinterpret_cast<float*>(curve.GetKeys().at(0.0f).transformation->GetTransformationData()));
			Assert::AreEqual(0.0f, keyData);

			keyData = *(reinterpret_cast<float*>(curve.GetKeys().at(20.0f).transformation->GetTransformationData()));
			Assert::AreEqual(1.0f, keyData);

			keyData = *(reinterpret_cast<float*>(curve.GetKeys().at(-20.0f).transformation->GetTransformationData()));
			Assert::AreEqual(1.0f, keyData);
		}

		TEST_METHOD(AddingNullKey_Fails)
		{
			TransformationCurve curve = TransformationCurve(TraitType::Morph, "Assaults");
			curve.AddKey(nullptr, 1.0f);

			//only origin key
			Assert::AreEqual((size_t)1, curve.GetKeys().size());
		}

		TEST_METHOD(AddingKeyOfDifferentType_Fails)
		{
			TransformationCurve curve = TransformationCurve(TraitType::None, "Assaults");

			std::shared_ptr<MockTransformation> morphTransform = std::make_shared<MockTransformation>(0.0f);
			curve.AddKey(morphTransform, 1.0f);

			//only origin key
			Assert::AreEqual((size_t)1, curve.GetKeys().size());
		}

		TEST_METHOD(AddingKeyAtSamePositionReplaces_Succeeds)
		{
			TransformationCurve curve = TransformationCurve(TraitType::Morph, "Assaults");

			std::shared_ptr<MockTransformation> morphTransform = std::make_shared<MockTransformation>(0.0f);
			std::shared_ptr<MockTransformation> morphTransform2 = std::make_shared<MockTransformation>(1.0f);

			curve.AddKey(morphTransform, 1.0f);
			curve.AddKey(morphTransform2, 1.0f);

			//size = origin + only 1 added key
			Assert::AreEqual((size_t)2, curve.GetKeys().size());

			float keyData = *(reinterpret_cast<float *>(curve.GetKeys().at(1.0f).transformation->GetTransformationData()));
			Assert::AreEqual(1.0f, keyData);
		}

		TEST_METHOD(AddingKeyWithWeight_Succeeds)
		{
			TransformationCurve curve = TransformationCurve(TraitType::Morph, "Assaults");

			std::shared_ptr<MockTransformation> morphTransform = std::make_shared<MockTransformation>(0.0f);
			curve.AddKey(morphTransform, 0.0f, 10.0f);

			Assert::AreEqual(10.0f, curve.GetKeys().at(0.0f).weight);
		}

		TEST_METHOD(EvaluateStandardLinearCurve_Succeeds)
		{
			MockActorTrackedProperties properties = MockActorTrackedProperties();
			TransformationCurve curve = TransformationCurve(TraitType::Morph, "Assaults");

			std::shared_ptr<MockTransformation> defaultTransform = std::make_shared<MockTransformation>(0.0f);
			std::shared_ptr<MockTransformation> smallTransform = std::make_shared<MockTransformation>(1.0f);
			std::shared_ptr<MockTransformation> bigTransform = std::make_shared<MockTransformation>(2.0f);

			//assaults == 10, the halfway point between transforms
			curve.AddKey(smallTransform, 5.0f);
			curve.AddKey(bigTransform, 15.0f, 3.0f);

			TransformationCurve::Result result = curve.Evaluate(properties, defaultTransform);
			float evaluatedTransformationData = *(reinterpret_cast<float*>(result.transformation->GetTransformationData()));

			Assert::AreEqual(1.5f, evaluatedTransformationData);
			Assert::AreEqual(2.0f, result.weight);
		}

		TEST_METHOD(EvaluateCurveUsingImplicitOrigin_Succeeds)
		{
			MockActorTrackedProperties properties = MockActorTrackedProperties();
			TransformationCurve curve = TransformationCurve(TraitType::Morph, "Assaults");

			std::shared_ptr<MockTransformation> defaultTransform = std::make_shared<MockTransformation>(-2.0f);
			std::shared_ptr<MockTransformation> finalTransform = std::make_shared<MockTransformation>(1.0f);

			//assaults == 10, halfway between the origin and the final transform
			curve.AddKey(finalTransform, 20.0f);

			//evaluate should use the default transform for the implicit origin's value
			TransformationCurve::Result result = curve.Evaluate(properties, defaultTransform);
			float evaluatedTransformationData = *(reinterpret_cast<float*>(result.transformation->GetTransformationData()));
			
			//-0.5 is halfway between -2 and 1, so default was used as the origin transform
			Assert::AreEqual(-0.5f, evaluatedTransformationData);
		}

		TEST_METHOD(EvaluateCurveUsingExplicitOrigin_Succeeds)
		{
			MockActorTrackedProperties properties = MockActorTrackedProperties();
			TransformationCurve curve = TransformationCurve(TraitType::Morph, "Assaults");

			std::shared_ptr<MockTransformation> defaultTransform = std::make_shared<MockTransformation>(-2.0f);

			std::shared_ptr<MockTransformation> explicitOriginTransform = std::make_shared<MockTransformation>(-1.0f);
			std::shared_ptr<MockTransformation> finalTransform = std::make_shared<MockTransformation>(1.0f);

			//assaults == 10, halfway between the origin and the final transform
			curve.AddKey(explicitOriginTransform, 0.0f);
			curve.AddKey(finalTransform, 20.0f);

			//evaluate should use the specified transform for the explciit origin's value
			TransformationCurve::Result result = curve.Evaluate(properties, defaultTransform);
			float evaluatedTransformationData = *(reinterpret_cast<float*>(result.transformation->GetTransformationData()));

			//0 is halfway between -1 and 1, so explicit origin was used as the origin transform
			Assert::AreEqual(0.0f, evaluatedTransformationData);
		}

		TEST_METHOD(EvaluateCurveWithNegativeKeys_Succeeds)
		{
			MockActorTrackedProperties properties = MockActorTrackedProperties();
			TransformationCurve curve = TransformationCurve(TraitType::Morph, "Blackjack Earnings");

			std::shared_ptr<MockTransformation> defaultTransform = std::make_shared<MockTransformation>(0.0f);
			std::shared_ptr<MockTransformation> eyeBagsTransform = std::make_shared<MockTransformation>(10.0f);

			//Blackjack Earnings == -5, halfway between the origin and the final transform
			curve.AddKey(eyeBagsTransform, -10.0f);

			TransformationCurve::Result result = curve.Evaluate(properties, defaultTransform);
			float evaluatedTransformationData = *(reinterpret_cast<float*>(result.transformation->GetTransformationData()));
			Assert::AreEqual(5.0f, evaluatedTransformationData);
		}

		TEST_METHOD(EvaluateCurveWithNoKeysReturnsDefault_Succeeds)
		{
			MockActorTrackedProperties properties = MockActorTrackedProperties();
			TransformationCurve curve = TransformationCurve(TraitType::Morph, "Blackjack Earnings");

			std::shared_ptr<MockTransformation> defaultTransform = std::make_shared<MockTransformation>(10.0f);

			TransformationCurve::Result result = curve.Evaluate(properties, defaultTransform);

			float evaluatedTransformationData = *(reinterpret_cast<float*>(result.transformation->GetTransformationData()));
			Assert::AreEqual(10.0f, evaluatedTransformationData);
		}

		TEST_METHOD(EvaluateCurvePastExtremeKeys_Succeeds)
		{
			MockActorTrackedProperties properties = MockActorTrackedProperties();

			TransformationCurve blackjackCurve = TransformationCurve(TraitType::Morph, "Blackjack Earnings");
			TransformationCurve noSleepCurve = TransformationCurve(TraitType::Morph, "Days Without Sleep");

			std::shared_ptr<MockTransformation> defaultTransform = std::make_shared<MockTransformation>(0.0f);
			std::shared_ptr<MockTransformation> eyebagTransform = std::make_shared<MockTransformation>(10.0f);

			blackjackCurve.AddKey(eyebagTransform, -1.0f);
			noSleepCurve.AddKey(eyebagTransform, 1.0f);

			TransformationCurve::Result result = blackjackCurve.Evaluate(properties, defaultTransform);
			float blackjackEvaluatedTransformationData = *(reinterpret_cast<float*>(result.transformation->GetTransformationData()));
			Assert::AreEqual(10.0f, blackjackEvaluatedTransformationData);

			result = noSleepCurve.Evaluate(properties, defaultTransform);
			float noSleepEvaluatedTransformationData = *(reinterpret_cast<float*>(result.transformation->GetTransformationData()));
			Assert::AreEqual(10.0f, blackjackEvaluatedTransformationData);
		}

		TEST_METHOD(EvaluateCurveWithDefaultOfWrongType_Fails)
		{
			MockActorTrackedProperties properties = MockActorTrackedProperties();
			TransformationCurve curve = TransformationCurve(TraitType::None, "Some Property");

			std::shared_ptr<MockTransformation> defaultTransform = std::make_shared<MockTransformation>(10.0f);

			bool succeeded;

			try
			{
				TransformationCurve::Result result = curve.Evaluate(properties, defaultTransform);
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

