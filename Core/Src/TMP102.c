/*
 * TMP102.c
 *
 *  Created on: May 29, 2021
 *      Author: kczem
 */
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "i2c.h"
#include "TMP102.h"
#include "usart.h"
void Get_Temp(unsigned char* buf,int16_t val,float temp_c, HAL_StatusTypeDef ret)
{

	buf[0] = REG_TEMP;
					      ret = HAL_I2C_Master_Transmit(&hi2c1, TMP102_ADDR, buf, 1, 1000);
					      if ( ret != HAL_OK ) {
					        strcpy((char*)buf, "Error Tx\r\n");
					      } else {

					        // Read 2 bytes from the temperature register
					        ret = HAL_I2C_Master_Receive(&hi2c1, TMP102_ADDR, buf, 2, 1000);
					        if ( ret != HAL_OK ) {
					          strcpy((char*)buf, "Error Rx\r\n");
					        } else {

					          //Combine the bytes
					          val = ((int16_t)buf[0] << 4) | (buf[1] >> 4);

					          // Convert to 2's complement, since temperature can be negative
					          if ( val > 0x7FF ) {
					            val |= 0xF000;
					          }

					          // Convert to float temperature value (Celsius)
					          temp_c = val * 0.0625;

					          // Convert temperature to decimal format
					          temp_c *= 100;
					          sprintf((char*)buf,
					                "%u.%u C",
					                ((unsigned int)temp_c / 100),
					                ((unsigned int)temp_c % 100));
					        }
					      }
					// HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 1000);




}
