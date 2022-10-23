#include "MyISP.h"
#include <math.h>
#include <string.h>

// RAW data processing
// chroma noise filtering

float __cnc(const char* is_color, float center, float avgG, float avgC1, float avgC2, float r_gain, float gr_gain, float gb_gain, float b_gain)
{
	float dampFactor = 1.0;
	float signalGap = center - MAX(avgG, avgC2);
	if (strcmp(is_color, "r") == 0)
	{
		if (r_gain <= 1.0) dampFactor = 1.0;
		else if (r_gain > 1.0 && r_gain <= 1.2) dampFactor = 0.5;
		else if (r_gain > 1.2) dampFactor = 0.3;
	}
	if (strcmp(is_color, "b") == 0)
	{
		if (b_gain <= 1.0) dampFactor = 1.0;
		else if (b_gain > 1.0 && b_gain <= 1.2) dampFactor = 0.5;
		else if (b_gain > 1.2) dampFactor = 0.3;
	}
	float chromaCorrected = MAX(avgG, avgC2) + dampFactor * signalGap;
	float signalMeter = 0.299 * avgC2 + 0.587 * avgG + 0.144 * avgC1;
	if (strcmp(is_color, "r") == 0) signalMeter = 0.299 * avgC1 + 0.587 * avgG + 0.144 * avgC2;
	else if (strcmp(is_color, "b") == 0) signalMeter = 0.299 * avgC2 + 0.587 * avgG + 0.144 * avgC1;
	float fade1 = 0, fade2 = 0;
	if (signalMeter <= 30) fade1 = 1.0;
	else if (signalMeter > 30 && signalMeter <= 50) fade1 = 0.9;
	else if (signalMeter > 50 && signalMeter <= 70) fade1 = 0.8;
	else if (signalMeter > 70 && signalMeter <= 100) fade1 = 0.7;
	else if (signalMeter > 100 && signalMeter <= 150) fade1 = 0.6;
	else if (signalMeter > 150 && signalMeter <= 200) fade1 = 0.3;
	else if (signalMeter > 200 && signalMeter <= 250) fade1 = 0.1;
	else fade1 = 0;
	if (avgC1 <= 30) fade2 = 1.0;
	else if (avgC1 > 30 && avgC1 <= 50) fade2 = 0.9;
	else if (avgC1 > 50 && avgC1 <= 70) fade2 = 0.8;
	else if (avgC1 > 70 && avgC1 <= 100) fade2 = 0.6;
	else if (avgC1 > 100 && avgC1 <= 150) fade2 = 0.5;
	else if (avgC1 > 150 && avgC1 <= 200) fade2 = 0.3;
	else fade2 = 0;
	float fadeTot = fade1 * fade2;
	return (1 - fadeTot) * center + fadeTot * chromaCorrected;
}

void __cnd(int y, int x, ImageRaw* img, float thres, int& is_noise, float& avgG, float& avgC1, float& avgC2)
{
	avgG = 0, avgC1 = 0, avgC2 = 0;
	is_noise = 0;

	for (int i = y - 4; i < y + 4; i++)
	{
		for (int j = x - 4; j < x + 4; j++)
		{
			if ((i % 2 == 1) && (j % 2 == 0))		avgG = avgG + img->at(i, j);
			else if ((i % 2 == 0) && (j % 2 == 1))	avgG = avgG + img->at(i, j);
			else if ((i % 2 == 0) && (j % 2 == 0))	avgC1 = avgC1 + img->at(i, j);
			else if ((i % 2 == 1) && (j % 2 == 1))	avgC2 = avgC2 + img->at(i, j);
		}
	}
	avgG = avgG / 40;
	avgC1 = avgC1 / 25;
	avgC2 = avgC2 / 16;
	float center = img->at(y, x);
	if ((center > avgG + thres) && (center > avgC2 + thres))
	{
		if ((avgC1 > avgG + thres) && (avgC1 > avgC2 + thres))
		{
			is_noise = 1;
		}
		else
		{
			is_noise = 0;
		}
	}
	else
	{
		is_noise = 0;
	}
}

