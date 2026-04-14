/*
 * bl_flash_layout.c
 *
 *  Created on: Mar 30, 2026
 *      Author: Paul Rojas
 */


#include "bl_jump2_appl.h"
#include "stm32f4xx.h"
#include "system_stm32f4xx.h"

typedef void (*pFunction)(void);

void BL_jump_2_appl(void)
{
	/* NOTE: flow performed by BL
	 * 1. Bootloader sets the mainstackpointer (appl_sp) to appl start address defined in flash
	 * 2. Reset handler address fetch inside the vector table address: offset +4 inside g_pfnVectors table
	 * 3. Application entry point jump / Reset handler jump: funcptr call as a SW reset performed by boot in order to
	 * 	  jump to the appl code 1st instruction to execute */

	uint32_t appl_sp;
	uint32_t appl_reset_handler;
	pFunction app_entryPoint;

	/*1.*/
	appl_sp = *(volatile uint32_t *) APPL_START_ADDR;

	/*2. */
	appl_reset_handler = *(volatile uint32_t*)(APPL_START_ADDR + 4); /* fetch of Reset_Handler addr inside Vector table */

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

uint8_t BL_is_appl_valid(void)
{
	/*1. parameters to compare: get header's application key to validate */
	uint32_t appl_header_val = APPL_HEADER_START_ADDR;
	const appl_header_t *ptr = (appl_header_t *)appl_header_val; /* memory location extracted and re_formatted for initialize ptr */

	/*2. compare the extracted content*/
	if(ptr->magic_number != APPL_VALID_KEY)
		return 1;

	/*3. verify if we are jumping to the correct reset handler address in the flash sector as a sanity check */
	uint32_t reset_handler_addr = *(uint32_t *)(APPL_START_ADDR + 4);

	if((reset_handler_addr & 0xFF00F000) != 0x800C000)
		return 2;

	return 0;
}
