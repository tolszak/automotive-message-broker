/*
	Copyright (C) 2012  Intel Corporation

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <iostream>
#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>
#include <glib.h>
#include <sstream>
//#include <json-glib/json-glib.h>
#include <listplusplus.h>
#include <vehicleproperty.h>
#include <abstractpropertytype.h>
#include "debugout.h"
#include "timestamp.h"
#include "testplugin.h"
#define __SMALLFILE__ std::string(__FILE__).substr(std::string(__FILE__).rfind("/")+1)
AbstractRoutingEngine *m_re;

#define TEST(success)	g_assert((success));
const std::string TestProptertyName1 = "TestPropertyName1";
const std::string TestProptertyName2 = "TestPropertyName2";

//std::list<ObdPid*> Obd2Amb::supportedPidsList;

void testBooleanToStringFromString()
{
	BasicPropertyType<bool> boolean(true);
	std::string isTrue = boolean.toString();
	boolean.fromString(boolean.toString());
	std::string isTrue2 = boolean.toString();

	g_assert(isTrue == isTrue2);
}

bool beginsWith(std::string a, std::string b)
{
	return (a.compare(0, b.length(), b) == 0);
}
void TestPlugin::updateProperty(VehicleProperty::Property property,AbstractPropertyType* value)
{

}

/**
 * Tests Core's methods:
 * * setSupported
 * * supported
 * * sourcesForProperty
 * * getPropertyInfo
 */

bool TestPlugin::testCoreSetSupported()
{
	PropertyList supported(routingEngine->supported());
	TEST(ListPlusPlus<VehicleProperty::Property>(&supported).contains(TestProptertyName1) == false);
	std::list<std::string> sources(routingEngine->sourcesForProperty(TestProptertyName1));
	TEST(ListPlusPlus<std::string>(&sources).contains(uuid()) == false);

	// if we were registered in Core, it will ask us and we will return it valid, but we haven't registered yet
	TEST(routingEngine->getPropertyInfo(TestProptertyName1, uuid()).isValid() == false);

	//
	// CALL setSupported
	//
	routingEngine->updateSupported(m_supportedProperties, PropertyList(), nullptr);// invalid input
	TEST(routingEngine->getPropertyInfo(TestProptertyName1, uuid()).isValid() == false);

	// valid call
	routingEngine->updateSupported(m_supportedProperties, PropertyList(), this);// we are register as source from now

	TEST(routingEngine->getPropertyInfo(TestProptertyName1, uuid()).isValid() == true);
	Zone::ZoneList zones(routingEngine->getPropertyInfo(TestProptertyName1, uuid()).zones());
	TEST(ListPlusPlus<Zone::Type>(&zones).contains(Zone::LeftSide) == true);

	supported = routingEngine->supported();
	TEST(ListPlusPlus<VehicleProperty::Property>(&supported).contains(TestProptertyName1) == true);
	TEST(ListPlusPlus<VehicleProperty::Property>(&supported).contains(TestProptertyName2) == true);
	TEST(ListPlusPlus<VehicleProperty::Property>(&supported).contains(VehicleProperty::ClutchStatus) == false);

	sources = routingEngine->sourcesForProperty(TestProptertyName1);
	TEST(ListPlusPlus<std::string>(&sources).contains(uuid()) == true);

	TEST(routingEngine->getPropertyInfo(TestProptertyName2, uuid()).isValid() == true);
	zones = routingEngine->getPropertyInfo(TestProptertyName2, uuid()).zones();
	TEST(ListPlusPlus<Zone::Type>(&zones).contains(Zone::FrontSide) == true);

	return true;
}

/**
 * Tests Core's methods:
 * * unregisterSink
 * * registerSink
 * * sourcesForProperty
 * * subscribeToProperty - 2 versions
 * * unsubscribeToProperty
 */

