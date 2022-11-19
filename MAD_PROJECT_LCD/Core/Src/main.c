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
#include "rng.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "ILI9341_Touchscreen.h"

#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include "am2320.h"
#include "snow_tiger.h"
#include "git.h"
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
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t count_1;
uint32_t sec;
uint32_t min;
void SetScreen(void);
void displayTime(uint32_t,int);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

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
  MX_USART3_UART_Init();
  MX_SPI5_Init();
  MX_TIM1_Init();
  MX_RNG_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  int posSelected = 3;
  int posCopy = 11;
  int isCopy =0;
  int x_axis = 50;
  int screen = 0;
  // screen 0 -> LIST PAGE
  // screen 1 -> PLAY PAGE
  int isPlay = 0;




  struct SlotMusic {
	char *Name;
    int Length;	//(sec)
  };
  struct SlotMusic s[10];
  s[0].Name = "Default1";
  s[0].Length = 8;
  s[1].Name = "Default2";
  s[1].Length = 210;
  s[2].Name = "Empty";
  s[3].Name = "Empty";
  s[4].Name = "Empty";
  s[5].Name = "Empty";
  s[6].Name = "Empty";
  s[7].Name = "Empty";
  s[8].Name = "Empty";
  s[9].Name = "Empty";

	if(screen == 1){
		 ILI9341_Draw_Text(s[posSelected].Name,100 ,50, WHITE,2, BLACK);
	}

  ILI9341_Init();//initial driver setup to drive ili9341
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


