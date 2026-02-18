module Aurion.Types;

import :Math;
import :Primitives;

namespace Aurion
{
	// -----------------------------
	// ------- 2D Matrix F32 -------
	// -----------------------------

	// Access
	f32& Mat2f32::operator[](size_t i) { return data[i]; }
	const f32& Mat2f32::operator[](size_t i) const { return data[i]; }

	// Unary
	Mat2f32 Mat2f32::operator-() const
	{
		return {
			-m11, -m12,
			-m21, -m22
		};
	}

	// Compound assignments
	Mat2f32& Mat2f32::operator+=(const Mat2f32& rhs)
	{
		m11 += rhs.m11; m12 += rhs.m12;
		m21 += rhs.m21; m22 += rhs.m22;
		return *this;
	}

	Mat2f32& Mat2f32::operator-=(const Mat2f32& rhs)
	{
		m11 -= rhs.m11; m12 -= rhs.m12;
		m21 -= rhs.m21; m22 -= rhs.m22;
		return *this;
	}

	Mat2f32& Mat2f32::operator*=(f32 scalar)
	{
		m11 *= scalar; m12 *= scalar;
		m21 *= scalar; m22 *= scalar;
		return *this;
	}

	Mat2f32& Mat2f32::operator/=(f32 scalar)
	{
		m11 /= scalar; m12 /= scalar;
		m21 /= scalar; m22 /= scalar;
		return *this;
	}

	// Binary operators
	Mat2f32 operator+(const Mat2f32& lhs, const Mat2f32& rhs)
	{
		return {
			lhs.m11 + rhs.m11, lhs.m12 + rhs.m12,
			lhs.m21 + rhs.m21, lhs.m22 + rhs.m22
		};
	}

	Mat2f32 operator-(const Mat2f32& lhs, const Mat2f32& rhs)
	{
		return {
			lhs.m11 - rhs.m11, lhs.m12 - rhs.m12,
			lhs.m21 - rhs.m21, lhs.m22 - rhs.m22
		};
	}

	Mat2f32 operator*(const Mat2f32& val, f32 scalar)
	{
		return {
			val.m11 * scalar, val.m12 * scalar,
			val.m21 * scalar, val.m22 * scalar
		};
	}

	Mat2f32 operator*(f32 scalar, const Mat2f32& vec)
	{
		return {
			vec.m11 * scalar, vec.m12 * scalar,
			vec.m21 * scalar, vec.m22 * scalar
		};
	}

	Mat2f32 operator/(const Mat2f32& val, f32 scalar)
	{
		return {
			val.m11 / scalar, val.m12 / scalar,
			val.m21 / scalar, val.m22 / scalar
		};
	}

	// Comparisons
	bool operator==(const Mat2f32& lhs, const Mat2f32& rhs)
	{
		return (lhs.m11 == rhs.m11) && (lhs.m12 == rhs.m12) &&
			   (lhs.m21 == rhs.m21) && (lhs.m22 == rhs.m22);
	}

	bool operator!=(const Mat2f32& lhs, const Mat2f32& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator<(const Mat2f32& lhs, const Mat2f32& rhs)
	{
		return (lhs.m11 < rhs.m11) && (lhs.m12 < rhs.m12) &&
			(lhs.m21 < rhs.m21) && (lhs.m22 < rhs.m22);
	}

	bool operator>(const Mat2f32& lhs, const Mat2f32& rhs)
	{
		return (lhs.m11 > rhs.m11) && (lhs.m12 > rhs.m12) &&
			(lhs.m21 > rhs.m21) && (lhs.m22 > rhs.m22);
	}

	bool operator<=(const Mat2f32& lhs, const Mat2f32& rhs)
	{
		return !(lhs > rhs);
	}

	bool operator>=(const Mat2f32& lhs, const Mat2f32& rhs)
	{
		return !(lhs < rhs);
	}

	// -----------------------------
	// ------- 2D Matrix F64 -------
	// -----------------------------

	// Access
	f64& dMat2::operator[](size_t i) { return data[i]; }
	const f64& dMat2::operator[](size_t i) const { return data[i]; }

	// Unary
	dMat2 dMat2::operator-() const
	{
		return {
			-m11, -m12,
			-m21, -m22
		};
	}

	// Compound assignments
	dMat2& dMat2::operator+=(const dMat2& rhs)
	{
		m11 += rhs.m11; m12 += rhs.m12;
		m21 += rhs.m21; m22 += rhs.m22;
		return *this;
	}

	dMat2& dMat2::operator-=(const dMat2& rhs)
	{
		m11 -= rhs.m11; m12 -= rhs.m12;
		m21 -= rhs.m21; m22 -= rhs.m22;
		return *this;
	}

	dMat2& dMat2::operator*=(f64 scalar)
	{
		m11 *= scalar; m12 *= scalar;
		m21 *= scalar; m22 *= scalar;
		return *this;
	}

	dMat2& dMat2::operator/=(f64 scalar)
	{
		m11 /= scalar; m12 /= scalar;
		m21 /= scalar; m22 /= scalar;
		return *this;
	}

	// Binary operators
	dMat2 operator+(const dMat2& lhs, const dMat2& rhs)
	{
		return {
			lhs.m11 + rhs.m11, lhs.m12 + rhs.m12,
			lhs.m21 + rhs.m21, lhs.m22 + rhs.m22
		};
	}

	dMat2 operator-(const dMat2& lhs, const dMat2& rhs)
	{
		return {
			lhs.m11 - rhs.m11, lhs.m12 - rhs.m12,
			lhs.m21 - rhs.m21, lhs.m22 - rhs.m22
		};
	}

	dMat2 operator*(const dMat2& val, f64 scalar)
	{
		return {
			val.m11 * scalar, val.m12 * scalar,
			val.m21 * scalar, val.m22 * scalar
		};
	}

	dMat2 operator*(f64 scalar, const dMat2& vec)
	{
		return {
			vec.m11 * scalar, vec.m12 * scalar,
			vec.m21 * scalar, vec.m22 * scalar
		};
	}

	dMat2 operator/(const dMat2& val, f64 scalar)
	{
		return {
			val.m11 / scalar, val.m12 / scalar,
			val.m21 / scalar, val.m22 / scalar
		};
	}

	// Comparisons
	bool operator==(const dMat2& lhs, const dMat2& rhs)
	{
		return (lhs.m11 == rhs.m11) && (lhs.m12 == rhs.m12) &&
			   (lhs.m21 == rhs.m21) && (lhs.m22 == rhs.m22);
	}

	bool operator!=(const dMat2& lhs, const dMat2& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator<(const dMat2& lhs, const dMat2& rhs)
	{
		return (lhs.m11 < rhs.m11) && (lhs.m12 < rhs.m12) &&
			(lhs.m21 < rhs.m21) && (lhs.m22 < rhs.m22);
	}

	bool operator>(const dMat2& lhs, const dMat2& rhs)
	{
		return (lhs.m11 > rhs.m11) && (lhs.m12 > rhs.m12) &&
			(lhs.m21 > rhs.m21) && (lhs.m22 > rhs.m22);
	}

	bool operator<=(const dMat2& lhs, const dMat2& rhs)
	{
		return !(lhs > rhs);
	}

	bool operator>=(const dMat2& lhs, const dMat2& rhs)
	{
		return !(lhs < rhs);
	}
}