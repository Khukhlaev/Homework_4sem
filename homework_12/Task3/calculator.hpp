#pragma once

#include <vector>
#include <random>
#include <chrono>
#include <thread>

#include <SFML/Graphics.hpp>

class Calculator
{
public:

    using field_t = std::vector < std::vector < unsigned int> > ;

public:

    explicit Calculator(std::size_t width, std::size_t height, unsigned int particles) :
            m_width(width), m_height(height), number_particles(particles),
            m_mersenne(std::chrono::system_clock::now().time_since_epoch().count()),
            m_dist_for_cells(1, 4), m_dist_for_side_cells(1, 3)
	{
		initialize();
	}

	~Calculator() noexcept = default;

private:

	void initialize();

	void calculateCell(int i, int j, unsigned int element);

public:

    const auto & field() const noexcept 
	{ 
		return m_field; 
	}

	const auto width () const noexcept 
	{ 
		return m_width;  
	}

	const auto height() const noexcept 
	{ 
		return m_height; 
	}

public:

	void calculate();

    unsigned int maxElement();

private:

	std::size_t m_width;
	std::size_t m_height;

	field_t m_field;

	const unsigned int number_particles;

	std::mt19937_64 m_mersenne;
	std::uniform_int_distribution<unsigned int> m_dist_for_cells, m_dist_for_side_cells;
};