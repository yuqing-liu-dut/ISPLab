#include "MyImage.h"
#include <stdio.h>
#include <string.h>

#define TRACE_DEBUG_LOG_WARNING (printf("[WARNING] %s(%d)-<%s>: ",__FILE__, __LINE__, __FUNCTION__), printf)
#define TRACE_DEBUG_LOG_ERROR (printf("[ERROR] %s(%d)-<%s>: ",__FILE__, __LINE__, __FUNCTION__), printf)


// Raw Image
// data type: uint16_t
// data range: 0 to 1024

ImageRaw::ImageRaw()
{

}

ImageRaw::ImageRaw(const ImageRaw& img)
{
	this->width = img.width;
	this->height = img.height;
	this->data = new uint16_t[this->width * this->height];
	memcpy(this->data, img.data, this->height * this->width * sizeof(uint16_t));
}

ImageRaw::ImageRaw(int height, int width)
{
	this->data = new uint16_t[width * height];
	this->width = width;
	this->height = height;
}

ImageRaw::ImageRaw(uint16_t* src, int height, int width)
{
	this->data = new uint16_t[width * height];
	this->width = width;
	this->height = height;
	memcpy(this->data, src, height * width * sizeof(uint16_t));
}

uint16_t& ImageRaw::at(int h, int w)
{
	return data[width * h + w];
}

void ImageRaw::clip(uint16_t min, uint16_t max)
{
	if (max < min) {
		TRACE_DEBUG_LOG_WARNING("clip max value:%d is smaller than min value:%d\n", max, min);
		return;
	}
	for (int y = 0; y < this->height; y++) {
		for (int x = 0; x < this->width; x++) {
			if (this->at(y, x) > max) this->at(y, x) = max;
			if (this->at(y, x) < min) this->at(y, x) = min;
		}
	}
}

void ImageRaw::padding(int round, PADDING_MODE mode, uint16_t value)
{
	uint16_t* dst = new uint16_t[(this->width + round * 2) * (this->height + round * 2)];
	switch (mode)
	{
	case PADDING_MODE_CONSTANT:
		for (int y = -round; y < this->height + round; y++) {
			for (int x = -round; x < this->width + round; x++) {
				if (y < 0 || y >= this->height || x < 0 || x >= this->width) dst[(y + round) * (this->width + round * 2) + (x + round)] = value;
				else dst[(y + round) * (this->width + round * 2) + (x + round)] = this->at(y, x);
			}
		}
		break;
	case PADDING_MODE_EDGE:
		for (int y = -round; y < this->height + round; y++) {
			for (int x = -round; x < this->width + round; x++) {
				int dst_y = y, dst_x = x;
				if (dst_y < 0) dst_y = 0;
				if (dst_y >= this->height) dst_y = this->height - 1;
				if (dst_x < 0) dst_x = 0;
				if (dst_x >= this->width) dst_x = this->width - 1;
				dst[(y + round) * (this->width + round * 2) + (x + round)] = this->at(dst_y, dst_x);
			}
		}
		break;
	case PADDING_MODE_REFLECT:
		for (int y = -round; y < this->height + round; y++) {
			for (int x = -round; x < this->width + round; x++) {
				int dst_y = y, dst_x = x;
				if (dst_y < 0) dst_y = -dst_y;
				if (dst_y >= this->height) dst_y = this->height * 2 - dst_y - 2;
				if (dst_x < 0) dst_x = -dst_x;
				if (dst_x >= this->width) dst_x = this->width * 2 - dst_x - 2;
				dst[(y + round) * (this->width + round * 2) + (x + round)] = this->at(dst_y, dst_x);
			}
		}
		break;
	default:
		throw "Unknown Padding Mode";
		break;
	}
	this->width = this->width + round * 2;
	this->height = this->height + round * 2;
	delete this->data;
	this->data = dst;
}

const int ImageRaw::getWidth()
{
	return this->width;
}

const int ImageRaw::getHeight()
{
	return this->height;
}

const uint16_t* ImageRaw::getData()
{
	return this->data;
}

ImageRaw::~ImageRaw()
{
	delete this->data;
	this->width = 0;
	this->height = 0;
}

void ImageRaw::print()
{
	for (int y = 0; y < this->getHeight(); y++) {
		for (int x = 0; x < this->getWidth(); x++) {
			printf("%d ", this->at(y, x));
		}
		printf("\n");
	}
}

void ImageRaw::operator=(const ImageRaw& src)
{
	this->data = src.data;
	this->width = src.width;
	this->height = src.height;
}




