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

#ifndef _PROPERTIES_H_H_H_
#define _PROPERTIES_H_H_H_

#include "dbusplugin.h"
#include "abstractdbusinterface.h"
#include "abstractroutingengine.h"

/** @interface VehicleSpeed : VehiclePropertyType **/
class VehicleSpeedProperty: public DBusSink
{
public:
	VehicleSpeedProperty(AbstractRoutingEngine* re, GDBusConnection* connection)
		:DBusSink("VehicleSpeed", re, connection, map<string, string>())
	{
		/** @attributeName VehicleSpeed
		 *  @type unsigned long
		 *  @access readonly
		 *  @attributeComment \brief  Must return Vehicle Speed in kilometers per hour.
		 **/
		wantPropertyVariant(VehicleProperty::VehicleSpeed,"VehicleSpeed", "i", AbstractProperty::Read);

	}


};

/** @interface EngineSpeed : VehiclePropertyType **/
class EngineSpeedProperty: public DBusSink
{
public:
	EngineSpeedProperty(AbstractRoutingEngine* re, GDBusConnection* connection)
		:DBusSink("EngineSpeed", re, connection, map<string, string>())
	{
		/** @attributeName EngineSpeed
		 *  @type unsigned long
		 *  @access readonly
		 *  @attributeComment \brief  Must return Engine Speed in rotations per minute.
		 **/
		wantPropertyVariant(VehicleProperty::EngineSpeed,"EngineSpeed", "i", AbstractProperty::Read);

	}


};

/** @interface VehiclePowerMode : VehiclePropertyType **/
class VehiclePowerModeProperty: public DBusSink
{
public:
	VehiclePowerModeProperty(AbstractRoutingEngine* re, GDBusConnection* connection)
		:DBusSink("VehiclePowerMode", re, connection, map<string, string>())
	{
		/**
		 * @enum const unsigned short VEHICLEPOWERMODE_OFF = 0,
		 * @enum const unsigned short VEHICLEPOWERMODE_ACCESSORY1 = 1,
		 * @enum const unsigned short VEHICLEPOWERMODE_ACCESSORY2 = 2,
		 * @enum const unsigned short VEHICLEPOWERMODE_RUN = 3;
		 **/


		/** @attributeName VehiclePowerMode
		 *  @type octet
		 *  @access readonly
		 *  @attributeComment \brief  Must return Vehicle Power mode (see VEHICLEPOWERMODE)
		 **/
		wantPropertyVariant(VehicleProperty::VehiclePowerMode, "VehiclePowerMode","i",AbstractProperty::Read);

	}
};

/** @interface TripMeter : VehiclePropertyType **/
class TripMeterProperty: public DBusSink
{
public:
	TripMeterProperty(AbstractRoutingEngine *re, GDBusConnection *connection)
	:DBusSink("TripMeter", re, connection, map<string, string>())
	{
		/** @attributeName TripMeters
		 *  @type sequence<unsigned long>
		 *  @attributeComment \brief  Must return trip metes. Changing any items in the array will reset the item's value to '0''.
		 **/
		wantPropertyVariant(VehicleProperty::TripMeters, "TripMeters", "aq", AbstractProperty::ReadWrite);


	}
};

/** @interface Acceleration : VehiclePropertyType **/
class AccelerationProperty: public DBusSink
{
public:
	AccelerationProperty(AbstractRoutingEngine *re, GDBusConnection *connection)
	:DBusSink("Acceleration", re, connection, map<string, string>())
	{
		/** @attributeName X
		 *  @type unsigned short
		 *  @access readonly
		 *  @attributeComment \brief  Must return acceleration on the "X" axis as 1/1000 G (gravitational force).
		 **/
		wantPropertyVariant(VehicleProperty::AccelerationX, "X", "q", AbstractProperty::Read);

		/** @attributeName Y
		 *  @type unsigned short
		 *  @access readonly
		 *  @attributeComment \brief  Must return acceleration on the "Y" axis as 1/1000 G (gravitational force).
		 **/
		wantPropertyVariant(VehicleProperty::AccelerationY, "Y", "q", AbstractProperty::Read);

		/** @attributeName Z
		 *  @type unsigned short
		 *  @access readonly
		 *  @attributeComment \brief  Must return acceleration on the "Z" axis as 1/1000 G (gravitational force).
		 **/
		wantPropertyVariant(VehicleProperty::AccelerationZ, "Z", "q", AbstractProperty::Read);

	}
};

/** @interface Transmission : VehiclePropertyType **/
class TransmissionProperty: public DBusSink
{
public:
	TransmissionProperty(AbstractRoutingEngine *re, GDBusConnection *connection)
	:DBusSink("Transmission", re, connection, map<string, string>())
	{

		wantPropertyVariant(VehicleProperty::TransmissionShiftPosition,
														  "ShiftPosition", "y", AbstractProperty::Read);

		wantPropertyVariant(VehicleProperty::TransmissionGearPosition,
														  "GearPosition", "y", AbstractProperty::Read);

		wantPropertyVariant(VehicleProperty::TransmissionMode,
														  "Mode", "y", AbstractProperty::Read);

		wantPropertyVariant(VehicleProperty::TransmissionGearType, "Type", "q", AbstractProperty::Read);
	}
};

