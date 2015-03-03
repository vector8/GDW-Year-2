#include "Utilities.h"
#include <string>
#include <vector>
#include <time.h>
#include <GL\glew.h>

#define BUFFER_OFFSET(i) ((char *)0 + (i))

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

	GLuint fullScreenQuadVAO = GL_NONE;
	GLuint fullScreenQuadVBO = GL_NONE;

	void initFullScreenQuad()
	{
		float VBO_DATA[] =
		{
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f
		};

		glGenVertexArrays(1, &fullScreenQuadVAO);
		glBindVertexArray(fullScreenQuadVAO);

		glEnableVertexAttribArray(0); // Vertices
		glEnableVertexAttribArray(1); // UV coords

		glGenBuffers(1, &fullScreenQuadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, fullScreenQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(float), VBO_DATA, GL_STATIC_DRAW);

		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)0);

		glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(3 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
		glBindVertexArray(GL_NONE);
	}

	void drawFullScreenQuad()
	{
		glBindVertexArray(fullScreenQuadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(GL_NONE);
	}
}