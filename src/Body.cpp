#include "Body.h"
#include "Renderer.h"

#include <GL/glew.h>

#include <iostream>
#include <cmath>
#include <vector>

// constexpr float MIN_MASS{1000.0f};
// constexpr float MAX_MASS{1e31};

// constexpr float MIN_RADIUS{0.0f};
// constexpr float MAX_RADIUS{100.0f};

/******************************************************************************/
/* definitions for body class                                                 */
/******************************************************************************/
Body::Body(float mass, float radius, Position position, Velocity velocity)
    : m_mass(mass), m_radius(radius), m_position(position), m_velocity(velocity)
{
};

void Body::update_position(float dt)
{
    m_position.x = m_position.x + m_velocity.x * dt;
    m_position.y = m_position.y + m_velocity.y * dt;
}

void Body::update_velocity(float dt)
{
    m_velocity.x = m_velocity.x + m_acceleration.x * dt;
    m_velocity.y = m_velocity.y + m_acceleration.y * dt;
}

void Body::update_acceleration(float force_x, float force_y)
{
    m_acceleration.x = force_x / m_mass;
    m_acceleration.y = force_y / m_mass;
}


/******************************************************************************/
/* definitions for system class                                               */
/******************************************************************************/

void System::add_body(Body& body)
{
    particles.push_back(body);
}

void System::update_system(float dt)
{
    // loop variables
    // could have many particles
    int system_size{static_cast<int>(particles.size())};

    // result container for calculating total forces in system
    std::vector<Force> system_forces(system_size, Force{0.0f, 0.0f}); 

    for(int i=0; i < system_size; ++i)
    {
        Force& force1{system_forces[i]};
        Body& body1{particles[i]};

        for(int j=i+1; j < system_size; ++j)
        {
            Body& body2{particles[j]};

            float dist, dist_x, dist_y;

            dist_x = body2.m_position.x - body1.m_position.x;
            dist_y = body2.m_position.y - body1.m_position.y;
            dist = std::sqrt(pow(dist_x, 2) + pow(dist_y, 2));

            const float softening{1e-3};
            dist += softening;

            // warns of a narrowing conversion
            //float mag_f{ static_cast<float>(GRAVITATION_CONSTANT*body1.get_mass()*body2.get_mass() / (dist*dist)) }; // Newtons law of gravitation
            float mag_f{10.0f};

            Force& force2{system_forces[j]};

            force1.x += mag_f * (dist_x / dist);
            force1.y += mag_f * (dist_y / dist);
            force2.x -= mag_f * (dist_x / dist);
            force2.y -= mag_f * (dist_y / dist);
        }

        body1.update_acceleration(force1.x, force1.y);
    }

    for(Body& body:particles)
    {
        body.update_velocity(dt);
        body.update_position(dt);
    }

}

void System::draw_system(float (*transformation)[4], const int& location)
{
    for(Body& body:particles)
    {
        transformation[3][0] = body.m_position.x;
        transformation[3][1] = body.m_position.y;

        GLCall(glBindVertexArray(body.get_vao()));
        GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, transformation[0]));
        GLCall(glDrawArrays(GL_TRIANGLE_FAN, 0, constants::RES+2));
    }
}
