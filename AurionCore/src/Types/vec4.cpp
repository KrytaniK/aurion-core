module Aurion.Types;

import :Math;
import :Primitives;

namespace Aurion
{
	// -------------------------------
	// ---------- Vector4f32 ---------
	// -------------------------------

	// Access
	f32& Vec4f32::operator[](size_t i) { return data[i]; }
	const f32& Vec4f32::operator[](size_t i) const { return data[i]; }

	// Unary
	Vec4f32 Vec4f32::operator-() const { return { -x, -y, -z, -w }; }

	// Compound assignments
	Vec4f32& Vec4f32::operator+=(const Vec4f32& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	Vec4f32& Vec4f32::operator-=(const Vec4f32& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	Vec4f32& Vec4f32::operator*=(f32 scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}

	Vec4f32& Vec4f32::operator/=(f32 scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}

	// Binary operators
	Vec4f32 operator+(const Vec4f32& lhs, const Vec4f32& rhs)
	{
		return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w };
	}

	Vec4f32 operator-(const Vec4f32& lhs, const Vec4f32& rhs)
	{
		return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w };
	}

	Vec4f32 operator*(const Vec4f32& val, f32 scalar)
	{
		return { val.x * scalar, val.y * scalar, val.z * scalar, val.w * scalar };
	}

	Vec4f32 operator*(f32 scalar, const Vec4f32& vec)
	{
		return { vec.x * scalar, vec.y * scalar, vec.z * scalar, vec.w * scalar };
	}

	Vec4f32 operator/(const Vec4f32& val, f32 scalar)
	{
		return { val.x / scalar, val.y / scalar, val.z / scalar, val.w / scalar };
	}

	// Comparisons
	bool operator==(const Vec4f32& lhs, const Vec4f32& rhs)
	{
		return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z) && (lhs.w == rhs.w);
	}

	bool operator!=(const Vec4f32& lhs, const Vec4f32& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator<(const Vec4f32& lhs, const Vec4f32& rhs)
	{
		return (lhs.x < rhs.x) && (lhs.y < rhs.y) && (lhs.z < rhs.z) && (lhs.w < rhs.w);
	}

	bool operator>(const Vec4f32& lhs, const Vec4f32& rhs)
	{
		return (lhs.x > rhs.x) && (lhs.y > rhs.y) && (lhs.z > rhs.z) && (lhs.w > rhs.w);
	}

	bool operator<=(const Vec4f32& lhs, const Vec4f32& rhs)
	{
		return !(lhs > rhs);
	}

	bool operator>=(const Vec4f32& lhs, const Vec4f32& rhs)
	{
		return !(lhs < rhs);
	}

	// -------------------------------
	// ---------- Vector4f64 ---------
	// -------------------------------

	// Access
	f64& Vec4f64::operator[](size_t i) { return data[i]; }
	const f64& Vec4f64::operator[](size_t i) const { return data[i]; }

	// Unary
	Vec4f64 Vec4f64::operator-() const { return { -x, -y, -z, -w }; }

	// Compound assignments
	Vec4f64& Vec4f64::operator+=(const Vec4f64& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	Vec4f64& Vec4f64::operator-=(const Vec4f64& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	Vec4f64& Vec4f64::operator*=(f64 scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}

	Vec4f64& Vec4f64::operator/=(f64 scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}

	// Binary operators
	Vec4f64 operator+(const Vec4f64& lhs, const Vec4f64& rhs)
	{
		return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w };
	}

	Vec4f64 operator-(const Vec4f64& lhs, const Vec4f64& rhs)
	{
		return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w };
	}

	Vec4f64 operator*(const Vec4f64& val, f64 scalar)
	{
		return { val.x * scalar, val.y * scalar, val.z * scalar, val.w * scalar };
	}

	Vec4f64 operator*(f64 scalar, const Vec4f64& vec)
	{
		return { vec.x * scalar, vec.y * scalar, vec.z * scalar, vec.w * scalar };
	}

	Vec4f64 operator/(const Vec4f64& val, f64 scalar)
	{
		return { val.x / scalar, val.y / scalar, val.z / scalar, val.w / scalar };
	}

	// Comparisons
	bool operator==(const Vec4f64& lhs, const Vec4f64& rhs)
	{
		return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z) && (lhs.w == rhs.w);
	}

	bool operator!=(const Vec4f64& lhs, const Vec4f64& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator<(const Vec4f64& lhs, const Vec4f64& rhs)
	{
		return (lhs.x < rhs.x) && (lhs.y < rhs.y) && (lhs.z < rhs.z) && (lhs.w < rhs.w);
	}

	bool operator>(const Vec4f64& lhs, const Vec4f64& rhs)
	{
		return (lhs.x > rhs.x) && (lhs.y > rhs.y) && (lhs.z > rhs.z) && (lhs.w > rhs.w);
	}

	bool operator<=(const Vec4f64& lhs, const Vec4f64& rhs)
	{
		return !(lhs > rhs);
	}

	bool operator>=(const Vec4f64& lhs, const Vec4f64& rhs)
	{
		return !(lhs < rhs);
	}
}