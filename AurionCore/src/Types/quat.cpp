module Aurion.Types;

import :Math;
import :Primitives;

namespace Aurion
{
	// ------------------------------
	// ------- Quaternion F32 -------
	// ------------------------------

	// Access
	f32& Quatf32::operator[](size_t i) { return data[i]; }
	const f32& Quatf32::operator[](size_t i) const { return data[i]; }

	// Unary
	Quatf32 Quatf32::operator-() const
	{
		return { -x, -y, -z, -w };
	}

	// Compound assignments
	Quatf32& Quatf32::operator+=(const Quatf32& rhs)
	{
		x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w;
		return *this;
	}

	Quatf32& Quatf32::operator-=(const Quatf32& rhs)
	{
		x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w;
		return *this;
	}

	Quatf32& Quatf32::operator*=(f32 scalar)
	{
		x *= scalar; y *= scalar; z *= scalar; w *= scalar;
		return *this;
	}

	Quatf32& Quatf32::operator/=(f32 scalar)
	{
		x /= scalar; y /= scalar; z /= scalar; w /= scalar;
		return *this;
	}

	// Binary operators
	Quatf32 operator+(const Quatf32& lhs, const Quatf32& rhs)
	{
		return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w };
	}

	Quatf32 operator-(const Quatf32& lhs, const Quatf32& rhs)
	{
		return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w };
	}

	Quatf32 operator*(const Quatf32& val, f32 scalar)
	{
		return { val.x * scalar, val.y * scalar, val.z * scalar, val.w * scalar };
	}

	Quatf32 operator*(f32 scalar, const Quatf32& vec)
	{
		return vec * scalar;
	}

	Quatf32 operator/(const Quatf32& val, f32 scalar)
	{
		return { val.x / scalar, val.y / scalar, val.z / scalar, val.w / scalar };
	}

	// Comparisons
	bool operator==(const Quatf32& lhs, const Quatf32& rhs)
	{
		return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z) && (lhs.w == rhs.w);
	}

	bool operator!=(const Quatf32& lhs, const Quatf32& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator<(const Quatf32& lhs, const Quatf32& rhs)
	{
		return (lhs.x < rhs.x) && (lhs.y < rhs.y) && (lhs.z < rhs.z) && (lhs.w < rhs.w);
	}

	bool operator>(const Quatf32& lhs, const Quatf32& rhs)
	{
		return (lhs.x > rhs.x) && (lhs.y > rhs.y) && (lhs.z > rhs.z) && (lhs.w > rhs.w);
	}

	bool operator<=(const Quatf32& lhs, const Quatf32& rhs)
	{
		return !(lhs > rhs);
	}

	bool operator>=(const Quatf32& lhs, const Quatf32& rhs)
	{
		return !(lhs < rhs);
	}

	// ------------------------------
	// ------- Quaternion F64 -------
	// ------------------------------

	// Access
	f64& dQuat::operator[](size_t i) { return data[i]; }
	const f64& dQuat::operator[](size_t i) const { return data[i]; }
	
	// Unary
	dQuat dQuat::operator-() const
	{
		return { -x, -y, -z, -w };
	}

	// Compound assignments
	dQuat& dQuat::operator+=(const dQuat& rhs)
	{
		x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w;
		return *this;
	}

	dQuat& dQuat::operator-=(const dQuat& rhs)
	{
		x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w;
		return *this;
	}

	dQuat& dQuat::operator*=(f64 scalar)
	{
		x *= scalar; y *= scalar; z *= scalar; w *= scalar;
		return *this;
	}

	dQuat& dQuat::operator/=(f64 scalar)
	{
		x /= scalar; y /= scalar; z /= scalar; w /= scalar;
		return *this;
	}

	// Binary operators
	dQuat operator+(const dQuat& lhs, const dQuat& rhs)
	{
		return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w };
	}

	dQuat operator-(const dQuat& lhs, const dQuat& rhs)
	{
		return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w };
	}

	dQuat operator*(const dQuat& val, f64 scalar)
	{
		return { val.x * scalar, val.y * scalar, val.z * scalar, val.w * scalar };
	}

	dQuat operator*(f64 scalar, const dQuat& vec)
	{
		return vec * scalar;
	}

	dQuat operator/(const dQuat& val, f64 scalar)
	{
		return { val.x / scalar, val.y / scalar, val.z / scalar, val.w / scalar };
	}

	// Comparisons
	bool operator==(const dQuat& lhs, const dQuat& rhs)
	{
		return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z) && (lhs.w == rhs.w);
	}

	bool operator!=(const dQuat& lhs, const dQuat& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator<(const dQuat& lhs, const dQuat& rhs)
	{
		return (lhs.x < rhs.x) && (lhs.y < rhs.y) && (lhs.z < rhs.z) && (lhs.w < rhs.w);
	}

	bool operator>(const dQuat& lhs, const dQuat& rhs)
	{
		return (lhs.x > rhs.x) && (lhs.y > rhs.y) && (lhs.z > rhs.z) && (lhs.w > rhs.w);
	}

	bool operator<=(const dQuat& lhs, const dQuat& rhs)
	{
		return !(lhs > rhs);
	}

	bool operator>=(const dQuat& lhs, const dQuat& rhs)
	{
		return !(lhs < rhs);
	}
}