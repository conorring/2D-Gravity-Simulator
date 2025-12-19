#ifndef BODY_H
#define BODY_H

#include <vector>

struct Position
{
    float x;
    float y;
};

typedef Position Velocity;
typedef Position Acceleration;
typedef Position Force;

class Body
{
    private:
        const float m_mass;
        const float m_radius;

    public:
        Body(float mass, float radius, Position position={0.0f, 0.0f}, Velocity velocity={0.0f, 0.0f});

        // getters
        float get_mass() {return m_mass;}
        float get_radius() {return m_radius;}

        Position m_position;
        Velocity m_velocity;
        Acceleration m_acceleration;

        void update_position(float dt);
        void update_velocity(float dt);
        void update_acceleration(float force_x, float force_y);
};

class System
{
    private:
        std::vector<Body> particles;

    public:
        void add_body(Body&);
        void update_system(float dt);
};

#endif
