#ifndef Artemis_TrigLUT_h__
#define Artemis_TrigLUT_h__

namespace artemis
{

// Thanks to Riven
// From: http://riven8192.blogspot.com/2009/08/fastmath-sincos-lookup-tables.html
class TrigLUT
{
public:
	static float sin(float rad)
   {
      if (!mIsInited)
         init();
		return mSin[(int) (rad * radToIndex) & SIN_MASK];
	}

	static float cos(float rad)
   {
      if (!mIsInited)
         init();
		return mCos[(int) (rad * radToIndex) & SIN_MASK];
	}

	static float sinDeg(float deg)
   {
      if (!mIsInited)
         init();
		return mSin[(int) (deg * degToIndex) & SIN_MASK];
	}

	static float cosDeg(float deg)
   {
      if (!mIsInited)
         init();
		return mCos[(int) (deg * degToIndex) & SIN_MASK];
	}

   static float toDegrees(float rad)
   {
      return rad*DEG;
   }

private:
   static const float RAD, DEG;
	static const int SIN_BITS = 12;
   static const int SIN_MASK = ~(-1 << SIN_BITS);
   static const int SIN_COUNT = SIN_MASK + 1;
	static const float radFull, radToIndex;
	static const float degFull, degToIndex;

   static bool mIsInited;
   static void init();
	static float mSin[SIN_COUNT], mCos[SIN_COUNT];
};

}
#endif // Artemis_TrigLUT_h__
