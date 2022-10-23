#include "MyISP.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <algorithm>


void NRchroma(ImageYUV& yuv, int d)
{
	ImageYUV* yuv_pad = new ImageYUV(yuv);
	yuv_pad->padding(d / 2, PADDING_MODE_EDGE);

	uint8_t* valU = new uint8_t[d * d];
	uint8_t* valV = new uint8_t[d * d];

	for (int y = 0; y < yuv.getHeight(); y++)
	{
		for (int x = 0; x < yuv.getWidth(); x++)
		{		
			for (int i = 0; i < d; i++)
			{
				for (int j = 0; j < d; j++)
				{
					valU[i * d + j] = yuv_pad->at(y + i, x + j).U;
					valV[i * d + j] = yuv_pad->at(y + i, x + j).V;
				}
			}

			std::sort(valU, valU + d * d);
			std::sort(valV, valV + d * d);

			yuv.at(y, x).U = valU[d * d / 2];
			yuv.at(y, x).V = valV[d * d / 2];

			
		}
	}

	delete[] valU;
	delete[] valV;

	delete yuv_pad;
}

void NRluma(ImageYUV& yuv, int d)
{
	ImageYUV* yuv_pad = new ImageYUV(yuv);
	yuv_pad->padding(d / 2, PADDING_MODE_EDGE);

	uint8_t* valY = new uint8_t[d * d];

	for (int y = 0; y < yuv.getHeight(); y++)
	{
		for (int x = 0; x < yuv.getWidth(); x++)
		{
			for (int i = 0; i < d; i++)
			{
				for (int j = 0; j < d; j++)
				{
					valY[i * d + j] = yuv_pad->at(y + i, x + j).Y;
				}
			}

			std::sort(valY, valY + d * d);

			yuv.at(y, x).Y = valY[d * d / 2];


		}
	}

	delete[] valY;

	delete yuv_pad;
}