#include "artemis/utils/TrigLUT.h"
#include "artemis/utils/FastMath.h"
#include <cmath>

namespace artemis
{
bool TrigLUT::mIsInited = false;

const float TrigLUT::RAD = (float) FastMath::PI / 180.0f;
const float TrigLUT::DEG = 180.0f / (float) FastMath::PI;
const float TrigLUT::radFull = (float) (FastMath::PI * 2.0);
const float TrigLUT::degFull = (float) (360.0);
const float TrigLUT::radToIndex = SIN_COUNT / radFull;
const float TrigLUT::degToIndex = SIN_COUNT / degFull;

float TrigLUT::mSin[SIN_COUNT];
float TrigLUT::mCos[SIN_COUNT];

void TrigLUT::init()
{
   for (int i = 0; i < SIN_COUNT; i++) {
      mSin[i] = (float) std::sin((i + 0.5f) / SIN_COUNT * radFull);
      mCos[i] = (float) std::cos((i + 0.5f) / SIN_COUNT * radFull);
   }
   mIsInited = true;
}

}