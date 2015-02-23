#pragma once

// Implemented instead of sf::Color so that we don't have to convert from uint (255) to float (1.0f) when working with OpenGL.
namespace flopse
{
	class Colour
	{
	private:
		// values should range from 0.0f to 1.0f
		float r;
		float g;
		float b;
		float a;

	public:
		// default colour = white
		Colour();
		Colour(float r, float g, float b, float a);
		virtual ~Colour();

		float getR() const;
		float getG() const;
		float getB() const;
		float getA() const;

		void setR(float r);
		void setG(float g);
		void setB(float b);
		void setA(float a);

		Colour& operator*=(const Colour &rhs)
		{
			setR(r * rhs.getR());
			setG(g * rhs.getG());
			setB(b * rhs.getB());
			setA(a * rhs.getA());
			return *this;
		}

		Colour& operator+=(const Colour &rhs)
		{
			setR(r + rhs.getR());
			setG(g + rhs.getG());
			setB(b + rhs.getB());
			setA(a + rhs.getA());
			return *this;
		}

		Colour& operator-=(const Colour &rhs)
		{
			setR(r - rhs.getR());
			setG(g - rhs.getG());
			setB(b - rhs.getB());
			setA(a - rhs.getA());
			return *this;
		}
	};

	bool operator==(const Colour &lhs, const Colour &rhs);
	bool operator!=(const Colour &lhs, const Colour &rhs);

	Colour operator*(const Colour &lhs, const Colour &rhs);
	Colour operator+(const Colour &lhs, const Colour &rhs);
	Colour operator-(const Colour &lhs, const Colour &rhs);
}