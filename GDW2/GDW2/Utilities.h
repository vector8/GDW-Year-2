#pragma once
#include <string>
#include <vector>

template <class T>
T lerp(float u, const T &p0, const T &p1)
{
	return p0 * (1.f - u) + p1 * u;
}

template <class T>
T catmull(float u, const T &p0, const T &p1, const T &p2, const T &p3)
{
	return 0.5f * ((u * u * u) * (-p0 + 3.f * p1 - 3.f * p2 + p3) +
		(u * u) * (2.f * p0 - 5.f * p1 + 4.f * p2 - p3) +
		(u)* (-p0 + p2) +
		(2.f * p1));
}

std::string ltrim(std::string s)
{
	size_t startIndex;
	std::string result;

	if (s.size() == 0)
		result = s;
	else
	{
		startIndex = s.find_first_not_of(" \n\r\t");

		if (startIndex == std::string::npos)
			result = "";
		else
			result = s.substr(startIndex);
	}

	return result;
}

std::string rtrim(std::string s)
{
	size_t endIndex;
	std::string result;

	if (s.size() == 0)
		result = s;
	else
	{
		endIndex = s.find_last_not_of(" \n\r\t");

		if (endIndex == std::string::npos)
			result = "";
		else
			result = s.substr(0, endIndex + 1);
	}

	return result;
}

std::string trim(std::string s)
{
	return ltrim(rtrim(s));
}

std::vector<std::string> split(std::string s, std::string delimiters, bool shouldTrim = true)
{
	std::vector<std::string> result;
	std::string temp;

	size_t start = 0;
	size_t end = s.find_first_of(delimiters);

	while (end != std::string::npos)
	{
		temp = s.substr(start, end - start);

		if (shouldTrim)
			temp = trim(temp);

		if (temp.size() > 0)
			result.push_back(temp);

		start = end + 1;
		end = s.find_first_of(delimiters, start);
	}

	// get the last token...
	temp = s.substr(start);

	if (shouldTrim)
		temp = trim(temp);

	if (temp.size() > 0)
		result.push_back(temp);

	return result;
}