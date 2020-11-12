#pragma once
struct Vector2
{
	float x;
	float y;

	Vector2();
	Vector2(float x, float y);
	float Length();
	Vector2 Normalize();

	static float Distance(const Vector2& v1, const Vector2& v2);
	static Vector2 Normalize(const Vector2& v);
	static float Cross(const Vector2& v1, const Vector2& v2);
	static float Dot(const Vector2& v1, const Vector2& v2);
	static Vector2 Lerp(const Vector2& s, const Vector2& e, float t);

	Vector2 operator+();
	Vector2 operator-();
	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(float s);
	Vector2& operator/=(float s);
};
const Vector2 operator+(const Vector2& v1, const Vector2& v2);
const Vector2 operator-(const Vector2& v1, const Vector2& v2);
const Vector2 operator*(const Vector2& v, float s);
const Vector2 operator*(float s, const Vector2& v);
const Vector2 operator/(const Vector2& v, float s);
const Vector2 operator/(float s, const Vector2& v);

struct Vector3
{
	float x;
	float y;
	float z;

	Vector3();
	Vector3(float x, float y, float z);
	float Length();
	Vector3 Normalize();

	static float Distance(const Vector3& v1, const Vector3& v2);
	static Vector3 Normalize(const Vector3& v);
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);
	static float Dot(const Vector3& v1, const Vector3& v2);
	static Vector3 Lerp(const Vector3& s, const Vector3& e, float t);

	Vector3 operator+();
	Vector3 operator-();
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);
};
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);
const Vector3 operator/(float s, const Vector3& v);