/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd_i2c.h"
#include "bh1750.h"
#include "bluetooth.h"
#include "TMP102.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
bool TReady=false;//flaga do przerwania
struct lcd_disp disp;
float BH1750_lux;
unsigned char buffer[40];
uint8_t size;
HAL_StatusTypeDef ret;
unsigned char buf[12];
int16_t val;
float temp_c;
unsigned char buf_komenda_rx[40];
bool DataValid=false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart){
	if(huart==&huart2){
		get_data(buf,buffer,buf_komenda_rx,DataValid);
		HAL_UART_Receive_IT(&huart2,buf_komenda_rx,strlen((char*)buf_komenda_rx));

	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	//I2C_HandleTypeDef nat;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_TIM11_Init();
  /* USER CODE BEGIN 2 */
  //LCD
  	disp.addr = (0x27 << 1);
    disp.bl = true;
    lcd_init(&disp);

    //bht1750 swiatlo
    BH1750_Init(&hi2c1);
    BH1750_SetMode(CONTINUOUS_HIGH_RES_MODE_2);
    //bluetooth
    setup_uart(&huart2);
    //licznik
    HAL_TIM_Base_Start_IT(&htim11);
    //bluetooth
    HAL_UART_Receive_IT(&huart2,buf_komenda_rx,strlen((char*)buf_komenda_rx));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  if(TReady)
	  {
		DataValid=false;
	  	Get_Temp(buf, val, temp_c, ret);
		 //sprintf((char *)disp.s_line,"stopni celcjusza");
		 //HAL_Delay(500);

		 BH1750_ReadLight(&BH1750_lux);

		 	  if(BH1750_OK == BH1750_ReadLight(&BH1750_lux))
		 	  {
		 		  size = sprintf(buffer, "%.2f Lux", BH1750_lux);
		 	  	 // HAL_UART_Transmit(&huart2, (uint8_t*)buffer, size, 100);
		 	  }else
		 	  {
		 		 size = sprintf(buffer, "slabo\n\r");
		 		  //HAL_UART_Transmit(&huart2, (uint8_t*)buffer, size, 100);
		 	  }
		 	 sprintf((char *)disp.f_line,buf);
		 	sprintf((char *)disp.s_line,buffer);
		 	lcd_display(&disp);
		 DataValid=true;
		 TReady=false;
	  }
	 // PROGRAM WYSWIETLA GWIAZDKI NA WYSWIETLACZU do testowania
/*sprintf((char *)disp.f_line, "     ******");
  sprintf((char *)disp.s_line, "");
  HAL_Delay(500);
  lcd_display(&disp);

  sprintf((char *)disp.f_line, "");
  sprintf((char *)disp.s_line, "     ******");
  HAL_Delay(500);
  lcd_display(&disp);*/
	  // Tell TMP102 that we want to read from the temperature register




/*
timer_val=__HAL_TIM_GET_COUNTER(&htim11);
HAL_Delay(50);
timer_val=__HAL_TIM_GET_COUNTER(&htim11)-timer_val;
uart_buf_len=sprintf(uart_buf,"%u us\r\n",timer_val);
HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf, uart_buf_len, 100);
HAL_Delay(1000);
*/
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
if(htim==&htim11)
{
TReady=true;
}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
