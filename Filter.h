
#ifndef FILTER_H
#define FILTER_H

class Filter
{
    public:
        Filter()
        {

        }
        virtual float filter(float value)
        {
            return value;
        }
};

#endif /* FILTER_H */