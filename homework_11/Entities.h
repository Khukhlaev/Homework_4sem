
#ifndef HOMEWORK_11_ENTITIES_H
#define HOMEWORK_11_ENTITIES_H

enum class Entities_types {
    asteroid,
    bullet,
    player,
    explosion
};

class Entity {

public:

    Entity(Animation& animation, float x, float y, const unsigned int window_width, const unsigned int window_height,
           float angle = 0.0f, float radius = 1.0f):
            m_life{true}, m_animation{animation}, m_x{x}, m_y{y}, m_angle{angle}, m_radius{radius},
            m_dx{0.0f}, m_dy{0.0f}, m_type{Entities_types::explosion},
            m_window_width {static_cast<float>(window_width)}, m_window_height{static_cast<float>(window_height)},
            deg_to_rad{0.017453f}
            {}


    void draw(sf::RenderWindow& app) {

        m_animation.setSpritePosition(m_x, m_y);
        m_animation.setSpriteRotation(m_angle + 90.0f);
        app.draw(m_animation.getSprite());

    }

    virtual void update() {};

public:

    virtual ~Entity() = default;

    bool isAlive() const {
        return m_life;
    }

    void kill() {
        m_life = false;
    }

    float getX() const {
        return m_x;
    }

    float getY() const {
        return m_y;
    }

    float getAngle() const {
        return m_angle;
    }

    float getRadius() const {
        return m_radius;
    }

    Animation& getAnimation() {
        return m_animation;
    }

protected:

    Animation m_animation;
    float m_x, m_y, m_dx, m_dy, m_angle, m_radius;
    bool m_life;
    Entities_types m_type;

    const float deg_to_rad;

    const float m_window_width, m_window_height;

};


class Asteroid : public Entity {

public:

    Asteroid(Animation& animation, float x, float y, const unsigned int window_width, const unsigned int window_height,
             float angle = 0.0f, float radius = 1.0f) :

        Entity(animation, x, y, window_width, window_height, angle, radius) {

        std::mt19937 mersenne (std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_real_distribution <float> urd(-4, 4);

        m_dx = urd(mersenne);
        m_dy = urd(mersenne);

        m_type = Entities_types::asteroid;

    }

    void update() override {

        m_x += m_dx;
        m_y += m_dy;

        if (m_x > m_window_width) m_x = 0;
        if (m_x < 0) m_x = m_window_width;

        if (m_y > m_window_height) m_y = 0;
        if (m_y < 0) m_y = m_window_height;
    }

};


class Bullet : public Entity {

public:

    Bullet(Animation& animation, float x, float y, const unsigned int window_width, const unsigned int window_height,
           float angle = 0.0f, float radius = 1.0f) :

        Entity(animation, x, y, window_width, window_height, angle, radius) {

        m_type = Entities_types::bullet;
    }

    void update() override {

        m_dx = std::cos(m_angle * deg_to_rad) * 6.0f;
        m_dy = std::sin(m_angle * deg_to_rad) * 6.0f;

        m_x += m_dx;
        m_y += m_dy;

        if (m_x > m_window_width || m_x < 0 || m_y > m_window_height || m_y < 0) m_life = false;
    }

};


class Player : public Entity {

public:

    Player(Animation& animation, float x, float y, const unsigned int window_width, const unsigned int window_height,
           float angle = 0.0f, float radius = 1.0f) :

            Entity(animation, x, y, window_width, window_height, angle, radius), m_thrust{false} {

        m_type = Entities_types::player;
    }

public:

    void modifyAngle(float delta) {
        m_angle += delta;
    }

    void startThrust() {
        m_thrust = true;
    }

    void stopThrust() {
        m_thrust = false;
    }

    bool getThrustState() const {
        return m_thrust;
    }


public:

    void update() override {

        if (m_thrust) {
            m_dx += std::cos(m_angle * deg_to_rad) * 0.2f;
            m_dy += std::sin(m_angle * deg_to_rad) * 0.2f;
        }
        else {
            m_dx *= speed_decrease;
            m_dy *= speed_decrease;
        }

        float speed = std::sqrt(m_dx * m_dx + m_dy * m_dy);

        if (speed > max_speed) {
            m_dx *= max_speed / speed;
            m_dy *= max_speed / speed;
        }

        m_x += m_dx;
        m_y += m_dy;

        if (m_x > m_window_width) m_x = 0;
        if (m_x < 0) m_x = m_window_width;

        if (m_y > m_window_height) m_y = 0;
        if (m_y < 0) m_y = m_window_height;
    }


private:
    bool m_thrust;

    const float speed_decrease = 0.99f;
    const float max_speed = 15.0f;

};


#endif //HOMEWORK_11_ENTITIES_H
