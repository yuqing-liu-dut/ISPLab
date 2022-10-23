#include "MyISP.h"

#include <stdio.h>
#include <math.h>
#include <string.h>

#define PI 3.1415926535

void HSC(ImageYUV& yuv, float hue, float saturation)
{
	float lut_sin = sin(hue * PI / 180);
	float lut_cos = cos(hue * PI / 180);

	float U, V;

	for (int y = 0; y < yuv.getHeight(); y++)
	{
		for (int x = 0; x < yuv.getWidth(); x++)
		{
			U = yuv.at(y, x).U;
			V = yuv.at(y, x).V;

			// Hue
			U = (U - 128) * lut_cos + (V - 128) * lut_sin;
			V = (V - 128) * lut_cos + (U - 128) * lut_sin;

			// Saturation
			U = saturation * (U) + 128;
			V = saturation * (V) + 128;

			if (U < 16)U = 16; if (U > 240) U = 240;
			if (V < 16)V = 16; if (V > 240) V = 240;

			yuv.at(y, x).U = U;
			yuv.at(y, x).V = V;
		}
	}
}