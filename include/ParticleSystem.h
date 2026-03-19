#ifndef BODY_H
#define BODY_H

#include <vector>

//TODO 
// 1.

namespace constants
{
    constexpr float GRAVITATION_CONSTANT{6.6743e-11f}; 
    constexpr float AU{1.496e11};
    constexpr float r_sun{6.957e8f};
}

struct Position
{
    float x;
    float y;
};

using Velocity = Position;
using Acceleration = Position;
using Force = Position;

class Body
{
    private:
        const float m_mass;
        const float m_radius;

        Position m_position;
        Position m_prev_position;
        Acceleration m_acceleration;

    public:
        Body(float mass, float radius, Position position={0.0f, 0.0f}, Position prev_position={0.0f, 0.0f}, Acceleration acceleration={0.0f, 0.0f});

        float get_mass() const { return m_mass; }
        Position get_position() const { return m_position; }
        Position get_prev_position() const { return m_prev_position; }
        float get_radius() const { return m_radius; }

        void set_position(Position position) { m_position = position; }
        void set_prev_position(Position prev_position) { m_prev_position = prev_position; }

        void update_position(float dt);
        void update_acceleration(Force force);
};

class System
{        
    private:
        std::vector<Body> particles;
        std::vector<Force> forces;

    public:
        // reserves space in particles and forces vector nothing else
        System(int no_particles);

        std::vector<Position> get_positions() const;

        void add_body(float mass, float radius, Position position={0.0f, 0.0f}, Position prev_position={0.0f, 0.0f}, Acceleration acceleration={0.0f, 0.0f});
        void update_forces();
        void update_accelerations();
        void update_system(float dt);
};

#endif
