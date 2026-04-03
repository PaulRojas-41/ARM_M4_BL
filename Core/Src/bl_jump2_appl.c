/*
 * bl_flash_layout.c
 *
 *  Created on: Mar 30, 2026
 *      Author: PRojas
 */


#include "bl_jump2_appl.h"
#include "stm32f4xx.h"
#include "system_stm32f4xx.h"

typedef void (*pFunction)(void);

void BL_jump_2_appl(void)
{
	/* NOTE: flow performed by BL
	 * 1. Bootloader reads Appl stack pointer init address in flash
	 * 2. Reset handler / vector table address: + 4 means the size of the g_pfnVectors address
	 * 3. Application entry point address: main() where will do the jump */

	uint32_t appl_sp;
	uint32_t appl_reset_handler;
	pFunction app_entryPoint;

	/*1.*/
	appl_sp = *(volatile uint32_t *) APPL_HEADER_START_ADDR;

	/*2. */
	appl_reset_handler = *(volatile uint32_t*)(APPL_HEADER_START_ADDR + 4); /* fetch: 0x0800 8004 */

	/*3. */
	app_entryPoint = (pFunction)appl_reset_handler;

	/* Disable interrupts: No boot interrupts can accidentally happen */
	__disable_irq();

	/* Systick disable: prevent any systick isr during boot exec */
	SysTick->CTRL = 0x00;
	SysTick->LOAD = 0x00;
	SysTick->VAL = 0x00;

	/* Boot sets the MSP to the appl's start value */
	__set_MSP(appl_sp);

	/* Jump to appl happens */
	app_entryPoint();
}

