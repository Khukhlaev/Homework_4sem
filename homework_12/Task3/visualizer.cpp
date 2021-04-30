#include "visualizer.hpp"

void Visualizer::initialize()
{
    m_image.create(m_window_width, m_window_height, sf::Color::White);

    m_colors.assign({
                            sf::Color(255, 230, 230),
                            sf::Color(246, 180, 180),
                            sf::Color(238, 140, 140),
//                            sf::Color(24, 82, 177),
//                            sf::Color(57, 125, 209),
//                            sf::Color(134, 181, 229),
//                            sf::Color(211, 236, 248),
//                            sf::Color(241, 233, 191),
//                            sf::Color(248, 201, 95),
                            sf::Color(250, 128, 114),
                            sf::Color(178, 34, 34),
                            sf::Color(139, 0, 0),
                            sf::Color(137, 14, 14),
                            sf::Color(128, 0, 0) });
}

void Visualizer::run()
{
    while (m_window.isOpen())
    {
        review();
        update();
        render();
    }
}

void Visualizer::review()
{
    sf::Event event;

    while (m_window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                m_window.close();
                break;
            default:
                break;
        }
    }
}

void Visualizer::update()
{
    m_calculator.calculate();

    m_max_number_in_cell = m_calculator.maxElement();
}


void Visualizer::renderImage(unsigned int x, unsigned int y, sf::Color color)
{
    for (auto j = y * m_window_height / m_number_cells; j < (y + 1) * m_window_height / m_number_cells - 1; j++)
    {
        for (auto i = x * m_window_width / m_number_cells; i < (x + 1) * m_window_height / m_number_cells - 1; i++)
        {
            m_image.setPixel(i, j, color);
        }
    }

}

void Visualizer::render()
{

    m_window.clear();

    const auto & field = m_calculator.field();

    for (auto y = 0U; y < m_calculator.height(); ++y)
    {
        for (auto x = 0U; x < m_calculator.width(); ++x)
        {
            const auto number_particles = field[y][x];

            if (number_particles == 0U)
            {
                renderImage(x, y, sf::Color::White);
            }
            else
            {
                renderImage(x, y, m_colors[static_cast < std::size_t > (
                        (static_cast<float>(number_particles) / static_cast<float> (m_max_number_in_cell))
                        * (m_colors.size() - 1.0f))]);
            }
        }
    }



    m_texture.loadFromImage(m_image);
    m_sprite.setTexture(m_texture, true);

    m_window.draw(m_sprite);

    m_window.display();
}