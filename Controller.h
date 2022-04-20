
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "JsonLoadable.h"

class Controller : public JsonLoadable
{
	public:
		Controller()
		{
			setOutputLimits(0.0, 1.0);
		}

		void setOutputLimits(float min, float max)
		{
			mOutputMin = min;
			mOutputMax = max;
		}
		float getOutputMin() const
		{
			return mOutputMin;
		}
		float getOutputMax() const
		{
			return mOutputMax;
		}

		void loadFromJson(JsonObject const &json)
		{
			float min = 0, max = 1;
			if (json.containsKey("min"))
				min = json["min"].as<float>();
			if (json.containsKey("max"))
				max = json["max"].as<float>();
			setOutputLimits(min, max);
		}

		virtual float compute(float input, float setpoint, float dt)
		{
			float output = mOutputMin;
			boundOutput(output);
			return output;
		}

		static Controller * fromJson(JsonObject const &json);

	protected:
		void boundOutput(float & output) const
		{
			if (output > mOutputMax)
				output = mOutputMax;
			else if (output < mOutputMin)
				output = mOutputMin;
		}
	
	private:
		float mOutputMin, mOutputMax;
};

#include "PID.h"

Controller * Controller::fromJson(JsonObject const &json)
{
	String type = json["type"].as<String>();
	type.toLowerCase();
	Controller * controller;
	if (type.compareTo("pid") == 0)
		controller = new PID();
	else
		controller = new Controller();
	controller->loadFromJson(json);
	return controller;
}

#endif /* CONTROLLER_H */