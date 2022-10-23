#define _CRT_SECURE_NO_WARNINGS

#include "MyISP.h"
#include "Config.h"

#include "MyOpenCV.h"

int main()
{
	int width = 1920, height = 1080;
	uint16_t* src = new uint16_t[width * height];

	Config config("config.csv");

	FILE* fp = fopen("test.RAW", "rb+");
	fread(src, sizeof(uint16_t) * height * width, 1, fp);
	fclose(fp);
	ImageRaw raw(src, height, width);

	DPC(raw, config.dpc_threshold, config.dpc_mode, config.dpc_clip); // dead pixel correction
	BLC(raw, config.blc_r, config.blc_gr, config.blc_gb, config.blc_b, config.blc_alpha, config.blc_beta, config.bayer_pattern, config.blc_clip); // black level correction
	LSC(raw, config.lsc_intensity, config.lsc_minr, config.lsc_maxr, config.lsc_clip); // lens shading correction
	NRraw(raw); // noise reduction for raw image, not implemented
	AAF(raw); // anti-aliasing filter
	AWB(raw, config.awb_r_gain, config.awb_gr_gain, config.awb_gb_gain, config.awb_b_gain, config.bayer_pattern, config.awb_clip); // auto white balance
	BitCvt(raw, config.bitcvt_bit); // convert 10-bit raw data to k-bit(k=8)
	CNF(raw, config.bayer_pattern, config.cnf_threshold, config.cnf_r_gain, config.cnf_gr_gain, config.cnf_gb_gain, config.cnf_b_gain, config.cnf_clip); // chroma noise filtering, this is for 8-bit raw data

	ImageRGB rgb = CFA(raw, config.cfa_mode, config.bayer_pattern, config.cfa_clip); // demosaicing

	CCM(rgb, config.ccm_param); // color correction
	GC(rgb, config.gc_bw, config.gc_gamma); // gamma correction

	fp = fopen("rgb.RAW", "wb+");
	fwrite(rgb.getData(), sizeof(PixelRGB) * height * width, 1, fp);
	fclose(fp);

	ImageYUV yuv = CSC(rgb); // rgb2yuv


	NRchroma(yuv, config.nr_chroma_d);
	NRluma(yuv, config.nr_luma_d);
	EE(yuv, config.ee_matrix, config.ee_gain);
	HSC(yuv, config.hsc_hue, config.hsc_saturation);
	BCC(yuv, config.bcc_brightness, config.bcc_brightness);

	ImageRGB yuv2rgb = CSC(yuv); // yuv2rgb

	fp = fopen("yuv2rgb.RAW", "wb+");
	fwrite(yuv2rgb.getData(), sizeof(PixelRGB) * height * width, 1, fp);
	fclose(fp);

	fp = fopen("yuv.RAW", "wb+");
	fwrite(yuv.getData(), sizeof(PixelYUV) * height * width, 1, fp);
	fclose(fp);

	cv::Mat img = toCvMat(rgb);
	cv::imshow("Image", img);
	cv::waitKey();
}