/** @interface CruiseControlStatus : VehiclePropertyType **/
class CruiseControlProperty: public DBusSink
{
public:
	CruiseControlProperty(AbstractRoutingEngine *re, GDBusConnection *connection)
	:DBusSink("CruiseControlStatus", re, connection, map<string, string>())
	{
		/** @attributeName Activated
		 *  @type boolean
		 *  @access readonly
		 *  @attributeComment \brief  Must return whether or not the Cruise Control system is active (true) or inactive (false)
		 **/
		wantPropertyVariant(VehicleProperty::CruiseControlActive, "Activated", "b", AbstractProperty::Read);

		/** @attributeName Speed
		 *  @type unsigned short
		 *  @access readonly
		 *  @attributeComment \brief  Must return target Cruise Control speed in kilometers per hour (kph).
		 **/
		wantPropertyVariant(VehicleProperty::CruiseControlSpeed, "Speed", "q", AbstractProperty::Read);

	}
};

/** @interface WheelBrake : VehiclePropertyType **/
class WheelBrakeProperty: public DBusSink
{
public:
	WheelBrakeProperty(AbstractRoutingEngine *re, GDBusConnection *connection)
		:DBusSink("WheelBrake", re, connection, map<string, string>())
	{
		/** @attributeName Engaged
		 *  @type boolean
		 *  @access readonly
		 *  @attributeComment \brief  Must return Wheel Brake status: Engaged = true, disengaged = false
		 **/
		wantPropertyVariant(VehicleProperty::WheelBrake, "Engaged", "b", AbstractProperty::Read);

	}
};

/** @interface LightStatus : VehiclePropertyType **/
class LightStatusProperty: public DBusSink
{
public:
	LightStatusProperty(AbstractRoutingEngine *re, GDBusConnection *connection)
		:DBusSink("LightStatus", re, connection, map<string, string>())
	{
		/** @attributeName Head
		 *  @type boolean
		 *  @access readonly
		 *  @attributeComment \brief  Must return headlight status: on = true, off = false.
		 **/
		wantPropertyVariant(VehicleProperty::LightHead, "Head", "b", AbstractProperty::Read);

		/** @attributeName RightTurn
		 *  @type boolean
		 *  @access readonly
		 *  @attributeComment \brief  Must return right turn signal status: on = true, off = false.
		 **/
		wantPropertyVariant(VehicleProperty::LightRightTurn, "RightTurn", "b", AbstractProperty::Read);

		/** @attributeName LeftTurn
		 *  @type boolean
		 *  @access readonly
		 *  @attributeComment \brief  Must return left turn signal status: on = true, off = false.
		 **/
		wantPropertyVariant(VehicleProperty::LightLeftTurn, "LeftTurn", "b", AbstractProperty::Read);

		/** @attributeName Brake
		 *  @type boolean
		 *  @access readonly
		 *  @attributeComment \brief  Must return  brake signal light status: on = true, off = false.
		 **/
		wantPropertyVariant(VehicleProperty::LightBrake, "Brake", "b", AbstractProperty::Read);

		/** @attributeName Fog
		 *  @type boolean
		 *  @access readonly
		 *  @attributeComment \brief  Must return fog light status: on = true, off = false.
		 **/
		wantPropertyVariant(VehicleProperty::LightFog, "Fog", "b", AbstractProperty::Read);

		/** @attributeName Hazard
		 *  @type boolean
		 *  @access readonly
		 *  @attributeComment \brief  Must return hazard light status: on = true, off = false.
		 **/
		wantPropertyVariant(VehicleProperty::LightHazard, "Hazard", "b", AbstractProperty::Read);

		/** @attributeName Parking
		 *  @type boolean
		 *  @access readonly
		 *  @attributeComment \brief  Must return parking light status: on = true, off = false.
		 **/
		wantPropertyVariant(VehicleProperty::LightParking, "Parking", "b", AbstractProperty::Read);

		/** @attributeName HighBeam
		 *  @type boolean
		 *  @access readonly
		 *  @attributeComment \brief  Must return high beam status: on = true, off = false.
		 **/
		wantPropertyVariant(VehicleProperty::LightHighBeam, "HighBeam", "b", AbstractProperty::Read);

	}
};

