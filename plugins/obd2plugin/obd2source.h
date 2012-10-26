
/*
Copyright (C) 2012 Intel Corporation

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


#ifndef OBD2SOURCE_H
#define OBD2SOURCE_H



#include <abstractsource.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include "obdlib.h"
#include <glib.h>


class ObdRequest
{
public:
  std::string req;
  std::string arg;
};

class ObdReply
{
public:
  std::string req;
  std::string reply;
};


class Obd2Amb
{
public:
	Obd2Amb()
	{
		propertyPidMap[VehicleProperty::VehicleSpeed] = "010D1\r";
		propertyPidMap[VehicleProperty::EngineSpeed] = "010C1\r";
		propertyPidMap[VehicleProperty::MassAirFlow] = "01101\r";
		propertyPidMap[VehicleProperty::AirIntakeTemperature] = "010F1\r";
		propertyPidMap[VehicleProperty::ThrottlePosition] = "01111\r";
		propertyPidMap[VehicleProperty::BatteryVoltage] = "ATRV\r";	propertyPidMap[VehicleProperty::EngineCoolantTemperature]  = "0105a\r";
		propertyPidMap[VehicleProperty::EngineLoad] = "01041/r";
		propertyPidMap[VehicleProperty::VIN] = "0902/r";
		propertyPidMap[VehicleProperty::WMI] = "0902/r";
		propertyPidMap[VehicleProperty::EngineOilTemperature] = "015C1\r";
		propertyPidMap[VehicleProperty::InteriorTemperature] = "01461\r";
	}

	map<VehicleProperty::Property, std::string> propertyPidMap;
};

class OBD2Source : public AbstractSource
{

public:
	OBD2Source(AbstractRoutingEngine* re, map<string, string> config);
	string uuid();
	int portHandle;
	void getPropertyAsync(AsyncPropertyReply *reply);
	void getRangePropertyAsync(AsyncRangePropertyReply *reply){}
	AsyncPropertyReply * setProperty(AsyncSetPropertyRequest request);
	void subscribeToPropertyChanges(VehicleProperty::Property property);
	void unsubscribeToPropertyChanges(VehicleProperty::Property property);
	PropertyList supported();
	PropertyList queuedRequests;
	bool clientConnected;
	PropertyList activeRequests;
	void engineSpeed(double speed);
	void vehicleSpeed(int speed);
	void mafValue(double maf);
	void engineCoolantTemp(int temp);
	PropertyList removeRequests;
	void setSupported(PropertyList list);
	void propertyChanged(VehicleProperty::Property property, AbstractPropertyType* value, string uuid) {}
	void supportedChanged(PropertyList) {}
	GAsyncQueue* commandQueue;
	GAsyncQueue* subscriptionAddQueue;
	GAsyncQueue* subscriptionRemoveQueue;
	GAsyncQueue* singleShotQueue;
	GAsyncQueue* responseQueue;
	void setConfiguration(map<string, string> config);
	//void randomizeProperties();
	std::string m_port;
	map<VehicleProperty::Property,AsyncPropertyReply*> propertyReplyMap;
	void updateProperty(VehicleProperty::Property property,AbstractPropertyType *value);
private:
	PropertyList m_supportedProperties;
	GMutex *threadQueueMutex;
	

};

#endif // OBD2SOURCE_H
