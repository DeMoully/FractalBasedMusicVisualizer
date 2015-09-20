// Defines a class that implements a fractal generation algorithm

#pragma once

#include <SFML/Graphics.hpp>
#include <ppl.h>
#include <complex>

#include "Complex.hpp"

class Fractal : public sf::Drawable
{
private:
	mutable sf::VertexArray m_points;
	unsigned int m_width;
	unsigned int m_height;
	mutable bool m_changed;
	Complex m_offset;
	sf::Color m_minHue;
	sf::Color m_maxHue;
	long double m_scale;
public:
	// Constructors
	Fractal(unsigned int width, unsigned int height) : m_points(sf::PrimitiveType::Points, width * height), m_width(width), m_height(height), m_changed(true), m_minHue(sf::Color::Black), m_maxHue(sf::Color::White), m_offset(0.f, 0.f), m_scale(1.0)
	{
	}
	// Accessors
	Complex getOffset() const
	{
		return m_offset;
	}
	// Mutators
	void setMinHue(const sf::Color & hue)
	{
		m_minHue = hue;
		m_changed = true;
	}
	void setMaxHue(const sf::Color & hue)
	{
		m_maxHue = hue;
		m_changed = true;
	}
	void setOffset(const Complex & offset)
	{
		m_offset = offset;
		m_changed = true;
	}
	void setScale(long double scale)
	{
		m_scale = scale;
		m_changed = true;
	}
	// Computational Functions
	void recompute() const
	{
		static long double log10_2 = log10(2.f);
		if (!m_changed)
			return;
		Concurrency::parallel_for(0, static_cast<int>(m_width), [&] (int x)
		{
			Concurrency::parallel_for(0, static_cast<int>(m_height), [&](int y)
			{
				m_points[y * m_width + x].position.x = static_cast<float>(x);
				m_points[y * m_width + x].position.y = static_cast<float>(y);
				long double widthMin = -1.7f, widthMax = 1.7f;
				long double heightMin = -1.5f, heightMax = 1.5f;

				long double scaledX = (((static_cast<long double>(x) / static_cast<long double>(m_width)) * (widthMax - widthMin)) - widthMax) * m_scale;
				long double scaledY = (((static_cast<long double>(y) / static_cast<long double>(m_height)) * (heightMax - heightMin)) - heightMax) * m_scale;

				Complex z(scaledX, scaledY);

				unsigned int iterations = 0, maxIterations = 100;
				while (iterations < maxIterations && z.magnitudeSquared() <= 4) // Iterate until the magnitude of z is > the maximum (2)
				{
					z = (z * z) + m_offset;
					++iterations;
				}
				long double magnitude = (iterations - (log10(log10(z.magnitude())) / log10_2)) / static_cast<long double>(maxIterations);
				m_points[y * m_width + x].color = m_minHue;
				m_points[y * m_width + x].color.r += magnitude * (m_maxHue - m_minHue).r;
				m_points[y * m_width + x].color.g += magnitude * (m_maxHue - m_minHue).g;
				m_points[y * m_width + x].color.b += magnitude * (m_maxHue - m_minHue).b;
			});
		});
		m_changed = false;
	}
	// Rendering Functions
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
		recompute();
		target.draw(m_points, states);
	}
};

