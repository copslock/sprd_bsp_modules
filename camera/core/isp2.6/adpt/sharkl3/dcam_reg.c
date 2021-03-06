/*
 * Copyright (C) 2015 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/printk.h>
#include <sprd_mm.h>

#include "dcam_reg.h"
#include "dcam_interface.h"


unsigned long g_dcam_regbase[DCAM_ID_MAX];
unsigned long g_dcam_aximbase;
unsigned long g_dcam_mmubase;

struct reg_add_val_tag {
	unsigned int addr;
	unsigned int valid;
	unsigned int dvalue;
	unsigned int rw;
	unsigned int wc; /* write clean */
};

/* Dcam0 register, reg data table */
static struct reg_add_val_tag dcam0_reg_val[] = {

	{ISP_BPC_PARAM, 0xC01F0FFF, 0x001E048F, 0xC01F0FFF, 0xC01F0FFF},
	{ISP_BPC_MAP_CTRL, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF},
	{ISP_BPC_BAD_PIXEL_TH0, 0x3FFFFFFF, 0x00002BFF, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_BPC_BAD_PIXEL_TH1, 0x3FFFFFFF, 0x03C0F3FF, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_BPC_BAD_PIXEL_TH2, 0x3FFFFFFF, 0x0781E3FF, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_BPC_BAD_PIXEL_TH3, 0x3FFFFFFF, 0x0C84B3FF, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_BPC_FLAT_TH, 0xFFFFFFFF, 0x78964078, 0xFFFFFFFF, 0xFFFFFFFF},
	{ISP_BPC_EDGE_RATIO, 0x01FF01FF, 0x0028001E, 0x01FF01FF, 0x01FF01FF},
	{ISP_BPC_BAD_PIXEL_PARAM, 0x0707FFFF, 0x02020000, 0x0707FFFF, 0x0707FFFF},
	{ISP_BPC_BAD_PIXEL_COEFF, 0x001F001F, 0x00000000, 0x001F001F, 0x001F001F},
	{ISP_BPC_LUTWORD0, 0x3FFFFFFF, 0x001FFC00, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_BPC_LUTWORD1, 0x3FFFFFFF, 0x00C3B804, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_BPC_LUTWORD2, 0x3FFFFFFF, 0x02D1A40A, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_BPC_LUTWORD3, 0x3FFFFFFF, 0x07A0F012, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_BPC_LUTWORD4, 0x3FFFFFFF, 0x10009C1C, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_BPC_LUTWORD5, 0x3FFFFFFF, 0x1BB07426, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_BPC_LUTWORD6, 0x3FFFFFFF, 0x2CD05832, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_BPC_LUTWORD7, 0x3FFFFFFF, 0x3200483D, 0x3FFFFFFF, 0x3FFFFFFF},
	{DCAM_BLC_PARA_R_B, 0x000FFFFF, 0x00010040, 0x000FFFFF, 0x000FFFFF},
	{DCAM_BLC_PARA_G, 0x000FFFFF, 0x00010040, 0x000FFFFF, 0x000FFFFF},
	{ISP_RGBG_RB, 0xFFFFFFFF, 0x10001000, 0xFFFFFFFF, 0xFFFFFFFF},
	{ISP_RGBG_G, 0xFFFFFFFF, 0x10001000, 0xFFFFFFFF, 0xFFFFFFFF},
	{ISP_RGBG_YRANDOM_PARAMETER0, 0xFFFFFF07, 0x00000103, 0xFFFFFF07, 0xFFFFFF07},
	{ISP_RGBG_YRANDOM_PARAMETER1, 0x07FF007F, 0x00000060, 0x07FF007F, 0x07FF007F},
	{ISP_RGBG_YRANDOM_PARAMETER2, 0xFFFFFFFF, 0x11111111, 0xFFFFFFFF, 0xFFFFFFFF},
	{DCAM_AEM_OFFSET, 0x1FFF1FFF, 0x00000000, 0x1FFF1FFF, 0x1FFF1FFF},
	{DCAM_AEM_BLK_NUM, 0x0000FFFF, 0x00002020, 0x0000FFFF, 0x0000FFFF},
	{DCAM_AEM_BLK_SIZE, 0x0000FFFF, 0x00001414, 0x0000FFFF, 0x0000FFFF},
	{DCAM_AEM_RED_THR, 0x03FF03FF, 0x005A0109, 0x03FF03FF, 0x03FF03FF},
	{DCAM_AEM_BLUE_THR, 0x03FF03FF, 0x005A0109, 0x03FF03FF, 0x03FF03FF},
	{DCAM_AEM_GREEN_THR, 0x03FF03FF, 0x005A016D, 0x03FF03FF, 0x03FF03FF},
	{ISP_AFL_PARAM0, 0x000000F3, 0x00000021, 0x000000F3, 0x000000F3},
	{ISP_AFL_PARAM1, 0x00FFFFFF, 0x00100000, 0x00FFFFFF, 0x00FFFFFF},
	{ISP_AFL_PARAM2, 0x00FFFFFF, 0x00100000, 0x00FFFFFF, 0x00FFFFFF},
	{ISP_AFL_COL_POS, 0x1FFF1FFF, 0x02800000, 0x1FFF1FFF, 0x1FFF1FFF},
	{ISP_AFL_REGION0, 0x00FFFFFF, 0x00100000, 0x00FFFFFF, 0x00FFFFFF},
	{ISP_AFL_REGION1, 0x00FFFFFF, 0x00100000, 0x00FFFFFF, 0x00FFFFFF},
	{ISP_AFL_REGION2, 0x1FFF1FFF, 0x02800000, 0x1FFF1FFF, 0x1FFF1FFF},
	{ISP_AFL_SUM1, 0x000FFFFF, 0x00000000, 0x000FFFFF, 0x000FFFFF},
	{ISP_AFL_SUM2, 0x000FFFFF, 0x00000000, 0x000FFFFF, 0x000FFFFF},
	{DCAM_CROP0_START, 0x9FFF1FFF, 0x03C00000, 0x9FFF1FFF, 0x9FFF1FFF},
	{DCAM_CROP0_X, 0x1FFF1FFF, 0x03C00000, 0x1FFF1FFF, 0x1FFF1FFF},
	{ISP_AWBC_GAIN0, 0x3FFF3FFF, 0x061B085B, 0x3FFF3FFF, 0x3FFF3FFF},
	{ISP_AWBC_GAIN1, 0x3FFF3FFF, 0x04000400, 0x3FFF3FFF, 0x3FFF3FFF},
	{ISP_AWBC_THRD, 0x3FFFFFFF, 0x3FCFF3FC, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_AWBC_OFFSET0, 0x07FF07FF, 0x00000000, 0x07FF07FF, 0x07FF07FF},
	{ISP_AWBC_OFFSET1, 0x07FF07FF, 0x00000000, 0x07FF07FF, 0x07FF07FF},
	{ISP_AFM_PARAMETERS, 0x0001FF37, 0x00000004, 0x0001FF37, 0x0001FF37},
	{ISP_AFM_ENHANCE_CTRL, 0x000077FF, 0x00000028, 0x000077FF, 0x000077FF},
	{ISP_AFM_CROP_START, 0x1FFF1FFF, 0x00000000, 0x1FFF1FFF, 0x1FFF1FFF},
	{ISP_AFM_CROP_SIZE, 0x1FFF1FFF, 0x03C00500, 0x1FFF1FFF, 0x1FFF1FFF},
	{ISP_AFM_WIN_RANGE0S, 0x1FFF1FFF, 0x00040004, 0x1FFF1FFF, 0x1FFF1FFF},
	{ISP_AFM_WIN_RANGE0E, 0x07FF07FF, 0x012C00F0, 0x07FF07FF, 0x07FF07FF},
	{ISP_AFM_WIN_RANGE1S, 0x000F001F, 0x00030004, 0x000F001F, 0x000F001F},
	{ISP_AFM_IIR_FILTER0, 0x0FFF0FFF, 0x012C017A, 0x0FFF0FFF, 0x0FFF0FFF},
	{ISP_AFM_IIR_FILTER1, 0x0FFF0FFF, 0x0EBC0D5C, 0x0FFF0FFF, 0x0FFF0FFF},
	{ISP_AFM_IIR_FILTER2, 0x0FFF0FFF, 0x04000200, 0x0FFF0FFF, 0x0FFF0FFF},
	{ISP_AFM_IIR_FILTER3, 0x0FFF0FFF, 0x0DE70200, 0x0FFF0FFF, 0x0FFF0FFF},
	{ISP_AFM_IIR_FILTER4, 0x0FFF0FFF, 0x02000F68, 0x0FFF0FFF, 0x0FFF0FFF},
	{ISP_AFM_IIR_FILTER5, 0x0FFF0FFF, 0x02000400, 0x0FFF0FFF, 0x0FFF0FFF},
	{ISP_AFM_ENHANCE_FV0_THD, 0xFFFFFFFF, 0x1FFFF000, 0xFFFFFFFF, 0xFFFFFFFF},
	{ISP_AFM_ENHANCE_FV1_THD, 0xFFFFFFFF, 0x1FFFF000, 0xFFFFFFFF, 0xFFFFFFFF},
	{ISP_AFM_ENHANCE_FV1_COEFF00, 0x3FFFFFFF, 0x10FBEFBE, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_AFM_ENHANCE_FV1_COEFF01, 0x00FFFFFF, 0x00FBEFBE, 0x00FFFFFF, 0x00FFFFFF},
	{ISP_AFM_ENHANCE_FV1_COEFF10, 0x3FFFFFFF, 0x0014317D, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_AFM_ENHANCE_FV1_COEFF11, 0x00FFFFFF, 0x00F7B0FB, 0x00FFFFFF, 0x00FFFFFF},
	{ISP_AFM_ENHANCE_FV1_COEFF20, 0x3FFFFFFF, 0x00EFD143, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_AFM_ENHANCE_FV1_COEFF21, 0x00FFFFFF, 0x000FBF45, 0x00FFFFFF, 0x00FFFFFF},
	{ISP_AFM_ENHANCE_FV1_COEFF30, 0x3FFFFFFF, 0x10E00E00, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_AFM_ENHANCE_FV1_COEFF31, 0x00FFFFFF, 0x00000000, 0x00FFFFFF, 0x00FFFFFF},
	{NR3_FAST_ME_PARAM, 0x000000FF, 0x00000009, 0x000000FF, 0x000000FF},
	{NR3_FAST_ME_ROI_PARAM0, 0x1FFF1FFF, 0x00000000, 0x1FFF1FFF, 0x1FFF1FFF},
	{NR3_FAST_ME_ROI_PARAM1, 0x1FFF1FFF, 0x028001E0, 0x1FFF1FFF, 0x1FFF1FFF},
};


