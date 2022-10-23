#include "MyISP.h"
#include <math.h>

// RAW data processing
// lens shading correction
void LSC(ImageRaw& raw, uint16_t intensity, int minR, int maxR, uint16_t clip)
{
	if (minR < 0) minR = 0;
	if (maxR < 0) maxR = sqrt(pow(raw.getHeight() / 2, 2) + pow(raw.getWidth() / 2, 2));
	for (int y = 0; y < raw.getHeight(); y++)
	{
		for (int x = 0; x < raw.getWidth(); x++)
		{
			int r = sqrt(pow((y - raw.getHeight() / 2), 2) + pow((x - raw.getWidth() / 2), 2));
			float factor = (r - minR) / (maxR - minR);
			raw.at(y, x) = raw.at(y, x) * (1 + intensity * (factor + 0.5));
		}
	}
	raw.clip(0, clip);
}

