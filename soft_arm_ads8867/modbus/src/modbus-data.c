/***************************************************************************
 * Copyright (C), suxin, All rights reserved.
 * File        :modbus-data.c
 * Decription  :
 *
 * Author      :  liuch
 * Version     :  v1.0
 * Date        :  2018/6/22
 * Histroy	   :
 **************************************************************************/

#include <stdlib.h>
#ifndef _MSC_VER
#include <stdint.h>
#else
#include "stdint.h"
#endif
#include <string.h>
#include <assert.h>


/*********************************************************************
 * @fn		modbus_set_bits_from_byte
 *
 * @brief	Sets many bits from a single byte value (all 8 bits of the byte value are set)
 *
 * @param	dest, index value
 *
 * @return	void
 *
 * @author	liuch
 *
 * @version V1.0
 *
 * @date	20180623

 */
void modbus_set_bits_from_byte(uint8_t *dest, int index, const uint8_t value)
{
    int i;

    for (i=0; i<8; i++) {
        dest[index+i] = (value & (1 << i)) ? 1 : 0;
    }
}

/*********************************************************************
 * @fn		modbus_set_bits_from_bytes
 *
 * @brief	Sets many bits from a table of bytes (only the bits between index and index + nb_bits are set)
 *
 * @param	dest, index nb_bits, tab_bytes
 *
 * @return	void
 *
 * @author	liuch
 *
 * @version V1.0
 *
 * @date	20180623
 */
void modbus_set_bits_from_bytes(uint8_t *dest, int index, unsigned int nb_bits,
                                const uint8_t *tab_byte)
{
    int i;
    int shift = 0;

    for (i = index; i < index + nb_bits; i++) {
        dest[i] = tab_byte[(i - index) / 8] & (1 << shift) ? 1 : 0;
        /* gcc doesn't like: shift = (++shift) % 8; */
        shift++;
        shift %= 8;
    }
}

/*********************************************************************
 * @fn		modbus_get_byte_from_bits
 *
 * @brief	Gets the byte value from many bits.To obtain a full byte, set nb_bits to 8.
 *
 * @param	src, index ,nb_bits
 *
 * @return	void
 *
 * @author	liuch
 *
 * @version V1.0
 *
 * @date	20180623
 */

uint8_t modbus_get_byte_from_bits(const uint8_t *src, int index,
                                  unsigned int nb_bits)
{
    int i;
    uint8_t value = 0;

    if (nb_bits > 8) {
        /* Assert is ignored if NDEBUG is set */
        assert(nb_bits < 8);
        nb_bits = 8;
    }

    for (i=0; i < nb_bits; i++) {
        value |= (src[index+i] << i);
    }

    return value;
}

/* Get a float from 4 bytes in Modbus format */
float modbus_get_float(const uint16_t *src)
{
    float f = 0.0f;
    uint32_t i;

    i = (((uint32_t)src[1]) << 16) + src[0];
    memcpy(&f, &i, sizeof(float));

    return f;
}

/* Set a float to 4 bytes in Modbus format */
void modbus_set_float(float f, uint16_t *dest)
{
    uint32_t i = 0;

    memcpy(&i, &f, sizeof(uint32_t));
    dest[0] = (uint16_t)i;
    dest[1] = (uint16_t)(i >> 16);
}
