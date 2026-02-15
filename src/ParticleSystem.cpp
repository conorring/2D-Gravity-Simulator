#include "ParticleSystem.h"

#include <cmath>
#include <vector>

#include "Renderer.h"

/******************************************************************************/
/* definitions for body class                                                 */
/******************************************************************************/
Body::Body(float mass, float radius, Position position, Position prev_position, Acceleration acceleration)
    : m_mass(mass), m_radius(radius), m_position(position), m_prev_position(prev_position), m_acceleration(acceleration)
{
};

void Body::update_position(float dt)
{
    // verlet integration
    Position next_position{};
    next_position.x = 2*m_position.x + m_acceleration.x*dt*dt + m_prev_position.x;
    next_position.y = 2*m_position.y + m_acceleration.y*dt*dt + m_prev_position.y;

    m_prev_position = m_position;
    m_position = next_position;
}

void Body::update_acceleration(Force force)
{
    m_acceleration.x = force.x / m_mass;
    m_acceleration.y = force.y / m_mass;
}


/******************************************************************************/
/* definitions for system class                                               */
/******************************************************************************/

System::System(int no_particles)
{
    particles.reserve(no_particles);
    forces.reserve(no_particles);
}

void System::add_body(float mass, float radius, Position position, Position prev_position, Acceleration acceleration)
{
    particles.emplace_back(mass, radius, position, prev_position, acceleration);
}

std::vector<Position> System::get_positions() const
{
    std::vector<Position> positions;
    for(auto& particle : particles)
    {
        positions.push_back({particle.get_position()});
    }

    return positions;
}

void System::update_system(float dt)
{
    std::size_t system_size{particles.size()};

    // need to reset to 0 as we are accumulating total force by summing over particles
    std::fill(forces.begin(), forces.end(), Force {0.0f, 0.0f});

    for(std::size_t i{0}; i < system_size; ++i)
    {
        Force& force1{forces[i]};
        Body& body1{particles[i]};

        for(std::size_t j=i+1; j < system_size; ++j)
        {
            Body& body2{particles[j]};

            float dist, dist_x, dist_y;

            dist_x = body2.get_position().x - body1.get_position().x;
            dist_y = body2.get_position().y - body1.get_position().y;
            dist = std::hypotf(dist_x, dist_y);

            const float softening{1e-4};
            dist += softening;

            float mag_f{ static_cast<float>(constants::GRAVITATION_CONSTANT*body1.get_mass()*body2.get_mass() / (dist*dist)) }; // Newtons law of gravitation

            Force& force2{forces[j]};

            force1.x += mag_f * (dist_x / dist);
            force1.y += mag_f * (dist_y / dist);
            force2.x -= mag_f * (dist_x / dist);
            force2.y -= mag_f * (dist_y / dist);
        }
        
        // can update inside loop as bodies after i already account for force from body i
        body1.update_acceleration(force1);
        body1.update_position(dt);
    }
}
