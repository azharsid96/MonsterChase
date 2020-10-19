#pragma once

namespace Engine
{
	namespace Math
	{

		class Point2D
		{
		public:
			Point2D(){}
			// standard constructor
			inline Point2D(float i_x, float i_y) :
				m_x(i_x),
				m_y(i_y)
			{
			}

			// standard constructor
			inline Point2D(int i_x, int i_y) :
				m_x(static_cast<float>(i_x)),
				m_y(static_cast<float>(i_y))
			{
			}

			// copy constructor - for completeness
			Point2D(const Point2D& other) :
				m_x(other.m_x),
				m_y(other.m_y)
			{
			}

			// assignment operator
			inline Point2D& operator=(const Point2D& other)
			{
				m_x = other.m_x;
				m_y = other.m_y;

				return *this;
			}


			// element accessors
			// - get individual element
			float x(void) const { return m_x; }
			float y(void) const { return m_y; }

			// - set individual element
			inline void x(float i_x)
			{
				m_x = i_x;
			}

			inline void y(float i_y)
			{
				m_y = i_y;
			}

			// modifying operators
			inline Point2D & operator+=(const Point2D& other)
			{
				m_x += other.m_x;
				m_y += other.m_y;

				return *this;
			}

			inline Point2D & operator-=(const Point2D & other)
			{
				m_x -= other.m_x;
				m_y -= other.m_y;

				return *this;
			}

			inline Point2D & operator*=(const Point2D & other)
			{
				m_x *= other.m_x;
				m_y *= other.m_y;

				return *this;
			}

			inline Point2D & operator*=(float other)
			{
				m_x *= other;
				m_y *= other;

				return *this;
			}

			inline Point2D & operator*=(int other)
			{
				m_x *= static_cast<float>(other * m_x);
				m_y *= static_cast<float>(other * m_y);

				return *this;
			}

			inline Point2D & operator/=(const Point2D & other)
			{
				m_x /= other.m_x;
				m_y /= other.m_y;

				return *this;
			}

			inline Point2D & operator/=(float other)
			{
				m_x /= other;
				m_y /= other;

				return *this;
			}

			inline Point2D & operator/=(int other)
			{
				m_x = float(static_cast<float>(m_x) / other);
				m_y = float(static_cast<float>(m_y) / other);

				return *this;
			}

			inline Point2D operator-(void)
			{
				return Point2D(-m_x,-m_y);
			}

			// - ( 0.0f, 0.0f, 0.0f )
			static const Point2D Zero;
		
		private:
			float m_x, m_y;
		};
	}
}

#include "Point2D-inl.h"