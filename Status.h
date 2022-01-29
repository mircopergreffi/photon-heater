
#ifndef STATUS_H
#define STATUS_H

enum fan_mode_t
{
	FAN_AUTO = 0,
	FAN_MANUAL = 1
};

class Status
{
	public:
		float dt;
		unsigned long currentTimestamp, lastTimestamp;

		float temperatureHeater;
		float temperatureAir; 
		// float temperatureResin;

		bool heater;
		float powerHeater;
		float heaterSetpoint;

		float airSetpoint;

		volatile fan_mode_t fanMode;
		float fanSpeed, fanManualSpeed;
};

#endif /* STATUS_H */