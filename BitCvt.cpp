#include "MyISP.h"
#include <math.h>

// RAW data processing
// 10bit to 8bit
void BitCvt(ImageRaw& raw, int bit)
{
	int maxval = 1 << bit;
	for (int y = 0; y < raw.getHeight(); y++)
	{
		for (int x = 0; x < raw.getWidth(); x++)
		{
			raw.at(y, x) = raw.at(y, x) * maxval / 1024;
		}
	}
}