bool TestPlugin::testSubscription()
{
	int oldSubscriptionsToSupportedCounter = subscriptionsToSupportedCounter;
	int oldSubscriptionsToUnsupportedCounter = subscriptionsToUnsupportedCounter;
	int oldUnsubscriptionsToSupportedCounter= unsubscriptionsToSupportedCounter;
	int oldUnsubscriptionsToUnsupportedCounter = unsubscriptionsToUnsupportedCounter;

	// ! subscription works without having sink registered ! so there is no need to test
	// routingEngine->registerSink(this) and routingEngine->unregisterSink(this)
	routingEngine->unregisterSink(this);
	routingEngine->registerSink(this);

	// VehicleProperty::ClutchStatus doesn't exist
	TEST(routingEngine->subscribeToProperty(VehicleProperty::ClutchStatus, uuid(), this) == false);
	TEST(oldSubscriptionsToSupportedCounter == subscriptionsToSupportedCounter);
	TEST(oldSubscriptionsToUnsupportedCounter == subscriptionsToUnsupportedCounter);

	// TestProptertyName1 exists in our testplugin
	TEST(routingEngine->subscribeToProperty(TestProptertyName1, uuid(), this) == true);
	TEST(++oldSubscriptionsToSupportedCounter == subscriptionsToSupportedCounter);
	TEST(oldSubscriptionsToUnsupportedCounter == subscriptionsToUnsupportedCounter);

	TEST(routingEngine->subscribeToProperty(TestProptertyName1, "", this) == false);
	TEST(oldSubscriptionsToSupportedCounter == subscriptionsToSupportedCounter);
	TEST(oldSubscriptionsToUnsupportedCounter == subscriptionsToUnsupportedCounter);

	// "other" source means no subscription callback in our plugin
	TEST(routingEngine->subscribeToProperty(TestProptertyName1, "other", this) == false);
	TEST(oldSubscriptionsToSupportedCounter == subscriptionsToSupportedCounter);
	TEST(oldSubscriptionsToUnsupportedCounter == subscriptionsToUnsupportedCounter);

	// we have subscribed TestProptertyName1 and not subscribed TestProptertyName2
	int oldSupportedPropertyChanges = supportedPropertyChanges;
	int oldPropertyChanges = propertyChanges;

	AbstractPropertyType* value = new BasicPropertyType<int>(TestProptertyName1, 22);
	routingEngine->updateProperty(value, "");
	TEST(oldSupportedPropertyChanges == supportedPropertyChanges);
	TEST(oldPropertyChanges == propertyChanges);
	routingEngine->updateProperty(value, uuid());
	TEST(++oldSupportedPropertyChanges == supportedPropertyChanges);
	TEST(++oldPropertyChanges == propertyChanges);
	delete value;

	value = new BasicPropertyType<short>(TestProptertyName2, 255);
	routingEngine->updateProperty(value, "");
	TEST(oldSupportedPropertyChanges == supportedPropertyChanges);
	TEST(oldPropertyChanges == propertyChanges);
	routingEngine->updateProperty(value, uuid());
	TEST(oldSupportedPropertyChanges == supportedPropertyChanges);
	TEST(oldPropertyChanges == propertyChanges);
	delete value;

	value = new BasicPropertyType<bool>(VehicleProperty::ClutchStatus, true);
	routingEngine->updateSupported({VehicleProperty::ClutchStatus},PropertyList(), this);

	class TestSink : public AbstractSink
	{
	public:
		TestSink(AbstractRoutingEngine* engine, map<string, string> config) : AbstractSink(engine, config) {}
		virtual const string uuid() { return "other"; }
		virtual void propertyChanged(AbstractPropertyType *value){ ++propertyChanges; }
		virtual void supportedChanged(const PropertyList & supportedProperties){};

		int propertyChanges = 0;
	};

	//some other sink is subscribed, not this plug-in!
	TestSink anotherSink(routingEngine, {});
	TEST(routingEngine->subscribeToProperty(VehicleProperty::ClutchStatus, &anotherSink) == true);
	TEST(oldSubscriptionsToSupportedCounter == subscriptionsToSupportedCounter);
	TEST(++oldSubscriptionsToUnsupportedCounter == subscriptionsToUnsupportedCounter);
	routingEngine->updateProperty(value, uuid());// uuid() != "other" no sink notified
	TEST(anotherSink.propertyChanges == 1);// one update
	TEST(oldSupportedPropertyChanges == supportedPropertyChanges);// no updates
	TEST(oldPropertyChanges == propertyChanges);// no updates
	TEST(routingEngine->unsubscribeToProperty(VehicleProperty::ClutchStatus, &anotherSink) == true);
	routingEngine->updateSupported(PropertyList(),{VehicleProperty::ClutchStatus}, this);
	TEST(oldUnsubscriptionsToSupportedCounter == unsubscriptionsToSupportedCounter);
	TEST(++oldUnsubscriptionsToUnsupportedCounter == unsubscriptionsToUnsupportedCounter);

	delete value;

	// unsubscription

	// VehicleProperty::ClutchStatus doesn't exist
	TEST(routingEngine->unsubscribeToProperty(VehicleProperty::ClutchStatus, this) == false);
	TEST(oldUnsubscriptionsToSupportedCounter == unsubscriptionsToSupportedCounter);
	TEST(oldUnsubscriptionsToUnsupportedCounter == unsubscriptionsToUnsupportedCounter);

	// TestProptertyName1 exists in our testplugin
	// subscribed 2x, lets try to unsubscribe 3x
	// we should get only one unsubscription callback
	TEST(routingEngine->unsubscribeToProperty(TestProptertyName1, this) == true);
	TEST(routingEngine->unsubscribeToProperty(TestProptertyName1, this) == false);
	TEST(routingEngine->unsubscribeToProperty(TestProptertyName1, this) == false);
	TEST(++oldUnsubscriptionsToSupportedCounter == unsubscriptionsToSupportedCounter);
	TEST(oldUnsubscriptionsToUnsupportedCounter == unsubscriptionsToUnsupportedCounter);

	// TestProptertyName2 not subscribed
	TEST(routingEngine->unsubscribeToProperty(TestProptertyName2, this) == false);;
	TEST(oldUnsubscriptionsToSupportedCounter == unsubscriptionsToSupportedCounter);
	TEST(oldUnsubscriptionsToUnsupportedCounter == unsubscriptionsToUnsupportedCounter);

	return true;
}

