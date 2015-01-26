#include "Utilities.h"
#include <string>
#include <vector>
#include <time.h>

namespace flopse
{
	sf::Color lerp(float u, const sf::Color &p0, const sf::Color &p1)
	{
		return sf::Color(p0.r * (1.f - u) + p1.r * u,
			p0.g * (1.f - u) + p1.g * u,
			p0.b * (1.f - u) + p1.b * u,
			p0.a * (1.f - u) + p1.a * u);
	}

	std::string ltrim(std::string s)
	{
		size_t startIndex;
		std::string result;

		if (s.size() == 0)
		{
			result = s;
		}
		else
		{
			startIndex = s.find_first_not_of(" \n\r\t");

			if (startIndex == std::string::npos)
			{
				result = "";
			}
			else
			{
				result = s.substr(startIndex);
			}
		}

		return result;
	}

	std::string rtrim(std::string s)
	{
		size_t endIndex;
		std::string result;

		if (s.size() == 0)
		{
			result = s;
		}
		else
		{
			endIndex = s.find_last_not_of(" \n\r\t");

			if (endIndex == std::string::npos)
			{
				result = "";
			}
			else
			{
				result = s.substr(0, endIndex + 1);
			}
		}

		return result;
	}

	std::string trim(std::string s)
	{
		return ltrim(rtrim(s));
	}

	std::vector<std::string> split(std::string s, std::string delimiters, bool shouldTrim)
	{
		std::vector<std::string> result;
		std::string temp;

		size_t start = 0;
		size_t end = s.find_first_of(delimiters);

		while (end != std::string::npos)
		{
			temp = s.substr(start, end - start);

			if (shouldTrim)
			{
				temp = trim(temp);
			}

			if (temp.size() > 0)
			{
				result.push_back(temp);
			}

			start = end + 1;
			end = s.find_first_of(delimiters, start);
		}

		// get the last token...
		temp = s.substr(start);

		if (shouldTrim)
		{
			temp = trim(temp);
		}

		if (temp.size() > 0)
		{
			result.push_back(temp);
		}

		return result;
	}

	void seedRandomNumberGenerator()
	{
		srand(static_cast<unsigned int>(time(NULL)));
	}

	void seedRandomNumberGenerator(unsigned int seed)
	{
		srand(seed);
	}

	float randomRangef(float min, float max)
	{
		return min + ((max - min) * rand()) / (RAND_MAX + 1.0f);
	}

	int randomRangei(int min, int max)
	{
		return rand() % (max + 1 - min) + min;
	}
}