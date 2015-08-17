#include "vector2.h"
#include <math.h>
#include <boost/math/constants/constants.hpp>

#define FLOAT_EPSILON 0.00001f
#define FLOAT_NAN 999999.9f

Vector2::Vector2(const Vector2& v)
{
	if (this != &v)
	{
		this->x = v.x;
		this->y = v.y;
	}
}

Vector2& Vector2::operator=(const Vector2& v)
{
	if (this != &v)
	{
		this->x = v.x;
		this->y = v.y;
	}

	return *this;
}

const bool Vector2::operator==(const Vector2& v) const
{
	if (fabs(this->x - v.x) > FLOAT_EPSILON
		|| fabs(this->y - v.y) > FLOAT_EPSILON)
	{
		return false;
	}
	return true;
}

const bool Vector2::operator!=(const Vector2& v) const
{
	if (fabs(this->x - v.x) > FLOAT_EPSILON
		|| fabs(this->y - v.y) > FLOAT_EPSILON)
	{
		return true;
	}
	return false;
}

const bool Vector2::operator<(const Vector2& v) const
{
	if (fabs(this->x - v.x) < FLOAT_EPSILON)
	{
		if (fabs(this->y - v.y) < FLOAT_EPSILON)
		{
			return false;
		}
		else if (this->y > v.y)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else if (this->x > v.x)
	{
		return false;
	}
	else
	{
		return true;
	}
}

const bool Vector2::operator>(const Vector2& v) const
{
	if (fabs(this->x - v.x) < FLOAT_EPSILON)
	{
		if (fabs(this->y - v.y) < FLOAT_EPSILON)
		{
			return false;
		}
		else if (this->y > v.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (this->x > v.x)
	{
		return true;
	}
	else
	{
		return false;
	}
}

const Vector2 Vector2::operator+(const Vector2& v) const
{
	Vector2 r = *this;
	r += v;
	return r;
}

const Vector2 Vector2::operator-(const Vector2& v) const
{
	Vector2 r = *this;
	r -= v;
	return r;
}

const Vector2 Vector2::operator*(const float& f) const
{
	Vector2 r = *this;
	r *= f;
	return r;
}

const Vector2 Vector2::operator/(const float& f) const
{
	Vector2 r = *this;
	r /= f;
	return r;
}

const Vector2 Vector2::operator*(const double& d) const
{
	Vector2 r = *this;
	r *= d;
	return r;
}

const Vector2 Vector2::operator/(const double& d) const
{
	Vector2 r = *this;
	r /= d;
	return r;
}

const Vector2& Vector2::operator+=(const Vector2& v)
{
	this->x += v.x;
	this->y += v.y;
	return *this;
}

const Vector2& Vector2::operator-=(const Vector2& v)
{
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}

const Vector2& Vector2::operator*=(const float& f)
{
	this->x *= f;
	this->y *= f;
	return *this;
}

const Vector2& Vector2::operator/=(const float& f)
{
	this->x /= f;
	this->y /= f;
	return *this;
}

const Vector2& Vector2::operator*=(const double& d)
{
	this->x = static_cast<float>(this->x * d);
	this->y = static_cast<float>(this->y * d);
	return *this;
}

const Vector2& Vector2::operator/=(const double& d)
{
	this->x = static_cast<float>(this->x / d);
	this->y = static_cast<float>(this->y / d);
	return *this;
}

void Vector2::SetNaN()
{
	this->x = FLOAT_NAN;
	this->y = FLOAT_NAN;
}

bool Vector2::IsNaN() const
{
	return (fabs(this->x - FLOAT_NAN) < 1
		|| fabs(this->y - FLOAT_NAN) < 1);
}

float Vector2::length() const
{
	return sqrt(x*x + y*y);
}

const Vector2 Vector2::normalized() const
{
	float fLength = length();
	if (fLength < FLOAT_EPSILON)
	{
		return Vector2();
	}
	return Vector2(x/fLength, y/fLength);
}

float Vector2::angle() const
{
	float fAngle = atan2f(x, y) * boost::math::float_constants::radian;
	if (x < 0)
	{
		return 360 + fAngle;
	}
	else
	{
		return fAngle;
	}
}

float Vector2::Distance(Vector2 a, Vector2 b)
{
	return sqrt((b.x-a.x) * (b.x-a.x) + (b.y-a.y) * (b.y-a.y));
}

Vector2 Vector2::Lerp(Vector2 a, Vector2 b, float alpha)
{
	return (b - a) * alpha + a;
}

Vector2 Vector2::FromAngle(float fAngle)
{
	float fRadian = fAngle * boost::math::float_constants::degree;
	return Vector2(sin(fRadian), cos(fRadian));
}


