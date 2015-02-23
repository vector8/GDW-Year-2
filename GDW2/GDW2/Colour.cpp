#include "Colour.h"

namespace flopse
{
	// default colour = white
	Colour::Colour() : r(1.f), g(1.f), b(1.f), a(1.f)
	{

	}

	Colour::Colour(float r, float g, float b, float a)
	{
		setR(r);
		setG(g);
		setB(b);
		setA(a);
	}

	Colour::~Colour()
	{

	}

	float Colour::getR() const
	{
		return r;
	}

	float Colour::getG() const
	{
		return g;
	}

	float Colour::getB() const
	{
		return b;
	}

	float Colour::getA() const
	{
		return a;
	}

	void Colour::setR(float r)
	{
		this->r = r;

		if (this->r < 0.f)
			this->r = 0.f;
		else if (this->r > 1.f)
			this->r = 1.f;
	}

	void Colour::setG(float g)
	{
		this->g = g;

		if (this->g < 0.f)
			this->g = 0.f;
		else if (this->g > 1.f)
			this->g = 1.f;
	}

	void Colour::setB(float b)
	{
		this->b = b;

		if (this->b < 0.f)
			this->b = 0.f;
		else if (this->b > 1.f)
			this->b = 1.f;
	}

	void Colour::setA(float a)
	{
		this->a = a;

		if (this->a < 0.f)
			this->a = 0.f;
		else if (this->a > 1.f)
			this->a = 1.f;
	}

	bool operator==(const Colour &lhs, const Colour &rhs)
	{
		return lhs.getR() == rhs.getR() &&
			lhs.getG() == rhs.getG() &&
			lhs.getB() == rhs.getB() &&
			lhs.getA() == rhs.getA();
	}

	bool operator!=(const Colour &lhs, const Colour &rhs)
	{
		return !(lhs == rhs);
	}

	Colour operator*(const Colour &lhs, const Colour &rhs)
	{
		return Colour(lhs.getR() * rhs.getR(), lhs.getG() * rhs.getG(), lhs.getB() * rhs.getB(), lhs.getA() * rhs.getA());
	}

	Colour operator+(const Colour &lhs, const Colour &rhs)
	{
		return Colour(lhs.getR() + rhs.getR(), lhs.getG() + rhs.getG(), lhs.getB() + rhs.getB(), lhs.getA() + rhs.getA());
	}

	Colour operator-(const Colour &lhs, const Colour &rhs)
	{
		return Colour(lhs.getR() - rhs.getR(), lhs.getG() - rhs.getG(), lhs.getB() - rhs.getB(), lhs.getA() - rhs.getA());
	}
}