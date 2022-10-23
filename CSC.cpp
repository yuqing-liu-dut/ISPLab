#include "MyISP.h"
#include <math.h>

// RGB data processing
// RGB to YUV

ImageYUV CSC(ImageRGB& rgb)
{
	ImageYUV yuv(rgb.getHeight(), rgb.getWidth());
	float Y, U, V;
	for (int y = 0; y < rgb.getHeight(); y++)
	{
		for (int x = 0; x < rgb.getWidth(); x++)
		{
			Y = ( 0.2568 * rgb.at(y, x).R + 0.5041 * rgb.at(y, x).G + 0.0979 * rgb.at(y, x).B) + 16;
			U = (-0.1479 * rgb.at(y, x).R - 0.2896 * rgb.at(y, x).G + 0.4375 * rgb.at(y, x).B) + 128;
			V = ( 0.4375 * rgb.at(y, x).R - 0.3666 * rgb.at(y, x).G - 0.0709 * rgb.at(y, x).B) + 128;
			if (Y < 0)Y = 0; if (Y > 255)Y = 255;
			if (U < 0)U = 0; if (U > 255)U = 255;
			if (V < 0)V = 0; if (V > 255)V = 255;
			yuv.at(y, x).Y = Y;
			yuv.at(y, x).U = U;
			yuv.at(y, x).V = V;
		}
	}
	return yuv;
}

ImageRGB CSC(ImageYUV& yuv)
{
	ImageRGB rgb(yuv.getHeight(), yuv.getWidth());
	float R, G, B;
	for (int y = 0; y < yuv.getHeight(); y++)
	{
		for (int x = 0; x < yuv.getWidth(); x++)
		{
			R = (1.1644 * yuv.at(y, x).Y						   + 1.6019 * yuv.at(y, x).V - 223.5521);
			G = (1.1644 * yuv.at(y, x).Y - 0.3928 * yuv.at(y, x).U - 0.8163 * yuv.at(y, x).V + 136.1381);
			B = (1.1644 * yuv.at(y, x).Y + 2.0253 * yuv.at(y, x).U							 - 278.0291);

			if (R < 0)R = 0; if (R > 255)R = 255;
			if (G < 0)G = 0; if (G > 255)G = 255;
			if (B < 0)B = 0; if (B > 255)B = 255;

			rgb.at(y, x).R = R;
			rgb.at(y, x).G = G;
			rgb.at(y, x).B = B;
		}
	}
	return rgb;
}