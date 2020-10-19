#include "Matrix4.h"

namespace Engine
{
	namespace Math
	{
		Matrix4::Matrix4()
		{
		}

		Matrix4::Matrix4(float i_11, float i_12, float i_13, float i_14, 
						float i_21, float i_22, float i_23, float i_24, 
						float i_31, float i_32, float i_33, float i_34, 
						float i_41, float i_42, float i_43, float i_44) :

		m_11(i_11), m_12(i_12), m_13(i_13), m_14(i_14),
		m_21(i_21), m_22(i_22), m_23(i_23), m_24(i_24),
		m_31(i_31), m_32(i_32), m_33(i_33), m_34(i_34),
		m_41(i_41), m_42(i_42), m_43(i_43), m_44(i_44)
		{
		}

		Matrix4::Matrix4(const Matrix4& i_other) :
			m_11(i_other.m_11), m_12(i_other.m_12), m_13(i_other.m_13), m_14(i_other.m_14),
			m_21(i_other.m_21), m_22(i_other.m_22), m_23(i_other.m_23), m_24(i_other.m_24),
			m_31(i_other.m_31), m_32(i_other.m_32), m_33(i_other.m_33), m_34(i_other.m_34),
			m_41(i_other.m_41), m_42(i_other.m_42), m_43(i_other.m_43), m_44(i_other.m_44)
		{
		}

		class Matrix4& Matrix4::operator=(const Matrix4& i_other)
		{

			if (Helpers::IsNaN(i_other.m_11))
			{
				bool trouble = true;
			}
			assert(!Helpers::IsNaN(i_other.m_11));
			
			m_11 = i_other.m_11;
			m_12 = i_other.m_12;
			m_13 = i_other.m_13;
			m_14 = i_other.m_14;

			m_21 = i_other.m_21;
			m_22 = i_other.m_22;
			m_23 = i_other.m_23;
			m_24 = i_other.m_24;

			m_31 = i_other.m_31;
			m_32 = i_other.m_32;
			m_33 = i_other.m_33;
			m_34 = i_other.m_34;

			m_41 = i_other.m_41;
			m_42 = i_other.m_42;
			m_43 = i_other.m_43;
			m_44 = i_other.m_44;

			return *this;
		}
		
		Matrix4 Matrix4::CreateIdentity()
		{
			return Matrix4(1, 0, 0, 0,
						   0, 1, 0, 0,
						   0, 0, 1, 0,
						   0, 0, 0, 1);

		}

		Matrix4 Matrix4::CreateScale(float i_scaleX, float i_scaleY, float i_scaleZ)
		{
			Matrix4 const Identity = CreateIdentity();

			Matrix4 ScaleMatrix = Identity;

			ScaleMatrix.m_11 = i_scaleX;
			ScaleMatrix.m_22 = i_scaleY;
			ScaleMatrix.m_33 = i_scaleZ;

			return ScaleMatrix;
		}

		//Column Matrix Translation
		Matrix4 Matrix4::CreateTranslation(Vector3 i_trans)
		{
			Matrix4 const Identity = CreateIdentity();

			Matrix4 TranslationMatrix = Identity;

			TranslationMatrix.m_14 = i_trans.x();
			TranslationMatrix.m_24 = i_trans.y();
			TranslationMatrix.m_34 = i_trans.z();
			
			return TranslationMatrix;
		}

		//Column Matrix Translation
		Matrix4 Matrix4::CreateTranslation(float i_transX, float i_transY, float i_transZ)
		{
			Matrix4 const Identity = CreateIdentity();

			Matrix4 TranslationMatrix = Identity;

			TranslationMatrix.m_14 = i_transX;
			TranslationMatrix.m_24 = i_transY;
			TranslationMatrix.m_34 = i_transZ;

			return TranslationMatrix;
		}

		Matrix4 Matrix4::CreateXRotation(float i_RotationRadians)
		{
			Matrix4 const Identity = CreateIdentity();

			Matrix4 RotationMatrixX = Identity;

			RotationMatrixX.m_22 = cos(i_RotationRadians);
			RotationMatrixX.m_23 = -sin(i_RotationRadians);
			RotationMatrixX.m_32 = sin(i_RotationRadians);
			RotationMatrixX.m_33 = cos(i_RotationRadians);

			return RotationMatrixX;
		}

