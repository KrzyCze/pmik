/*
 * bluetooth.c
 *
 *  Created on: May 29, 2021
 *      Author: kczem
 */

// bluetooth jeszcze nam nie działa wiec chyba nie ma co oceniac tu
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "i2c.h"
#include "bluetooth.h"
#include "TMP102.h"
#include "usart.h"
#include <stdbool.h>


void get_data(unsigned char* temp,unsigned char* lux, unsigned char* buf_komenda_rx, bool DataValid){
if(strcmp(buf_komenda_rx,"GET_DATA") == 0){
	int licznik=0;
	while(DataValid==false)
	{
licznik++;

if(licznik==10){
			break;
}
	}
	if(DataValid==true){
	HAL_UART_Transmit(&huart2, temp, strlen((char*)temp), 100);
	HAL_UART_Transmit(&huart2, lux, strlen((char*)lux), 100);
	}
	else
	{
	HAL_UART_Transmit(&huart2, "brak danych", strlen("brak danych"), 100);

	}
}
else{
	HAL_UART_Transmit(&huart2, "bledna komenda", strlen("bledna komenda"), 100);
}
};


/*do wyrzucenia?
 * void setup_uart(UART_HandleTypeDef* uart){
	na_pewno_nie_HAL_Delay1000();
	HAL_UART_Transmit(uart,"AT+UART=9600,0,0\r\n",strlen("AT+UART=9600,0,0\r\n"),100);
	HAL_Delay(100);
	HAL_UART_Transmit(uart,"AT+NAME=krystyna\r\n",strlen("AT+NAME=krystyna\r\n"),100);
	HAL_Delay(100);
	HAL_UART_Transmit(uart,"AT+ROLE=0\r\n",strlen("AT+ROLE=0\r\n"),100);
	HAL_Delay(100);
}

void na_pewno_nie_HAL_Delay1000(){
	HAL_Delay(1000);
}

*/

