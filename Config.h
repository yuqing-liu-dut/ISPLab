#pragma once
#include "MyType.h"

class Config {
public:
	Config();
	Config(const char* filepath);

	// RAW settings
	BAYER_PATTERN	bayer_pattern = BAYER_PATTERN_RGGB;

	// DPC parameter
	float		dpc_threshold = 30;
	DPC_MODE	dpc_mode = DPC_MODE_GRADIENT;
	float		dpc_clip = 1023;

	// BLC parameter
	float		blc_r = 0;
	float		blc_gr = 0;
	float		blc_gb = 0;
	float		blc_b = 0;
	float		blc_alpha = 0;
	float		blc_beta = 0;
	float		blc_clip = 1023;

	// LSC parameter
	float lsc_intensity = 0;
	float lsc_minr = -1;
	float lsc_maxr = -1;
	float lsc_clip = 1023;

	// NRraw parameter

	// AAF parameter

	// AWB parameter
	float		awb_r_gain = 1.5;
	float		awb_gr_gain = 1.0;
	float		awb_gb_gain = 1.0;
	float		awb_b_gain = 1.1;
	float		awb_clip = 1023;

	// Bit convert parameter
	float		bitcvt_bit = 8;

	// CNF parameter
	float		cnf_threshold = 0;
	float		cnf_r_gain = 1.5;
	float		cnf_gr_gain = 1.0;
	float		cnf_gb_gain = 1.0;
	float		cnf_b_gain = 1.1;
	float		cnf_clip = 255;

	// Demosaicing
	// CFA parameter
	DEMOSAICING_MODE	cfa_mode = DEMOSAIC_MALVAR;
	float				cfa_clip = 255;

	// RGB settings
	// CCM parameter
	float ccm_param[3][4] = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0}
	};

	// GC parameter
	float		gc_bw = 8;
	float		gc_gamma = 0.5;

	// CSC parameter
	// (build-in)
	/*		
	float		csc_matrix[3][4] = {
		{ 0.257,  0.504,  0.098, 16},
		{-0.148, -0.291,  0.439, 128},
		{ 0.439, -0.368, -0.071, 128}
	};
	*/

	// YUV settings

	// NR parameters
	float nr_chroma_d = 1;
	float nr_luma_d = 1;

	// EE parameters
	float ee_matrix[3][3] = {
		{ 0, -1, 0},
		{-1, 4, -1},
		{ 0, -1, 0}
	};
	float ee_gain = 0.5;

	// HSC parameters
	float hsc_hue = 0.1;
	float hsc_saturation = 1.1;

	// BCC parameters
	float bcc_brightness = 0;
	float bcc_contrast = 0.1;
};