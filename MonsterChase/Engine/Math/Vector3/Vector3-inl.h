#pragma once
namespace Engine
{
	namespace Math
	{
		inline Vector3 operator+(const Vector3 & lhs, const Vector3 & rhs)
		{
			return Vector3(lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z());
		}

		inline Vector3 operator-(const Vector3 & lhs, const Vector3 & rhs)
		{
			return Vector3(lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z());
		}

		inline Vector3 operator*(const Vector3 & lhs, const Vector3 & rhs)
		{
			return Vector3(lhs.x() * rhs.x(), lhs.y() * rhs.x(), lhs.z() * rhs.z());
		}

		inline Vector3 operator*(const Vector3 & lhs, float rhs)
		{
			return Vector3(lhs.x() * rhs, lhs.y() * rhs, lhs.z() * rhs);
		}

		inline Vector3 operator*(const Vector3 & lhs, int rhs)
		{
			return Vector3(float(rhs * lhs.x()), float(rhs * lhs.y()), float(rhs * lhs.z()));
		}

		inline Vector3 operator*(float lhs, const Vector3 & rhs)
		{
			return Vector3(lhs * rhs.x(), lhs * rhs.y(), lhs * rhs.z());
		}

		inline Vector3 operator/(const Vector3 & lhs, const Vector3 & rhs)
		{
			return Vector3(lhs.x() / rhs.x(), lhs.y() / rhs.y(), lhs.z() / rhs.z());
		}

		inline Vector3 operator/(const Vector3 & lhs, float rhs)
		{
			return Vector3(lhs.x() / rhs, lhs.y() / rhs, lhs.z() / rhs);
		}

		inline Vector3 operator/(const Vector3 & lhs, int rhs)
		{
			return Vector3(float(static_cast<float>(lhs.x()) / rhs), float(static_cast<float>(lhs.y()) / rhs), float(static_cast<float>(lhs.z()) / rhs));
		}

		inline bool operator==(const Vector3 & lhs, const Vector3 & rhs)
		{
			return ((lhs.x() == rhs.x()) && (lhs.y() == rhs.y())) && (lhs.z() == rhs.z());
		}

		inline bool operator!=(const Vector3 & lhs, const Vector3 & rhs)
		{
			return !operator==(lhs, rhs);
		}

		inline float dot(const Vector3 & lhs, const Vector3 & rhs)
		{
			return lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z();
		}
	}
}