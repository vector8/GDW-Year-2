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

template <class T>
T bezier(float u, const T &p0, const T &c0, const T &c1, const T &p1)
{
	return (u * u * u) * (-p0 + 3.f * c0 - 3.f * c1 + p1) +
		(u * u) * (3.f * p0 - 6.f * c0 + 3.f * c1) +
		(u)* (-3.f * p0 + 3.f * c0) +
		p0;
}