#include "system.hpp"

void System::initialize()
{
	const auto size = std::size(m_particles);

	for (auto i = 0U; i < size; ++i)
	{
	    for (auto j = 0U; j < m_particles[i].size(); j++)
	    {
	        if (j < m_particles[i].size() - 1)
	            m_links.emplace_back(particle(i,j), particle(i, j + 1), 0.5f);

	        if (i < size - 1)
                m_links.emplace_back(particle(i,j), particle(i + 1, j), 0.5f);

	        if (i < size - 1 && j < m_particles[i].size() - 1)
                m_links.emplace_back(particle(i,j), particle(i + 1, j + 1), 0.5f);

	        if (i < size - 1 && j > 0)
                m_links.emplace_back(particle(i,j), particle(i + 1, j - 1), 0.5f);

	        if (i < size - 2 && j < m_particles[i].size() - 1)
                m_links.emplace_back(particle(i,j), particle(i + 2, j + 1), 0.5f);

            if (i < size - 1 && j < m_particles[i].size() - 2)
                m_links.emplace_back(particle(i,j), particle(i + 1, j + 2), 0.5f);

            if (i < size - 2 && j > 0)
                m_links.emplace_back(particle(i,j), particle(i + 2, j - 1), 0.5f);

            if (i < size - 1 && j > 1)
                m_links.emplace_back(particle(i,j), particle(i + 1, j - 2), 0.5f);

	    }
	}
}

void System::push(const sf::Vector2f force) const
{

	for (const auto& vector : m_particles)
	{
	    for (const auto& particle : vector)
		    particle->move(force);
	}
}

void System::update() const
{

    for (const auto& vector : m_particles)
    {
        for (const auto& particle : vector)
        {
            particle->move(0.25f);

            if (particle->position().y + particle->radius() > m_max_point.y)
            {
                particle->set_y(m_max_point.y - particle->radius());
            }

            if (particle->position().y - particle->radius() < m_min_point.y)
            {
                particle->set_y(m_min_point.y + particle->radius());
            }

            if (particle->position().x + particle->radius() > m_max_point.x)
            {
                particle->set_x(m_max_point.x - particle->radius());
            }

            if (particle->position().x - particle->radius() < m_min_point.x)
            {
                particle->set_x(m_min_point.x + particle->radius());
            }
        }
    }


	for (auto i = 0U; i < std::size(m_links); ++i)
	{
		m_links[i].update();
	}

}