SetScreen();


  while (1)
  {
	    //Touchpad
	  	uint16_t x_pos = 0;
	 	uint16_t y_pos = 0;
	  	int16_t position_array[2];
	  	if(TP_Touchpad_Pressed()){
	  	   if(TP_Read_Coordinates(position_array) == TOUCHPAD_DATA_OK){
	  		   y_pos = 240-position_array[0];
	  		   x_pos = position_array[1];

	  		   //Up
	  		   if(x_pos>=10 && x_pos<=45 && y_pos>=70 && y_pos<=115 && screen ==0){
	  			   if(posSelected >0){
		  			   posSelected -=1;
	  			   }
	  		   }

	  		   //Down
	  		   else if(x_pos>=10 && x_pos<=45 && y_pos>=120 && y_pos<=155 && screen ==0){
	  			   if(posSelected <9){
	  				 posSelected +=1;
	  			   	   }
	  		   }

	  		   //PLAY(GO TO PLAY PAGE)
	  		   else if(x_pos>=30 && x_pos<=90 && y_pos>=200 && y_pos<=230 && s[posSelected].Name != "Empty"&& !isCopy && screen ==0){
	  			   screen = 1;
	  			   sec = 0;
		  	       __HAL_TIM_SET_COUNTER(&htim1,0);
	  			   SetScreen();
	  		   }


	  		   //COPY
	  		   else if(x_pos>=100 && x_pos<=160 && y_pos>=200 && y_pos<=230 && s[posSelected].Name != "Empty" &&  !isCopy && screen ==0){
	  			   posCopy = posSelected;
	  			   isCopy = 1;


	  		   }

	  		 //PASTE
	  		   else if(x_pos>=100 && x_pos<=160 && y_pos>=200 && y_pos<=230 && isCopy && screen ==0){
	   			   s[posSelected].Name = s[posCopy].Name;
	   			   s[posSelected].Length = s[posCopy].Length;
	   			   isCopy = 0;
	   			   posCopy = 11;

	  		   }

	  		   //CANCEL COPY
	  		   else if(x_pos>=30 && x_pos<=90 && y_pos>=200 && y_pos<=230 && isCopy && screen ==0){
	  			   isCopy = 0;
	  			   posCopy = 11;
	  		   }

	  		   //DELETE
	  		   else if(x_pos>=170 && x_pos<=230 && y_pos>=200 && y_pos<=230 && screen ==0){
	  			   s[posSelected].Name = "Empty";
	  		   }

	  		  //PLAY MUSIC
	  		   else if(x_pos>=30 && x_pos<=70 && y_pos>=110 && y_pos<=150 && isPlay == 0 && screen ==1){
	  			  	isPlay =1;
	  		   }

	  		   //BACK TO PLAY PAGE
	  		   else if(x_pos>=240 && x_pos<=300 && y_pos>=200 && y_pos<=230 && screen == 0){
	  			   screen = 1;
	  			   SetScreen();
	  		   }

	  		   //BACK TO LIST PAGE
	  		   else if(x_pos>=240 && x_pos<=300 && y_pos>=180 && y_pos<=210 && screen == 1){
	  			   screen = 0;
	  			   SetScreen();
	  		   }
	  	   }
	  	}



	  	//PAGE LIST OF MUSIC
	  	if(screen == 0){


	  		//UP/DOWN button
	  		ILI9341_Draw_Rectangle(10, 70, 35, 35, WHITE);
			ILI9341_Draw_Text("/\\",20 ,80, BLACK,2, WHITE);
	  		ILI9341_Draw_Rectangle(10, 120, 35, 35, WHITE);
			ILI9341_Draw_Text("\\/",20 ,130, BLACK,2, WHITE);


			//MENU (PLAY/COPY/DELETE/BACK)
	  		if(s[posSelected].Name != "Empty" && !isCopy){
				ILI9341_Draw_Rectangle(30, 200, 60, 30, RED);
				ILI9341_Draw_Text("PLAY",40 ,200, BLACK,2, RED);

				ILI9341_Draw_Rectangle(170, 200, 60, 30, RED);
				ILI9341_Draw_Text("DELETE",180 ,210, BLACK,1.5, RED);
			}
			else if (!isCopy){
				ILI9341_Draw_Rectangle(30, 200, 60, 30, WHITE);
				ILI9341_Draw_Text("PLAY",40 ,200, BLACK,2, WHITE);

				ILI9341_Draw_Rectangle(170, 200, 60, 30, WHITE);
				ILI9341_Draw_Text("DELETE",180 ,210, BLACK,1, WHITE);
			}

			if(s[posSelected].Name != "Empty"  && !isCopy){
				ILI9341_Draw_Rectangle(100, 200, 60, 30, RED);
				ILI9341_Draw_Text("COPY",110 ,200, BLACK,2, RED);
			}
			else if (!isCopy){
				ILI9341_Draw_Rectangle(100, 200, 60, 30, WHITE);
				ILI9341_Draw_Text("COPY",110 ,200, BLACK,2, WHITE);;
			}
			ILI9341_Draw_Rectangle(240, 200, 60, 30, RED);
			ILI9341_Draw_Text("BACK",260 ,210, BLACK,1, RED);

			if(isCopy){
				ILI9341_Draw_Rectangle(30, 200, 60, 30, RED);
				ILI9341_Draw_Text("X",60 ,200, BLACK,2, RED);

				ILI9341_Draw_Rectangle(100, 200, 60, 30, RED);
				ILI9341_Draw_Text("PASTE",120 ,210, BLACK,1, RED);

				ILI9341_Draw_Rectangle(170, 200, 60, 30, WHITE);
				ILI9341_Draw_Text("DELETE",180 ,210, BLACK,1.5, WHITE);
			}


			//List Music 1-5
			if(posSelected < 5){
			for(int i=0;i<5;i++){
				if(i == posSelected && i != posCopy){
					ILI9341_Draw_Rectangle(60, x_axis, 230, 20, YELLOW);
					ILI9341_Draw_Text(s[i].Name,80 ,x_axis, BLACK,2, YELLOW);
				}
				else if(i == posCopy){
		  			ILI9341_Draw_Rectangle(60, x_axis, 230, 20, GREEN);
		  			ILI9341_Draw_Text(s[i].Name,80 ,x_axis, BLACK,2, GREEN);
				}
				else{
					ILI9341_Draw_Rectangle(60, x_axis, 230, 20, WHITE);
					ILI9341_Draw_Text(s[i].Name,80 ,x_axis, BLACK,2, WHITE);
				}
				if(x_axis>=170){
					x_axis=20;
				}
				x_axis+=30;
			}
			}


			//List Music 6-10
			else{
				for(int j=5;j<10;j++){
						if(j == posSelected){
							ILI9341_Draw_Rectangle(60, x_axis, 230, 20, YELLOW);
							ILI9341_Draw_Text(s[j].Name,80 ,x_axis, BLACK,2, YELLOW);

						}
						else{
							ILI9341_Draw_Rectangle(60, x_axis, 230, 20, WHITE);
							ILI9341_Draw_Text(s[j].Name,80 ,x_axis, BLACK,2, WHITE);
						}
						if(x_axis>=170){
							x_axis=20;
						}
						x_axis+=30;
					}
			}


			HAL_Delay(10);
	  	}

	  	//PLAY PAGE
	  	else if(screen == 1){

	  		// TITLE
	  		ILI9341_Draw_Text(s[posSelected].Name,100 ,50, WHITE,2, BLACK);


		  	//PLAY/PAUSE button
		  	ILI9341_Draw_Rectangle(30, 110, 40, 40, WHITE);
			ILI9341_Draw_Text("PALY/",35 ,120, BLACK,1, WHITE);
			ILI9341_Draw_Text("PAUSE",35 ,130, BLACK,1, WHITE);


		  	//Music level range
	  		ILI9341_Draw_Rectangle(90, 110, 10, 20, WHITE);
	  		ILI9341_Draw_Rectangle(110, 110, 10, 20, WHITE);
	  		ILI9341_Draw_Rectangle(130, 110, 10, 20, WHITE);
	  		ILI9341_Draw_Rectangle(150, 110, 10, 20, WHITE);
	  		ILI9341_Draw_Rectangle(170, 110, 10, 20, WHITE);
	  		ILI9341_Draw_Rectangle(190, 110, 10, 20, WHITE);
	  		ILI9341_Draw_Rectangle(210, 110, 10, 20, WHITE);
	  		ILI9341_Draw_Rectangle(230, 110, 10, 20, WHITE);


		  	displayTime(sec,1);
		  	displayTime(s[posSelected].Length,0);


	  		//BACK
	  		ILI9341_Draw_Rectangle(240, 200, 60, 30, RED);
			ILI9341_Draw_Text("BACK",260 ,210, BLACK,1, RED);


	  		int level = s[posSelected].Length/8;


	  	    if(isPlay == 1){

	  	    	//start timer

	  	    	HAL_TIM_Base_Start_IT(&htim1);
		  	    while (1){
		  	    	displayTime(sec,1);

		  	    	if(sec>=(level)){
		  		  		ILI9341_Draw_Rectangle(90, 110, 10, 20, RED);
		  	    	}
		  	    	if(sec>=(level*2)){
		  		  		ILI9341_Draw_Rectangle(110, 110, 10, 20, RED);
		  	    	}
		  	    	if(sec>=(level*3)){
		  	    		ILI9341_Draw_Rectangle(130, 110, 10, 20, RED);
		  	    	}
		  	    	if(sec>=(level*4)){
		  		  		ILI9341_Draw_Rectangle(150, 110, 10, 20, RED);
		  	    	}
		  	    	if(sec>=(level*5)){
		  		  		ILI9341_Draw_Rectangle(170, 110, 10, 20, RED);
		  	    	}
		  	    	if(sec>=(level*6)){
		  	    		ILI9341_Draw_Rectangle(190, 110, 10, 20, RED);
		  	    	}
		  	    	if(sec>=(level*7)){
		  	    		ILI9341_Draw_Rectangle(210, 110, 10, 20, RED);
		  	    	}

   		  		   //END
   		  		   if(sec == s[posSelected].Length){
   		  			   isPlay =0;
   		  			   ILI9341_Draw_Rectangle(230, 110, 10, 20, RED);
   		  			   HAL_Delay(300);
   		  			   HAL_TIM_Base_Stop_IT(&htim1);
   		  			   sec = 0;
   			  	       __HAL_TIM_SET_COUNTER(&htim1,0);
   		  			   break;
   		  		   }

		  	    	if(TP_Touchpad_Pressed()){
		  	    		  	   if(TP_Read_Coordinates(position_array) == TOUCHPAD_DATA_OK){
		  	    		  		   y_pos = 240-position_array[0];
		  	    		  		   x_pos = position_array[1];

		  	    		  		   //PAUSE
		  	    		  		   if(x_pos>=30 && x_pos<=70 && y_pos>=110 && y_pos<=150){
		  	    		  			   isPlay =0;
		  	    				  	   HAL_TIM_Base_Stop_IT(&htim1);
		  	    		  			   break;
		  	    		  		   }

		  	    		  		   //BACK TO LIST PAGE
		  	    		  		   if(x_pos>=240 && x_pos<=300 && y_pos>=180 && y_pos<=210 && screen == 1){
		  	    				  	   isPlay = 0;
		  	    		  			   screen = 0;
		  	    				  	   HAL_TIM_Base_Stop_IT(&htim1);
		  	    		  			   break;
		  	    		  		   }

		  	    		  	   }
		  	    	}
		  	    	HAL_Delay(400);

		  	    }
	  	    }


			HAL_Delay(10);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 200;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
extern void displayTime (uint32_t sec,int isStart){
	char str[80];
	sprintf(str, "%01d.%02d", sec/60,sec%60);
	//HAL_UART_Transmit(&huart3, (uint8_t*) str, strlen(str), 100);
	if(!isStart){
  		ILI9341_Draw_Text(str,200 ,140, WHITE,2, BLACK);
	}
	else{
		ILI9341_Draw_Text(str,80 ,140, WHITE,2, BLACK);

	}
}

void SetScreen(void){
	 //Screen Setting
	 ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1);
	 ILI9341_Fill_Screen(BLACK);

	//TOPIC
	 ILI9341_Draw_Text("D",90 ,15, RED,2, BLACK);
	 ILI9341_Draw_Text("N",105 ,15, YELLOW,2, BLACK);
	 ILI9341_Draw_Text("K",120 ,15, BLUE,2, BLACK);
	 ILI9341_Draw_Text("MUSIC BOX",140 ,15, WHITE,2, BLACK);
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
  while(1)
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
