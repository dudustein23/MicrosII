/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Controle de Brilho do LED pela Serial Utilizando PWM
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  *	Este programa visa acionar um LED através de uma saída PWM (PA5 - LED placa).
  *	Ele controla o brilho do LED de acordo com seu duty cicle (ciclo de trabalho).
  * Como o LED irá piscar numa frequência de 100 Hz, não iremos conseguir distinguir
  * suas piscadas, logo, iremos facilmente distinguir o tempo em que ele fica ativo
  * em relação ao tempo em que ele fica inativo (ciclo de trabalho). Como o ARR está
  * configurado para 999, e duty_cicle = CCR (mexer no Pulse) / ARR, devemos alterar
  * o valor de CCR para 199 (20%), 399 (40%), 599 (60%), 799 (80%) e 999 (100%), por
  * meio da Serial.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
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
// Tempo de espera caso não haja resposta: 1000ms.

	uint32_t borda[2]={0,0};

	enum t_bordas{atual, anterior};
	float periodo;
#define size 100
#define TOUT_SER 1000
// Variável que diz o tamanho da mensagem.
#define TAM_MSG 100
uint8_t erro_ao_digitar[TAM_MSG] = "Tu digitou um valor errado.\r\n";
// Mensagem de instrução.
uint8_t msg[TAM_MSG] = "Digite o duty cicle que deseja 0: 0%; 1: 20%; 2: 40%; 3: 60%; 4: 80%; 5: 100%.\r\n";
// Comando digitado pelo usuário.
uint8_t comando[1];
uint32_t duty;
float t_alto;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_TIM5_Init();
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */

	HAL_TIM_Base_Start(&htim10);
	HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);  // Incializa o PWM.

  HAL_UART_Transmit(&huart2, msg, strlen(msg), TOUT_SER);
  msg[0]='\0';
  HAL_UART_Receive_IT(&huart2, comando, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	if (msg[0]!='\0') {
		HAL_UART_Transmit(&huart2, msg, strlen(msg),TOUT_SER);
		msg[0]='\0';
	}
	if (__HAL_TIM_GET_FLAG(&htim10, TIM_FLAG_UPDATE)){
			__HAL_TIM_CLEAR_FLAG(&htim10, TIM_FLAG_UPDATE);
		  snprintf(msg,size,"Duty medido: %li ms\n\n\r", duty);
		  HAL_UART_Transmit(&huart2, msg, strlen(msg), TOUT_SER);
	}
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (comando[0] == '0') {
		strcpy(msg,"\n DUTY CICLE = 0% \r\n");
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
	}
	else if (comando[0] == '1') {
		strcpy(msg,"\n DUTY CICLE = 20% \r\n");
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 199);
	}
	else if (comando[0] == '2') {
		strcpy(msg,"\n DUTY CICLE = 40% \r\n");
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 399);
	}
	else if (comando[0] == '3') {
		strcpy(msg,"\n DUTY CICLE = 60% \r\n");
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 599);
	}
	else if (comando[0] == '4') {
		strcpy(msg,"\n DUTY CICLE = 80% \r\n");
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 799);
	}
	else if (comando[0] == '5') {
		strcpy(msg,"\n DUTY CICLE = 100% \r\n");
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 999);
	}
	else {
		strcpy(msg,"\n Tu deverias ter digitado um valor entre 0 e 5.\r\n");
	}

	HAL_UART_Receive_IT(&huart2, comando, 1);  // Chama novamente a IT do USART2.
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
		periodo = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
	}
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2){
		t_alto = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_2);
	}
	duty = round((t_alto*100/periodo));
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
