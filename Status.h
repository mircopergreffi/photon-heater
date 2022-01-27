
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
        float powerHeater;
        bool heater;
        float temperatureSetpoint;
        float temperatureHeater;
        float temperatureAir; 
        // float temperatureResin;
        volatile fan_mode_t fanMode;
        float fanManualSpeed;
};

#endif /* STATUS_H */