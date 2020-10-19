#pragma once
#include "GLib.h"
#include "Math/Vector2/Point2D.h"

#include <cmath>
#include <stdint.h>
#include <vector>// For int32_t, etc.

namespace Helpers
{
	GLib::Sprite * CreateSprite(const char * i_pFilename, float & o_width, float & o_height);
	void * LoadFile(const char * i_pFilename, size_t & o_sizeFile);
	std::vector<uint8_t> LoadFileToBuffer(const char* i_pFilename);

#pragma region NANCompare
	inline bool IsNaN (float i_value)
	{
		volatile float val = i_value;
		return val != val;
	}
#pragma endregion

#pragma region OptimizedZeroCompare
	// semi-safe but fast floating point compare
	inline bool AreEqual_Eps(float i_lhs, float i_rhs, float i_maxDiff)
	{
		return fabs(i_lhs - i_rhs) < i_maxDiff;
	}

	// safe check for zero
	inline bool IsZero(float i_val)
	{
		return AreEqual_Eps(i_val, 0.0f, .000000001f);
	}
#pragma endregion

#pragma region RelativeEpsilonComparison
	inline bool AreEqual_Rel(float i_lhs, float i_rhs, float i_maxDiff)
	{
		// Don’t waste the time if they happen to be equal (bit-wise)
		if (i_lhs == i_rhs)
			return true;

		// Calculate a relative difference between the two numbers
		float relDiff;

		if (fabs(i_rhs) > fabs(i_lhs))
			relDiff = static_cast<float>(fabs((i_lhs - i_rhs) / i_rhs));
		else
			relDiff = static_cast<float>(fabs((i_lhs - i_rhs) / i_lhs));

		// Is it less than the maximum tolerable
		return relDiff <= i_maxDiff;
	}
#pragma endregion

#pragma region ULPs
	union Float_t
	{
		Float_t(float num = 0.0f) : f(num) {}
		// Portable extraction of components.
		bool Negative() const { return i < 0; }
		int32_t RawMantissa() const { return i & ((1 << 23) - 1); }
		int32_t RawExponent() const { return (i >> 23) & 0xFF; }

		int32_t i;
		float f;
#ifdef _DEBUG
		struct
		{   // Bitfields for exploration. Do not use in production code.
			uint32_t mantissa : 23;
			uint32_t exponent : 8;
			uint32_t sign : 1;
		} parts;
#endif
	};

	inline bool AlmostEqualUlps(float A, float B, int maxUlpsDiff)
	{
		Float_t uA(A);
		Float_t uB(B);

		// Different signs means they do not match.
		if (uA.Negative() != uB.Negative())
		{
			// Check for equality to make sure +0==-0
			if (A == B)
				return true;
			return false;
		}

		// Find the difference in ULPs.
		int ulpsDiff = abs(uA.i - uB.i);
		if (ulpsDiff <= maxUlpsDiff)
			return true;

		return false;
	}
#pragma endregion

}