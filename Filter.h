
#ifndef FILTER_H
#define FILTER_H

class Filter
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
		virtual void loadFromJson(JsonObject json)
		{
			
		}
};

#endif /* FILTER_H */