float __cnf(const char* is_color, int y, int x, ImageRaw* img, float thres, float r_gain, float gr_gain, float gb_gain, float b_gain)
{
	int is_noise;
	float avgG, avgC1, avgC2;
	__cnd(y, x, img, thres, is_noise, avgG, avgC1, avgC2);

	float pix_out;
	if (is_noise == 1)
	{
		pix_out = __cnc(is_color, img->at(y, x), avgG, avgC1, avgC2, r_gain, gr_gain, gb_gain, b_gain);
	}
	else
	{
		pix_out = img->at(y, x);
	}

	return pix_out;
}





void CNF(ImageRaw& img, BAYER_PATTERN bayer_pattern, float threshold, float r_gain, float gr_gain, float gb_gain, float b_gain, uint16_t clip)
{
	ImageRaw* img_pad = new ImageRaw(img);
	img_pad->padding(4, PADDING_MODE_REFLECT);

	uint16_t r, gr, gb, b;

	for (int y = 0; y < img_pad->getHeight() - 8 - 1; y += 2) {
		for (int x = 0; x < img_pad->getWidth() - 8 - 1; x += 2) {
			switch (bayer_pattern) {
			case BAYER_PATTERN_RGGB:

				r = img_pad->at(y + 4, x + 4);
				gr = img_pad->at(y + 4, x + 5);
				gb = img_pad->at(y + 5, x + 4);
				b = img_pad->at(y + 5, x + 5);

				img.at(y, x) = __cnf("r", y + 4, x + 4, img_pad, threshold, r_gain, gr_gain, gb_gain, b_gain);
				img.at(y, x + 1) = gr;
				img.at(y + 1, x) = gb;
				img.at(y + 1, x + 1) = __cnf("b", y + 5, x + 5, img_pad, threshold, r_gain, gr_gain, gb_gain, b_gain);

				break;
			case BAYER_PATTERN_BGGR:

				b = img_pad->at(y + 4, x + 4);
				gb = img_pad->at(y + 4, x + 5);
				gr = img_pad->at(y + 5, x + 4);
				r = img_pad->at(y + 5, x + 5);

				img.at(y, x) = __cnf("b", y + 4, x + 4, img_pad, threshold, r_gain, gr_gain, gb_gain, b_gain);
				img.at(y, x + 1) = gb;
				img.at(y + 1, x) = gr;
				img.at(y + 1, x + 1) = __cnf("r", y + 5, x + 5, img_pad, threshold, r_gain, gr_gain, gb_gain, b_gain);

				break;
			case BAYER_PATTERN_GBRG:

				gb = img_pad->at(y + 4, x + 4);
				b = img_pad->at(y + 4, x + 5);
				r = img_pad->at(y + 5, x + 4);
				gr = img_pad->at(y + 5, x + 5);

				img.at(y, x) = gb;
				img.at(y, x + 1) = __cnf("b", y + 4, x + 5, img_pad, threshold, r_gain, gr_gain, gb_gain, b_gain);
				img.at(y + 1, x) = __cnf("r", y + 5, x + 4, img_pad, threshold, r_gain, gr_gain, gb_gain, b_gain);
				img.at(y + 1, x + 1) = gr;

				break;
			case BAYER_PATTERN_GRBG:

				gr = img_pad->at(y + 4, x + 4);
				r = img_pad->at(y + 4, x + 5);
				b = img_pad->at(y + 5, x + 4);
				gb = img_pad->at(y + 5, x + 5);

				img.at(y, x) = gr;
				img.at(y, x + 1) = __cnf("r", y + 4, x + 5, img_pad, threshold, r_gain, gr_gain, gb_gain, b_gain);
				img.at(y + 1, x) = __cnf("b", y + 5, x + 4, img_pad, threshold, r_gain, gr_gain, gb_gain, b_gain);
				img.at(y + 1, x + 1) = gb;

				break;
			case BAYER_PATTERN_UNKNOWN:
			default:
				TRACE_DEBUG_LOG_ERROR("Unknown Bayer Pattern:%s\n", bayer_pattern);
				break;

			}
		}
	}

	img.clip(0, clip);
	delete img_pad;
}