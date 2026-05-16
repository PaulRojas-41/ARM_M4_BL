/*
 * bl_flash_layout.c
 *
 *  Created on: Mar 30, 2026
 *      Author: Paul Rojas
 */


#include "bl_jump2_appl.h"
#include "crc32.h"
#include "stm32f4xx.h"
#include "system_stm32f4xx.h"

typedef void (*pFunction)(void);

void BL_jump_2_appl(void)
{
	/* NOTE: flow performed by BL
	 * 1. Bootloader sets the mainstackpointer (appl_sp)
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

uint8_t BL_appl_valid_error_code;

uint8_t BL_is_appl_valid(void)
{
	/*1. parameters that Bootloader verifies before jump or give control to the application
	 * : get header's application key to validate */
	uint32_t appl_header_val = APPL_HEADER_START_ADDR;
	const appl_header_t *appl_header = (appl_header_t *)appl_header_val; /* we make appl's header value behave as an address */

	/*2. Basic validation before jump to the appl: read the "Valid Key" store inside header's application flash slot */
	if(appl_header->magic_number != APPL_VALID_KEY)
	{
		BL_appl_valid_error_code |= 0x00000001;
	}

	/*3. Sanity check: verify if we are jumping to the correct reset handler address defined in the flash sector */
	uint32_t reset_handler_addr = *(uint32_t *)(APPL_START_ADDR + 4);

	if((reset_handler_addr & 0xFF00F000) != 0x800C000)
	{
		BL_appl_valid_error_code |= 0x00000002;
	}

	/*4. Application Size verification */
	if(appl_header->appl_size == 0 || appl_header->appl_size > APPL_MAX_SIZE)
	{
		BL_appl_valid_error_code |= 0x00000003;
	}

	/*5. CRC32 value calculated verification */
	uint32_t bl_crc32_result = bl_get_crc32((uint8_t *)APPL_START_ADDR, appl_header->appl_size);

	if(appl_header->crc32 != bl_crc32_result)
	{
		BL_appl_valid_error_code |= 0x00000004;
	}

	return BL_appl_valid_error_code;
}