/** @interface InteriorLightStatus : VehiclePropertyType **/
class InteriorLightStatusProperty: public DBusSink
{
public:
	InteriorLightStatusProperty(AbstractRoutingEngine *re, GDBusConnection *connection)
		:DBusSink("InteriorLightStatus", re, connection, map<string, string>())
	{
		/** @attributeName Passenger
		 *  @type boolean
		 *  @access readonly
		 *  @attributeComment \brief  Must return passenger interior light status: on = true, off = false
		 **/
		wantPropertyVariant(VehicleProperty::InteriorLightPassenger, "Passenger", "b", AbstractProperty::Read);

		/** @attributeName Driver
		 *  @type boolean
		 *  @access readonly
		 *  @attributeComment \brief  Must return driver interior light status: on = true, off = false
		 **/
		wantPropertyVariant(VehicleProperty::InteriorLightPassenger, "Driver", "b", AbstractProperty::Read);

		/** @attributeName Center
		 *  @type boolean
		 *  @access readonly
		 *  @attributeComment \brief  Must return center interior light status: on = true, off = false
		 **/
		wantPropertyVariant(VehicleProperty::InteriorLightCenter, "Center", "b", AbstractProperty::Read);

	}
};

/** @interface Horn : VehiclePropertyType **/
class HornProperty: public DBusSink
{
public:
	HornProperty(AbstractRoutingEngine *re, GDBusConnection *connection)
		:DBusSink("Horn", re, connection, map<string, string>())
	{
		/** @attributeName On
		 *  @type boolean
		 *  @access readonly
		 *  @attributeComment \brief  Must return horn status: on = true, off = false
		 **/
		wantPropertyVariant(VehicleProperty::Horn,"On","b",AbstractProperty::Read);

	}
};

/** @interface Fuel : VehiclePropertyType **/
class FuelProperty: public DBusSink
{
public:
	FuelProperty(AbstractRoutingEngine *re, GDBusConnection *connection)
		:DBusSink("Fuel", re, connection, map<string, string>())
	{
		wantPropertyVariant(VehicleProperty::FuelLevel,"Level", "y", AbstractProperty::Read);

		wantPropertyVariant(VehicleProperty::FuelRange,"Range", "q", AbstractProperty::Read);

		wantPropertyVariant(VehicleProperty::FuelConsumption,"InstantConsumption", "q", AbstractProperty::Read);

		wantPropertyVariant(VehicleProperty::FuelEconomy,"InstantEconomy", "q", AbstractProperty::Read);

		wantPropertyVariant(VehicleProperty::FuelAverageEconomy,"AverageEconomy", "q", AbstractProperty::ReadWrite);

	}
};

/** @interface EngineOil : VehiclePropertyType **/
class EngineOilProperty: public DBusSink
{
public:
	EngineOilProperty(AbstractRoutingEngine *re, GDBusConnection *connection)
			:DBusSink("EngineOil", re, connection, map<string, string>())
	{
		/** @attributeName Remaining
		 *  @type unsigned short
		 *  @access readonly
		 *  @attributeComment \brief  Must return remaining engine oil as percentage of fullness.
		 **/
		wantPropertyVariant(VehicleProperty::EngineOilRemaining, "Remaining", "q", AbstractProperty::Read);

		/** @attributeName Temperature
		 *  @type long
		 *  @access readonly
		 *  @attributeComment \brief  Must return Engine Oil Temperature in Celcius.
		 **/
		wantPropertyVariant(VehicleProperty::EngineOilTemperature, "Temperature", "i", AbstractProperty::Read);

		/** @attributeName Pressure
		 *  @type unsigned short
		 *  @access readonly
		 *  @attributeComment \brief  Must return Engine Oil Pressure in kPa.
		 **/
		wantPropertyVariant(VehicleProperty::EngineOilPressure, "Pressure", "q", AbstractProperty::Read);

	}
};

/** @interface Location : VehiclePropertyType **/
class LocationProperty: public DBusSink
{
public:
	LocationProperty(AbstractRoutingEngine *re, GDBusConnection *connection)
			:DBusSink("Location", re, connection, map<string, string>())
	{
		/** @attributeName Latitude
		 *  @type double
		 *  @access readonly
		 *  @attributeComment \brief  Must return latitude in Deg.Min (-180, +180)
		 **/
		wantPropertyVariant(VehicleProperty::Latitude, "Latitude", "d", AbstractProperty::Read);

		/** @attributeName Longitude
		 *  @type double
		 *  @access readonly
		 *  @attributeComment \brief  Must return longitude in Deg.Min (-90, +90)
		 **/
		wantPropertyVariant(VehicleProperty::Longitude, "Longitude", "d", AbstractProperty::Read);

		/** @attributeName Altitude
		 *  @type double
		 *  @access readonly
		 *  @attributeComment \brief  Must return altitude in meters above sea-level (0).
		 **/
		wantPropertyVariant(VehicleProperty::Altitude, "Altitude", "d", AbstractProperty::Read);

		/** @attributeName Direction
		 *  @type unsigned short
		 *  @access readonly
		 *  @attributeComment \brief  Must return direction in Degrees  (0-360)
		 **/
		wantPropertyVariant(VehicleProperty::Direction, "Direction", "q", AbstractProperty::Read);

	}
};

#endif
