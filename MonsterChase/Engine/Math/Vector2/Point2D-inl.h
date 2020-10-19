#pragma once
namespace Engine
{
	namespace Math
	{
		// stand alone operators
		inline Point2D operator+(const Point2D & lhs, const Point2D & rhs)
		{
			return Point2D(lhs.x() + rhs.x(), lhs.y() + rhs.y());
		}

		inline Point2D operator-(const Point2D & lhs, const Point2D & rhs)
		{
			return Point2D(lhs.x() - rhs.x(), lhs.y() - rhs.y());
		}

		inline Point2D operator*(const Point2D & lhs, const Point2D & rhs)
		{
			return Point2D(lhs.x() * rhs.x(), lhs.y() * rhs.x());
		}

		inline Point2D operator*(const Point2D & lhs, float rhs)
		{
			return Point2D(lhs.x() * rhs, lhs.y() * rhs);
		}

		inline Point2D operator*(const Point2D & lhs, int rhs)
		{
			return Point2D(float(rhs * lhs.x()), float(rhs * lhs.y()));
		}

		inline Point2D operator*(float lhs, const Point2D & rhs)
		{
			return Point2D(lhs * rhs.x(), lhs * rhs.y());
		}

		inline Point2D operator/(const Point2D & lhs, const Point2D & rhs)
		{
			return Point2D(lhs.x() / rhs.x(), lhs.y() / rhs.y());
		}

		inline Point2D operator/(const Point2D & lhs, float rhs)
		{
			return Point2D(lhs.x() / rhs, lhs.y() / rhs);
		}

		inline Point2D operator/(const Point2D & lhs, int rhs)
		{
			return Point2D(float(static_cast<float>(lhs.x()) / rhs), float(static_cast<float>(lhs.y()) / rhs));
		}

		inline bool operator==(const Point2D & lhs, const Point2D & rhs)
		{
			return (lhs.x() == rhs.x()) && (lhs.y() == rhs.y());
		}

		inline bool operator!=(const Point2D & lhs, const Point2D & rhs)
		{
			return !operator==(lhs, rhs);
		}

		inline float dot(const Point2D & lhs, const Point2D & rhs)
		{
			return lhs.x() * rhs.x() + lhs.y() * rhs.y();
		}
	}
}