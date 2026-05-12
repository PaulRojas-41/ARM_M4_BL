/*
 * crc32.c
 *
 *  Created on: May 11, 2026
 *      Author: Paul Rojas
 */

#include "stm32f4xx.h"
#include "system_stm32f4xx.h"
#include "crc32.h"
#include "cmsis_gcc.h"

#define crcReset() (CRC->CR = 1)
#define crcRead() (__REV(CRC->DR) ^ 0xFFFFFFFF)
#define crcWrite(x) (CRC->DR = __REV(x))
#define U32_BIG_ENDIAN(x) ((0xFF000000 & x) >> 24) | ((0x00FF0000 & x) >> 8) | ((0x0000FF00 & x) << 8) | ((0x000000FF & x) << 24)

uint32_t bl_get_crc32(uint8_t *data, uint32_t length)
{

	uint32_t crc = 0xFFFFFFFF;
	uint32_t block_index = 0;
	uint32_t data_index = 0;

	for(block_index = 0; block_index < length; block_index++) //Appl size sans l'en-tête d'application: 21,248 bytes
	{
		crc = crc ^ data[block_index];

		for(data_index = 0; data_index < 8; data_index++) // octet par octet
		{
			if(crc & 0x00000001)
			{
				crc = (crc >> 1) ^ 0xEDB88320;
			}
			else
			{
				crc = (crc >> 1);
			}
		}
	}

	return crc ^ 0xFFFFFFFF;
}
