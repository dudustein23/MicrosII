/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum
{
  BUTTON_PULL_DOWN = 0,
  BUTTON_PULL_UP
}BUTTON_PUPD;
typedef enum
{
  SOLTO = 0,
  AO_PRESSIONAR,
  PRESSIONADO,
  AO_SOLTAR
}ESTADO_BOTAO;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t b1=0;
uint8_t b2=0;
uint8_t b3=0;
uint8_t b1a=0;
uint8_t b2a=0;
uint8_t b3a=0;
uint8_t L1=0;
uint8_t L2=0;
uint8_t L3=0;
ESTADO_BOTAO botao1 = SOLTO;
ESTADO_BOTAO botao2 = SOLTO;
ESTADO_BOTAO botao3 = SOLTO;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void gerenciador_botao(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BUTTON_PUPD pull, uint8_t * atual, uint8_t * anterior, ESTADO_BOTAO * estado);
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
  MX_TIM10_Init();
  MX_TIM11_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim10);
    HAL_TIM_Base_Start(&htim11);
    HAL_TIM_Base_Start(&htim2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  		gerenciador_botao(Button1_GPIO_Port, Button1_Pin, BUTTON_PULL_UP,  &b1, &b1a, &botao1);
  		gerenciador_botao(Button2_GPIO_Port, Button2_Pin, BUTTON_PULL_UP,  &b2, &b2a, &botao2);
  		gerenciador_botao(Button3_GPIO_Port, Button3_Pin, BUTTON_PULL_UP,  &b3, &b3a, &botao3);

  		if (botao1 == AO_PRESSIONAR) {
  			if (L1 == 0) {
  				L1 = 1;
  			} else {
  				L1 = 0;
  			}
  		}
  		if (L1 == SOLTO) {
  			HAL_GPIO_WritePin(L1_GPIO_Port, L1_Pin, 0);
  		}

  		if (botao2 == AO_PRESSIONAR) {
  			if (L2 == 0) {
  				L2 = 1;
  			} else {
  				L2 = 0;
  			}
  		}
  		if (L2 == SOLTO) {
  			HAL_GPIO_WritePin(L2_GPIO_Port, L2_Pin, 0);
  		}

  		if (botao3 == AO_PRESSIONAR) {
  		  			if (L3 == 0) {
  		  				L3 = 1;
  		  			} else {
  		  				L3 = 0;
  		  			}
  		  		}
  		  		if (L3 == SOLTO) {
  		  			HAL_GPIO_WritePin(L3_GPIO_Port, L3_Pin, 0);
  		  		}



  		if (__HAL_TIM_GET_FLAG(&htim10, TIM_FLAG_UPDATE)) {
  			__HAL_TIM_CLEAR_FLAG(&htim10, TIM_FLAG_UPDATE);
  			if (L2 == 1){
  				HAL_GPIO_TogglePin(L2_GPIO_Port, L2_Pin);
  			}
  		}
  		if (__HAL_TIM_GET_FLAG(&htim11, TIM_FLAG_UPDATE)) {
  			__HAL_TIM_CLEAR_FLAG(&htim11, TIM_FLAG_UPDATE);
  			if (L1 == 1){
  				HAL_GPIO_TogglePin(L1_GPIO_Port, L1_Pin);
  			}
  		}
  		if (__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_UPDATE)) {
			__HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);
			if (L3 == 1){
				HAL_GPIO_TogglePin(L3_GPIO_Port, L3_Pin);
			}
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

void gerenciador_botao(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BUTTON_PUPD pull, uint8_t * atual, uint8_t * anterior, ESTADO_BOTAO * estado) {
	ESTADO_BOTAO estado_botao(uint8_t atual, uint8_t * anterior);
	if ((*atual == 0) && (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) != pull)){
		for (int i = 0; i < 80000; i++);
		if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) != pull){
			*atual = 1;
		}
	}
	if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == pull) {
		*atual = 0;
	}
	*estado = estado_botao(*atual, anterior);
}

ESTADO_BOTAO estado_botao(uint8_t atual, uint8_t * anterior) {
	if ((*anterior == 0) && (atual == 1)) {
		*anterior = atual;
		return AO_PRESSIONAR;
	} else if ((*anterior == 1) && (atual == 1)) {
		*anterior = atual;
		return PRESSIONADO;
	} else if ((*anterior == 1) && (atual == 0)) {
		*anterior = atual;
		return AO_SOLTAR;
	}
	*anterior = atual;
	return SOLTO;
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