// RGB image
// data type: uint8_t
// data range: 0 to 255

ImageRGB::ImageRGB()
{

}

ImageRGB::ImageRGB(const ImageRGB& img)
{
	this->width = img.width;
	this->height = img.height;
	this->data = new PixelRGB[this->width * this->height];
	memcpy(this->data, img.data, this->height * this->width * sizeof(PixelRGB));
}

ImageRGB::ImageRGB(int height, int width)
{
	this->data = new PixelRGB[width * height];
	this->width = width;
	this->height = height;
}

ImageRGB::ImageRGB(PixelRGB* src, int height, int width)
{
	this->data = new PixelRGB[width * height];
	this->width = width;
	this->height = height;

	memcpy(this->data, src, height * width * sizeof(PixelRGB));
}

PixelRGB& ImageRGB::at(int h, int w)
{
	return data[width * h + w];
}

void ImageRGB::clip(uint8_t min, uint8_t max)
{
	if (max < min) {
		TRACE_DEBUG_LOG_WARNING("clip max value:%d is smaller than min value:%d\n", max, min);
		return;
	}
	for (int y = 0; y < this->height; y++) {
		for (int x = 0; x < this->width; x++) {
			if (this->at(y, x).R > max) this->at(y, x).R = max;
			if (this->at(y, x).G > max) this->at(y, x).G = max;
			if (this->at(y, x).B > max) this->at(y, x).B = max;

			if (this->at(y, x).R < min) this->at(y, x).R = min;
			if (this->at(y, x).G < min) this->at(y, x).G = min;
			if (this->at(y, x).B < min) this->at(y, x).B = min;
		}
	}
}

void ImageRGB::padding(int round, PADDING_MODE mode, PixelRGB value)
{
	PixelRGB* dst = new PixelRGB[(this->width + round * 2) * (this->height + round * 2)];
	switch (mode)
	{
	case PADDING_MODE_CONSTANT:
		for (int y = -round; y < this->height + round; y++) {
			for (int x = -round; x < this->width + round; x++) {
				if (y < 0 || y >= this->height || x < 0 || x >= this->width) dst[(y + round) * (this->width + round * 2) + (x + round)] = value;
				else dst[(y + round) * (this->width + round * 2) + (x + round)] = this->at(y, x);
			}
		}
		break;
	case PADDING_MODE_EDGE:
		for (int y = -round; y < this->height + round; y++) {
			for (int x = -round; x < this->width + round; x++) {
				int dst_y = y, dst_x = x;
				if (dst_y < 0) dst_y = 0;
				if (dst_y >= this->height) dst_y = this->height - 1;
				if (dst_x < 0) dst_x = 0;
				if (dst_x >= this->width) dst_x = this->width - 1;
				dst[(y + round) * (this->width + round * 2) + (x + round)] = this->at(dst_y, dst_x);
			}
		}
		break;
	case PADDING_MODE_REFLECT:
		for (int y = -round; y < this->height + round; y++) {
			for (int x = -round; x < this->width + round; x++) {
				int dst_y = y, dst_x = x;
				if (dst_y < 0) dst_y = -dst_y;
				if (dst_y >= this->height) dst_y = this->height * 2 - dst_y - 2;
				if (dst_x < 0) dst_x = -dst_x;
				if (dst_x >= this->width) dst_x = this->width * 2 - dst_x - 2;
				dst[(y + round) * (this->width + round * 2) + (x + round)] = this->at(dst_y, dst_x);
			}
		}
		break;
	default:
		throw "Unknown Padding Mode";
		break;
	}
	this->width = this->width + round * 2;
	this->height = this->height + round * 2;
	delete this->data;
	this->data = dst;
}

const int ImageRGB::getWidth()
{
	return this->width;
}

const int ImageRGB::getHeight()
{
	return this->height;
}

const PixelRGB* ImageRGB::getData()
{
	return this->data;
}

ImageRGB::~ImageRGB()
{
	delete this->data;
	this->width = 0;
	this->height = 0;
}

void ImageRGB::print()
{
	for (int y = 0; y < this->getHeight(); y++) {
		for (int x = 0; x < this->getWidth(); x++) {
			printf("(%d,%d,%d) ", this->at(y, x).R, this->at(y, x).G, this->at(y, x).B);
		}
		printf("\n");
	}
}

void ImageRGB::operator=(const ImageRGB& src)
{
	this->data = src.data;
	this->width = src.width;
	this->height = src.height;
}



