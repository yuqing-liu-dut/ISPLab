#define _CRT_SECURE_NO_WARNINGS

#include "Config.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


Config::Config()
{
	
}

Config::Config(const char* filepath)
{
	FILE* fp = fopen(filepath, "r");
	char row[512];
	char* key;
	char* val;

	while (fgets(row, 512, fp) != NULL)
	{
		printf("Rows:%s", row);
		key = strtok(row, ",");
		val = strtok(NULL, ",");

		if (key == NULL || val == NULL) { throw "Wrong config file"; }

		// RAW settings
		if (strcmp(key, "bayer_pattern") == 0)
		{
			switch (atoi(val))
			{
			case 0: this->bayer_pattern = BAYER_PATTERN_RGGB; break;
			case 1: this->bayer_pattern = BAYER_PATTERN_BGGR; break;
			case 2: this->bayer_pattern = BAYER_PATTERN_GBRG; break;
			case 3: this->bayer_pattern = BAYER_PATTERN_GRBG; break;
			default:
				this->bayer_pattern = BAYER_PATTERN_UNKNOWN; break;
			}
		}

		// DPC
		if (strcmp(key, "dpc_threshold") == 0) { this->dpc_threshold = atof(val); }
		if (strcmp(key, "dpc_mode") == 0)
		{
			switch (atoi(val))
			{
			case 0: this->dpc_mode = DPC_MODE_MEAN; break;
			case 1: this->dpc_mode = DPC_MODE_GRADIENT; break;
			default:
				this->dpc_mode = DPC_MODE_UNKNOWN; break;
			}
		}
		if (strcmp(key, "dpc_clip") == 0) { this->dpc_clip = atof(val); }

		// BLC
		if (strcmp(key, "blc_r") == 0) { this->blc_r = atof(val); }
		if (strcmp(key, "blc_gr") == 0) { this->blc_gr = atof(val); }
		if (strcmp(key, "blc_gb") == 0) { this->blc_gb = atof(val); }
		if (strcmp(key, "blc_b") == 0) { this->blc_b = atof(val); }
		if (strcmp(key, "blc_alpha") == 0) { this->blc_alpha = atof(val); }
		if (strcmp(key, "blc_beta") == 0) { this->blc_beta = atof(val); }
		if (strcmp(key, "blc_clip") == 0) { this->blc_clip = atof(val); }

		// LSC 
		if (strcmp(key, "lsc_intensity") == 0) { this->lsc_intensity = atof(val); }
		if (strcmp(key, "lsc_minr") == 0) { this->lsc_minr = atof(val); }
		if (strcmp(key, "lsc_maxr") == 0) { this->lsc_maxr = atof(val); }
		if (strcmp(key, "lsc_clip") == 0) { this->lsc_clip = atof(val); }

		// NRraw

		// AAf
		
		// AWB
		if (strcmp(key, "awb_r_gain") == 0) { this->awb_r_gain = atof(val); }
		if (strcmp(key, "awb_gr_gain") == 0) { this->awb_gr_gain = atof(val); }
		if (strcmp(key, "awb_gb_gain") == 0) { this->awb_gb_gain = atof(val); }
		if (strcmp(key, "awb_b_gain") == 0) { this->awb_b_gain = atof(val); }
		if (strcmp(key, "awb_clip") == 0) { this->awb_clip = atof(val); }

		// Bit convert
		if (strcmp(key, "bitcvt_bit") == 0) { this->bitcvt_bit = atof(val); }

		// CNF
		if (strcmp(key, "cnf_threshold") == 0) { this->cnf_threshold = atof(val); }
		if (strcmp(key, "cnf_r_gain") == 0) { this->cnf_r_gain = atof(val); }
		if (strcmp(key, "cnf_gr_gain") == 0) { this->cnf_gr_gain = atof(val); }
		if (strcmp(key, "cnf_gb_gain") == 0) { this->cnf_gb_gain = atof(val); }
		if (strcmp(key, "cnf_b_gain") == 0) { this->cnf_b_gain = atof(val); }
		if (strcmp(key, "cnf_clip") == 0) { this->cnf_clip = atof(val); }

		// CFA
		if (strcmp(key, "cfa_mode") == 0)
		{
			switch (atoi(val))
			{
			case 0: this->cfa_mode = DEMOSAIC_MALVAR; break;
			default:
				this->cfa_mode = DEMOSAIC_UNKNOWN; break;
			}
		}
		if (strcmp(key, "cfa_clip") == 0) { this->cfa_clip = atof(val); }

		// CCM
		if (strcmp(key, "ccm_param_0_0") == 0) { this->ccm_param[0][0] = atof(val); }
		if (strcmp(key, "ccm_param_0_1") == 0) { this->ccm_param[0][1] = atof(val); }
		if (strcmp(key, "ccm_param_0_2") == 0) { this->ccm_param[0][2] = atof(val); }
		if (strcmp(key, "ccm_param_0_3") == 0) { this->ccm_param[0][3] = atof(val); }
		if (strcmp(key, "ccm_param_1_0") == 0) { this->ccm_param[1][0] = atof(val); }
		if (strcmp(key, "ccm_param_1_1") == 0) { this->ccm_param[1][1] = atof(val); }
		if (strcmp(key, "ccm_param_1_2") == 0) { this->ccm_param[1][2] = atof(val); }
		if (strcmp(key, "ccm_param_1_3") == 0) { this->ccm_param[1][3] = atof(val); }
		if (strcmp(key, "ccm_param_2_0") == 0) { this->ccm_param[2][0] = atof(val); }
		if (strcmp(key, "ccm_param_2_1") == 0) { this->ccm_param[2][1] = atof(val); }
		if (strcmp(key, "ccm_param_2_2") == 0) { this->ccm_param[2][2] = atof(val); }
		if (strcmp(key, "ccm_param_2_3") == 0) { this->ccm_param[2][3] = atof(val); }
		
		// GC
		if (strcmp(key, "gc_bw") == 0) { this->gc_bw = atof(val); }
		if (strcmp(key, "gc_gamma") == 0) { this->gc_gamma = atof(val); }

		// CSC
		
		// NR parameters
		if (strcmp(key, "nr_chroma_d") == 0) { this->nr_chroma_d = atof(val); }
		if (strcmp(key, "nr_luma_d") == 0) { this->nr_luma_d = atof(val); }

		// EE parameters
		if (strcmp(key, "ee_matrix_0_0") == 0) { this->ee_matrix[0][0] = atof(val); }
		if (strcmp(key, "ee_matrix_0_1") == 0) { this->ee_matrix[0][1] = atof(val); }
		if (strcmp(key, "ee_matrix_0_2") == 0) { this->ee_matrix[0][2] = atof(val); }
		if (strcmp(key, "ee_matrix_1_0") == 0) { this->ee_matrix[1][0] = atof(val); }
		if (strcmp(key, "ee_matrix_1_1") == 0) { this->ee_matrix[1][1] = atof(val); }
		if (strcmp(key, "ee_matrix_1_2") == 0) { this->ee_matrix[1][2] = atof(val); }
		if (strcmp(key, "ee_matrix_2_0") == 0) { this->ee_matrix[2][0] = atof(val); }
		if (strcmp(key, "ee_matrix_2_1") == 0) { this->ee_matrix[2][1] = atof(val); }
		if (strcmp(key, "ee_matrix_2_2") == 0) { this->ee_matrix[2][2] = atof(val); }
		if (strcmp(key, "ee_gain") == 0) { this->ee_gain = atof(val); }

		// HSC parameters
		if (strcmp(key, "hsc_hue") == 0) { this->hsc_hue = atof(val); }
		if (strcmp(key, "hsc_saturation") == 0) { this->hsc_saturation = atof(val); }

		// BCC parameters
		if (strcmp(key, "bcc_brightness") == 0) { this->bcc_brightness = atof(val); }
		if (strcmp(key, "bcc_contrast") == 0) { this->bcc_contrast = atof(val); }

		while (val != NULL) { val = strtok(NULL, ","); }
	}
}

