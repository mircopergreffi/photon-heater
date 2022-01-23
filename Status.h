
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
        volatile float powerHeater;
        volatile float temperatureSetpoint;
        volatile float temperatureHeater;
        volatile float temperatureAir; 
        // volatile double temperatureResin;
        volatile fan_mode_t fanMode;
        volatile float fanManualSpeed;
};

#endif /* STATUS_H */