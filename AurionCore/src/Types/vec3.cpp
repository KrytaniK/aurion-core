module;

#include <cstddef>

module Aurion.Types;

import :Math;
import :Primitives;

namespace Aurion
{
	// -------------------------------
	// ---------- Vector3f32 ---------
	// -------------------------------

	// Access
	f32& Vec3f32::operator[](size_t i) { return data[i]; }
	const f32& Vec3f32::operator[](size_t i) const { return data[i]; }

	// Unary
	Vec3f32 Vec3f32::operator-() const { return { -x, -y, -z }; }

	// Compound assignments
	Vec3f32& Vec3f32::operator+=(const Vec3f32& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	Vec3f32& Vec3f32::operator-=(const Vec3f32& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	Vec3f32& Vec3f32::operator*=(f32 scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	Vec3f32& Vec3f32::operator/=(f32 scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	// Binary operators
	Vec3f32 operator+(const Vec3f32& lhs, const Vec3f32& rhs)
	{
		return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
	}

	Vec3f32 operator-(const Vec3f32& lhs, const Vec3f32& rhs)
	{
		return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
	}

	Vec3f32 operator*(const Vec3f32& val, f32 scalar)
	{
		return { val.x * scalar, val.y * scalar, val.z * scalar };
	}

	Vec3f32 operator*(f32 scalar, const Vec3f32& vec)
	{
		return { vec.x * scalar, vec.y * scalar, vec.z * scalar };
	}

	Vec3f32 operator/(const Vec3f32& val, f32 scalar)
	{
		return { val.x / scalar, val.y / scalar, val.z / scalar };
	}

	// Comparisons
	bool operator==(const Vec3f32& lhs, const Vec3f32& rhs)
	{
		return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
	}

	bool operator!=(const Vec3f32& lhs, const Vec3f32& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator<(const Vec3f32& lhs, const Vec3f32& rhs)
	{
		return (lhs.x < rhs.x) && (lhs.y < rhs.y) && (lhs.z < rhs.z);
	}

	bool operator>(const Vec3f32& lhs, const Vec3f32& rhs)
	{
		return (lhs.x > rhs.x) && (lhs.y > rhs.y) && (lhs.z > rhs.z);
	}

	bool operator<=(const Vec3f32& lhs, const Vec3f32& rhs)
	{
		return !(lhs > rhs);
	}

	bool operator>=(const Vec3f32& lhs, const Vec3f32& rhs)
	{
		return !(lhs < rhs);
	}

	// -------------------------------
	// ---------- Vector3f64 ---------
	// -------------------------------

	// Access
	f64& Vec3f64::operator[](size_t i) { return data[i]; }
	const f64& Vec3f64::operator[](size_t i) const { return data[i]; }

	// Unary
	Vec3f64 Vec3f64::operator-() const { return { -x, -y, -z }; }

	// Compound assignments
	Vec3f64& Vec3f64::operator+=(const Vec3f64& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	Vec3f64& Vec3f64::operator-=(const Vec3f64& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	Vec3f64& Vec3f64::operator*=(f64 scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	Vec3f64& Vec3f64::operator/=(f64 scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	// Binary operators
	Vec3f64 operator+(const Vec3f64& lhs, const Vec3f64& rhs)
	{
		return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
	}

	Vec3f64 operator-(const Vec3f64& lhs, const Vec3f64& rhs)
	{
		return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
	}

	Vec3f64 operator*(const Vec3f64& val, f64 scalar)
	{
		return { val.x * scalar, val.y * scalar, val.z * scalar };
	}

	Vec3f64 operator*(f64 scalar, const Vec3f64& vec)
	{
		return { vec.x * scalar, vec.y * scalar, vec.z * scalar };
	}

	Vec3f64 operator/(const Vec3f64& val, f64 scalar)
	{
		return { val.x / scalar, val.y / scalar, val.z / scalar };
	}

	// Comparisons
	bool operator==(const Vec3f64& lhs, const Vec3f64& rhs)
	{
		return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
	}

	bool operator!=(const Vec3f64& lhs, const Vec3f64& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator<(const Vec3f64& lhs, const Vec3f64& rhs)
	{
		return (lhs.x < rhs.x) && (lhs.y < rhs.y) && (lhs.z < rhs.z);
	}

	bool operator>(const Vec3f64& lhs, const Vec3f64& rhs)
	{
		return (lhs.x > rhs.x) && (lhs.y > rhs.y) && (lhs.z > rhs.z);
	}

	bool operator<=(const Vec3f64& lhs, const Vec3f64& rhs)
	{
		return !(lhs > rhs);
	}

	bool operator>=(const Vec3f64& lhs, const Vec3f64& rhs)
	{
		return !(lhs < rhs);
	}
}