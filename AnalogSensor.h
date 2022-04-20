

#ifndef ANALOG_SENSOR_H
#define ANALOG_SENSOR_H

#include <driver/adc.h>
#include <esp_adc_cal.h>

#include "Filter.h"
#include "LowPassFilter.h"
#include "Sensor.h"

class AnalogSensor : public Sensor
{
	public:
		AnalogSensor()
		{
			mFilter = nullptr;
			mAtten = ADC_ATTEN_11db;
		}
		void loadFromJson(JsonObject const &json) throw(InvalidJsonException)
		{
			Sensor::loadFromJson(json);

			mPin = json["pin"].as<int>();
			mChannel = (adc1_channel_t) mPin;
			mName = json["name"].as<String>();

			if (mFilter != nullptr)
				delete mFilter;
			if (json.containsKey("filter"))
				mFilter = Filter::fromJson(json["filter"]);
			else
				mFilter = nullptr;
			
			if (json.containsKey("attenuation"))
			{
				String attenuation = json["attenuation"].as<String>();
				if (attenuation.compareTo("0dB") == 0)
					mAtten = ADC_ATTEN_0db;
				else if (attenuation.compareTo("2.5dB") == 0)
					mAtten = ADC_ATTEN_2_5db;
				else if (attenuation.compareTo("6dB") == 0)
					mAtten = ADC_ATTEN_6db;
				else if (attenuation.compareTo("11dB") == 0)
					mAtten = ADC_ATTEN_11db;
			}

			setupADC();
		}
		float readValue(float dt)
		{
			float value = ((float) (esp_adc_cal_raw_to_voltage(adc1_get_raw(mChannel), &mADCChars)));
			// Convert millivolts to volts
			value /= 1000.0;
			if (mFilter != nullptr)
				value = mFilter->filter(value, dt);
			return value;
		}

	private:
		void setupADC()
		{
			adc1_config_width(ADC_WIDTH_BIT_12);
			adc1_config_channel_atten(mChannel, mAtten);
			esp_adc_cal_characterize(ADC_UNIT_1, mAtten, ADC_WIDTH_BIT_12, 1100, &mADCChars);
		}

		int mPin;
		String mName;
		Filter * mFilter;
		adc1_channel_t mChannel;
		adc_atten_t mAtten;
		esp_adc_cal_characteristics_t mADCChars;
};

#endif /* ANALOG_SENSOR_H */