		Matrix4 Matrix4::CreateYRotation(float i_RotationRadians)
		{
			Matrix4 const Identity = CreateIdentity();

			Matrix4 RotationMatrixY = Identity;

			RotationMatrixY.m_11 = cos(i_RotationRadians);
			RotationMatrixY.m_13 = sin(i_RotationRadians);
			RotationMatrixY.m_31 = -sin(i_RotationRadians);
			RotationMatrixY.m_33 = cos(i_RotationRadians);

			return RotationMatrixY;
		}

		Matrix4 Matrix4::CreateZRotation(float i_RotationRadians)
		{
			Matrix4 const Identity = CreateIdentity();

			Matrix4 RotationMatrixZ = Identity;

			RotationMatrixZ.m_11 = cos(i_RotationRadians);
			RotationMatrixZ.m_12 = -sin(i_RotationRadians);
			RotationMatrixZ.m_21 = sin(i_RotationRadians);
			RotationMatrixZ.m_22 = cos(i_RotationRadians);

			return RotationMatrixZ;
		}

		void Matrix4::Invert()
		{
			*this = GetInverse();
		}

		Matrix4 Matrix4::GetInverse() const
		{
			float determinant = m_11*m_22*m_33*m_44 + m_11*m_23*m_34*m_42 + m_11*m_24*m_32*m_43 - m_11*m_24*m_33*m_42 - m_11*m_23*m_32*m_44 - m_11*m_22*m_34*m_43
							  - m_12*m_21*m_33*m_44 - m_13*m_21*m_34*m_42 - m_14*m_21*m_32*m_43 + m_14*m_21*m_33*m_42 + m_13*m_21*m_32*m_44 + m_12*m_21*m_34*m_43
							  + m_12*m_23*m_31*m_44 + m_13*m_24*m_31*m_42 + m_14*m_22*m_31*m_43 - m_14*m_23*m_31*m_42 - m_13*m_22*m_31*m_44 - m_12*m_24*m_31*m_43
							  - m_12*m_23*m_34*m_41 - m_13*m_24*m_32*m_41 - m_14*m_22*m_33*m_41 + m_14*m_23*m_32*m_41 + m_13*m_22*m_34*m_41 + m_12*m_24*m_33*m_41;

			if (determinant != 0)
			{
				float adj_11 = m_22 * m_33*m_44 + m_23 * m_34*m_42 + m_24 * m_32*m_43 - m_24 * m_33*m_42 - m_23 * m_32*m_44 - m_22 * m_34*m_43;
				float adj_12 = -m_12 * m_33*m_44 - m_13 * m_34*m_42 - m_14 * m_32*m_43 + m_14 * m_33*m_42 + m_13 * m_32*m_44 + m_12 * m_34*m_43;
				float adj_13 = m_12 * m_23*m_44 + m_13 * m_24*m_42 + m_14 * m_22*m_43 - m_14 * m_23*m_42 - m_13 * m_22*m_44 - m_12 * m_24*m_43;
				float adj_14 = -m_12 * m_23*m_34 - m_13 * m_24*m_32 - m_14 * m_22*m_33 + m_14 * m_23*m_32 + m_13 * m_22*m_34 + m_12 * m_24*m_33;

				float adj_21 = -m_21 * m_33*m_44 - m_23 * m_34*m_41 - m_24 * m_31*m_43 + m_24 * m_33*m_41 + m_23 * m_31*m_44 + m_21 * m_34*m_43;
				float adj_22 = m_11 * m_33*m_44 + m_13 * m_34*m_41 + m_14 * m_31*m_43 - m_14 * m_33*m_41 - m_13 * m_31*m_44 - m_11 * m_34*m_43;
				float adj_23 = -m_11 * m_23*m_44 - m_13 * m_24*m_41 - m_14 * m_21*m_43 + m_14 * m_23*m_41 + m_13 * m_21*m_44 + m_11 * m_24*m_43;
				float adj_24 = m_11 * m_23*m_34 + m_13 * m_24*m_31 + m_14 * m_21*m_33 - m_14 * m_23*m_31 - m_13 * m_21*m_34 - m_11 * m_24*m_33;

				float adj_31 = m_21 * m_32*m_44 + m_22 * m_34*m_41 + m_24 * m_31*m_42 - m_24 * m_32*m_41 - m_22 * m_31*m_44 - m_21 * m_34*m_42;
				float adj_32 = -m_11 * m_32*m_44 - m_12 * m_34*m_41 - m_14 * m_31*m_42 + m_14 * m_32*m_41 + m_12 * m_31*m_44 + m_11 * m_34*m_42;
				float adj_33 = m_11 * m_22*m_44 + m_12 * m_24*m_41 + m_14 * m_21*m_42 - m_14 * m_22*m_41 - m_12 * m_21*m_44 - m_11 * m_24*m_42;
				float adj_34 = -m_11 * m_22*m_34 - m_12 * m_24*m_31 - m_14 * m_21*m_32 + m_14 * m_22*m_31 + m_12 * m_21*m_34 + m_11 * m_24*m_32;

				float adj_41 = -m_21 * m_32*m_43 - m_22 * m_33*m_41 - m_23 * m_31*m_42 + m_23 * m_32*m_41 + m_22 * m_31*m_43 + m_21 * m_33*m_42;
				float adj_42 = m_11 * m_32*m_43 + m_12 * m_33*m_41 + m_13 * m_31*m_42 - m_13 * m_32*m_41 - m_12 * m_31*m_43 - m_11 * m_33*m_42;
				float adj_43 = -m_11 * m_22*m_43 - m_12 * m_23*m_41 - m_13 * m_21*m_42 + m_13 * m_22*m_41 + m_12 * m_21*m_43 + m_11 * m_23*m_42;
				float adj_44 = m_11 * m_22*m_33 + m_12 * m_23*m_31 + m_13 * m_21*m_32 - m_13 * m_22*m_31 - m_12 * m_21*m_33 - m_11 * m_23*m_32;

				float inv_11 = 1 / determinant * adj_11;
				float inv_12 = 1 / determinant * adj_12;
				float inv_13 = 1 / determinant * adj_13;
				float inv_14 = 1 / determinant * adj_14;

				float inv_21 = 1 / determinant * adj_21;
				float inv_22 = 1 / determinant * adj_22;
				float inv_23 = 1 / determinant * adj_23;
				float inv_24 = 1 / determinant * adj_24;

				float inv_31 = 1 / determinant * adj_31;
				float inv_32 = 1 / determinant * adj_32;
				float inv_33 = 1 / determinant * adj_33;
				float inv_34 = 1 / determinant * adj_34;

				float inv_41 = 1 / determinant * adj_41;
				float inv_42 = 1 / determinant * adj_42;
				float inv_43 = 1 / determinant * adj_43;
				float inv_44 = 1 / determinant * adj_44;

				return Matrix4(inv_11, inv_12, inv_13, inv_14,
					inv_21, inv_22, inv_23, inv_24,
					inv_31, inv_32, inv_33, inv_34,
					inv_41, inv_42, inv_43, inv_44);
			}
			else
			{
				return Identity;
			}
			
		}

