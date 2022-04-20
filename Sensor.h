
#ifndef SENSOR_H
#define SENSOR_H

#include "JsonLoadable.h"

struct SensorOutOfRange : public std::exception 
{
	const char * what() const throw()
	{
		return "Sensor value out of range";
	}
};

class Sensor : public JsonLoadable
{
	public:
		virtual float readValue(float dt)
		{
			return 0;
		}

		void loadFromJson(JsonObject const &json)
		{

		}

		static Sensor * fromJson(JsonObject const &json) throw (InvalidJsonException);
};

#include "AnalogSensor.h"
#include "ResistorSensor.h"
#include "NTCSensor.h"

Sensor * fromJson(JsonObject const &json) throw (InvalidJsonException)
{
	String type = json["type"].as<String>();
	type.toLowerCase();
	Sensor * sensor;
	if (type.compareTo("analog") == 0)
		sensor = new AnalogSensor();
	else if (type.compareTo("resistor") == 0)
		sensor = new ResistorSensor();
	else if (type.compareTo("ntc") == 0)
		sensor = new NTCSensor();
	else
		throw InvalidJsonException("Undefined sensor type");
	sensor->loadFromJson(json);
	return sensor;
}

#endif /* SENSOR_H */