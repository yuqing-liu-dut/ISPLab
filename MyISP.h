#pragma once
#include "MyType.h"
#include "MyImage.h"
#include "MyUtil.h"

// Raw data processing
// dead pixel correction
void DPC(ImageRaw& raw, uint16_t threshold = 30, DPC_MODE mode = DPC_MODE_GRADIENT, uint16_t clip = 1023);
// black level correction
void BLC(ImageRaw& raw, uint16_t r = 0, uint16_t gr = 0, uint16_t gb = 0, uint16_t b = 0, float alpha = 0, float beta = 0, BAYER_PATTERN bayer_pattern = BAYER_PATTERN_RGGB, uint16_t clip = 1023);
// lens shading correction
void LSC(ImageRaw& raw, uint16_t intensity = 0, int minR = -1, int maxR = -1, uint16_t clip = 1023);
// noise reduction for raw image
void NRraw(ImageRaw& raw);	// unimplemented
// anti-aliasing filter
void AAF(ImageRaw& raw);
// auto white balance
void AWB(ImageRaw& raw, float r_gain = 1.5, float gr_gain = 1.0, float gb_gain = 1.0, float b_gain = 1.1, BAYER_PATTERN bayer_pattern = BAYER_PATTERN_RGGB, uint16_t clip = 1023);

// convert 10-bit raw data to k-bit(k=8)
void BitCvt(ImageRaw& raw, int bit=8);

// chroma noise filtering, this is for 8-bit raw data
void CNF(ImageRaw& raw, BAYER_PATTERN bayer_pattern = BAYER_PATTERN_RGGB, float threshold = 0, float r_gain = 1.5, float gr_gain = 1.0, float gb_gain = 1.0, float b_gain = 1.1, uint16_t clip = 255);


// Demosaicing
ImageRGB CFA(ImageRaw& raw, DEMOSAICING_MODE cfa_mode = DEMOSAIC_MALVAR, BAYER_PATTERN bayer_pattern = BAYER_PATTERN_RGGB, uint8_t cfa_clip = 255);


// RGB data processing
// color correction matrix
void CCM(ImageRGB& rgb, float ccm[][4]);
// gamma correction
void GC(ImageRGB& rgb, int bw = 8, float gamma = 0.5);


// Convert RGB to YUV
ImageYUV CSC(ImageRGB& rgb);
ImageRGB CSC(ImageYUV& yuv);


// YUV processing
// noise reduction
void NRchroma(ImageYUV& yuv, int d = 1); // median filter
void NRluma(ImageYUV& yuv, int d = 1); // median filter
// edge enhancement
void EE(ImageYUV& yuv, float ee[][3], float gain=0.5);
// hue & saturation control
void HSC(ImageYUV& yuv, float hue=0.1, float saturation=1.1);
// brightness & contrast control
void BCC(ImageYUV& yuv, uint8_t brightness=0, float contrast=0.1);