bool TestPlugin::testSetAndGet()
{
	bool replySuccess(false);
	int replyError(-1);
	std::string replySignalName("");

	// Invalid request test
	AsyncSetPropertyRequest requestInvalid;
	requestInvalid.timeout = 0;
	AsyncPropertyReply* reply = routingEngine->setProperty(requestInvalid);
	TEST(reply == nullptr);

	requestInvalid.property = "NotExists";
	requestInvalid.completed = [&](AsyncPropertyReply* reply)
	{
		replySuccess = reply->success;
		replyError = reply->error;
		delete reply;
	};

	reply = routingEngine->setProperty(requestInvalid);

	TEST(replySuccess == false);
	TEST(replyError == -1);

	AsyncSetPropertyRequest request;
	request.timeout = 0;
	request.property = TestProptertyName1;
	request.zoneFilter = Zone::LeftSide;
	request.value = VehicleProperty::getPropertyTypeForPropertyNameValue(request.property, "1");
	request.completed = requestInvalid.completed;

	reply = routingEngine->setProperty(request);
	delete request.value;
	request.value = nullptr;

	TEST(replySuccess == true);
	TEST(replyError == AsyncPropertyReply::NoError);
}

bool TestPlugin::testCoreUpdateSupported()
{
	bool success = false;

	PropertyList toAdd;
	toAdd.push_back(VehicleProperty::ClutchStatus);

	routingEngine->updateSupported(toAdd,PropertyList(), this);

	PropertyList supported = routingEngine->supported();

	success = ListPlusPlus<VehicleProperty::Property>(&supported).contains(VehicleProperty::ClutchStatus);

	PropertyList toRemove = toAdd;

	routingEngine->updateSupported(PropertyList(),toRemove, this);

	supported = routingEngine->supported();

	success &= !ListPlusPlus<VehicleProperty::Property>(&supported).contains(VehicleProperty::ClutchStatus);

	return success;
}

void TestPlugin::setConfiguration(map<string, string> config)
{
	// 	//Config has been passed, let's start stuff up.
}

