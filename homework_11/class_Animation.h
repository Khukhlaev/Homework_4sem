#ifndef HOMEWORK_11_CLASS_ANIMATION_H
#define HOMEWORK_11_CLASS_ANIMATION_H

class Animation {

public:
    Animation() = default;

    Animation(sf::Texture& t, int x, int y, int width, int height, int count, float Speed) : m_frame{0},
        m_speed {Speed}{

        for (int i = 0; i < count; i++)
            m_frames.emplace_back(x + i * width, y, width, height);

        m_sprite.setTexture(t);
        m_sprite.setOrigin(static_cast<float>(width) / 2, static_cast<float>(height) / 2);
        m_sprite.setTextureRect(m_frames[0]);
    }

    void update() {

        m_frame += m_speed;
        auto n = m_frames.size();

        if (m_frame >= n) m_frame -= n;

        if (n > 0)
            m_sprite.setTextureRect(m_frames[static_cast<int>(m_frame)]);

    }

    void setSpritePosition (float x, float y) {
        m_sprite.setPosition(x, y);
    }

    void setSpriteRotation (float angle) {
        m_sprite.setRotation(angle);
    }

    sf::Sprite& getSprite() {
        return m_sprite;
    }

public:

    bool isEnd() const {
        return m_frame + m_speed >= m_frames.size();
    }

private:
    float m_frame{}, m_speed{};
    std::vector<sf::IntRect> m_frames;
    sf::Sprite m_sprite;

};

#endif //HOMEWORK_11_CLASS_ANIMATION_H
