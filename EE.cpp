#include "MyISP.h"

#include <stdio.h>
#include <math.h>
#include <string.h>


void EE(ImageYUV& yuv, float ee[][3], float gain)
{
	ImageYUV* yuv_pad = new ImageYUV(yuv);
	yuv_pad->padding(1);

	for (int y = 0; y < yuv.getHeight(); y++)
	{
		for (int x = 0; x < yuv.getWidth(); x++)
		{
			float Y = yuv.at(y, x).Y;
			float lap = 0;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					lap = lap + ee[i][j] * yuv_pad->at(y + i, x + j).Y;
				}
			}
			Y = Y + gain * lap;
			if (Y < 0)Y = 0;
			if (Y > 255)Y = 255;
			yuv.at(y, x).Y = Y;
		}
	}

	delete yuv_pad;
}
