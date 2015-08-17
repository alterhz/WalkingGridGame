//-------------------------------------------
// author : alterhz
// date : 8/17/2015
// description : 

#ifndef _VECTOR2_H_
#define _VECTOR2_H_

class Vector2
{
public:
	float x;
	float y;

	Vector2()
		: x(0), y(0)
	{
	}

	Vector2(float _x, float _y)
		: x(_x), y(_y)
	{
	}

	Vector2(const Vector2& v);

	Vector2& operator =(const Vector2& v);

	const bool operator == (const Vector2& v) const;
	const bool operator != (const Vector2& v) const;
	const bool operator < (const Vector2& v) const;
	const bool operator > (const Vector2& v) const;

	const Vector2 operator + (const Vector2& v) const;
	const Vector2 operator - (const Vector2& v) const;
	const Vector2 operator * (const float& f) const;
	const Vector2 operator / (const float& f) const;
	const Vector2 operator * (const double& d) const;
	const Vector2 operator / (const double& d) const;
	const Vector2& operator += (const Vector2& v);
	const Vector2& operator -= (const Vector2& v);
	const Vector2& operator *= (const float& f);
	const Vector2& operator /= (const float& f);
	const Vector2& operator *= (const double& d);
	const Vector2& operator /= (const double& d);

	void SetNaN();
	bool IsNaN() const;

	float length() const;
	const Vector2 normalized() const;
	float angle() const;

	static float Distance(Vector2 a, Vector2 b);
	static Vector2 Lerp(Vector2 a, Vector2 b, float alpha);
	static Vector2 FromAngle(float fAngle);
};

#endif