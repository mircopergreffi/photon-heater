
#ifndef FILTER_H
#define FILTER_H

#include "JsonLoadable.h"

/**
 * Base class for filters, applies no filter.
 */

class Filter : public JsonLoadable
{
	public:
		Filter()
		{

		}
		// Apply the filter
		virtual float filter(float value, float dt)
		{
			return value;
		}
		// Loads values and parameters from json object
		void loadFromJson(JsonObject const &json)
		{
			
		}

		static Filter * fromJson(JsonObject const &json);
};

#include "LowPassFilter.h"
#include "HighPassFilter.h"

Filter * Filter::fromJson(JsonObject const &json)
{
	String type = json["type"].as<String>();
	type.toLowerCase();
	Filter * filter;
	if (type.compareTo("lowpass") == 0)
		filter = new LowPassFilter();
	else if (type.compareTo("highpass") == 0)
		filter = new HighPassFilter();
	else
		filter = new Filter();
	filter->loadFromJson(json);
	return filter;
}

#endif /* FILTER_H */