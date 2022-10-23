#include "MyISP.h"
#include <math.h>
#include <string.h>

PixelRGB malvar(const char* is_color, float center, int y, int x, ImageRaw* img, uint8_t cfa_clip)
{
	float r = 0, g = 0, b = 0;
	if (strcmp(is_color, "r") == 0)
	{
		r = center;
		g = 4 * img->at(y, x) - img->at(y - 2, x) - img->at(y, x - 2) - img->at(y + 2, x) - img->at(y, x + 2) +
			2 * (img->at(y + 1, x) + img->at(y, x + 1) + img->at(y - 1, x) + img->at(y, x - 1));
		b = 6 * img->at(y, x) - 3 * (img->at(y - 2, x) + img->at(y, x - 2) + img->at(y + 2, x) + img->at(y, x + 2)) / 2 +
			2 * (img->at(y - 1, x - 1) + img->at(y - 1, x + 1) + img->at(y + 1, x - 1) + img->at(y + 1, x + 1));
		g = g / 8;
		b = b / 8;
	}
	if (strcmp(is_color, "gr") == 0)
	{
		r = 5 * img->at(y, x) - img->at(y, x - 2) - img->at(y - 1, x - 1) - img->at(y + 1, x - 1) - img->at(y - 1, x + 1) - img->at(y + 1, x + 1) - img->at(y, x + 2)
			+ (img->at(y - 2, x) + img->at(y + 2, x)) / 2 + 4 * (img->at(y, x - 1) + img->at(y, x + 1));
		g = center;
		b = 5 * img->at(y, x) - img->at(y - 2, x) - img->at(y - 1, x - 1) - img->at(y - 1, x + 1) - img->at(y + 2, x) - img->at(y + 1, x - 1) - img->at(y + 1, x + 1)
			+ (img->at(y, x - 2) + img->at(y, x + 2)) / 2 + 4 * (img->at(y - 1, x) + img->at(y + 1, x));
		r = r / 8;
		b = b / 8;
	}
	if (strcmp(is_color, "gb") == 0)
	{
		r = 5 * img->at(y, x) - img->at(y - 2, x) - img->at(y - 1, x - 1) - img->at(y - 1, x + 1) - img->at(y + 2, x) - img->at(y + 1, x - 1) - img->at(y + 1, x + 1)
			+ (img->at(y, x - 2) + img->at(y, x + 2)) / 2 + 4 * (img->at(y - 1, x) + img->at(y + 1, x));
		g = center;
		b = 5 * img->at(y, x) - img->at(y, x - 2) - img->at(y - 1, x - 1) - img->at(y + 1, x - 1) - img->at(y - 1, x + 1) - img->at(y + 1, x + 1) - img->at(y, x + 2)
			+ (img->at(y - 2, x) + img->at(y + 2, x)) / 2 + 4 * (img->at(y, x - 1) + img->at(y, x + 1));
		r = r / 8;
		b = b / 8;
	}
	if (strcmp(is_color, "b") == 0)
	{
		r = 6 * img->at(y, x) - 3 * (img->at(y - 2, x) + img->at(y, x - 2) + img->at(y + 2, x) + img->at(y, x + 2)) / 2
			+ 2 * (img->at(y - 1, x - 1) + img->at(y - 1, x + 1) + img->at(y + 1, x - 1) + img->at(y + 1, x + 1));
		g = 4 * img->at(y, x) - img->at(y - 2, x) - img->at(y, x - 2) - img->at(y + 2, x) - img->at(y, x + 2)
			+ 2 * (img->at(y + 1, x) + img->at(y, x + 1) + img->at(y - 1, x) + img->at(y, x - 1));
		b = center;
		r = r / 8;
		g = g / 8;
	}
	PixelRGB pixel;
	if (r < 0)r = 0;
	if (g < 0)g = 0;
	if (b < 0)b = 0;
	if (r > cfa_clip)r = cfa_clip;
	if (g > cfa_clip)g = cfa_clip;
	if (b > cfa_clip)b = cfa_clip;
	pixel.R = r;
	pixel.G = g;
	pixel.B = b;
	return pixel;
}

