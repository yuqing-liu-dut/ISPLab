#include "MyISP.h"
#include <math.h>

// RGB data processing
// auto white balance
void GC(ImageRGB& rgb, int bw, float gamma)
{
	float maxval = pow(2, bw)-1;

	for (int y = 0; y < rgb.getHeight(); y++)
	{
		for (int x = 0; x < rgb.getWidth(); x++)
		{
			rgb.at(y, x).R = rgb.at(y, x).R * pow((rgb.at(y, x).R / maxval), gamma);
			rgb.at(y, x).G = rgb.at(y, x).G * pow((rgb.at(y, x).G / maxval), gamma);
			rgb.at(y, x).B = rgb.at(y, x).B * pow((rgb.at(y, x).B / maxval), gamma);
		}
	}

}

