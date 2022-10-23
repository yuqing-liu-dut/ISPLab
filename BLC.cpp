#include "MyISP.h"

// RAW data processing
// black level correction
void BLC(ImageRaw& raw, uint16_t r, uint16_t gr, uint16_t gb, uint16_t b, float alpha, float beta, BAYER_PATTERN bayer_pattern, uint16_t clip)
{
	switch (bayer_pattern)
	{
	case BAYER_PATTERN_RGGB:
		for (int y = 0; y < raw.getHeight(); y+=2)
		{
			for (int x = 0; x < raw.getWidth(); x+=2)
			{
				raw.at(y, x)			+= r;									// r
				raw.at(y + 1, x + 1)	+= b;									// b
				raw.at(y, x + 1)		+= gr + alpha * raw.at(y, x);			// gr
				raw.at(y + 1, x)		+= gb + beta * raw.at(y + 1, x + 1);	// gb
			}
		}
		break;
	case BAYER_PATTERN_BGGR:
		for (int y = 0; y < raw.getHeight(); y+=2)
		{
			for (int x = 0; x < raw.getWidth(); x+=2)
			{
				raw.at(y, x) += b;
				raw.at(y + 1, x + 1) += r;
				raw.at(y, x + 1) += gb + beta * raw.at(y, x);
				raw.at(y + 1, x) += gr + alpha * raw.at(y + 1, x + 1);
			}
		}
		break;
	case BAYER_PATTERN_GBRG:
		for (int y = 0; y < raw.getHeight(); y+= 2)
		{
			for (int x = 0; x < raw.getWidth(); x+= 2)
			{
				raw.at(y, x + 1) += b;
				raw.at(y + 1, x) += r;
				raw.at(y, x) += gb + beta * raw.at(y, x + 1);
				raw.at(y + 1, x + 1) += gr + alpha * raw.at(y + 1, x);
			}
		}
		break;
	case BAYER_PATTERN_GRBG:
		for (int y = 0; y < raw.getHeight(); y+= 2)
		{
			for (int x = 0; x < raw.getWidth(); x+= 2)
			{
				raw.at(y + 1, x) += b;
				raw.at(y, x + 1) += r;
				raw.at(y + 1, x + 1) += gb + beta * raw.at(y + 1, x);	
				raw.at(y, x) += gr + alpha * raw.at(y, x + 1);
			}
		}
		break;
	case BAYER_PATTERN_UNKNOWN:
	default:
		TRACE_DEBUG_LOG_ERROR("Unknown Bayer Pattern:%s\n", bayer_pattern);
		throw "Unknown bayer pattern";
		break;
	}
	raw.clip(0, clip);
}

