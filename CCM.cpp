#include "MyISP.h"
#include <math.h>

// RGB data processing
// color correction
void CCM(ImageRGB& img, float ccm[][4])
{
	float mulval[3][3] = { 0 };

	for (int y = 0; y < img.getHeight(); y++)
	{
		for (int x = 0; x < img.getWidth(); x++)
		{
			mulval[0][0] = ccm[0][0] * img.at(y, x).R;
			mulval[0][1] = ccm[0][1] * img.at(y, x).G;
			mulval[0][2] = ccm[0][2] * img.at(y, x).B;

			mulval[1][0] = ccm[1][0] * img.at(y, x).R;
			mulval[1][1] = ccm[1][1] * img.at(y, x).G;
			mulval[1][2] = ccm[1][2] * img.at(y, x).B;

			mulval[2][0] = ccm[2][0] * img.at(y, x).R;
			mulval[2][1] = ccm[2][1] * img.at(y, x).G;
			mulval[2][2] = ccm[2][2] * img.at(y, x).B;

			float R = mulval[0][0] + mulval[0][1] + mulval[0][2] + ccm[0][3];
			float G = mulval[1][0] + mulval[1][1] + mulval[1][2] + ccm[1][3];
			float B = mulval[2][0] + mulval[2][1] + mulval[2][2] + ccm[2][3];

			if (R < 0) R = 0;
			if (G < 0) G = 0;
			if (B < 0) B = 0;
			if (R > 255) R = 255;
			if (G > 255) G = 255;
			if (B > 255) B = 255;

			img.at(y, x).R = uint8_t(R);
			img.at(y, x).G = uint8_t(G);
			img.at(y, x).B = uint8_t(B);
		}
	}
}

