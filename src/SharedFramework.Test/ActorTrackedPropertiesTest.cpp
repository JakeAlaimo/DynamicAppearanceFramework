#include "pch.h"
#include "CppUnitTest.h"
#include "../SharedFramework/src/state/ActorTrackedProperties.h"
#include "MockTraitTransformationReader.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SharedFrameworkTest 
{
	TEST_CLASS(ActorTrackedPropertiesTest)
	{
	public:
		TEST_METHOD(ConstructorWithoutStartingValues_Succeeds)
		{
			MockTraitTransformationReader reader = MockTraitTransformationReader();
			ActorTrackedProperties properties = ActorTrackedProperties(reader);

			Assert::AreEqual((size_t)3, properties.GetAllTrackedProperties().size());
			Assert::AreEqual((size_t)3, properties.SetTrackedPropertyValue("Assaults", 10.0f).size());
			Assert::AreEqual(10.0f, properties.GetTrackedPropertyValue("Assaults"));
		}

		TEST_METHOD(ConstructorWithStartingValues_Succeeds)
		{
			std::map<std::string, float> cachedTrackedProperties;
			cachedTrackedProperties["Assaults"] = 50.0f;
			cachedTrackedProperties["Days Without Sleep"] = 3.0f;

			MockTraitTransformationReader reader = MockTraitTransformationReader();
			ActorTrackedProperties properties = ActorTrackedProperties(reader, cachedTrackedProperties);

			Assert::AreEqual(50.0f, properties.GetTrackedPropertyValue("Assaults"));
			Assert::AreEqual(3.0f, properties.GetTrackedPropertyValue("Days Without Sleep"));
			Assert::AreEqual(0.0f, properties.GetTrackedPropertyValue("Magicka Percent Remaining"));

			Assert::AreEqual((size_t)3, properties.GetAllTrackedProperties().size());
			Assert::AreEqual((size_t)3, properties.SetTrackedPropertyValue("Assaults", 10.0f).size());
			Assert::AreEqual(10.0f, properties.GetTrackedPropertyValue("Assaults"));
		}

		TEST_METHOD(GetSetActorName_Succeeds)
		{
			MockTraitTransformationReader reader = MockTraitTransformationReader();
			ActorTrackedProperties properties = ActorTrackedProperties(reader);
			properties.SetActorName("Actor");
			Assert::AreEqual((std::string)"Actor", properties.GetActorName());
		}

		TEST_METHOD(GetActorNameUnspecified_Succeeds)
		{
			MockTraitTransformationReader reader = MockTraitTransformationReader();
			ActorTrackedProperties properties = ActorTrackedProperties(reader);
			Assert::AreEqual((std::string)"", properties.GetActorName());
		}

	};
}

