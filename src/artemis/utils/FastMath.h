#ifndef Artemis_FastMath_h__
#define Artemis_FastMath_h__

#include <cmath>

namespace artemis
{

class FastMath
{
public:
   const static double PI;
	const static double SQUARED_PI;
	const static double HALF_PI;
	const static double TWO_PI;
	const static double THREE_PI_HALVES;

private:
   const static double _sin_a;
	const static double _sin_b;
	const static double _sin_p;

	const static double _asin_a;
	const static double _asin_b;
	const static double _asin_c;
	const static double _asin_d;

	const static double _atan_a;

public:
   static double cos(double x)
   {
		return sin(x + ((x > HALF_PI) ? -THREE_PI_HALVES : HALF_PI));
	}

	static double sin(double x)
   {
		x = _sin_a * x * std::fabs(x) + _sin_b * x;
		return _sin_p * (x * std::fabs(x) - x) + x;
	}

	static double tan(double x)
   {
		return sin(x) / cos(x);
	}

	static double asin(double x)
   {
		return x * (std::fabs(x) * (std::fabs(x) * _asin_a + _asin_b) + _asin_c) + (x>=0?1:-1) * (_asin_d - std::sqrt(1 - x * x));
	}

	static double acos(double x)
   {
		return HALF_PI - asin(x);
	}

	static double atan(double x)
   {
		return (std::fabs(x) < 1) ? x / (1 + _atan_a * x * x) : (x>=0?1:-1) * HALF_PI - x / (x * x + _atan_a);
	}

	static double inverseSqrt(double x)
   {
		double xhalves = 0.5 * x;
      union
      {
         double d;
         unsigned long long int l;
      } c;
      c.d = x;
      c.l = 0x5FE6EB50C7B537AAl - (c.l >> 1);
		x = c.d;
		return x * (1.5 - xhalves * x * x); // more iterations possible
	}

	static double sqrt(double x)
   {
		return x * inverseSqrt(x);
	}

};
}
#endif // Artemis_FastMath_h__
