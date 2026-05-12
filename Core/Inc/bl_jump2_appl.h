/*
 * bl_flash_layout.h
 *
 *  Created on: Mar 30, 2026
 *      Author: Paul Rojas
 */

#ifndef INC_BL_JUMP2_APPL_H_
#define INC_BL_JUMP2_APPL_H_

#include "stm32f4xx.h"
#include "system_stm32f4xx.h"

/* global macros */
#define BL_START_ADDR   		0x08000000 /* 32KB */
#define APPL_HEADER_START_ADDR	0x08008000 /* 16KB */
#define APPL_START_ADDR 		0x0800C000 /* 1024KB */
#define APPL_MAX_SIZE           100*1024
#define APPL_VALID_KEY          0xAAAACCCC

/* global objects */
typedef struct{
	uint32_t magic_number;
	uint32_t crc32;
	uint32_t appl_size;
	uint32_t ota_flag;
	uint32_t checksum;
}appl_header_t;

/* global methods */
void BL_jump_2_appl(void);
uint8_t BL_is_appl_valid(void);

#endif /* INC_BL_JUMP2_APPL_H_ */
