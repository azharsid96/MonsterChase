#pragma once

#include "Math/Vector3/Vector3.h"


namespace Engine
{
	namespace Math
	{
		class Vector4
		{
		public:
			Vector4() {}
			// standard constructor
			inline Vector4(float i_x, float i_y, float i_z, float i_w) :
				m_x(i_x),
				m_y(i_y),
				m_z(i_z),
				m_w(i_w)
			{
			}

			// standard constructor (using Vector3)
			inline Vector4(Vector3 i_vector3, float i_w) :
				m_x(i_vector3.x()),
				m_y(i_vector3.y()),
				m_z(i_vector3.z()),
				m_w(i_w)
			{
			}

			// standard constructor for ints
			inline Vector4(int i_x, int i_y, int i_z, int i_w) :
				m_x(static_cast<float>(i_x)),
				m_y(static_cast<float>(i_y)),
				m_z(static_cast<float>(i_z)),
				m_w(static_cast<float>(i_w))
			{
			}

			// standard constructor for ints (using Vector3)
			inline Vector4(Vector3 i_vector3, int i_w) :
				m_x(static_cast<float>(i_vector3.x())),
				m_y(static_cast<float>(i_vector3.y())),
				m_z(static_cast<float>(i_vector3.z())),
				m_w(static_cast<float>(i_w))
			{
			}

			// copy constructor - for completeness
			Vector4(const Vector4& other) :
				m_x(other.m_x),
				m_y(other.m_y),
				m_z(other.m_z),
				m_w(other.m_w)
			{
			}

			// assignment operator
			inline Vector4& operator=(const Vector4& other)
			{
				m_x = other.m_x;
				m_y = other.m_y;
				m_z = other.m_z;
				m_w = other.m_w;

				return *this;
			}

			// element accessors
			// - get individual element
			float x(void) const { return m_x; }
			float y(void) const { return m_y; }
			float z(void) const { return m_z; }
			float w(void) const { return m_w; }

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

			inline void w(float i_w)
			{
				m_w = i_w;
			}

			// modifying operators
			inline Vector4 & operator+=(const Vector4& other)
			{
				m_x += other.m_x;
				m_y += other.m_y;
				m_z += other.m_z;
				m_w += other.m_w;

				return *this;
			}

			inline Vector4 & operator-=(const Vector4 & other)
			{
				m_x -= other.m_x;
				m_y -= other.m_y;
				m_z -= other.m_z;
				m_w -= other.m_w;

				return *this;
			}

			inline Vector4 & operator*=(const Vector4 & other)
			{
				m_x *= other.m_x;
				m_y *= other.m_y;
				m_z *= other.m_z;
				m_w *= other.m_w;

				return *this;
			}

			inline Vector4 & operator*=(float other)
			{
				m_x *= other;
				m_y *= other;
				m_z *= other;
				m_w *= other;

				return *this;
			}

			inline Vector4 & operator*=(int other)
			{
				m_x *= static_cast<float>(other * m_x);
				m_y *= static_cast<float>(other * m_y);
				m_z *= static_cast<float>(other * m_z);
				m_w *= static_cast<float>(other * m_w);

				return *this;
			}

			inline Vector4 & operator/=(const Vector4 & other)
			{
				m_x /= other.m_x;
				m_y /= other.m_y;
				m_z /= other.m_z;
				m_w /= other.m_w;

				return *this;
			}

			inline Vector4 & operator/=(float other)
			{
				m_x /= other;
				m_y /= other;
				m_z /= other;
				m_w /= other;

				return *this;
			}

			inline Vector4 & operator/=(int other)
			{
				m_x = float(static_cast<float>(m_x) / other);
				m_y = float(static_cast<float>(m_y) / other);
				m_z = float(static_cast<float>(m_z) / other);
				m_w = float(static_cast<float>(m_w) / other);

				return *this;
			}

			inline Vector4 operator-(void)
			{
				return Vector4(-m_x, -m_y, -m_z, -m_w);
			}

			// - ( 0.0f, 0.0f, 0.0f, 0.0f )
			static const Vector4 Zero;

		private:
			float m_x, m_y, m_z, m_w;
		};
	}
}

#include "Vector4-inl.h"