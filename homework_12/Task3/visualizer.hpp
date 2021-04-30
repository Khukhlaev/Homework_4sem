#pragma once

#include <SFML/Graphics.hpp>

#include "calculator.hpp"

class Visualizer
{
public:

    Visualizer(unsigned int number_particles, unsigned int window_height, unsigned int window_width,
               unsigned int number_cells) :
            m_window_width(window_width), m_window_height(window_height),
            m_window(sf::VideoMode(m_window_width, m_window_height),
                     "BROWNIAN MOTION", sf::Style::Close), m_number_cells(number_cells),
            m_calculator(number_cells, number_cells, number_particles)
    {
        initialize();
    }

    ~Visualizer() noexcept = default;

private:

    void initialize();

public:

    void run();

private:

    void review();
    void update();
    void render();

    void renderImage(unsigned int x, unsigned int y, sf::Color color);


private:
    const unsigned int m_window_height, m_window_width;

    unsigned int m_number_cells; // How many cells on one axis

    sf::RenderWindow m_window;
    sf::Image        m_image;
    sf::Sprite       m_sprite;
    sf::Texture      m_texture;

    Calculator m_calculator;

    std::vector < sf::Color > m_colors;

    unsigned int m_max_number_in_cell;
};