		void Matrix4::Transpose()
		{
			*this = GetTranspose();
		}
		
		Matrix4 Matrix4::GetTranspose() const
		{
			assert( !Helpers::IsNaN( m_11 ) );
			
			return Matrix4(m_11, m_21, m_31, m_41,
				m_12, m_22, m_32, m_42,
				m_13, m_23, m_33, m_43,
				m_14, m_24, m_34, m_44);
		}

		Matrix4 Matrix4::operator*(const Matrix4& i_Vector) const
		{
			Matrix4 Result = CreateIdentity();
			Result = Multiply(i_Vector, Result);
			return Result;
		}

		//Row Vector Multiplication
		Vector4 Matrix4::MultiplyLeft(const Vector4& i_Vector) const
		{
			Vector4 result;

			result.x(i_Vector.x() * m_11 + i_Vector.y() * m_21 + i_Vector.z() * m_31 + i_Vector.w() * m_41);
			result.y(i_Vector.x() * m_12 + i_Vector.y() * m_22 + i_Vector.z() * m_32 + i_Vector.w() * m_42);
			result.z(i_Vector.x() * m_13 + i_Vector.y() * m_23 + i_Vector.z() * m_33 + i_Vector.w() * m_43);
			result.w(i_Vector.x() * m_14 + i_Vector.y() * m_24 + i_Vector.z() * m_34 + i_Vector.w() * m_44);

			return result;
		}
		