/* Dcam1 register, reg data table */
static struct reg_add_val_tag dcam1_reg_val[] = {
	{ISP_BPC_PARAM, 0xC01F0FFF, 0x001E048F, 0xC01F0FFF, 0xC01F0FFF},
	{ISP_BPC_MAP_CTRL, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF},
	{ISP_BPC_BAD_PIXEL_TH0, 0x3FFFFFFF, 0x00002BFF, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_BPC_BAD_PIXEL_TH1, 0x3FFFFFFF, 0x03C0F3FF, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_BPC_BAD_PIXEL_TH2, 0x3FFFFFFF, 0x0781E3FF, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_BPC_BAD_PIXEL_TH3, 0x3FFFFFFF, 0x0C84B3FF, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_BPC_FLAT_TH, 0xFFFFFFFF, 0x78964078, 0xFFFFFFFF, 0xFFFFFFFF},
	{ISP_BPC_EDGE_RATIO, 0x01FF01FF, 0x0028001E, 0x01FF01FF, 0x01FF01FF},
	{ISP_BPC_BAD_PIXEL_PARAM, 0x0707FFFF, 0x02020000, 0x0707FFFF, 0x0707FFFF},
	{ISP_BPC_BAD_PIXEL_COEFF, 0x001F001F, 0x00000000, 0x001F001F, 0x001F001F},
	{ISP_BPC_LUTWORD0, 0x3FFFFFFF, 0x001FFC00, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_BPC_LUTWORD1, 0x3FFFFFFF, 0x00C3B804, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_BPC_LUTWORD2, 0x3FFFFFFF, 0x02D1A40A, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_BPC_LUTWORD3, 0x3FFFFFFF, 0x07A0F012, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_BPC_LUTWORD4, 0x3FFFFFFF, 0x10009C1C, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_BPC_LUTWORD5, 0x3FFFFFFF, 0x1BB07426, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_BPC_LUTWORD6, 0x3FFFFFFF, 0x2CD05832, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_BPC_LUTWORD7, 0x3FFFFFFF, 0x3200483D, 0x3FFFFFFF, 0x3FFFFFFF},
	{DCAM_BLC_PARA_R_B, 0x800FFFFF, 0x80010040, 0x800FFFFF, 0x800FFFFF},
	{DCAM_BLC_PARA_G, 0x000FFFFF, 0x00010040, 0x000FFFFF, 0x000FFFFF},
	{ISP_RGBG_RB, 0xFFFFFFFF, 0x10001000, 0xFFFFFFFF, 0xFFFFFFFF},
	{ISP_RGBG_G, 0xFFFFFFFF, 0x10001000, 0xFFFFFFFF, 0xFFFFFFFF},
	{ISP_RGBG_YRANDOM_PARAMETER0, 0xFFFFFF07, 0x00000103, 0xFFFFFF07, 0xFFFFFF07},
	{ISP_RGBG_YRANDOM_PARAMETER1, 0x07FF007F, 0x00000060, 0x07FF007F, 0x07FF007F},
	{ISP_RGBG_YRANDOM_PARAMETER2, 0xFFFFFFFF, 0x11111111, 0xFFFFFFFF, 0xFFFFFFFF},
	{DCAM_AEM_OFFSET, 0x1FFF1FFF, 0x00000000, 0x1FFF1FFF, 0x1FFF1FFF},
	{DCAM_AEM_BLK_NUM, 0x0000FFFF, 0x00002020, 0x0000FFFF, 0x0000FFFF},
	{DCAM_AEM_BLK_SIZE, 0x0000FFFF, 0x00001414, 0x0000FFFF, 0x0000FFFF},
	{DCAM_AEM_RED_THR, 0x03FF03FF, 0x005A0109, 0x03FF03FF, 0x03FF03FF},
	{DCAM_AEM_BLUE_THR, 0x03FF03FF, 0x005A0109, 0x03FF03FF, 0x03FF03FF},
	{DCAM_AEM_GREEN_THR, 0x03FF03FF, 0x005A016D, 0x03FF03FF, 0x03FF03FF},
	{ISP_AFL_PARAM0, 0x000000F3, 0x00000021, 0x000000F3, 0x000000F3},
	{ISP_AFL_PARAM1, 0x00FFFFFF, 0x00100000, 0x00FFFFFF, 0x00FFFFFF},
	{ISP_AFL_PARAM2, 0x00FFFFFF, 0x00100000, 0x00FFFFFF, 0x00FFFFFF},
	{ISP_AFL_COL_POS, 0x1FFF1FFF, 0x02800000, 0x1FFF1FFF, 0x1FFF1FFF},
	{ISP_AFL_REGION0, 0x00FFFFFF, 0x00100000, 0x00FFFFFF, 0x00FFFFFF},
	{ISP_AFL_REGION1, 0x00FFFFFF, 0x00100000, 0x00FFFFFF, 0x00FFFFFF},
	{ISP_AFL_REGION2, 0x1FFF1FFF, 0x02800000, 0x1FFF1FFF, 0x1FFF1FFF},
	{ISP_AFL_SUM1, 0x000FFFFF, 0x00000000, 0x000FFFFF, 0x000FFFFF},
	{ISP_AFL_SUM2, 0x000FFFFF, 0x00000000, 0x000FFFFF, 0x000FFFFF},
	{DCAM_CROP0_START, 0x9FFF1FFF, 0x03C00000, 0x9FFF1FFF, 0x9FFF1FFF},
	{DCAM_CROP0_X, 0x1FFF1FFF, 0x03C00000, 0x1FFF1FFF, 0x1FFF1FFF},
	{ISP_AWBC_GAIN0, 0x3FFF3FFF, 0x061B085B, 0x3FFF3FFF, 0x3FFF3FFF},
	{ISP_AWBC_GAIN1, 0x3FFF3FFF, 0x04000400, 0x3FFF3FFF, 0x3FFF3FFF},
	{ISP_AWBC_THRD, 0x3FFFFFFF, 0x3FCFF3FC, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_AWBC_OFFSET0, 0x07FF07FF, 0x00000000, 0x07FF07FF, 0x07FF07FF},
	{ISP_AWBC_OFFSET1, 0x07FF07FF, 0x00000000, 0x07FF07FF, 0x07FF07FF},
	{ISP_AFM_PARAMETERS, 0x0001FF37, 0x00000006, 0x0001FF37, 0x0001FF37},
	{ISP_AFM_ENHANCE_CTRL, 0x000077FF, 0x00000028, 0x000077FF, 0x000077FF},
	{ISP_AFM_CROP_START, 0x1FFF1FFF, 0x00000000, 0x1FFF1FFF, 0x1FFF1FFF},
	{ISP_AFM_CROP_SIZE, 0x1FFF1FFF, 0x00000000, 0x1FFF1FFF, 0x1FFF1FFF},
	{ISP_AFM_WIN_RANGE0S, 0x1FFF1FFF, 0x00040004, 0x1FFF1FFF, 0x1FFF1FFF},
	{ISP_AFM_WIN_RANGE0E, 0x07FF07FF, 0x012C00F0, 0x07FF07FF, 0x07FF07FF},
	{ISP_AFM_WIN_RANGE1S, 0x000F001F, 0x00030004, 0x000F001F, 0x000F001F},
	{ISP_AFM_IIR_FILTER0, 0x0FFF0FFF, 0x012C017A, 0x0FFF0FFF, 0x0FFF0FFF},
	{ISP_AFM_IIR_FILTER1, 0x0FFF0FFF, 0x0EBC0D5C, 0x0FFF0FFF, 0x0FFF0FFF},
	{ISP_AFM_IIR_FILTER2, 0x0FFF0FFF, 0x04000200, 0x0FFF0FFF, 0x0FFF0FFF},
	{ISP_AFM_IIR_FILTER3, 0x0FFF0FFF, 0x0DE70200, 0x0FFF0FFF, 0x0FFF0FFF},
	{ISP_AFM_IIR_FILTER4, 0x0FFF0FFF, 0x02000F68, 0x0FFF0FFF, 0x0FFF0FFF},
	{ISP_AFM_IIR_FILTER5, 0x0FFF0FFF, 0x02000400, 0x0FFF0FFF, 0x0FFF0FFF},
	{ISP_AFM_ENHANCE_FV0_THD, 0xFFFFFFFF, 0x1FFFF000, 0xFFFFFFFF, 0xFFFFFFFF},
	{ISP_AFM_ENHANCE_FV1_THD, 0xFFFFFFFF, 0x1FFFF000, 0xFFFFFFFF, 0xFFFFFFFF},
	{ISP_AFM_ENHANCE_FV1_COEFF00, 0x3FFFFFFF, 0x10FBEFBE, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_AFM_ENHANCE_FV1_COEFF01, 0x00FFFFFF, 0x00FBEFBE, 0x00FFFFFF, 0x00FFFFFF},
	{ISP_AFM_ENHANCE_FV1_COEFF10, 0x3FFFFFFF, 0x0014317D, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_AFM_ENHANCE_FV1_COEFF11, 0x00FFFFFF, 0x00F7B0FB, 0x00FFFFFF, 0x00FFFFFF},
	{ISP_AFM_ENHANCE_FV1_COEFF20, 0x3FFFFFFF, 0x00EFD143, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_AFM_ENHANCE_FV1_COEFF21, 0x00FFFFFF, 0x000FBF45, 0x00FFFFFF, 0x00FFFFFF},
	{ISP_AFM_ENHANCE_FV1_COEFF30, 0x3FFFFFFF, 0x10E00E00, 0x3FFFFFFF, 0x3FFFFFFF},
	{ISP_AFM_ENHANCE_FV1_COEFF31, 0x00FFFFFF, 0x00000000, 0x00FFFFFF, 0x00FFFFFF},
	{NR3_FAST_ME_PARAM, 0x000000FF, 0x00000009, 0x000000FF, 0x000000FF},
	{NR3_FAST_ME_ROI_PARAM0, 0x1FFF1FFF, 0x00000000, 0x1FFF1FFF, 0x1FFF1FFF},
	{NR3_FAST_ME_ROI_PARAM1, 0x1FFF1FFF, 0x028001E0, 0x1FFF1FFF, 0x1FFF1FFF},
};

