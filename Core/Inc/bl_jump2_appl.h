/*
 * bl_flash_layout.h
 *
 *  Created on: Mar 30, 2026
 *      Author: PRojas
 */

#ifndef INC_BL_JUMP2_APPL_H_
#define INC_BL_JUMP2_APPL_H_

/* global macros */
#define BL_START_ADDR   		0x08000000 /* 32KB */
#define APPL_HEADER_START_ADDR	0x08008000 /* 16KB */
#define APPL_START_ADDR 		0x0800C000 /* 1024KB */


/* global methods */
void BL_jump_2_appl(void);

#endif /* INC_BL_JUMP2_APPL_H_ */
