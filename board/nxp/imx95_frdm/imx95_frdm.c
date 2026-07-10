// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2025 NXP
 */

#include <asm/gpio.h>
#include <asm/arch/clock.h>
#include <asm/mach-imx/sys_proto.h>
#include <asm/mach-imx/boot_mode.h>
#include <env.h>
#include <fdt_support.h>

#ifdef CONFIG_OF_BOARD_SETUP
int ft_board_setup(void *blob, struct bd_info *bd)
{
	int ret;
	int banks = CONFIG_NR_DRAM_BANKS;
	u64 base[CONFIG_NR_DRAM_BANKS];
	u64 size[CONFIG_NR_DRAM_BANKS];
	int i;

	for (i = 0; i < banks; i++) {
		base[i] = gd->bd->bi_dram[i].start;
		size[i] = gd->bd->bi_dram[i].size;
	}

	ret = fdt_fixup_memory_banks(blob, base, size, banks);
	if (ret) {
		printf("Failed to setup memory bank values on fdt.\n");
		return ret;
	}

	return 0;
}
#endif

static bool is_usb_boot(void)
{
	return get_boot_device() == USB_BOOT;
}

int board_early_init_f(void)
{
	/* UART1: A55, UART2: M33, UART3: M7 */
	init_uart_clk(0);

	return 0;
}

int board_init(void)
{
	return 0;
}

int board_late_init(void)
{
	if (IS_ENABLED(CONFIG_ENV_IS_IN_MMC))
		board_late_mmc_env_init();

	if (is_usb_boot()) {
		env_set("bootcmd", "fastboot 0");
		env_set("bootdelay", "0");
	}

	return 0;
}

int board_phys_sdram_size(phys_size_t *size)
{
	*size = PHYS_SDRAM_SIZE + PHYS_SDRAM_2_SIZE;

	return 0;
}
