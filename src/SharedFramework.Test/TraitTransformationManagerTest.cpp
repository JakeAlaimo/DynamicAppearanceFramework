#include "pch.h"
#include "CppUnitTest.h"
#include "MockConfigurationParser.h"
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
			MockConfigurationParser reader = MockConfigurationParser();
			TraitTransformationManager manager = TraitTransformationManager(reader);
		}

		TEST_METHOD(ApplyAllTransformations_Succeeds)
		{
			std::shared_ptr<MockActorTrackedProperties> properties = std::make_shared<MockActorTrackedProperties>();
			MockConfigurationParser reader = MockConfigurationParser();
			ActorState state = ActorState(properties, reader.defaultTransformations);

			TraitTransformationManager manager = TraitTransformationManager(reader);

			std::vector<std::shared_ptr<ITransformation>> finalTransformations = manager.ApplyAllTransformationGroups(state);

			Assert::AreEqual((size_t) 2, finalTransformations.size());
			Assert::AreEqual(0.625f, *(float*)(finalTransformations[0]->GetTransformationData()));
			Assert::AreEqual(0.625f, *(float*)(finalTransformations[1]->GetTransformationData()));
		}

		TEST_METHOD(ApplyAllTransformationsNoGroupsReturnsEmpty_Succeeds)
		{
			std::shared_ptr<MockActorTrackedProperties> properties = std::make_shared<MockActorTrackedProperties>();
			MockConfigurationParser reader = MockConfigurationParser();
			ActorState state = ActorState(properties, reader.defaultTransformations);

			reader.ClearGroups(); // just for testing
			TraitTransformationManager manager = TraitTransformationManager(reader);

			std::vector<std::shared_ptr<ITransformation>> finalTransformations = manager.ApplyAllTransformationGroups(state);
			Assert::AreEqual((size_t)0, finalTransformations.size());
		}

		TEST_METHOD(RevertAllTransformations_Succeeds)
		{
			std::shared_ptr<MockActorTrackedProperties> properties = std::make_shared<MockActorTrackedProperties>();
			MockConfigurationParser reader = MockConfigurationParser();
			ActorState state = ActorState(properties, reader.defaultTransformations);

			TraitTransformationManager manager = TraitTransformationManager(reader);

			std::vector<std::shared_ptr<ITransformation>> finalTransformations = manager.RevertAllTransformationGroups(state);

			Assert::AreEqual((size_t)2, finalTransformations.size());
			Assert::AreEqual(0.5f, *(float*)(finalTransformations[0]->GetTransformationData()));
			Assert::AreEqual(0.5f, *(float*)(finalTransformations[1]->GetTransformationData()));
		}

		TEST_METHOD(RevertAllTransformationsNoGroupsReturnsEmpty_Succeeds)
		{
			std::shared_ptr<MockActorTrackedProperties> properties = std::make_shared<MockActorTrackedProperties>();
			MockConfigurationParser reader = MockConfigurationParser();
			ActorState state = ActorState(properties, reader.defaultTransformations);

			reader.ClearGroups(); // just for testing
			TraitTransformationManager manager = TraitTransformationManager(reader);

			std::vector<std::shared_ptr<ITransformation>> finalTransformations = manager.RevertAllTransformationGroups(state);
			Assert::AreEqual((size_t)0, finalTransformations.size());
		}

		TEST_METHOD(ApplyTransformationGroup_Succeeds)
		{
			std::shared_ptr<MockActorTrackedProperties> properties = std::make_shared<MockActorTrackedProperties>();
			MockConfigurationParser reader = MockConfigurationParser();
			ActorState state = ActorState(properties, reader.defaultTransformations);

			TraitTransformationManager manager = TraitTransformationManager(reader);

			std::shared_ptr<ITransformation> finalTransformation = manager.ApplyTransformationGroup("Jaw Width", state);
			Assert::AreEqual(0.625f, *(float*)(finalTransformation->GetTransformationData()));
		}

		TEST_METHOD(ApplyMissingTransformationGroupReturnsNull_Succeeds)
		{
			std::shared_ptr<MockActorTrackedProperties> properties = std::make_shared<MockActorTrackedProperties>();
			MockConfigurationParser reader = MockConfigurationParser();
			ActorState state = ActorState(properties, reader.defaultTransformations);

			TraitTransformationManager manager = TraitTransformationManager(reader);

			std::shared_ptr<ITransformation> finalTransformation = manager.ApplyTransformationGroup("Missing Group", state);
			Assert::IsNull(finalTransformation.get());
		}

		TEST_METHOD(RevertTransformationGroup_Succeeds)
		{
			std::shared_ptr<MockActorTrackedProperties> properties = std::make_shared<MockActorTrackedProperties>();
			MockConfigurationParser reader = MockConfigurationParser();
			ActorState state = ActorState(properties, reader.defaultTransformations);

			TraitTransformationManager manager = TraitTransformationManager(reader);

			std::shared_ptr<ITransformation> finalTransformation = manager.RevertTransformationGroup("Jaw Width", state);
			Assert::AreEqual(0.5f, *(float*)(finalTransformation->GetTransformationData()));
		}

		TEST_METHOD(RevertMissingTransformationGroupReturnsNull_Succeeds)
		{
			std::shared_ptr<MockActorTrackedProperties> properties = std::make_shared<MockActorTrackedProperties>();
			MockConfigurationParser reader = MockConfigurationParser();
			ActorState state = ActorState(properties, reader.defaultTransformations);

			TraitTransformationManager manager = TraitTransformationManager(reader);

			std::shared_ptr<ITransformation> finalTransformation = manager.RevertTransformationGroup("Missing Group", state);
			Assert::IsNull(finalTransformation.get());
		}
	};
}

