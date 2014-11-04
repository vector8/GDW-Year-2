#pragma once
#include <string>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

template <class T>
T lerp(float u, const T &p0, const T &p1);

template <class T>
T catmull(float u, const T &p0, const T &p1, const T &p2, const T &p3);

template <class T>
T bezier(float u, const T &p0, const T &c0, const T &c1, const T &p1);

std::string ltrim(std::string s);

std::string rtrim(std::string s);

std::string trim(std::string s);

std::vector<std::string> split(std::string s, std::string delimiters, bool shouldTrim = true);