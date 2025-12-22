#ifndef BODY_H
#define BODY_H

#include <GL/glew.h>

#include <vector>

constexpr float GRAVITATION_CONSTANT{6.6743e10-11};

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
        GLuint m_vao;

    public:
        Body(float mass, float radius, Position position={0.0f, 0.0f}, Velocity velocity={0.0f, 0.0f});

        // getters
        float get_mass() {return m_mass;}
        float get_radius() {return m_radius;}
        unsigned int get_vao() {return m_vao;}

        // setters
        void set_vao(GLuint vao) {m_vao = vao;}

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
        
    public:
        void add_body(Body&);
        void update_system(float dt);
        void draw_system(float (*)[4], const int&);
        std::vector<Body> particles;

};

#endif
