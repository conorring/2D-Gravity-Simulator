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
    next_position.x = 2*m_position.x + m_acceleration.x*dt*dt - m_prev_position.x;
    next_position.y = 2*m_position.y + m_acceleration.y*dt*dt - m_prev_position.y;

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
    forces.emplace_back(0.0f, 0.0f);
}

std::vector<Position> System::get_positions() const
{
    std::vector<Position> positions;
    positions.reserve(particles.size());
    for(auto& particle : particles)
    {
        positions.push_back({particle.get_position()});
    }

    return positions;
}

void System::update_forces()
{
    if(forces.size() != particles.size())
    {
        forces.resize(particles.size());
    }

    for(std::size_t i{0}; i < particles.size(); i++)
    {
        forces[i] = {0.0f, -9.81f * particles[i].get_mass()};
    }
}


void System::update_system(float dt)
{
    update_forces();

    // first integrate positions using Verlet
    for(std::size_t i{0}; i < particles.size(); i++)
    {
        Body& body_i = particles[i];
        body_i.update_acceleration(forces[i]);
        body_i.update_position(dt);

        Position pos_i{body_i.get_position()};
        if(pos_i.y < -1.0f) pos_i.y = -1.0f;
        if(pos_i.x < -1.0f) pos_i.x = -1.0f;
        if(pos_i.x > 1.0f)  pos_i.x = 1.0f;

        body_i.set_position(pos_i);
    }

    // then resolve collisions (pairwise)
    for(std::size_t i{0}; i < particles.size(); i++)
    {
        for(std::size_t j{i + 1}; j < particles.size(); j++)
        {
            Body& body_i = particles[i];
            Body& body_j = particles[j];

            if(penetration(body_i, body_j) > 0.0f)
            {
                resolve_collision(body_i, body_j, dt, 0.5f);
            }
        }
    }
}




// helper functions not class member functions
void resolve_collision(Body& body_a, Body& body_b, const float dt, const float restitution)
{
    const Position pos_a = body_a.get_position();
    const Position pos_b = body_b.get_position();

    Position delta{pos_b.x - pos_a.x, pos_b.y - pos_a.y};
    float dist = std::hypotf(delta.x, delta.y);

    const float r_sum = body_a.get_radius() + body_b.get_radius();
    const float penetration_depth = r_sum - dist;
    if(penetration_depth <= 0.0f) return;

    // normalize
    delta.x /= dist;
    delta.y /= dist;

    // separate bodies along collision normal
    const float half_penetration = 0.5f * penetration_depth;
    body_a.set_position({pos_a.x - delta.x * half_penetration, pos_a.y - delta.y * half_penetration});
    body_b.set_position({pos_b.x + delta.x * half_penetration, pos_b.y + delta.y * half_penetration});

    // estimate velocities from Verlet positions
    // think this should be done before we separate bodies along collision line
    const Position prev_a = body_a.get_prev_position();
    const Position prev_b = body_b.get_prev_position();
    Velocity vel_a{(body_a.get_position().x - prev_a.x) / dt, (body_a.get_position().y - prev_a.y) / dt};
    Velocity vel_b{(body_b.get_position().x - prev_b.x) / dt, (body_b.get_position().y - prev_b.y) / dt};

    // relative velocity along normal
    const float rel_vel = (vel_b.x - vel_a.x) * delta.x + (vel_b.y - vel_a.y) * delta.y;

    if(rel_vel > 0.0f)
    {
        // already separating
        return;
    }


    const float inv_mass_a = 1.0f / body_a.get_mass();
    const float inv_mass_b = 1.0f / body_b.get_mass();

    const float impulse = -(1.0f + restitution) * rel_vel / (inv_mass_a + inv_mass_b);

    const float impulse_a = impulse * inv_mass_a;
    const float impulse_b = impulse * inv_mass_b;

    // apply impulses by adjusting previous positions to reflect the new velocity
    const Position new_vel_a{vel_a.x - impulse_a * delta.x, vel_a.y - impulse_a * delta.y};
    const Position new_vel_b{vel_b.x + impulse_b * delta.x, vel_b.y + impulse_b * delta.y};

    body_a.set_prev_position({body_a.get_position().x - new_vel_a.x * dt, body_a.get_position().y - new_vel_a.y * dt});
    body_b.set_prev_position({body_b.get_position().x - new_vel_b.x * dt, body_b.get_position().y - new_vel_b.y * dt});
}

float penetration(const Body& body1, const Body& body2)
{
    float r1{body1.get_radius()};
    float r2{body2.get_radius()};
    float dist{bodies_distance(body1, body2)};

    return r1+r2-dist;
}

float bodies_distance(const Body& body1, const Body& body2)
{
    float dist, dist_x, dist_y;

    dist_x = body2.get_position().x - body1.get_position().x;
    dist_y = body2.get_position().y - body1.get_position().y;
    dist = std::hypotf(dist_x, dist_y);

    return dist;
}