// YUV image
// data type: uint8_t
// data range: 0 to 255

ImageYUV::ImageYUV()
{

}

ImageYUV::ImageYUV(const ImageYUV& img)
{
	this->width = img.width;
	this->height = img.height;
	this->data = new PixelYUV[this->width * this->height];
	memcpy(this->data, img.data, this->height * this->width * sizeof(PixelYUV));
}

ImageYUV::ImageYUV(int height, int width)
{
	this->data = new PixelYUV[width * height];
	this->width = width;
	this->height = height;
}

ImageYUV::ImageYUV(PixelYUV* src, int height, int width)
{
	this->data = new PixelYUV[width * height];
	this->width = width;
	this->height = height;

	memcpy(this->data, src, height * width * sizeof(PixelYUV));
}

PixelYUV& ImageYUV::at(int h, int w)
{
	return data[width * h + w];
}

void ImageYUV::clip(uint8_t min, uint8_t max)
{
	if (max < min) {
		TRACE_DEBUG_LOG_WARNING("clip max value:%d is smaller than min value:%d\n", max, min);
		return;
	}
	for (int y = 0; y < this->height; y++) {
		for (int x = 0; x < this->width; x++) {
			if (this->at(y, x).Y > max) this->at(y, x).Y = max;
			if (this->at(y, x).U > max) this->at(y, x).U = max;
			if (this->at(y, x).V > max) this->at(y, x).V = max;

			if (this->at(y, x).Y < min) this->at(y, x).Y = min;
			if (this->at(y, x).U < min) this->at(y, x).U = min;
			if (this->at(y, x).V < min) this->at(y, x).V = min;
		}
	}
}

void ImageYUV::padding(int round, PADDING_MODE mode, PixelYUV value)
{
	PixelYUV* dst = new PixelYUV[(this->width + round * 2) * (this->height + round * 2)];
	switch (mode)
	{
	case PADDING_MODE_CONSTANT:
		for (int y = -round; y < this->height + round; y++) {
			for (int x = -round; x < this->width + round; x++) {
				if (y < 0 || y >= this->height || x < 0 || x >= this->width) dst[(y + round) * (this->width + round * 2) + (x + round)] = value;
				else dst[(y + round) * (this->width + round * 2) + (x + round)] = this->at(y, x);
			}
		}
		break;
	case PADDING_MODE_EDGE:
		for (int y = -round; y < this->height + round; y++) {
			for (int x = -round; x < this->width + round; x++) {
				int dst_y = y, dst_x = x;
				if (dst_y < 0) dst_y = 0;
				if (dst_y >= this->height) dst_y = this->height - 1;
				if (dst_x < 0) dst_x = 0;
				if (dst_x >= this->width) dst_x = this->width - 1;
				dst[(y + round) * (this->width + round * 2) + (x + round)] = this->at(dst_y, dst_x);
			}
		}
		break;
	case PADDING_MODE_REFLECT:
		for (int y = -round; y < this->height + round; y++) {
			for (int x = -round; x < this->width + round; x++) {
				int dst_y = y, dst_x = x;
				if (dst_y < 0) dst_y = -dst_y;
				if (dst_y >= this->height) dst_y = this->height * 2 - dst_y - 2;
				if (dst_x < 0) dst_x = -dst_x;
				if (dst_x >= this->width) dst_x = this->width * 2 - dst_x - 2;
				dst[(y + round) * (this->width + round * 2) + (x + round)] = this->at(dst_y, dst_x);
			}
		}
		break;
	default:
		throw "Unknown Padding Mode";
		break;
	}
	this->width = this->width + round * 2;
	this->height = this->height + round * 2;
	delete this->data;
	this->data = dst;
}

const int ImageYUV::getWidth()
{
	return this->width;
}

const int ImageYUV::getHeight()
{
	return this->height;
}

const PixelYUV* ImageYUV::getData()
{
	return this->data;
}

ImageYUV::~ImageYUV()
{
	delete this->data;
	this->width = 0;
	this->height = 0;
}

void ImageYUV::print()
{
	for (int y = 0; y < this->getHeight(); y++) {
		for (int x = 0; x < this->getWidth(); x++) {
			printf("(%d,%d,%d) ", this->at(y, x).Y, this->at(y, x).U, this->at(y, x).V);
		}
		printf("\n");
	}
}

void ImageYUV::operator=(const ImageYUV& src)
{
	this->data = src.data;
	this->width = src.width;
	this->height = src.height;
}
