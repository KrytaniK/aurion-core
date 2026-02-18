module Aurion.Types;

import :Math;
import :Primitives;

namespace Aurion
{
	// -------------------------------
	// ---------- Vector2f32 ---------
	// -------------------------------

	// Access
	f32& Vec2f32::operator[](size_t i) { return data[i]; }
	const f32& Vec2f32::operator[](size_t i) const { return data[i]; }

	// Unary
	Vec2f32 Vec2f32::operator-() const { return { -x, -y }; }

	// Compound assignments
	Vec2f32& Vec2f32::operator+=(const Vec2f32& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Vec2f32& Vec2f32::operator-=(const Vec2f32& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	Vec2f32& Vec2f32::operator*=(f32 scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vec2f32& Vec2f32::operator/=(f32 scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	// Binary operators
	Vec2f32 operator+(const Vec2f32& lhs, const Vec2f32& rhs)
	{
		return { lhs.x + rhs.x, lhs.y + rhs.y };
	}

	Vec2f32 operator-(const Vec2f32& lhs, const Vec2f32& rhs)
	{
		return { lhs.x - rhs.x, lhs.y - rhs.y };
	}

	Vec2f32 operator*(const Vec2f32& val, f32 scalar)
	{
		return { val.x * scalar, val.y * scalar };
	}

	Vec2f32 operator*(f32 scalar, const Vec2f32& vec)
	{
		return { vec.x * scalar, vec.y * scalar };
	}

	Vec2f32 operator/(const Vec2f32& val, f32 scalar)
	{
		return { val.x / scalar, val.y / scalar };
	}

	// Comparisons
	bool operator==(const Vec2f32& lhs, const Vec2f32& rhs)
	{
		return (lhs.x == rhs.x) && (lhs.y == rhs.y);
	}

	bool operator!=(const Vec2f32& lhs, const Vec2f32& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator<(const Vec2f32& lhs, const Vec2f32& rhs)
	{
		return (lhs.x < rhs.x) && (lhs.y < rhs.y);
	}

	bool operator>(const Vec2f32& lhs, const Vec2f32& rhs)
	{
		return (lhs.x > rhs.x) && (lhs.y > rhs.y);
	}

	bool operator<=(const Vec2f32& lhs, const Vec2f32& rhs)
	{
		return !(lhs > rhs);
	}

	bool operator>=(const Vec2f32& lhs, const Vec2f32& rhs)
	{
		return !(lhs < rhs);
	}

	// -------------------------------
	// ---------- Vector2f64 ---------
	// -------------------------------

	// Access
	f64& Vec2f64::operator[](size_t i) { return data[i]; }
	const f64& Vec2f64::operator[](size_t i) const { return data[i]; }

	// Unary
	Vec2f64 Vec2f64::operator-() const { return { -x, -y }; }

	// Compound assignments
	Vec2f64& Vec2f64::operator+=(const Vec2f64& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Vec2f64& Vec2f64::operator-=(const Vec2f64& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	Vec2f64& Vec2f64::operator*=(f64 scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vec2f64& Vec2f64::operator/=(f64 scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	// Binary operators
	Vec2f64 operator+(const Vec2f64& lhs, const Vec2f64& rhs)
	{
		return { lhs.x + rhs.x, lhs.y + rhs.y };
	}

	Vec2f64 operator-(const Vec2f64& lhs, const Vec2f64& rhs)
	{
		return { lhs.x - rhs.x, lhs.y - rhs.y };
	}

	Vec2f64 operator*(const Vec2f64& val, f64 scalar)
	{
		return { val.x * scalar, val.y * scalar };
	}

	Vec2f64 operator*(f64 scalar, const Vec2f64& vec)
	{
		return { vec.x * scalar, vec.y * scalar };
	}

	Vec2f64 operator/(const Vec2f64& val, f64 scalar)
	{
		return { val.x / scalar, val.y / scalar };
	}

	// Comparisons
	bool operator==(const Vec2f64& lhs, const Vec2f64& rhs)
	{
		return (lhs.x == rhs.x) && (lhs.y == rhs.y);
	}

	bool operator!=(const Vec2f64& lhs, const Vec2f64& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator<(const Vec2f64& lhs, const Vec2f64& rhs)
	{
		return (lhs.x < rhs.x) && (lhs.y < rhs.y);
	}

	bool operator>(const Vec2f64& lhs, const Vec2f64& rhs)
	{
		return (lhs.x > rhs.x) && (lhs.y > rhs.y);
	}

	bool operator<=(const Vec2f64& lhs, const Vec2f64& rhs)
	{
		return !(lhs > rhs);
	}

	bool operator>=(const Vec2f64& lhs, const Vec2f64& rhs)
	{
		return !(lhs < rhs);
	}
}