TestPlugin::TestPlugin(AbstractRoutingEngine *re, map<string, string> config)
	: AbstractSource(re, config),
	  m_supportedProperties({TestProptertyName1, TestProptertyName2}),
	  subscriptionsToSupportedCounter(0),
	  subscriptionsToUnsupportedCounter(0),
	  unsubscriptionsToSupportedCounter(0),
	  unsubscriptionsToUnsupportedCounter(0),
	  propertyChanges(0),
	  supportedPropertyChanges(0)
{

	cout<<"Capabilities: "<<endl;

	for(auto prop : VehicleProperty::capabilities())
	{
		cout<<prop<<endl;
	}

	DebugOut() << "Testing Core::setSupported... " << endl;
	testCoreSetSupported();

	DebugOut() << "Testing MapPropertyType... " << endl;
	MapPropertyType<BasicPropertyType<Zone::Type>,BasicPropertyType<Door::Status>> propmap("something");
	MapPropertyType<BasicPropertyType<Zone::Type>,BasicPropertyType<Door::Status>> propmaptwo("something");
	propmap.append(Zone::RearLeft,Door::Ajar);
	GVariant *var = propmap.toVariant();
	gsize dictsize = g_variant_n_children(var);
	//DebugOut() << var << endl;
	propmaptwo.fromVariant(var);

	g_assert(propmaptwo.toString() == propmap.toString());

	DebugOut() << "Testing ListPropertyType... " << endl;
	VehicleProperty::TripMetersType* tfirst = new VehicleProperty::TripMetersType();
	VehicleProperty::TripMetersType* tsecond = new VehicleProperty::TripMetersType();
	BasicPropertyType<uint16_t> v1(0);
	BasicPropertyType<uint16_t> v2(5);
	BasicPropertyType<uint16_t> v3(10);
	tfirst->append(&v1);
	tfirst->append(&v2);
	tfirst->append(&v3);
	tsecond->fromVariant(tfirst->toVariant());

	g_assert (tfirst->toString() == tsecond->toString());

	testBooleanToStringFromString();

	g_assert (testCoreUpdateSupported());

	testSubscription();

	testSetAndGet();
}

TestPlugin::~TestPlugin()
{
	DebugOut() << "TestPlugin Destructor called!!!"<<endl;
}

PropertyList TestPlugin::supported()
{
	return m_supportedProperties;
}

int TestPlugin::supportedOperations()
{
	return Get | Set;
}

extern "C" AbstractSource * create(AbstractRoutingEngine* routingengine, map<string, string> config)
{
	return new TestPlugin(routingengine, config);
	
}
const string TestPlugin::uuid()
{
	return "f77af740-f1f8-11e1-aff1-0800200c9a66";
}
void TestPlugin::subscribeToPropertyChanges(VehicleProperty::Property property)
{
	if(ListPlusPlus<VehicleProperty::Property>(&m_supportedProperties).contains(property))
		++subscriptionsToSupportedCounter;
	else
		++subscriptionsToUnsupportedCounter;
}


void TestPlugin::unsubscribeToPropertyChanges(VehicleProperty::Property property)
{
	if(ListPlusPlus<VehicleProperty::Property>(&m_supportedProperties).contains(property))
		++unsubscriptionsToSupportedCounter;
	else
		++unsubscriptionsToUnsupportedCounter;
}


void TestPlugin::getPropertyAsync(AsyncPropertyReply *reply)
{
	
}

AsyncPropertyReply *TestPlugin::setProperty(AsyncSetPropertyRequest request )
{
	AsyncPropertyReply* reply = new AsyncPropertyReply (request);
	reply->success = true;
	reply->error = AsyncPropertyReply::NoError;
	if(reply->completed)
		reply->completed(reply);
	return reply;
}

PropertyInfo TestPlugin::getPropertyInfo(VehicleProperty::Property property)
{
	if(!ListPlusPlus<VehicleProperty::Property>(&m_supportedProperties).contains(property))
		return PropertyInfo::invalid();

	if(property == TestProptertyName1){
		return PropertyInfo(10, {Zone::LeftSide});
	}
	else if(property == TestProptertyName2){
		return PropertyInfo(30, {Zone::FrontSide});
	}
	else{
		return PropertyInfo(60, {Zone::None});
	}
}

void TestPlugin::propertyChanged(AbstractPropertyType *value)
{
	++propertyChanges;
	if(value && ListPlusPlus<VehicleProperty::Property>(&m_supportedProperties).contains(value->name))
		supportedPropertyChanges++;

}
