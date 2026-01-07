module Aurion.Types;

import :Math;
import :Primitives;

namespace Aurion
{
	// -----------------------------
	// ------- 4D Matrix F32 -------
	// -----------------------------

	// Access
	f32& Mat4f32::operator[](size_t i) { return data[i]; }
	const f32& Mat4f32::operator[](size_t i) const { return data[i]; }
	
	// Unary
	Mat4f32 Mat4f32::operator-() const
	{
		return {
			-m11, -m12, -m13, -m14,
			-m21, -m22, -m23, -m24,
			-m31, -m32, -m33, -m34,
			-m41, -m42, -m43, -m44
		};
	}

	// Compound assignments
	Mat4f32& Mat4f32::operator+=(const Mat4f32& rhs)
	{
		m11 += rhs.m11; m12 += rhs.m12; m13 += rhs.m13; m14 += rhs.m14;
		m21 += rhs.m21; m22 += rhs.m22; m23 += rhs.m23; m24 += rhs.m24;
		m31 += rhs.m31; m32 += rhs.m32; m33 += rhs.m33; m34 += rhs.m34;
		m41 += rhs.m41; m42 += rhs.m42; m43 += rhs.m43; m44 += rhs.m44;
		return *this;
	}

	Mat4f32& Mat4f32::operator-=(const Mat4f32& rhs)
	{
		m11 -= rhs.m11; m12 -= rhs.m12; m13 -= rhs.m13; m14 -= rhs.m14;
		m21 -= rhs.m21; m22 -= rhs.m22; m23 -= rhs.m23; m24 -= rhs.m24;
		m31 -= rhs.m31; m32 -= rhs.m32; m33 -= rhs.m33; m34 -= rhs.m34;
		m41 -= rhs.m41; m42 -= rhs.m42; m43 -= rhs.m43; m44 -= rhs.m44;
		return *this;
	}

	Mat4f32& Mat4f32::operator*=(f32 scalar)
	{
		m11 *= scalar; m12 *= scalar; m13 *= scalar; m14 *= scalar;
		m21 *= scalar; m22 *= scalar; m23 *= scalar; m24 *= scalar;
		m31 *= scalar; m32 *= scalar; m33 *= scalar; m34 *= scalar;
		m41 *= scalar; m42 *= scalar; m43 *= scalar; m44 *= scalar;
		return *this;
	}

	Mat4f32& Mat4f32::operator/=(f32 scalar)
	{
		m11 /= scalar; m12 /= scalar; m13 /= scalar; m14 /= scalar;
		m21 /= scalar; m22 /= scalar; m23 /= scalar; m24 /= scalar;
		m31 /= scalar; m32 /= scalar; m33 /= scalar; m34 /= scalar;
		m41 /= scalar; m42 /= scalar; m43 /= scalar; m44 /= scalar;
		return *this;
	}

	// Binary operators
	Mat4f32 operator+(const Mat4f32& lhs, const Mat4f32& rhs)
	{
		return {
			lhs.m11 + rhs.m11, lhs.m12 + rhs.m12, lhs.m13 + rhs.m13, lhs.m14 + rhs.m14,
			lhs.m21 + rhs.m21, lhs.m22 + rhs.m22, lhs.m23 + rhs.m23, lhs.m24 + rhs.m24,
			lhs.m31 + rhs.m31, lhs.m32 + rhs.m32, lhs.m33 + rhs.m33, lhs.m34 + rhs.m34,
			lhs.m41 + rhs.m41, lhs.m42 + rhs.m42, lhs.m43 + rhs.m43, lhs.m44 + rhs.m44
		};
	}

	Mat4f32 operator-(const Mat4f32& lhs, const Mat4f32& rhs)
	{
		return {
			lhs.m11 - rhs.m11, lhs.m12 - rhs.m12, lhs.m13 - rhs.m13, lhs.m14 - rhs.m14,
			lhs.m21 - rhs.m21, lhs.m22 - rhs.m22, lhs.m23 - rhs.m23, lhs.m24 - rhs.m24,
			lhs.m31 - rhs.m31, lhs.m32 - rhs.m32, lhs.m33 - rhs.m33, lhs.m34 - rhs.m34,
			lhs.m41 - rhs.m41, lhs.m42 - rhs.m42, lhs.m43 - rhs.m43, lhs.m44 - rhs.m44
		};
	}

