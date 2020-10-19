#pragma once
namespace Engine
{
	namespace Math
	{
		class Vector3
		{
		public:
			Vector3() {}
			// standard constructor
			inline Vector3(float i_x, float i_y, float i_z) :
				m_x(i_x),
				m_y(i_y),
				m_z(i_z)
			{
			}

			// standard constructor
			inline Vector3(int i_x, int i_y, int i_z) :
				m_x(static_cast<float>(i_x)),
				m_y(static_cast<float>(i_y)),
				m_z(static_cast<float>(i_z))
			{
			}

			// copy constructor - for completeness
			Vector3(const Vector3& other) :
				m_x(other.m_x),
				m_y(other.m_y),
				m_z(other.m_z)
			{
			}

			// assignment operator
			inline Vector3& operator=(const Vector3& other)
			{
				m_x = other.m_x;
				m_y = other.m_y;
				m_z = other.m_z;

				return *this;
			}

			// element accessors
			// - get individual element
			float x(void) const { return m_x; }
			float y(void) const { return m_y; }
			float z(void) const { return m_z; }

			// - set individual element
			inline void x(float i_x)
			{
				m_x = i_x;
			}

			inline void y(float i_y)
			{
				m_y = i_y;
			}

			inline void z(float i_z)
			{
				m_z = i_z;
			}

			// modifying operators
			inline Vector3 & operator+=(const Vector3& other)
			{
				m_x += other.m_x;
				m_y += other.m_y;
				m_z += other.m_z;

				return *this;
			}

			inline Vector3 & operator-=(const Vector3 & other)
			{
				m_x -= other.m_x;
				m_y -= other.m_y;
				m_z -= other.m_z;

				return *this;
			}

			inline Vector3 & operator*=(const Vector3 & other)
			{
				m_x *= other.m_x;
				m_y *= other.m_y;
				m_z *= other.m_z;

				return *this;
			}

			inline Vector3 & operator*=(float other)
			{
				m_x *= other;
				m_y *= other;
				m_z *= other;

				return *this;
			}

			inline Vector3 & operator*=(int other)
			{
				m_x *= static_cast<float>(other * m_x);
				m_y *= static_cast<float>(other * m_y);
				m_z *= static_cast<float>(other * m_z);

				return *this;
			}

			inline Vector3 & operator/=(const Vector3 & other)
			{
				m_x /= other.m_x;
				m_y /= other.m_y;
				m_z /= other.m_z;

				return *this;
			}

			inline Vector3 & operator/=(float other)
			{
				m_x /= other;
				m_y /= other;
				m_z /= other;

				return *this;
			}

			inline Vector3 & operator/=(int other)
			{
				m_x = float(static_cast<float>(m_x) / other);
				m_y = float(static_cast<float>(m_y) / other);
				m_z = float(static_cast<float>(m_z) / other);

				return *this;
			}

			inline Vector3 operator-(void)
			{
				return Vector3(-m_x, -m_y, -m_z);
			}

			// - ( 0.0f, 0.0f, 0.0f )
			static const Vector3 Zero;
			
		private:
			float m_x, m_y, m_z;
		};
	}
}

#include "Vector3-inl.h"