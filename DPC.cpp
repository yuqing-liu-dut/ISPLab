#include "MyISP.h"

// RAW data processing
// dead pixel correction
void DPC(ImageRaw& raw, uint16_t thres, DPC_MODE mode, uint16_t clip)
{
	ImageRaw* raw_pad = new ImageRaw(raw);
	raw_pad->padding(2, PADDING_MODE_REFLECT);

	for (int y = 0; y < raw_pad->getHeight() - 4; y++)
	{
		for (int x = 0; x < raw_pad->getWidth() - 4; x++)
		{
			uint16_t p0, p1, p2, p3, p4, p5, p6, p7, p8;
			uint16_t dv, dh, ddl, ddr, minimal;

			p0 = raw_pad->at(y + 2, x + 2);
			p1 = raw_pad->at(y, x);
			p2 = raw_pad->at(y, x + 2);
			p3 = raw_pad->at(y, x + 4);
			p4 = raw_pad->at(y + 2, x);
			p5 = raw_pad->at(y + 2, x + 4);
			p6 = raw_pad->at(y + 4, x);
			p7 = raw_pad->at(y + 4, x + 2);
			p8 = raw_pad->at(y + 4, x + 4);
			if ((ABS(p1 - p0) > thres) && (ABS(p2 - p0) > thres) && (ABS(p3 - p0) > thres) && (ABS(p4 - p0) > thres) && 
				(ABS(p5 - p0) > thres) && (ABS(p6 - p0) > thres) && (ABS(p7 - p0) > thres) && (ABS(p8 - p0) > thres))
			{
				switch (mode)
				{
				case DPC_MODE_MEAN:
					p0 = (p2 + p4 + p5 + p7) / 4;
					break;
				case DPC_MODE_GRADIENT:
					dv			= ABS(2 * p0 - p2 - p7);
					dh			= ABS(2 * p0 - p4 - p5);
					ddl		= ABS(2 * p0 - p1 - p8);
					ddr		= ABS(2 * p0 - p3 - p6);
					minimal	= MIN(MIN(MIN(dv, dh), ddl), ddr);
					if (minimal == dv)			p0 = (p2 + p7 + 1) / 2;
					else if (minimal == dh)		p0 = (p4 + p5 + 1) / 2;
					else if (minimal == ddl)	p0 = (p1 + p8 + 1) / 2;
					else						p0 = (p3 + p6 + 1) / 2;
					break;
				case DPC_MODE_UNKNOWN:
					TRACE_DEBUG_LOG_ERROR("Unknown DPC Mode:%s\n", mode);
					throw "Unknown DPC mode";
					break;
				default:
					TRACE_DEBUG_LOG_ERROR("Unknown DPC Mode:%s\n", mode);
					throw "Unknown DPC mode";
					break;
				}
				raw.at(y, x) = p0;
			}
		}
	}
	raw.clip(0, clip);
	delete raw_pad;
}
