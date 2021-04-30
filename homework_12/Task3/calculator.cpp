#include "calculator.hpp"

void Calculator::initialize()
{
	m_field.resize(m_height);

//    for (auto i = 0U; i < m_field.size(); i++)
//    {
//        m_field[i].resize(m_width, 0);
//        for (auto j = 0U; j < m_field[i].size(); j++)
//        {
//            if (i == m_width / 2 && j == m_height / 2) m_field[i][j] = number_particles;
//        }
//    }
	for (auto & row : m_field)
	{
		row.resize(m_width);

		for (auto & element : row)
		{
		    element = m_dist_for_cells(m_mersenne) * number_particles / (4 * m_width * m_height);
		}

	}
}

void Calculator::calculate()
{

    field_t old_field = m_field;

    m_field.clear();

    m_field.resize(old_field.size());

    for (auto row = 0U; row < old_field.size(); row++)
    {
        m_field[row].resize(old_field[row].size(), 0);
    }

    for (auto i = 0U; i < old_field.size(); i++)
    {
        for (auto j = 0U; j < old_field[i].size(); j++)
        {
            calculateCell(i, j, old_field[i][j]);
        }
    }

}

void Calculator::calculateCell(int i, int j, unsigned int element)
{

    unsigned int rand_number;
    // Corners
    if ((i == 0 || i == m_field.size() - 1) && (j == 0 || j == m_field[i].size() - 1))
    {
        for (auto k = 0U; k < element; k++)
        {

            rand_number = m_dist_for_cells(m_mersenne);
            if (rand_number % 2 == 0)
            {
                if (i == 0) m_field[i + 1][j]++;
                else m_field[i - 1][j]++;
            }
            else
            {
                if (j == 0) m_field[i][j + 1]++;
                else m_field[i][j - 1]++;
            }

        }
    }


    // Side cells
    else if (i == 0 || j == 0 || i == m_field.size() - 1 || j == m_field[i].size() - 1)
    {


        for (auto k = 0U; k < element; k++)
        {

            rand_number = m_dist_for_side_cells(m_mersenne);
            switch (rand_number)
            {

                case 1U:

                    if (i == 0) m_field[i + 1][j]++;
                    else m_field[i - 1][j]++;
                    break;

                case 2U:

                    if (j == m_field[i].size() - 1) m_field[i][j - 1]++;
                    else m_field[i][j + 1]++;
                    break;

                case 3U:

                    if (i == 0 || i == m_field.size() - 1) m_field[i][j - 1]++;
                    else m_field[i + 1][j]++;
                    break;



            }
        }
    }



    // Other cells
    else
    {
        for (auto k = 0U; k < element; k++)
        {

            rand_number = m_dist_for_cells(m_mersenne);
            switch (rand_number) {

                case 1U:
                    m_field[i + 1][j]++;
                    break;

                case 2U:
                    m_field[i - 1][j]++;
                    break;

                case 3U:
                    m_field[i][j + 1]++;
                    break;

                case 4U:
                    m_field[i][j - 1]++;
                    break;

            }
        }

    }
}


unsigned int Calculator::maxElement()
{
    unsigned int result = 0U;
    for (const auto& row : m_field)
    {
        for (auto element : row)
            result = std::max(result, element);
    }

    return result;
}