#ifndef PLANET_H
#define PLANET_H

class Body
{
public:
    Body();
    
private:
    const double mass;              // in kg
    const double radius;            // in m
    double position[2];
    double velocity[2];             // in m/s
    double acceleration[2];         // in m/s^2
};

#endif
