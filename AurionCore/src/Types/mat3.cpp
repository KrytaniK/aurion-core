module Aurion.Types;

import :Math;
import :Primitives;

namespace Aurion
{
	// -----------------------------
	// ------- 3D Matrix F32 -------
	// -----------------------------

	// Access
	f32& Mat3f32::operator[](size_t i) { return data[i]; }
	const f32& Mat3f32::operator[](size_t i) const { return data[i]; }

	// Unary
	Mat3f32 Mat3f32::operator-() const
	{
		return {
			-m11, -m12, -m13,
			-m21, -m22, -m23,
			-m31, -m32, -m33
		};
	}

	// Compound assignments
	Mat3f32& Mat3f32::operator+=(const Mat3f32& rhs)
	{
		m11 += rhs.m11; m12 += rhs.m12; m13 += rhs.m13;
		m21 += rhs.m21; m22 += rhs.m22; m23 += rhs.m23;
		m31 += rhs.m31; m32 += rhs.m32; m33 += rhs.m33;
		return *this;
	}

	Mat3f32& Mat3f32::operator-=(const Mat3f32& rhs)
	{
		m11 -= rhs.m11; m12 -= rhs.m12; m13 -= rhs.m13;
		m21 -= rhs.m21; m22 -= rhs.m22; m23 -= rhs.m23;
		m31 -= rhs.m31; m32 -= rhs.m32; m33 -= rhs.m33;
		return *this;
	}

	Mat3f32& Mat3f32::operator*=(f32 scalar)
	{
		m11 *= scalar; m12 *= scalar; m13 *= scalar;
		m21 *= scalar; m22 *= scalar; m23 *= scalar;
		m31 *= scalar; m32 *= scalar; m33 *= scalar;
		return *this;
	}

	Mat3f32& Mat3f32::operator/=(f32 scalar)
	{
		m11 /= scalar; m12 /= scalar; m13 /= scalar;
		m21 /= scalar; m22 /= scalar; m23 /= scalar;
		m31 /= scalar; m32 /= scalar; m33 /= scalar;
		return *this;
	}

	// Binary operators
	Mat3f32 operator+(const Mat3f32& lhs, const Mat3f32& rhs)
	{
		return {
			lhs.m11 + rhs.m11, lhs.m12 + rhs.m12, lhs.m13 + rhs.m13,
			lhs.m21 + rhs.m21, lhs.m22 + rhs.m22, lhs.m23 + rhs.m23,
			lhs.m31 + rhs.m31, lhs.m32 + rhs.m32, lhs.m33 + rhs.m33
		};
	}

	Mat3f32 operator-(const Mat3f32& lhs, const Mat3f32& rhs)
	{
		return {
			lhs.m11 - rhs.m11, lhs.m12 - rhs.m12, lhs.m13 - rhs.m13,
			lhs.m21 - rhs.m21, lhs.m22 - rhs.m22, lhs.m23 - rhs.m23,
			lhs.m31 - rhs.m31, lhs.m32 - rhs.m32, lhs.m33 - rhs.m33
		};
	}

	Mat3f32 operator*(const Mat3f32& val, f32 scalar)
	{
		return {
			val.m11 * scalar, val.m12 * scalar, val.m13 * scalar,
			val.m21 * scalar, val.m22 * scalar, val.m23 * scalar,
			val.m31 * scalar, val.m32 * scalar, val.m33 * scalar
		};
	}

	Mat3f32 operator*(f32 scalar, const Mat3f32& vec)
	{
		return vec * scalar;
	}

	Mat3f32 operator/(const Mat3f32& val, f32 scalar)
	{
		return {
			val.m11 / scalar, val.m12 / scalar, val.m13 / scalar,
			val.m21 / scalar, val.m22 / scalar, val.m23 / scalar,
			val.m31 / scalar, val.m32 / scalar, val.m33 / scalar
		};
	}

	// Comparisons
	bool operator==(const Mat3f32& lhs, const Mat3f32& rhs)
	{
		return (lhs.m11 == rhs.m11) && (lhs.m12 == rhs.m12) && (lhs.m13 == rhs.m13) &&
			   (lhs.m21 == rhs.m21) && (lhs.m22 == rhs.m22) && (lhs.m23 == rhs.m23) &&
			   (lhs.m31 == rhs.m31) && (lhs.m32 == rhs.m32) && (lhs.m33 == rhs.m33);
	}

