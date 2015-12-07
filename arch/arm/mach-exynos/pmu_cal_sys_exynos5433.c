/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd. All rights reserved.
 *		http://www.samsung.com
 *
 * Chip Abstraction Layer for System power down support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifdef CONFIG_CAL_SYS_PWRDOWN
#include <linux/types.h>
#include <linux/io.h>
#include <mach/pmu_cal_sys.h>
#else
/* firmware only */
#include "util.h"
#include "pmu_cal_sys_exynos5433.h"
struct exynos_pmu_conf {
	unsigned int reg;
	unsigned int val[NUM_SYS_POWERDOWN];
};
#define PMU_TABLE_END		NULL
#endif

#define	CENTRAL_LOWPWR_CFG	0x10000

#define	writebits(addr, base, mask, val) \
	__raw_writel((__raw_readl(addr) & ~((mask) << (base))) | \
		(((mask) & (val)) << (base)), addr)

#define readbits(addr, base, mask) \
	((__raw_readl(addr) >> (base)) & (mask))

static struct exynos_pmu_conf exynos5433_pmu_config[] = {
	/* { .addr = address, .val = { AFTR, STOP, DSTOP, DSTOP_PSR, LPD, LPA, ALPA, SLEEP } } */
	{ ATLAS_CPU0_SYS_PWR_REG,			{ 0x0, 0xF, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8 } },
	{ DIS_IRQ_ATLAS_CPU0_CENTRAL_SYS_PWR_REG,	{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ ATLAS_CPU1_SYS_PWR_REG,			{ 0x0, 0xF, 0x0, 0x0, 0x0, 0x0,	0x0, 0x8 } },
	{ DIS_IRQ_ATLAS_CPU1_CENTRAL_SYS_PWR_REG,	{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,	0x0, 0x0 } },
	{ ATLAS_CPU2_SYS_PWR_REG,			{ 0x0, 0xF, 0x0, 0x0, 0x0, 0x0,	0x0, 0x8 } },
	{ DIS_IRQ_ATLAS_CPU2_CENTRAL_SYS_PWR_REG,	{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,	0x0, 0x0 } },
	{ ATLAS_CPU3_SYS_PWR_REG,			{ 0x0, 0xF, 0x0, 0x0, 0x0, 0x0,	0x0, 0x8 } },
	{ DIS_IRQ_ATLAS_CPU3_CENTRAL_SYS_PWR_REG,	{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,	0x0, 0x0 } },
	{ APOLLO_CPU0_SYS_PWR_REG,			{ 0x0, 0xF, 0x0, 0x0, 0x0, 0x0,	0x0, 0x8 } },
	{ DIS_IRQ_APOLLO_CPU0_CENTRAL_SYS_PWR_REG,	{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ APOLLO_CPU1_SYS_PWR_REG,			{ 0x0, 0xF, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8 } },
	{ DIS_IRQ_APOLLO_CPU1_CENTRAL_SYS_PWR_REG,	{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ APOLLO_CPU2_SYS_PWR_REG,			{ 0x0, 0xF, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8 } },
	{ DIS_IRQ_APOLLO_CPU2_CENTRAL_SYS_PWR_REG,	{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ APOLLO_CPU3_SYS_PWR_REG,			{ 0x0, 0xF, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8 } },
	{ DIS_IRQ_APOLLO_CPU3_CENTRAL_SYS_PWR_REG,	{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ ATLAS_NONCPU_SYS_PWR_REG,			{ 0x0, 0xF, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8 } },
	{ APOLLO_NONCPU_SYS_PWR_REG,			{ 0x0, 0xF, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8 } },
	{ A5IS_SYS_PWR_REG,				{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ DIS_IRQ_A5IS_LOCAL_SYS_PWR_REG,		{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ DIS_IRQ_A5IS_CENTRAL_SYS_PWR_REG,		{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ ATLAS_L2_SYS_PWR_REG,				{ 0x0, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7 } },
	{ APOLLO_L2_SYS_PWR_REG,			{ 0x0, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7 } },
	{ CLKSTOP_CMU_TOP_SYS_PWR_REG,			{ 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ CLKRUN_CMU_TOP_SYS_PWR_REG,			{ 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ RESET_CMU_TOP_SYS_PWR_REG,			{ 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ RESET_CPUCLKSTOP_SYS_PWR_REG,			{ 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x0 } },
	{ CLKSTOP_CMU_MIF_SYS_PWR_REG,			{ 0x1, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0 } },
	{ CLKRUN_CMU_MIF_SYS_PWR_REG,			{ 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x0 } },
	{ RESET_CMU_MIF_SYS_PWR_REG,			{ 0x1, 0x1, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0 } },
	{ DDRPHY_DLLLOCK_SYS_PWR_REG,			{ 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1 } },
	{ DISABLE_PLL_CMU_TOP_SYS_PWR_REG,		{ 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ DISABLE_PLL_AUD_PLL_SYS_PWR_REG,		{ 0x1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x0 } },
	{ DISABLE_PLL_CMU_MIF_SYS_PWR_REG,		{ 0x1, 0x1, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0 } },
	{ TOP_BUS_SYS_PWR_REG,				{ 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ TOP_RETENTION_SYS_PWR_REG,			{ 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1 } },
	{ TOP_PWR_SYS_PWR_REG,				{ 0x3, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3 } },
	{ TOP_BUS_MIF_SYS_PWR_REG,			{ 0x7, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x0 } },
	{ TOP_RETENTION_MIF_SYS_PWR_REG,		{ 0x1, 0x1, 0x0, 0x0, 0x1, 0x0, 0x0, 0x1 } },
	{ TOP_PWR_MIF_SYS_PWR_REG,			{ 0x3, 0x3, 0x0, 0x0, 0x3, 0x0, 0x0, 0x3 } },
	{ LOGIC_RESET_SYS_PWR_REG,			{ 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ OSCCLK_GATE_SYS_PWR_REG,			{ 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1 } },
	{ SLEEP_RESET_SYS_PWR_REG,			{ 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x0 } },
	{ LOGIC_RESET_MIF_SYS_PWR_REG,			{ 0x1, 0x1, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0 } },
	{ OSCCLK_GATE_MIF_SYS_PWR_REG,			{ 0x1, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x1 } },
	{ SLEEP_RESET_MIF_SYS_PWR_REG,			{ 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x0 } },
	{ MEMORY_TOP_SYS_PWR_REG,			{ 0x3, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ PAD_RETENTION_LPDDR3_SYS_PWR_REG,		{ 0x1, 0x1, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0 } },
	{ PAD_RETENTION_JTAG_SYS_PWR_REG,		{ 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ PAD_RETENTION_TOP_SYS_PWR_REG,		{ 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ PAD_RETENTION_UART_SYS_PWR_REG,		{ 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ PAD_RETENTION_EBIA_SYS_PWR_REG,		{ 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ PAD_RETENTION_EBIB_SYS_PWR_REG,		{ 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ PAD_RETENTION_SPI_SYS_PWR_REG,		{ 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ PAD_RETENTION_MIF_SYS_PWR_REG,		{ 0x1, 0x1, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0 } },
	{ PAD_ISOLATION_SYS_PWR_REG,			{ 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1 } },
	{ PAD_RETENTION_USBXTI_SYS_PWR_REG,		{ 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ PAD_RETENTION_BOOTLDO_SYS_PWR_REG,		{ 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ PAD_ISOLATION_MIF_SYS_PWR_REG,		{ 0x1, 0x1, 0x0, 0x0, 0x1, 0x0, 0x0, 0x1 } },
	{ PAD_RETENTION_FSYSGENIO_SYS_PWR_REG,		{ 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ PAD_ALV_SEL_SYS_PWR_REG,			{ 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ XXTI_SYS_PWR_REG,				{ 0x1, 0x0, 0x0, 0x1, 0x1, 0x1, 0x1, 0x0 } },
	{ XXTI26_SYS_PWR_REG,				{ 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ EXT_REGULATOR_SYS_PWR_REG,			{ 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x0 } },
	{ GPIO_MODE_SYS_PWR_REG,			{ 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ GPIO_MODE_FSYS_SYS_PWR_REG,			{ 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ GPIO_MODE_MIF_SYS_PWR_REG,			{ 0x1, 0x1, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0 } },
	{ GPIO_MODE_AUD_SYS_PWR_REG,			{ 0x1, 0x1, 0x0, 0x0, 0x0, 0x1, 0x1, 0x0 } },
	{ GSCL_SYS_PWR_REG,				{ 0xF, 0xF, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ CAM0_SYS_PWR_REG,				{ 0xF, 0xF, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ MSCL_SYS_PWR_REG,				{ 0xF, 0xF, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ G3D_SYS_PWR_REG,				{ 0xF, 0xF, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ DISP_SYS_PWR_REG,				{ 0xF, 0xF, 0x0, 0xF, 0xF, 0x0, 0x0, 0x0 } },
	{ CAM1_SYS_PWR_REG,				{ 0xF, 0xF, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ AUD_SYS_PWR_REG,				{ 0xF, 0xF, 0x0, 0x0, 0x0, 0xF, 0xF, 0x0 } },
	{ FSYS_SYS_PWR_REG,				{ 0xF, 0xF, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ BUS2_SYS_PWR_REG,				{ 0xF, 0xF, 0x0, 0x0, 0xF, 0x0, 0x0, 0x0 } },
	{ G2D_SYS_PWR_REG,				{ 0xF, 0xF, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ ISP_SYS_PWR_REG,				{ 0xF, 0xF, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ MFC_SYS_PWR_REG,				{ 0xF, 0xF, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ HEVC_SYS_PWR_REG,				{ 0xF, 0xF, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
	{ RESET_SLEEP_FSYS_SYS_PWR_REG,			{ 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x0 } },
	{ RESET_SLEEP_BUS2_SYS_PWR_REG,			{ 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x0 } },
	{ PMU_TABLE_END, },
};

void set_pmu_sys_pwr_reg_conf(enum sys_powerdown mode)
{
	unsigned int i;

	for (i = 0; exynos5433_pmu_config[i].reg != PMU_TABLE_END; i++)
		__raw_writel(exynos5433_pmu_config[i].val[mode],
				exynos5433_pmu_config[i].reg);
}

void set_pmu_central_seq_mif_conf(bool enabled)
{
	unsigned long tmp;

	tmp = __raw_readl(CENTRAL_SEQ_MIF_CONFIGURATION);
	tmp = enabled ? tmp & ~CENTRAL_LOWPWR_CFG : tmp | CENTRAL_LOWPWR_CFG;
	__raw_writel(tmp, CENTRAL_SEQ_MIF_CONFIGURATION);
}

void set_pmu_central_seq_conf(bool enabled)
{
	unsigned long tmp;

	tmp = __raw_readl(CENTRAL_SEQ_CONFIGURATION);
	tmp = enabled ? tmp & ~CENTRAL_LOWPWR_CFG : tmp | CENTRAL_LOWPWR_CFG;
	__raw_writel(tmp, CENTRAL_SEQ_CONFIGURATION);

	set_pmu_central_seq_mif_conf(enabled);
}

void set_pmu_prepare_sys_powerdown(enum sys_powerdown mode)
{
	if (mode == SYS_SLEEP) {
		writebits(ATLAS_NONCPU_OPTION, 2, 1, 1);
		writebits(ATLAS_NONCPU_OPTION, 3, 1, 0);
		writebits(ATLAS_L2_OPTION, 4, 1, 0);
		writebits(APOLLO_NONCPU_OPTION, 2, 1, 1);
		writebits(APOLLO_NONCPU_OPTION, 3, 1, 0);
		writebits(APOLLO_L2_OPTION, 4, 1, 0);
	} else {
		writebits(ATLAS_NONCPU_OPTION, 2, 1, 1);
		writebits(ATLAS_NONCPU_OPTION, 3, 1, 1);
		writebits(ATLAS_L2_OPTION, 4, 1, 1);
		writebits(APOLLO_NONCPU_OPTION, 2, 1, 1);
		writebits(APOLLO_NONCPU_OPTION, 3, 1, 1);
		writebits(APOLLO_L2_OPTION, 4, 1, 1);
	}

#ifdef EVT1	/* FIXME: use vma */
	Outp32(0x105c2608, 0x10);
	Outp32(0x105c2628, 0x10);

	Outp32(0x105c2408, 0x01);
	Outp32(0x105c2428, 0x01);
#endif

	/*
	 * FIXME: use vma
	 * workaround for audio block h-prime evt0 evt1 .2014/03/15 phk
	 * SetBits(0x11820020, 7, 0x1, 0x1);	// aud_option scfeedback
	 * LPI_MASK_ATLAS_ASYNCBRIDGE ATB_AUD_CSSYS Disable LPI
	 * power mode enter LPI Mask from EVT0
	 *
	 * writebits(0x11820020, 7, 0x1, 0x1);
	 */
}

void set_pmu_early_wakeup(enum sys_powerdown mode)
{
	set_pmu_central_seq_conf(false);
}

#ifdef CONFIG_CAL_SYS_PWRDOWN
void pmu_cal_sys_init(void)
{
	exynos_sys_powerdown_conf = set_pmu_sys_pwr_reg_conf;
	exynos_central_sequencer_ctrl = set_pmu_central_seq_conf;
}
#endif
