// Defines a complex class

#pragma once

template <typename T>
T min(const T & a, const T & b)
{
	return a < b ? a : b;
}

template <typename T>
T max(const T & a, const T & b)
{
	return a > b ? a : b;
}

int sign(long double a)
{
	if (a == 0)
		return 0;
	return a < 0 ? -1 : 1;
}


class Complex
{
private:
	long double m_real;
	long double m_imaginary;
public:
	// Constructors
	Complex() : m_real(0), m_imaginary(0)
	{
	}
	Complex(long double real, long double imaginary) : m_real(real), m_imaginary(imaginary)
	{
	}
	// Accessors
	long double getReal() const
	{
		return m_real;
	}
	long double getImaginary() const
	{
		return m_imaginary;
	}
	// Mutators
	void setReal(long double real)
	{
		m_real = real;
	}
	void setImaginary(long double imaginary)
	{
		m_imaginary = imaginary;
	}
	// Utility
	long double magnitudeSquared() const
	{
		return m_real * m_real + m_imaginary * m_imaginary;
	}
	long double magnitude() const
	{
		return sqrt(magnitudeSquared());
	}
	std::string toString() const
	{
		return "<" + std::to_string(m_real) + (m_imaginary < 0 ? " - " : " + ") + std::to_string(std::abs(m_imaginary)) + "i>";
	}
	// Overloaded Operators
	Complex operator + (const Complex & rhs) const
	{
		return Complex(m_real + rhs.getReal(), m_imaginary + rhs.getImaginary());
	}
	Complex operator - (const Complex & rhs) const
	{
		return Complex(m_real - rhs.getReal(), m_imaginary - rhs.getImaginary());
	}
	Complex operator * (const Complex & rhs) const
	{
		return Complex(m_real * rhs.getReal() - m_imaginary * rhs.getImaginary(), m_imaginary * rhs.getReal() + m_real * rhs.getImaginary());
	}
	Complex operator * (long double rhs) const
	{
		return Complex(m_real * rhs, m_imaginary * rhs);
	}
	Complex operator / (long double rhs) const
	{
		// Returns a complex number of 0 + 0i if rhs == 0, otherwise returns (this / rhs)
		return rhs ? Complex(m_real / rhs, m_imaginary / rhs) : Complex();
	}
	Complex & operator = (const Complex & rhs)
	{
		m_real = rhs.getReal();
		m_imaginary = rhs.getImaginary();
		return *this;
	}
};

Complex operator * (long double lhs, const Complex & rhs)
{
	return rhs * lhs;
}