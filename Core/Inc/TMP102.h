/*
 * TMP102.h
 *
 *  Created on: May 29, 2021
 *      Author: kczem
 */

#ifndef INC_TMP102_H_
#define INC_TMP102_H_
static const uint8_t TMP102_ADDR=0x48<<1;
static const uint8_t REG_TEMP=0x00;
void Get_Temp(uint8_t* buf,int16_t val,float temp_c, HAL_StatusTypeDef ret);

#endif /* INC_TMP102_H_ */