	Mat4f32 operator*(const Mat4f32& val, f32 scalar)
	{
		return {
			val.m11 * scalar, val.m12 * scalar, val.m13 * scalar, val.m14 * scalar,
			val.m21 * scalar, val.m22 * scalar, val.m23 * scalar, val.m24 * scalar,
			val.m31 * scalar, val.m32 * scalar, val.m33 * scalar, val.m34 * scalar,
			val.m41 * scalar, val.m42 * scalar, val.m43 * scalar, val.m44 * scalar
		};
	}

	Mat4f32 operator*(f32 scalar, const Mat4f32& vec)
	{
		return vec * scalar;
	}

	Mat4f32 operator/(const Mat4f32& val, f32 scalar)
	{
		return {
			val.m11 / scalar, val.m12 / scalar, val.m13 / scalar, val.m14 / scalar,
			val.m21 / scalar, val.m22 / scalar, val.m23 / scalar, val.m24 / scalar,
			val.m31 / scalar, val.m32 / scalar, val.m33 / scalar, val.m34 / scalar,
			val.m41 / scalar, val.m42 / scalar, val.m43 / scalar, val.m44 / scalar
		};
	}

	// Comparisons
	bool operator==(const Mat4f32& lhs, const Mat4f32& rhs)
	{
		return (lhs.m11 == rhs.m11) && (lhs.m12 == rhs.m12) && (lhs.m13 == rhs.m13) && (lhs.m14 == rhs.m14) &&
			(lhs.m21 == rhs.m21) && (lhs.m22 == rhs.m22) && (lhs.m23 == rhs.m23) && (lhs.m24 == rhs.m24) &&
			(lhs.m31 == rhs.m31) && (lhs.m32 == rhs.m32) && (lhs.m33 == rhs.m33) && (lhs.m34 == rhs.m34) &&
			(lhs.m41 == rhs.m41) && (lhs.m42 == rhs.m42) && (lhs.m43 == rhs.m43) && (lhs.m44 == rhs.m44);
	}

