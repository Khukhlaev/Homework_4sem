#include <memory>

#ifndef HOMEWORK_11_CLASS_GAME_H
#define HOMEWORK_11_CLASS_GAME_H

class Game {

public:
    Game() : m_window{sf::VideoMode(m_window_width, m_window_height), "Asteroids"}, m_textures{7},
             m_asteroids{15}, m_player_lives{100}, asteroid_radius{25}, small_asteroid_radius{15},
             player_radius{15}, bullet_radius{10}, m_player_score{0},
             probability_for_new_asteroids{1.0f/150}{

        // Text setup
        if (!m_font.loadFromFile("../arial.ttf")) {
            std::cerr << "error..." << std::endl;
        }

        m_text.setFont(m_font);
        m_text.setCharacterSize(32);
        m_text.setFillColor(sf::Color::Red);
        m_text.setPosition(50, 50);


        // Animation setup
        m_textures[0].loadFromFile("../images/background.jpg");
        m_textures[1].loadFromFile("../images/spaceship.png");
        m_textures[2].loadFromFile("../images/fire_blue.png");
        m_textures[3].loadFromFile("../images/explosions/type_C.png"); // Rock explosion
        m_textures[4].loadFromFile("../images/explosions/type_B.png"); // Ship explosion
        m_textures[5].loadFromFile("../images/rock.png");
        m_textures[6].loadFromFile("../images/rock_small.png");

        m_textures[0].setSmooth(true);
        m_textures[1].setSmooth(true);


        m_background = sf::Sprite(m_textures[0]);

        m_anim_player           = Animation(m_textures[1], 40, 0, 40, 40, 1, 0);
        m_anim_player_go        = Animation(m_textures[1], 40, 40, 40, 40, 1, 0);
        m_anim_bullet           = Animation(m_textures[2], 0, 0, 32, 64, 16, 0.8);
        m_anim_rock_explosion   = Animation(m_textures[3], 0, 0, 256, 256, 48, 0.5);
        m_anim_ship_explosion   = Animation(m_textures[4], 0, 0, 192, 192, 64, 0.5);
        m_anim_rock             = Animation(m_textures[5], 0, 0, 64, 64, 16, 0.2);
        m_anim_small_rock       = Animation(m_textures[6], 0, 0, 64, 64, 16, 0.2);


        m_window.setFramerateLimit(60u);


        // Objects setup
        makePlayer();

        std::mt19937 mersenne (std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_real_distribution <float> urd_width(0, m_window_width);
        std::uniform_real_distribution <float> urd_height(0, m_window_height);
        std::uniform_real_distribution <float> urd_angle(0, 360);

        for (auto& asteroid : m_asteroids) {
            asteroid = std::make_shared<Asteroid>(m_anim_rock, urd_width(mersenne), urd_height(mersenne),
                                               m_window_width, m_window_height,
                                               urd_angle(mersenne), asteroid_radius);

        }

        
    }


    void run() {

        // Main loop
        while (m_window.isOpen() && isPlayerAlive()) {

            processingEvents();

            std::mt19937 mersenne (std::chrono::system_clock::now().time_since_epoch().count());
            std::uniform_real_distribution <float> urd_height(0, m_window_height);
            std::uniform_real_distribution <float> urd_angle(0, 360);

            for (const auto& asteroid : m_asteroids) {

                for (const auto& bullet : m_bullets) {

                    if (isCollideBullet(asteroid, bullet)) {

                        asteroid->kill();
                        bullet->kill();

                        makeRockExplosion(asteroid->getX(), asteroid->getY());

                        m_player_score++;

                        if (asteroid->getRadius() == asteroid_radius) {
                            // Spawn 2 small asteroids instead of a big one
                            makeSmallAsteroid(asteroid->getX(), asteroid->getY(), urd_angle(mersenne));
                            makeSmallAsteroid(asteroid->getX(), asteroid->getY(), urd_angle(mersenne));
                        }

                    }

                }

                if (isCollide(m_player, asteroid)) {

                    asteroid->kill();

                    makeShipExplosion(m_player->getX(), m_player->getY());

                    m_player_lives--;

                    m_player.reset();

                    makePlayer();
                }

            }


            if (m_player->getThrustState()) m_player->getAnimation() = m_anim_player_go;
            else m_player->getAnimation() = m_anim_player;


            for (const auto& explosion : m_explosions) {
                if (explosion->getAnimation().isEnd()) explosion->kill();
            }


            std::bernoulli_distribution bool_distrib (probability_for_new_asteroids);

            if (bool_distrib(mersenne)) {
                makeAsteroid(0, urd_height(mersenne), urd_angle(mersenne));
            }


            update();


            draw();

        }

    }

private:
    const unsigned int m_window_width = sf::VideoMode::getDesktopMode().width;
    const unsigned int m_window_height = sf::VideoMode::getDesktopMode().height;

    const float player_radius, bullet_radius, asteroid_radius, small_asteroid_radius;
    const float probability_for_new_asteroids;

    int m_player_lives;
    int m_player_score;

    sf::Font m_font;
    sf::Text m_text;

    sf::RenderWindow m_window;

    std::vector<sf::Texture> m_textures;
    sf::Sprite m_background;
    Animation m_anim_player, m_anim_player_go, m_anim_bullet, m_anim_rock_explosion, m_anim_ship_explosion;
    Animation m_anim_rock, m_anim_small_rock;

    std::shared_ptr<Player> m_player;

    std::vector<std::shared_ptr<Asteroid>> m_asteroids;
    std::vector<std::shared_ptr<Bullet>> m_bullets;
    std::vector<std::shared_ptr<Entity>> m_explosions;

private:

    bool isPlayerAlive() const {
        return (m_player_lives > 0);
    }

    void makePlayer() {
        m_player = std::make_shared<Player>(m_anim_player, static_cast<float>(m_window_width) / 2.0f,
                                            static_cast<float>(m_window_height) / 2.0f, m_window_width,
                                            m_window_height, 0.0f, player_radius);

    }


    void makeBullet() {

        std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(m_anim_bullet, m_player->getX(),
                                                                  m_player->getY(), m_window_width,
                                                                  m_window_height,
                                                                  m_player->getAngle(), bullet_radius);

        m_bullets.push_back(bullet);
    }


    void makeRockExplosion(float x, float y) {

        std::shared_ptr<Entity> explosion = std::make_shared<Entity>(m_anim_rock_explosion, x, y, m_window_width,
                                                                     m_window_height);

        m_explosions.push_back(explosion);

    }

    void makeShipExplosion(float x, float y) {

        std::shared_ptr<Entity> explosion = std::make_shared<Entity>(m_anim_ship_explosion, x, y, m_window_width,
                                                                     m_window_height);

        m_explosions.push_back(explosion);

    }

    void makeAsteroid(float x, float y, float angle) {

        m_asteroids.push_back(std::make_shared<Asteroid>(m_anim_rock, x, y,
                                                         m_window_width, m_window_width, angle, asteroid_radius));

    }

    void makeSmallAsteroid(float x, float y, float angle) {

        m_asteroids.push_back(std::make_shared<Asteroid>(m_anim_small_rock, x, y,
                                                         m_window_width, m_window_width, angle, small_asteroid_radius));

    }

    void processingEvents() {

        sf::Event event{};

        while (m_window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                m_window.close();

            if (event.type == sf::Event::KeyPressed) {

                if (event.key.code == sf::Keyboard::Space) {
                    makeBullet();
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) m_player->modifyAngle(3.0f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) m_player->modifyAngle(-3.0f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) m_player->startThrust();
        else m_player->stopThrust();

    }



private:

    static bool isCollideBullet(const std::shared_ptr<Asteroid>& a, const std::shared_ptr<Bullet>& b) {
        float x = b->getX() - a->getX();
        float y = b->getY() - a->getY();
        float r = a->getRadius() + b->getRadius();

        return x * x + y * y <= r * r;
    }


    template<typename EntityPointer1, typename EntityPointer2>
    bool isCollide(EntityPointer1 a, EntityPointer2 b) {
        float x = b->getX() - a->getX();
        float y = b->getY() - a->getY();
        float r = a->getRadius() + b->getRadius();

        return x * x + y * y <= r * r;
    }


    template<typename VectorPointers>
    void update_vector(VectorPointers& vector) {

        for (const auto& i : vector) {
            i->update();

            i->getAnimation().update();
        }
    }

    template<typename VectorPointers>
    void erase_vector(VectorPointers& vector) {

        vector.erase(std::remove_if(vector.begin(), vector.end(), [](auto x){return !x->isAlive();}), vector.end());
    }

    void update() {

        update_vector(m_asteroids);
        update_vector(m_bullets);
        update_vector(m_explosions);

        m_player->update();

        erase_vector(m_asteroids);
        erase_vector(m_bullets);
        erase_vector(m_explosions);


        m_text.setString("Score: " + std::to_string(m_player_score) +
        "\nLives: " + std::to_string(m_player_lives));

    }

    template<typename VectorPointers>
    void draw_vector(VectorPointers& vector) {

        for (const auto& i : vector) {
            i->draw(m_window);
        }

    }

    void draw() {

        m_window.draw(m_background);
        m_window.draw(m_text);

        draw_vector(m_asteroids);
        draw_vector(m_explosions);
        draw_vector(m_bullets);

        m_player->draw(m_window);


        m_window.display();
    }


};

#endif //HOMEWORK_11_CLASS_GAME_H