	bool operator!=(const Mat3f32& lhs, const Mat3f32& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator<(const Mat3f32& lhs, const Mat3f32& rhs)
	{
		return (lhs.m11 < rhs.m11) && (lhs.m12 < rhs.m12) && (lhs.m13 < rhs.m13) &&
			(lhs.m21 < rhs.m21) && (lhs.m22 < rhs.m22) && (lhs.m23 < rhs.m23) &&
			(lhs.m31 < rhs.m31) && (lhs.m32 < rhs.m32) && (lhs.m33 < rhs.m33);
	}

	bool operator>(const Mat3f32& lhs, const Mat3f32& rhs)
	{
		return (lhs.m11 > rhs.m11) && (lhs.m12 > rhs.m12) && (lhs.m13 > rhs.m13) &&
			(lhs.m21 > rhs.m21) && (lhs.m22 > rhs.m22) && (lhs.m23 > rhs.m23) &&
			(lhs.m31 > rhs.m31) && (lhs.m32 > rhs.m32) && (lhs.m33 > rhs.m33);
	}

	bool operator<=(const Mat3f32& lhs, const Mat3f32& rhs)
	{
		return !(lhs > rhs);
	}

	bool operator>=(const Mat3f32& lhs, const Mat3f32& rhs)
	{
		return !(lhs < rhs);
	}

	// -----------------------------
	// ------- 3D Matrix F64 -------
	// -----------------------------

	// Access
	f64& Mat3f64::operator[](size_t i) { return data[i]; }
	const f64& Mat3f64::operator[](size_t i) const { return data[i]; }

	// Unary
	Mat3f64 Mat3f64::operator-() const
	{
		return {
			-m11, -m12, -m13,
			-m21, -m22, -m23,
			-m31, -m32, -m33
		};
	}

	// Compound assignments
	Mat3f64& Mat3f64::operator+=(const Mat3f64& rhs)
	{
		m11 += rhs.m11; m12 += rhs.m12; m13 += rhs.m13;
		m21 += rhs.m21; m22 += rhs.m22; m23 += rhs.m23;
		m31 += rhs.m31; m32 += rhs.m32; m33 += rhs.m33;
		return *this;
	}

	Mat3f64& Mat3f64::operator-=(const Mat3f64& rhs)
	{
		m11 -= rhs.m11; m12 -= rhs.m12; m13 -= rhs.m13;
		m21 -= rhs.m21; m22 -= rhs.m22; m23 -= rhs.m23;
		m31 -= rhs.m31; m32 -= rhs.m32; m33 -= rhs.m33;
		return *this;
	}

	Mat3f64& Mat3f64::operator*=(f64 scalar)
	{
		m11 *= scalar; m12 *= scalar; m13 *= scalar;
		m21 *= scalar; m22 *= scalar; m23 *= scalar;
		m31 *= scalar; m32 *= scalar; m33 *= scalar;
		return *this;
	}

	Mat3f64& Mat3f64::operator/=(f64 scalar)
	{
		m11 /= scalar; m12 /= scalar; m13 /= scalar;
		m21 /= scalar; m22 /= scalar; m23 /= scalar;
		m31 /= scalar; m32 /= scalar; m33 /= scalar;
		return *this;
	}

	// Binary operators
	Mat3f64 operator+(const Mat3f64& lhs, const Mat3f64& rhs)
	{
		return {
			lhs.m11 + rhs.m11, lhs.m12 + rhs.m12, lhs.m13 + rhs.m13,
			lhs.m21 + rhs.m21, lhs.m22 + rhs.m22, lhs.m23 + rhs.m23,
			lhs.m31 + rhs.m31, lhs.m32 + rhs.m32, lhs.m33 + rhs.m33
		};
	}

	Mat3f64 operator-(const Mat3f64& lhs, const Mat3f64& rhs)
	{
		return {
			lhs.m11 - rhs.m11, lhs.m12 - rhs.m12, lhs.m13 - rhs.m13,
			lhs.m21 - rhs.m21, lhs.m22 - rhs.m22, lhs.m23 - rhs.m23,
			lhs.m31 - rhs.m31, lhs.m32 - rhs.m32, lhs.m33 - rhs.m33
		};
	}

	Mat3f64 operator*(const Mat3f64& val, f64 scalar)
	{
		return {
			val.m11 * scalar, val.m12 * scalar, val.m13 * scalar,
			val.m21 * scalar, val.m22 * scalar, val.m23 * scalar,
			val.m31 * scalar, val.m32 * scalar, val.m33 * scalar
		};
	}

	Mat3f64 operator*(f64 scalar, const Mat3f64& vec)
	{
		return vec * scalar;
	}

	Mat3f64 operator/(const Mat3f64& val, f64 scalar)
	{
		return {
			val.m11 / scalar, val.m12 / scalar, val.m13 / scalar,
			val.m21 / scalar, val.m22 / scalar, val.m23 / scalar,
			val.m31 / scalar, val.m32 / scalar, val.m33 / scalar
		};
	}

	// Comparisons
	bool operator==(const Mat3f64& lhs, const Mat3f64& rhs)
	{
		return (lhs.m11 == rhs.m11) && (lhs.m12 == rhs.m12) && (lhs.m13 == rhs.m13) &&
			   (lhs.m21 == rhs.m21) && (lhs.m22 == rhs.m22) && (lhs.m23 == rhs.m23) &&
			   (lhs.m31 == rhs.m31) && (lhs.m32 == rhs.m32) && (lhs.m33 == rhs.m33);
	}

	bool operator!=(const Mat3f64& lhs, const Mat3f64& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator<(const Mat3f64& lhs, const Mat3f64& rhs)
	{
		return (lhs.m11 < rhs.m11) && (lhs.m12 < rhs.m12) && (lhs.m13 < rhs.m13) &&
			(lhs.m21 < rhs.m21) && (lhs.m22 < rhs.m22) && (lhs.m23 < rhs.m23) &&
			(lhs.m31 < rhs.m31) && (lhs.m32 < rhs.m32) && (lhs.m33 < rhs.m33);
	}

	bool operator>(const Mat3f64& lhs, const Mat3f64& rhs)
	{
		return (lhs.m11 > rhs.m11) && (lhs.m12 > rhs.m12) && (lhs.m13 > rhs.m13) &&
			(lhs.m21 > rhs.m21) && (lhs.m22 > rhs.m22) && (lhs.m23 > rhs.m23) &&
			(lhs.m31 > rhs.m31) && (lhs.m32 > rhs.m32) && (lhs.m33 > rhs.m33);
	}

	bool operator<=(const Mat3f64& lhs, const Mat3f64& rhs)
	{
		return !(lhs > rhs);
	}

	bool operator>=(const Mat3f64& lhs, const Mat3f64& rhs)
	{
		return !(lhs < rhs);
	}
}