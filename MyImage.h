#pragma once
#include <stdint.h>

enum PADDING_MODE {
	PADDING_MODE_CONSTANT,
	PADDING_MODE_EDGE,
	PADDING_MODE_REFLECT
};


struct PixelRGB
{
	uint8_t B = 0;
	uint8_t G = 0;
	uint8_t R = 0;
};

struct PixelYUV
{
	uint8_t Y = 0;
	uint8_t U = 0;
	uint8_t V = 0;
};


class ImageRaw
{
private:
	uint16_t* data = nullptr;
	int width = 0;
	int height = 0;
	
public:
	ImageRaw();
	ImageRaw(int height, int width);
	ImageRaw(uint16_t* src, int height, int width);
	ImageRaw(const ImageRaw& img);

	~ImageRaw();

	uint16_t& at(int h, int w);

	void clip(uint16_t min = 0, uint16_t max = 1024);
	void padding(int round, PADDING_MODE mode = PADDING_MODE_CONSTANT, uint16_t value = 0);

	const int getHeight();
	const int getWidth();
	const uint16_t* getData();

	void print();
	void operator=(const ImageRaw& src);
};


class ImageRGB
{
private:
	PixelRGB* data = nullptr;
	int width = 0;
	int height = 0;

public:
	ImageRGB();
	ImageRGB(int height, int width);
	ImageRGB(PixelRGB* src, int height, int width);
	ImageRGB(const ImageRGB& img);

	~ImageRGB();

	PixelRGB& at(int h, int w);

	void clip(uint8_t min = 0, uint8_t max = 255);
	void padding(int round, PADDING_MODE mode = PADDING_MODE_CONSTANT, PixelRGB value = PixelRGB());

	const int getHeight();
	const int getWidth();
	const PixelRGB* getData();

	void print();
	void operator=(const ImageRGB& src);
};


class ImageYUV
{
private:
	PixelYUV* data = nullptr;
	int width = 0;
	int height = 0;

public:
	ImageYUV();
	ImageYUV(int height, int width);
	ImageYUV(PixelYUV* src, int height, int width);
	ImageYUV(const ImageYUV& img);

	~ImageYUV();

	PixelYUV& at(int h, int w);

	void clip(uint8_t min = 0, uint8_t max = 255);
	void padding(int round, PADDING_MODE mode = PADDING_MODE_CONSTANT, PixelYUV value = PixelYUV());

	const int getHeight();
	const int getWidth();
	const PixelYUV* getData();

	void print();
	void operator=(const ImageYUV& src);
};