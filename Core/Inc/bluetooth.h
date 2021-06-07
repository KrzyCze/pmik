/*
 * bluetooth.h
 *
 *  Created on: May 29, 2021
 *      Author: kczem
 */

#ifndef INC_BLUETOOTH_H_
#define INC_BLUETOOTH_H_
#include <stdbool.h>
void get_data(unsigned char* temp,unsigned char* lux, unsigned char* buf_komenda_rx, bool DataValid);
//void setup_uart(UART_HandleTypeDef* uart);
//void na_pewno_nie_HAL_Delay1000();
#endif /* INC_BLUETOOTH_H_ */
