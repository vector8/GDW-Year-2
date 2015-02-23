#pragma once
#include <string>
#include <vector>
#include <SFML\Graphics.hpp>

namespace flopse
{
	#ifndef M_PI
	#define M_PI 3.14159265359
	#endif

	template <class T>
	T lerp(float u, const T &p0, const T &p1);

	sf::Color lerp(float u, const sf::Color &p0, const sf::Color &p1);

	template <class T>
	T catmull(float u, const T &p0, const T &p1, const T &p2, const T &p3);

	template <class T>
	T bezier(float u, const T &p0, const T &c0, const T &c1, const T &p1);

	std::string ltrim(std::string s);

	std::string rtrim(std::string s);

	std::string trim(std::string s);

	std::vector<std::string> split(std::string s, std::string delimiters, bool shouldTrim = true);

	void seedRandomNumberGenerator();
	void seedRandomNumberGenerator(unsigned int seed);

	float randomRangef(float min, float max);
	int randomRangei(int min, int max);

	void initFullScreenQuad();
	void drawFullScreenQuad();

	#include "Utilities.tpp"
}