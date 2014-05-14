#ifndef Artemis_Utils_h__
#define Artemis_Utils_h__

#include "artemis/utils/FastMath.h"
#include "artemis/utils/TrigLUT.h"
#include <cmath>

namespace artemis
{

class Utils
{
public:
   static float cubicInterpolation(float v0, float v1, float v2, float v3, float t)
   {
		float t2 = t * t;
		float a0 = v3 - v2 - v0 + v1;
		float a1 = v0 - v1 - a0;
		float a2 = v2 - v0;
		float a3 = v1;

		return (a0 * (t * t2)) + (a1 * t2) + (a2 * t) + a3;
	}

	static float quadraticBezierInterpolation(float a, float b, float c, float t)
   {
		return (((1.f - t) * (1.f - t)) * a) + (((2.f * t) * (1.f - t)) * b) + ((t * t) * c);
	}

	static float lengthOfQuadraticBezierCurve(float x0, float y0, float x1, float y1, float x2, float y2)
   {
		if ((x0 == x1 && y0 == y1) || (x1 == x2 && y1 == y2)) {
			return distance(x0, y0, x2, y2);
		}

		float ax, ay, bx, by;
		ax = x0 - 2 * x1 + x2;
		ay = y0 - 2 * y1 + y2;
		bx = 2 * x1 - 2 * x0;
		by = 2 * y1 - 2 * y0;
		float A = 4 * (ax * ax + ay * ay);
		float B = 4 * (ax * bx + ay * by);
		float C = bx * bx + by * by;

		float Sabc = 2.f * (float) std::sqrt(A + B + C);
		float A_2 = (float) std::sqrt(A);
		float A_32 = 2.f * A * A_2;
		float C_2 = 2.f * std::sqrt(C);
		float BA = B / A_2;

		return (A_32 * Sabc + A_2 * B * (Sabc - C_2) + (4.f * C * A - B * B) * std::log((2 * A_2 + BA + Sabc) / (BA + C_2))) / (4 * A_32);
	}

	static float lerp(float a, float b, float t)
   {
		if (t < 0)
			return a;
		return a + t * (b - a);
	}

	static float distance(float x1, float y1, float x2, float y2)
   {
		return euclideanDistance(x1, y1, x2, y2);
	}

	static bool doCirclesCollide(float x1, float y1, float radius1, float x2, float y2, float radius2)
   {
		float dx = x2 - x1;
		float dy = y2 - y1;
		float d = radius1 + radius2;
		return (dx * dx + dy * dy) < (d * d);
	}

	static float euclideanDistanceSq2D(float x1, float y1, float x2, float y2)
   {
		float dx = x1 - x2;
		float dy = y1 - y2;
		return dx * dx + dy * dy;
	}

	static float manhattanDistance(float x1, float y1, float x2, float y2)
   {
		return std::abs(x1 - x2) + std::abs(y1 - y2);
	}

	static float euclideanDistance(float x1, float y1, float x2, float y2)
   {
		float a = x1 - x2;
		float b = y1 - y2;

		return (float) FastMath::sqrt(a * a + b * b);
	}

	static float angleInDegrees(float ownerRotation, float x1, float y1, float x2, float y2)
   {
		float dg = std::abs(ownerRotation - angleInDegrees(x1, y1, x2, y2)) /*% 360*/;
      while (dg >= 360.f)
         dg -= 360.f;
      return dg;
	}

	static float angleInDegrees(float originX, float originY, float targetX, float targetY)
   {
		return (float) TrigLUT::toDegrees(std::atan2(targetY - originY, targetX - originX));
	}

	static float angleInRadians(float originX, float originY, float targetX, float targetY)
   {
		return (float) std::atan2(targetY - originY, targetX - originX);
	}

	static bool shouldRotateCounterClockwise(float angleFrom, float angleTo)
   {
		float diff = (angleFrom - angleTo)/* % 360*/;
      while (diff >= 360.f)
         diff -= 360.f;
      while (diff <= -360.f)
         diff += 360.f;
		return diff > 0 ? diff < 180 : diff < -180;
	}

	static float getRotatedX(float currentX, float currentY, float pivotX, float pivotY, float angleDegrees)
   {
		float x = currentX - pivotX;
		float y = currentY - pivotY;
		float xr = (x * TrigLUT::cosDeg(angleDegrees)) - (y * TrigLUT::sinDeg(angleDegrees));
		return xr + pivotX;
	}

	static float getRotatedY(float currentX, float currentY, float pivotX, float pivotY, float angleDegrees)
   {
		float x = currentX - pivotX;
		float y = currentY - pivotY;
		float yr = (x * TrigLUT::sinDeg(angleDegrees)) + (y * TrigLUT::cosDeg(angleDegrees));
		return yr + pivotY;
	}

	static float getXAtEndOfRotatedLineByOrigin(float x, float lineLength, float angleDegrees)
   {
		return x + TrigLUT::cosDeg(angleDegrees) * lineLength;
	}

	static float getYAtEndOfRotatedLineByOrigin(float y, float lineLength, float angleDegrees)
   {
		return y + TrigLUT::sinDeg(angleDegrees) * lineLength;
	}

	static bool collides(float x1, float y1, float radius1, float x2, float y2, float radius2)
   {
		float d = Utils::distance(x1, y1, x2, y2);

		d -= radius1 + radius2;

		return d < 0;
	}

	/*static String readFileContents(String file)
   {
		InputStream is = Utils.class.getClassLoader().getResourceAsStream(file);
		String contents = "";
		try {
			if (is != null) {
				Writer writer = new StringWriter();

				char[] buffer = new char[1024];
				Reader reader = new BufferedReader(new InputStreamReader(is, "UTF-8"));
				int n;
				while ((n = reader.read(buffer)) != -1) {
					writer.write(buffer, 0, n);
				}

				contents = writer.toString();
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				is.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}

		return contents;
	}*/

};
}
#endif // Artemis_Utils_h__
