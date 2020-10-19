#pragma once

#include "math.h"

namespace Engine
{
	namespace Math
	{
		inline Vector4 operator+(const Vector4 & lhs, const Vector4 & rhs)
		{
			return Vector4(lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z(), lhs.w() + rhs.w());
		}

		inline Vector4 operator-(const Vector4 & lhs, const Vector4 & rhs)
		{
			return Vector4(lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z(), lhs.w() - rhs.w());
		}

		inline Vector4 operator*(const Vector4 & lhs, const Vector4 & rhs)
		{
			return Vector4(lhs.x() * rhs.x(), lhs.y() * rhs.x(), lhs.z() * rhs.z(), lhs.w() * rhs.w());
		}

		inline Vector4 operator*(const Vector4 & lhs, float rhs)
		{
			return Vector4(lhs.x() * rhs, lhs.y() * rhs, lhs.z() * rhs, lhs.w() * rhs);
		}

		inline Vector4 operator*(const Vector4 & lhs, int rhs)
		{
			return Vector4(float(rhs * lhs.x()), float(rhs * lhs.y()), float(rhs * lhs.z()), float(rhs * lhs.w()));
		}

		inline Vector4 operator*(float lhs, const Vector4 & rhs)
		{
			return Vector4(lhs * rhs.x(), lhs * rhs.y(), lhs * rhs.z(), lhs * rhs.w());
		}

		inline Vector4 operator/(const Vector4 & lhs, const Vector4 & rhs)
		{
			return Vector4(lhs.x() / rhs.x(), lhs.y() / rhs.y(), lhs.z() / rhs.z(), lhs.w() / rhs.w());
		}

		inline Vector4 operator/(const Vector4 & lhs, float rhs)
		{
			return Vector4(lhs.x() / rhs, lhs.y() / rhs, lhs.z() / rhs, lhs.w() / rhs);
		}

		inline Vector4 operator/(const Vector4 & lhs, int rhs)
		{
			return Vector4(float(static_cast<float>(lhs.x()) / rhs), float(static_cast<float>(lhs.y()) / rhs), float(static_cast<float>(lhs.z()) / rhs),
				float(static_cast<float>(lhs.w()) / rhs));
		}

		inline bool operator==(const Vector4 & lhs, const Vector4 & rhs)
		{
			return ((lhs.x() == rhs.x()) && (lhs.y() == rhs.y())) && ((lhs.z() == rhs.z()) && (lhs.w() == rhs.w()));
		}

		inline bool operator!=(const Vector4 & lhs, const Vector4 & rhs)
		{
			return !operator==(lhs, rhs);
		}

		inline float dot(const Vector4 & lhs, const Vector4 & rhs)
		{
			return lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z() + lhs.w() * rhs.w();
		}

		inline float length(const Vector4 & i_vector)
		{
			return sqrtf(powf(i_vector.x(), 2.0) + powf(i_vector.y(), 2.0) + powf(i_vector.z(), 2.0) + powf(i_vector.w(), 2.0));
		}

		inline Vector4 Normalize(const Vector4 & i_vector)
		{
			float denominator = length(i_vector);
			return operator*(1.0f / denominator, i_vector);
		}
	}
}