/* Need to update */
/* dcam other register, reg data table */
static struct reg_add_val_tag axim_reg_val[] = {
	{AXIM_DBG_STS, 0xFFF7FFFF, 0xAEA605C0, 0xFFF7FFFF, 0xFFF7FFFF}
};

static int reg_set_dcam_value(unsigned int id, struct reg_add_val_tag *p)
{
	DCAM_REG_MWR(id, p->addr, p->rw, p->dvalue);

	return 0;
}

static int reg_set_axim_value(struct reg_add_val_tag *p)
{
	DCAM_AXIM_MWR(p->addr, p->rw, p->dvalue);

	return 0;
}

int dcam_reg_set_default_value(int idx)
{
	int i, j;

	if (idx == DCAM_ID_0) {
		/* dcam0 */
		j = sizeof(dcam0_reg_val) / sizeof(struct reg_add_val_tag);
		for (i = 0; i < j; i++)
			reg_set_dcam_value(0, &dcam0_reg_val[i]);
	} else if (idx == DCAM_ID_1) {
		/* dcam1 */
		j = sizeof(dcam1_reg_val) / sizeof(struct reg_add_val_tag);
		for (i = 0; i < j; i++)
			reg_set_dcam_value(1, &dcam1_reg_val[i]);
	}

	return 0;
}

int dcam_aximreg_set_default_value(void)
{
	int i, j;

	/* axim */
	j = sizeof(axim_reg_val) / sizeof(struct reg_add_val_tag);
	pr_info("DCAM AXIM set default reg=%d\n", j);
	for (i = 0; i < j; i++)
		reg_set_axim_value(&axim_reg_val[i]);

	return 0;
}

