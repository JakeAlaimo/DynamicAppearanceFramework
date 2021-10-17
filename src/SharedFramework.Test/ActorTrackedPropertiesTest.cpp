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
			ActorTrackedProperties properties = ActorTrackedProperties(&reader);

			Assert::AreEqual((size_t)3, properties.GetAllTrackedProperties().size());
			Assert::AreEqual((size_t)2, properties.SetTrackedPropertyValue("Assaults", 10.0f).size());
			Assert::AreEqual(10.0f, properties.GetTrackedPropertyValue("Assaults"));
		}

		TEST_METHOD(ConstructorWithStartingValues_Succeeds)
		{
			std::map<std::string, float> cachedTrackedProperties;
			cachedTrackedProperties["Assaults"] = 50.0f;
			cachedTrackedProperties["Days Without Sleep"] = 3.0f;

			MockTraitTransformationReader reader = MockTraitTransformationReader();
			ActorTrackedProperties properties = ActorTrackedProperties(&reader, cachedTrackedProperties);

			Assert::AreEqual(50.0f, properties.GetTrackedPropertyValue("Assaults"));
			Assert::AreEqual(3.0f, properties.GetTrackedPropertyValue("Days Without Sleep"));
			Assert::AreEqual(0.0f, properties.GetTrackedPropertyValue("Magicka Percent Remaining"));

			Assert::AreEqual((size_t)3, properties.GetAllTrackedProperties().size());
			Assert::AreEqual((size_t)2, properties.SetTrackedPropertyValue("Assaults", 10.0f).size());
			Assert::AreEqual(10.0f, properties.GetTrackedPropertyValue("Assaults"));
		}

		TEST_METHOD(GracefullyHandlesNoReaderProvided_Succeeds)
		{
			ActorTrackedProperties properties = ActorTrackedProperties(nullptr);

			Assert::AreEqual((size_t)0, properties.GetAllTrackedProperties().size());
			Assert::AreEqual((size_t)0, properties.SetTrackedPropertyValue("some property that isn't tracked", 1.0f).size());
			Assert::AreEqual(0.0f, properties.GetTrackedPropertyValue("some property that isn't tracked"));

			// repeat with the other constructor 

			std::map<std::string, float> cachedTrackedProperties;
			cachedTrackedProperties["property 1"] = 50.0f;
			cachedTrackedProperties["property 2"] = 25.0f;
			
			properties = ActorTrackedProperties(nullptr, cachedTrackedProperties);

			Assert::AreEqual((size_t)0, properties.GetAllTrackedProperties().size());
			Assert::AreEqual((size_t)0, properties.SetTrackedPropertyValue("property 1", 51.0f).size());
			Assert::AreEqual(0.0f, properties.GetTrackedPropertyValue("property 2"));
		}

		TEST_METHOD(GetSetActorName_Succeeds)
		{
			ActorTrackedProperties properties = ActorTrackedProperties(nullptr);
			properties.SetActorName("Actor");
			Assert::AreEqual((std::string)"Actor", properties.GetActorName());
		}

		TEST_METHOD(GetActorNameUnspecified_Succeeds)
		{
			ActorTrackedProperties properties = ActorTrackedProperties(nullptr);
			Assert::AreEqual((std::string)"", properties.GetActorName());
		}

	};
}

