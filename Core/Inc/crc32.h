/*
 * crc32.h
 *
 *  Created on: Apr 19, 2026
 *      Author: PRojas
 */

#ifndef INC_CRC32_H_
#define INC_CRC32_H_

#include "stm32f4xx.h"
#include "system_stm32f4xx.h"

uint32_t bl_get_crc32(uint8_t *data, uint32_t length);
uint32_t test_endianess(uint8_t *data, uint32_t length);

#endif /* INC_CRC32_H_ */
