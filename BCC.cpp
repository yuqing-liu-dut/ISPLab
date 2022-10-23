#include "MyISP.h"

#include <stdio.h>
#include <math.h>
#include <string.h>


void BCC(ImageYUV& yuv, uint8_t brightness, float contrast)
{
	for (int y = 0; y < yuv.getHeight(); y++)
	{
		for (int x = 0; x < yuv.getWidth(); x++) 
		{
			float Y = yuv.at(y, x).Y;
			Y = Y + brightness;
			Y = Y + (Y - 127) * contrast;
			if (Y < 0) Y = 0;
			if (Y > 255)Y = 255;
			yuv.at(y, x).Y = Y;
		}
	}
}
