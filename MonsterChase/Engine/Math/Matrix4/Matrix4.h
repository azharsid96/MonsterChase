#pragma once
#include "../Vector3/Vector3.h"
#include "../Vector4/Vector4.h"
#include "../Vector2/Point2D.h"
#include "../../Helpers/Helpers.h"

#include <assert.h>
#include <math.h>

namespace Engine
{
	namespace Math
	{
		class Matrix4
		{
		public:
			//Default constructor
			Matrix4();

			//Value based constructor
			Matrix4(
				float i_11, float i_12, float i_13, float i_14,
				float i_21, float i_22, float i_23, float i_24,
				float i_31, float i_32, float i_33, float i_34,
				float i_41, float i_42, float i_43, float i_44);

			//Copy constructor
			Matrix4(const Matrix4 & i_other);

			//Assignment operator
			Matrix4 & operator=(const Matrix4 & i_other);

			//Matrix creators
			static Matrix4 CreateIdentity( void );
			
			static Matrix4 CreateXRotation( float i_RotationRadians );
			static Matrix4 CreateYRotation( float i_RotationRadians );
			static Matrix4 CreateZRotation( float i_RotationRadians );

			static Matrix4 CreateTranslation( Vector3 i_trans );
			static Matrix4 CreateTranslation( float i_transX, float i_transY, float i_transZ );
			
			static Matrix4 CreateScale( float i_scaleX, float i_scaleY, float i_scaleZ );

			//Invert this matrix instance
			void Invert( void );
			//Gets the matrix that is the inverse of this instance but does not modify this instance itself
			Matrix4 GetInverse( void ) const;

			// Transpose this matrix instance
			void Transpose( void );
			//Gets the matrix that is the transpose of this instance but does not modify this instance itself
			Matrix4 GetTranspose( void ) const;

			//Multiply Matrix * Matrix
			//return *this * i_other
			Matrix4 operator*( const Matrix4 &i_Vector ) const;

			//Multiply Vector by Matrix
			//return v * M
			Vector4 MultiplyLeft( const Vector4 &i_Vector ) const;
			//return M * v
			Vector4 MultiplyRight( const Vector4 &i_Vector ) const;

			//Matrix == Matrix
			bool operator==( const Matrix4 & i_other ) const;

			static const Matrix4 Identity;

		private:
			float m_11, m_12, m_13, m_14, //row 1
				  m_21, m_22, m_23, m_24, //row 2
				  m_31, m_32, m_33, m_34, //row 3
				  m_41, m_42, m_43, m_44; //row 4

			Matrix4 Multiply( const Matrix4 & i_other, Matrix4 o_out ) const;

			//internal inverse function used by Invert() and GetInverse()
			Matrix4 & InverseSSE( Matrix4 & o_out ) const;
			Matrix4 & MultiplySSE( const Matrix4 & i_other, Matrix4 & o_out ) const;
		};

		////Allows us to use V = M * V; (column vector)
		//inline Vector4 operator*(const Matrix4 & i_mtx, const Vector4 & i_vec) { Matrix4::MultiplyRight(i_vec); };
		////Allows us to use V = V * M; (row vector)
		//inline Vector4 operator*( const Vector4 & i_vec, const Matrix4 & i_mtx ) { Matrix4::MultiplyLeft(i_vec); };
	}
}