		//Column Vector Multiplication
		Vector4 Matrix4::MultiplyRight(const Vector4& i_Vector) const
		{
			Vector4 result;

			result.x(i_Vector.x() * m_11 + i_Vector.y() * m_12 + i_Vector.z() * m_13 + i_Vector.w() * m_14);
			result.y(i_Vector.x() * m_21 + i_Vector.y() * m_22 + i_Vector.z() * m_23 + i_Vector.w() * m_24);
			result.z(i_Vector.x() * m_31 + i_Vector.y() * m_32 + i_Vector.z() * m_33 + i_Vector.w() * m_34);
			result.w(i_Vector.x() * m_41 + i_Vector.y() * m_42 + i_Vector.z() * m_43 + i_Vector.w() * m_44);

			return result;
		}

		bool Matrix4::operator==(const Matrix4& i_other) const
		{
			return (m_11 == i_other.m_11 &&
					m_12 == i_other.m_12 &&
					m_13 == i_other.m_13 &&
					m_14 == i_other.m_14 &&

					m_21 == i_other.m_21 &&
					m_22 == i_other.m_22 &&
					m_23 == i_other.m_23 &&
					m_24 == i_other.m_24 &&

					m_31 == i_other.m_31 &&
					m_32 == i_other.m_32 &&
					m_33 == i_other.m_33 &&
					m_34 == i_other.m_34 &&

					m_41 == i_other.m_41 &&
					m_42 == i_other.m_42 &&
					m_43 == i_other.m_43 &&
					m_44 == i_other.m_44 );
		}

		const Matrix4 Matrix4::Identity(1, 0, 0, 0,
										0, 1, 0, 0,
										0, 0, 1, 0,
										0, 0, 0, 1);

		Matrix4 Matrix4::Multiply(const Matrix4& i_other, Matrix4 o_out) const
		{
			o_out.m_11 = m_11 * i_other.m_11 + m_12 * i_other.m_21 + m_13 * i_other.m_31 + m_14 * i_other.m_41;
			o_out.m_12 = m_11 * i_other.m_12 + m_12 * i_other.m_22 + m_13 * i_other.m_32 + m_14 * i_other.m_42;
			o_out.m_13 = m_11 * i_other.m_13 + m_12 * i_other.m_23 + m_13 * i_other.m_33 + m_14 * i_other.m_43;
			o_out.m_14 = m_11 * i_other.m_14 + m_12 * i_other.m_24 + m_13 * i_other.m_34 + m_14 * i_other.m_44;

			o_out.m_21 = m_21 * i_other.m_11 + m_22 * i_other.m_21 + m_23 * i_other.m_31 + m_24 * i_other.m_41;
			o_out.m_22 = m_21 * i_other.m_12 + m_22 * i_other.m_22 + m_23 * i_other.m_32 + m_24 * i_other.m_42;
			o_out.m_23 = m_21 * i_other.m_13 + m_22 * i_other.m_23 + m_23 * i_other.m_33 + m_24 * i_other.m_43;
			o_out.m_24 = m_21 * i_other.m_14 + m_22 * i_other.m_24 + m_23 * i_other.m_34 + m_24 * i_other.m_44;

			o_out.m_31 = m_31 * i_other.m_11 + m_32 * i_other.m_21 + m_33 * i_other.m_31 + m_34 * i_other.m_41;
			o_out.m_32 = m_31 * i_other.m_12 + m_32 * i_other.m_22 + m_33 * i_other.m_32 + m_34 * i_other.m_42;
			o_out.m_33 = m_31 * i_other.m_13 + m_32 * i_other.m_23 + m_33 * i_other.m_33 + m_34 * i_other.m_43;
			o_out.m_34 = m_31 * i_other.m_14 + m_32 * i_other.m_24 + m_33 * i_other.m_34 + m_34 * i_other.m_44;

			o_out.m_41 = m_41 * i_other.m_11 + m_42 * i_other.m_21 + m_43 * i_other.m_31 + m_44 * i_other.m_41;
			o_out.m_42 = m_41 * i_other.m_12 + m_42 * i_other.m_22 + m_43 * i_other.m_32 + m_44 * i_other.m_42;
			o_out.m_43 = m_41 * i_other.m_13 + m_42 * i_other.m_23 + m_43 * i_other.m_33 + m_44 * i_other.m_43;
			o_out.m_44 = m_41 * i_other.m_14 + m_42 * i_other.m_24 + m_43 * i_other.m_34 + m_44 * i_other.m_44;
			
			return o_out;
		}
	}
}