ImageRGB CFA(ImageRaw& img, DEMOSAICING_MODE cfa_mode, BAYER_PATTERN bayer_pattern, uint8_t cfa_clip)
{
	ImageRaw* img_pad = new ImageRaw(img);
	img_pad->padding(2, PADDING_MODE_REFLECT);

	int raw_h = img.getHeight(), raw_w = img.getWidth();

	ImageRGB cfa_img(raw_h, raw_w);

	float r, gr, gb, b;

	for (int y = 0; y < img_pad->getHeight() - 4 - 1; y += 2)
	{
		for (int x = 0; x < img_pad->getWidth() - 4 - 1; x += 2)
		{
			switch (bayer_pattern)
			{
			case BAYER_PATTERN_RGGB:
				r = img_pad->at(y + 2, x + 2);
				gr = img_pad->at(y + 2, x + 3);
				gb = img_pad->at(y + 3, x + 2);
				b = img_pad->at(y + 3, x + 3);
				switch (cfa_mode)
				{
				case DEMOSAIC_MALVAR:
					cfa_img.at(y, x) = malvar("r", r, y + 2, x + 2, img_pad, cfa_clip);
					cfa_img.at(y, x + 1) = malvar("gr", gr, y + 2, x + 3, img_pad, cfa_clip);
					cfa_img.at(y + 1, x) = malvar("gb", gb, y + 3, x + 2, img_pad, cfa_clip);
					cfa_img.at(y + 1, x + 1) = malvar("b", b, y + 3, x + 3, img_pad, cfa_clip);
					break;
				case DEMOSAIC_UNKNOWN:
				default:
					TRACE_DEBUG_LOG_ERROR("Unknown Pattern:%s\n", cfa_mode);
					throw "Unimplemented mode";
					break;
				}
				break;
			case BAYER_PATTERN_BGGR:
				b = img_pad->at(y + 2, x + 2);
				gb = img_pad->at(y + 2, x + 3);
				gr = img_pad->at(y + 3, x + 2);
				r = img_pad->at(y + 3, x + 3);
				switch (cfa_mode)
				{
				case DEMOSAIC_MALVAR:
					cfa_img.at(y, x) = malvar("b", b, y + 2, x + 2, img_pad, cfa_clip);
					cfa_img.at(y, x + 1) = malvar("gb", gb, y + 2, x + 3, img_pad, cfa_clip);
					cfa_img.at(y + 1, x) = malvar("gr", gr, y + 3, x + 2, img_pad, cfa_clip);
					cfa_img.at(y + 1, x + 1) = malvar("r", r, y + 3, x + 3, img_pad, cfa_clip);
					break;
				case DEMOSAIC_UNKNOWN:
				default:
					TRACE_DEBUG_LOG_ERROR("Unknown Pattern:%s\n", cfa_mode);
					throw "Unimplemented mode";
					break;
				}
				break;
			case BAYER_PATTERN_GBRG:
				gb = img_pad->at(y + 2, x + 2);
				b = img_pad->at(y + 2, x + 3);
				r = img_pad->at(y + 3, x + 2);
				gr = img_pad->at(y + 3, x + 3);
				switch (cfa_mode)
				{
				case DEMOSAIC_MALVAR:
					cfa_img.at(y, x) = malvar("gb", gb, y + 2, x + 2, img_pad, cfa_clip);
					cfa_img.at(y, x + 1) = malvar("b", b, y + 2, x + 3, img_pad, cfa_clip);
					cfa_img.at(y + 1, x) = malvar("r", r, y + 3, x + 2, img_pad, cfa_clip);
					cfa_img.at(y + 1, x + 1) = malvar("gr", gr, y + 3, x + 3, img_pad, cfa_clip);
					break;
				case DEMOSAIC_UNKNOWN:
				default:
					TRACE_DEBUG_LOG_ERROR("Unknown Pattern:%s\n", cfa_mode);
					throw "Unimplemented mode";
					break;
				}
				break;
			case BAYER_PATTERN_GRBG:
				gr = img_pad->at(y + 2, x + 2);
				r = img_pad->at(y + 2, x + 3);
				b = img_pad->at(y + 3, x + 2);
				gb = img_pad->at(y + 3, x + 3);
				switch (cfa_mode)
				{
				case DEMOSAIC_MALVAR:
					cfa_img.at(y, x) = malvar("gr", gr, y + 2, x + 2, img_pad, cfa_clip);
					cfa_img.at(y, x + 1) = malvar("r", r, y + 2, x + 3, img_pad, cfa_clip);
					cfa_img.at(y + 1, x) = malvar("b", b, y + 3, x + 2, img_pad, cfa_clip);
					cfa_img.at(y + 1, x + 1) = malvar("gb", gb, y + 3, x + 3, img_pad, cfa_clip);
					break;
				case DEMOSAIC_UNKNOWN:
				default:
					TRACE_DEBUG_LOG_ERROR("Unknown Pattern:%s\n", cfa_mode);
					throw "Unimplemented mode";
					break;
				}
				break;
			case BAYER_PATTERN_UNKNOWN:
			default:
				TRACE_DEBUG_LOG_ERROR("Unknown Pattern:%s\n", bayer_pattern);
				throw "Unimplemented mode";
				break;
			}
		}
	}

	delete img_pad;
	return cfa_img;
}