	bool operator!=(const Mat4f32& lhs, const Mat4f32& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator<(const Mat4f32& lhs, const Mat4f32& rhs)
	{
		return (lhs.m11 < rhs.m11) && (lhs.m12 < rhs.m12) && (lhs.m13 < rhs.m13) && (lhs.m14 < rhs.m14) &&
			(lhs.m21 < rhs.m21) && (lhs.m22 < rhs.m22) && (lhs.m23 < rhs.m23) && (lhs.m24 < rhs.m24) &&
			(lhs.m31 < rhs.m31) && (lhs.m32 < rhs.m32) && (lhs.m33 < rhs.m33) && (lhs.m34 < rhs.m34) &&
			(lhs.m41 < rhs.m41) && (lhs.m42 < rhs.m42) && (lhs.m43 < rhs.m43) && (lhs.m44 < rhs.m44);
	}

	bool operator>(const Mat4f32& lhs, const Mat4f32& rhs)
	{
		return (lhs.m11 < rhs.m11) && (lhs.m12 < rhs.m12) && (lhs.m13 < rhs.m13) && (lhs.m14 < rhs.m14) &&
			(lhs.m21 < rhs.m21) && (lhs.m22 < rhs.m22) && (lhs.m23 < rhs.m23) && (lhs.m24 < rhs.m24) &&
			(lhs.m31 < rhs.m31) && (lhs.m32 < rhs.m32) && (lhs.m33 < rhs.m33) && (lhs.m34 < rhs.m34) &&
			(lhs.m41 < rhs.m41) && (lhs.m42 < rhs.m42) && (lhs.m43 < rhs.m43) && (lhs.m44 < rhs.m44);
	}

	bool operator<=(const Mat4f32& lhs, const Mat4f32& rhs)
	{
		return !(lhs > rhs);
	}

	bool operator>=(const Mat4f32& lhs, const Mat4f32& rhs)
	{
		return !(lhs < rhs);
	}

	// -----------------------------
	// ------- 4D Matrix F64 -------
	// -----------------------------

	// Access
	f64& Mat4f64::operator[](size_t i) { return data[i]; }
	const f64& Mat4f64::operator[](size_t i) const { return data[i]; }

	// Unary
	Mat4f64 Mat4f64::operator-() const
	{
		return {
			-m11, -m12, -m13, -m14,
			-m21, -m22, -m23, -m24,
			-m31, -m32, -m33, -m34,
			-m41, -m42, -m43, -m44
		};
	}

	// Compound assignments
	Mat4f64& Mat4f64::operator+=(const Mat4f64& rhs)
	{
		m11 += rhs.m11; m12 += rhs.m12; m13 += rhs.m13; m14 += rhs.m14;
		m21 += rhs.m21; m22 += rhs.m22; m23 += rhs.m23; m24 += rhs.m24;
		m31 += rhs.m31; m32 += rhs.m32; m33 += rhs.m33; m34 += rhs.m34;
		m41 += rhs.m41; m42 += rhs.m42; m43 += rhs.m43; m44 += rhs.m44;
		return *this;
	}

	Mat4f64& Mat4f64::operator-=(const Mat4f64& rhs)
	{
		m11 -= rhs.m11; m12 -= rhs.m12; m13 -= rhs.m13; m14 -= rhs.m14;
		m21 -= rhs.m21; m22 -= rhs.m22; m23 -= rhs.m23; m24 -= rhs.m24;
		m31 -= rhs.m31; m32 -= rhs.m32; m33 -= rhs.m33; m34 -= rhs.m34;
		m41 -= rhs.m41; m42 -= rhs.m42; m43 -= rhs.m43; m44 -= rhs.m44;
		return *this;
	}

	Mat4f64& Mat4f64::operator*=(f64 scalar)
	{
		m11 *= scalar; m12 *= scalar; m13 *= scalar; m14 *= scalar;
		m21 *= scalar; m22 *= scalar; m23 *= scalar; m24 *= scalar;
		m31 *= scalar; m32 *= scalar; m33 *= scalar; m34 *= scalar;
		m41 *= scalar; m42 *= scalar; m43 *= scalar; m44 *= scalar;
		return *this;
	}

	Mat4f64& Mat4f64::operator/=(f64 scalar)
	{
		m11 /= scalar; m12 /= scalar; m13 /= scalar; m14 /= scalar;
		m21 /= scalar; m22 /= scalar; m23 /= scalar; m24 /= scalar;
		m31 /= scalar; m32 /= scalar; m33 /= scalar; m34 /= scalar;
		m41 /= scalar; m42 /= scalar; m43 /= scalar; m44 /= scalar;
		return *this;
	}

	// Binary operators
	Mat4f64 operator+(const Mat4f64& lhs, const Mat4f64& rhs)
	{
		return {
			lhs.m11 + rhs.m11, lhs.m12 + rhs.m12, lhs.m13 + rhs.m13, lhs.m14 + rhs.m14,
			lhs.m21 + rhs.m21, lhs.m22 + rhs.m22, lhs.m23 + rhs.m23, lhs.m24 + rhs.m24,
			lhs.m31 + rhs.m31, lhs.m32 + rhs.m32, lhs.m33 + rhs.m33, lhs.m34 + rhs.m34,
			lhs.m41 + rhs.m41, lhs.m42 + rhs.m42, lhs.m43 + rhs.m43, lhs.m44 + rhs.m44
		};
	}

	Mat4f64 operator-(const Mat4f64& lhs, const Mat4f64& rhs)
	{
		return {
			lhs.m11 - rhs.m11, lhs.m12 - rhs.m12, lhs.m13 - rhs.m13, lhs.m14 - rhs.m14,
			lhs.m21 - rhs.m21, lhs.m22 - rhs.m22, lhs.m23 - rhs.m23, lhs.m24 - rhs.m24,
			lhs.m31 - rhs.m31, lhs.m32 - rhs.m32, lhs.m33 - rhs.m33, lhs.m34 - rhs.m34,
			lhs.m41 - rhs.m41, lhs.m42 - rhs.m42, lhs.m43 - rhs.m43, lhs.m44 - rhs.m44
		};
	}

	Mat4f64 operator*(const Mat4f64& val, f64 scalar)
	{
		return {
			val.m11 * scalar, val.m12 * scalar, val.m13 * scalar, val.m14 * scalar,
			val.m21 * scalar, val.m22 * scalar, val.m23 * scalar, val.m24 * scalar,
			val.m31 * scalar, val.m32 * scalar, val.m33 * scalar, val.m34 * scalar,
			val.m41 * scalar, val.m42 * scalar, val.m43 * scalar, val.m44 * scalar
		};
	}

	Mat4f64 operator*(f64 scalar, const Mat4f64& vec)
	{
		return vec * scalar;
	}

	Mat4f64 operator/(const Mat4f64& val, f64 scalar)
	{
		return {
			val.m11 / scalar, val.m12 / scalar, val.m13 / scalar, val.m14 / scalar,
			val.m21 / scalar, val.m22 / scalar, val.m23 / scalar, val.m24 / scalar,
			val.m31 / scalar, val.m32 / scalar, val.m33 / scalar, val.m34 / scalar,
			val.m41 / scalar, val.m42 / scalar, val.m43 / scalar, val.m44 / scalar
		};
	}

	// Comparisons
	bool operator==(const Mat4f64& lhs, const Mat4f64& rhs)
	{
		return (lhs.m11 == rhs.m11) && (lhs.m12 == rhs.m12) && (lhs.m13 == rhs.m13) && (lhs.m14 == rhs.m14) &&
			(lhs.m21 == rhs.m21) && (lhs.m22 == rhs.m22) && (lhs.m23 == rhs.m23) && (lhs.m24 == rhs.m24) &&
			(lhs.m31 == rhs.m31) && (lhs.m32 == rhs.m32) && (lhs.m33 == rhs.m33) && (lhs.m34 == rhs.m34) &&
			(lhs.m41 == rhs.m41) && (lhs.m42 == rhs.m42) && (lhs.m43 == rhs.m43) && (lhs.m44 == rhs.m44);
	}

	bool operator!=(const Mat4f64& lhs, const Mat4f64& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator<(const Mat4f64& lhs, const Mat4f64& rhs)
	{
		return (lhs.m11 < rhs.m11) && (lhs.m12 < rhs.m12) && (lhs.m13 < rhs.m13) && (lhs.m14 < rhs.m14) &&
			(lhs.m21 < rhs.m21) && (lhs.m22 < rhs.m22) && (lhs.m23 < rhs.m23) && (lhs.m24 < rhs.m24) &&
			(lhs.m31 < rhs.m31) && (lhs.m32 < rhs.m32) && (lhs.m33 < rhs.m33) && (lhs.m34 < rhs.m34) &&
			(lhs.m41 < rhs.m41) && (lhs.m42 < rhs.m42) && (lhs.m43 < rhs.m43) && (lhs.m44 < rhs.m44);
	}

	bool operator>(const Mat4f64& lhs, const Mat4f64& rhs)
	{
		return (lhs.m11 < rhs.m11) && (lhs.m12 < rhs.m12) && (lhs.m13 < rhs.m13) && (lhs.m14 < rhs.m14) &&
			(lhs.m21 < rhs.m21) && (lhs.m22 < rhs.m22) && (lhs.m23 < rhs.m23) && (lhs.m24 < rhs.m24) &&
			(lhs.m31 < rhs.m31) && (lhs.m32 < rhs.m32) && (lhs.m33 < rhs.m33) && (lhs.m34 < rhs.m34) &&
			(lhs.m41 < rhs.m41) && (lhs.m42 < rhs.m42) && (lhs.m43 < rhs.m43) && (lhs.m44 < rhs.m44);
	}

	bool operator<=(const Mat4f64& lhs, const Mat4f64& rhs)
	{
		return !(lhs > rhs);
	}

	bool operator>=(const Mat4f64& lhs, const Mat4f64& rhs)
	{
		return !(lhs < rhs);
	}
}