#include "Vector.h"

Vector2::Vector2() :x(0), y(0)
{
}

Vector2::Vector2(float x, float y) : x(x), y(y)
{
}

Vector2 Vector2::operator+()
{
	return *this;
}

Vector2 Vector2::operator-()
{
	return Vector2(-x, -y);
}

Vector2 & Vector2::operator+=(const Vector2 & v)
{
	x += v.x;
	y += v.y;
	return *this;
}

Vector2 & Vector2::operator-=(const Vector2 & v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

Vector2 & Vector2::operator*=(float s)
{
	x *= s;
	y *= s;
	return *this;
}

Vector2 & Vector2::operator/=(float s)
{
	x /= s;
	y /= s;
	return *this;
}

const Vector2 operator+(const Vector2 & v1, const Vector2 & v2)
{
	Vector2 a(v1);
	return a += v2;
}

const Vector2 operator-(const Vector2 & v1, const Vector2 & v2)
{
	Vector2 a(v1);
	return a -= v2;
}

const Vector2 operator*(const Vector2 & v, float s)
{
	Vector2 a(v);
	return a *= s;
}

const Vector2 operator*(float s, const Vector2 & v)
{
	return v * s;
}

const Vector2 operator/(const Vector2 & v, float s)
{
	Vector2 a(v);
	return a /= s;
}

const Vector2 operator/(float s, const Vector2 & v)
{
	return v / s;
}

Vector3::Vector3() :x(0), y(0), z(0)
{
}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
{
}

Vector3 Vector3::operator+()
{
	return *this;
}

Vector3 Vector3::operator-()
{
	return Vector3(-x, -y, -z);
}

Vector3 & Vector3::operator+=(const Vector3 & v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3 & Vector3::operator-=(const Vector3 & v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3 & Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3 & Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

const Vector3 operator+(const Vector3 & v1, const Vector3 & v2)
{
	Vector3 a(v1);
	return a += v2;
}

const Vector3 operator-(const Vector3 & v1, const Vector3 & v2)
{
	Vector3 a(v1);
	return a -= v2;
}

const Vector3 operator*(const Vector3& v, float s)
{
	Vector3 a(v);
	return a *= s;
}

const Vector3 operator*(float s,const Vector3& v)
{
	return v * s;
}

const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 a(v);
	return a /= s;
}

const Vector3 operator/(float s, const Vector3& v)
{

	return v / s;
}