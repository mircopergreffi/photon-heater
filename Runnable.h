
#ifndef RUNNABLE_H
#define RUNNABLE_H

#include "Sensor.h"

class Runnable
{
    public:
        virtual void readSensors(float dt) throw(SensorOutOfRange) = 0;
        virtual void run(float dt) = 0;
};

#endif /* RUNNABLE_H */