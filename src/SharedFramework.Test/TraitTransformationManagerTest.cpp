#include "pch.h"
#include "CppUnitTest.h"
#include "MockTraitTransformationReader.h"
#include "MockActorTrackedProperties.h"
#include "../SharedFramework/src/TraitTransformationManager.h"

#include "MockTransformation.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SharedFrameworkTest
{
	TEST_CLASS(TraitTransformationManagerTest)
	{
	public:
		TEST_METHOD(Constructor_Succeeds)
		{
			std::shared_ptr<MockActorTrackedProperties> properties = std::make_shared<MockActorTrackedProperties>();
			MockTraitTransformationReader reader = MockTraitTransformationReader();

			TraitTransformationManager manager = TraitTransformationManager(properties, reader);
		}

		TEST_METHOD(ConstructorMissingActorProps_Fails)
		{
			MockTraitTransformationReader reader = MockTraitTransformationReader();

			bool succeeded;
			try
			{
				TraitTransformationManager manager = TraitTransformationManager(nullptr, reader);
				succeeded = true;
			}
			catch (...)
			{
				succeeded = false;
			}
			Assert::IsFalse(succeeded);
		}

		TEST_METHOD(ApplyAllTransformations_Succeeds)
		{
			std::shared_ptr<MockActorTrackedProperties> properties = std::make_shared<MockActorTrackedProperties>();
			MockTraitTransformationReader reader = MockTraitTransformationReader();
			TraitTransformationManager manager = TraitTransformationManager(properties, reader);

			std::vector<std::shared_ptr<ITransformation>> finalTransformations = manager.ApplyAllTransformationGroups();

			Assert::AreEqual((size_t) 2, finalTransformations.size());
			Assert::AreEqual(0.625f, *(float*)(finalTransformations[0]->GetTransformationData()));
			Assert::AreEqual(0.625f, *(float*)(finalTransformations[1]->GetTransformationData()));
		}

		TEST_METHOD(ApplyAllTransformationsNoGroupsReturnsEmpty_Succeeds)
		{
			std::shared_ptr<MockActorTrackedProperties> properties = std::make_shared<MockActorTrackedProperties>();
			MockTraitTransformationReader reader = MockTraitTransformationReader();

			reader.ClearGroups(); // just for testing
			TraitTransformationManager manager = TraitTransformationManager(properties, reader);

			std::vector<std::shared_ptr<ITransformation>> finalTransformations = manager.ApplyAllTransformationGroups();
			Assert::AreEqual((size_t)0, finalTransformations.size());
		}

		TEST_METHOD(RevertAllTransformations_Succeeds)
		{
			std::shared_ptr<MockActorTrackedProperties> properties = std::make_shared<MockActorTrackedProperties>();
			MockTraitTransformationReader reader = MockTraitTransformationReader();
			TraitTransformationManager manager = TraitTransformationManager(properties, reader);

			std::vector<std::shared_ptr<ITransformation>> finalTransformations = manager.RevertAllTransformationGroups();

			Assert::AreEqual((size_t)2, finalTransformations.size());
			Assert::AreEqual(0.5f, *(float*)(finalTransformations[0]->GetTransformationData()));
			Assert::AreEqual(0.5f, *(float*)(finalTransformations[1]->GetTransformationData()));
		}

		TEST_METHOD(RevertAllTransformationsNoGroupsReturnsEmpty_Succeeds)
		{
			std::shared_ptr<MockActorTrackedProperties> properties = std::make_shared<MockActorTrackedProperties>();
			MockTraitTransformationReader reader = MockTraitTransformationReader();

			reader.ClearGroups(); // just for testing
			TraitTransformationManager manager = TraitTransformationManager(properties, reader);

			std::vector<std::shared_ptr<ITransformation>> finalTransformations = manager.RevertAllTransformationGroups();
			Assert::AreEqual((size_t)0, finalTransformations.size());
		}

		TEST_METHOD(ApplyTransformationGroup_Succeeds)
		{
			std::shared_ptr<MockActorTrackedProperties> properties = std::make_shared<MockActorTrackedProperties>();
			MockTraitTransformationReader reader = MockTraitTransformationReader();
			TraitTransformationManager manager = TraitTransformationManager(properties, reader);

			std::shared_ptr<ITransformation> finalTransformation = manager.ApplyTransformationGroup("Jaw Width");
			Assert::AreEqual(0.625f, *(float*)(finalTransformation->GetTransformationData()));
		}

		TEST_METHOD(ApplyMissingTransformationGroupReturnsNull_Succeeds)
		{
			std::shared_ptr<MockActorTrackedProperties> properties = std::make_shared<MockActorTrackedProperties>();
			MockTraitTransformationReader reader = MockTraitTransformationReader();
			TraitTransformationManager manager = TraitTransformationManager(properties, reader);

			std::shared_ptr<ITransformation> finalTransformation = manager.ApplyTransformationGroup("Missing Group");
			Assert::IsNull(finalTransformation.get());
		}

		TEST_METHOD(RevertTransformationGroup_Succeeds)
		{
			std::shared_ptr<MockActorTrackedProperties> properties = std::make_shared<MockActorTrackedProperties>();
			MockTraitTransformationReader reader = MockTraitTransformationReader();
			TraitTransformationManager manager = TraitTransformationManager(properties, reader);

			std::shared_ptr<ITransformation> finalTransformation = manager.RevertTransformationGroup("Jaw Width");
			Assert::AreEqual(0.5f, *(float*)(finalTransformation->GetTransformationData()));
		}

		TEST_METHOD(RevertMissingTransformationGroupReturnsNull_Succeeds)
		{
			std::shared_ptr<MockActorTrackedProperties> properties = std::make_shared<MockActorTrackedProperties>();
			MockTraitTransformationReader reader = MockTraitTransformationReader();
			TraitTransformationManager manager = TraitTransformationManager(properties, reader);

			std::shared_ptr<ITransformation> finalTransformation = manager.RevertTransformationGroup("Missing Group");
			Assert::IsNull(finalTransformation.get());
		}

		TEST_METHOD(SetDefaultToNull_Fails)
		{
			std::shared_ptr<MockActorTrackedProperties> properties = std::make_shared<MockActorTrackedProperties>();
			MockTraitTransformationReader reader = MockTraitTransformationReader();
			TraitTransformationManager manager = TraitTransformationManager(properties, reader);

			bool succeeded;
			try
			{
				manager.SetDefaultTransformationForTrait("Jaw Width", nullptr);
				succeeded = true;
			}
			catch (...)
			{
				succeeded = false;
			}
			Assert::IsFalse(succeeded);
		}

		TEST_METHOD(SetDefaultToMissingGroup_Fails)
		{
			std::shared_ptr<MockActorTrackedProperties> properties = std::make_shared<MockActorTrackedProperties>();
			MockTraitTransformationReader reader = MockTraitTransformationReader();
			TraitTransformationManager manager = TraitTransformationManager(properties, reader);

			std::shared_ptr<MockTransformation> newDefault = std::make_shared<MockTransformation>(0);

			bool succeeded;
			try
			{
				manager.SetDefaultTransformationForTrait("Missing Group", newDefault);
				succeeded = true;
			}
			catch (...)
			{
				succeeded = false;
			}
			Assert::IsFalse(succeeded);
		}

		TEST_METHOD(SetDefaultForTraitWrongType_Fails)
		{
			std::shared_ptr<MockActorTrackedProperties> properties = std::make_shared<MockActorTrackedProperties>();
			MockTraitTransformationReader reader = MockTraitTransformationReader();
			TraitTransformationManager manager = TraitTransformationManager(properties, reader);

			std::shared_ptr<MockTransformation> newDefault = std::make_shared<MockTransformation>(0);
			newDefault->ForceTraitType(TraitType::None); // just for testing purposes - not intended functionality

			bool succeeded;
			try
			{
				manager.SetDefaultTransformationForTrait("Jaw Width", newDefault);
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

