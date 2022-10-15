/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Untref_IC_2022_2
  ******************************************************************************
*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
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
 RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */
char time[12];
char date[12];
char buffer1[12];
char hora = 23;
char minuto = 59;
char segundo = 50;
char anio = 22;    		//año 2022 -> 00010110
char mes = 8;     		//mes enero-> 00000001 (1) .... dici-> 00001100 (12)
char diasem = 0;   		//1 Lun , 2 Mar......, 0 Dom
char dia = 2;      	//dia 01 -> 00000001.....31 -> 00011111
char Alarma_hora = 0;
char Alarma_min = 0;
char Alarma_seg = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM3_Init(void);

/* USER CODE BEGIN PFP */
void Delay_useg(uint32_t timer);
char conv_int_hex (char cih);
void lcd_init (void);
void lcd_send_cmd (char cmd);
void send_to_lcd (char data, int rs);
void lcd_send_data (char data);
void lcd_send_string (char *str);
void lcd_put_cur(int row, int col);
void lcd_clear (void);
static void set_reloj(void);
void get_time(void);
void display_time (void);
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc);
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
  MX_RTC_Init();
  MX_TIM3_Init();

  /* USER CODE BEGIN 2 */
  #define Alarma_On    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
  #define Alarma_Off   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
  #define Row1_On      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
  #define Row1_Off     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
  #define Row2_On      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
  #define Row2_Off     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
  #define Row3_On      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
  #define Row3_Off     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
  #define Row4_On      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
  #define Row4_Off     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
  #define Column1_On   HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)==1
  #define Column1_Off  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)==0
  #define Column2_On   HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)==1
  #define Column2_Off  HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)==0
  #define Column3_On   HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)==1
  #define Column3_Off  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)==0
  #define Column4_On   HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)==1
  #define Column4_Off  HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)==0
  #define Led1_On      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
  #define Led1_Off     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
  #define Switch1_On   HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)==0
  #define Switch1_Off  HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)==1

  /*********** Define the LCD PINS below ****************/
  #define RS_Pin 			GPIO_PIN_15
  #define RS_GPIO_Port 		GPIOA
  //#define RW_Pin 			GPIO_PIN_2
  //#define RW_GPIO_Port 	GPIOA
  #define EN_Pin 			GPIO_PIN_5
  #define EN_GPIO_Port 		GPIOA
  #define D4_Pin 			GPIO_PIN_3
  #define D4_GPIO_Port 		GPIOB
  #define D5_Pin 			GPIO_PIN_4
  #define D5_GPIO_Port 		GPIOA
  #define D6_Pin 			GPIO_PIN_5
  #define D6_GPIO_Port 		GPIOB
  #define D7_Pin 			GPIO_PIN_3
  #define D7_GPIO_Port 		GPIOA

  /*
    ******************************************************************************
      * Display commands
      *
      * lcd_init ();
      * lcd_clear ();
      * lcd_put_cur(Row, Col);  // Position  (0, 0) --> Home position
      * lcd_send_string("HOLA MUNDO");
      * lcd_send_cmd(0x02);  	// Return Home
      * lcd_send_cmd(0x00);  	// Display Off
      * lcd_send_cmd(0x08);  	// Display On
      * lcd_send_cmd(0x0C);  	// Cursor Off  Blink Off
      * lcd_send_cmd(0x0E);  	// Cursor On   Blink Off
      * lcd_send_cmd(0x0D);  	// Cursor Off  Blink On
      * lcd_send_cmd(0x0F);  	// Cursor On   Blink On
      * lcd_send_cmd(0x06);  	// Position to Right  Display fix
      * lcd_send_cmd(0x04);  	// Position to Left   Display fix
      * lcd_send_cmd(0x07);  	// Position to Right  Cursor fix
      * lcd_send_cmd(0x05);  	// Position to Left   Cursor fix
      * lcd_send_cmd(0x10);  	// Cursor move to left
      * lcd_send_cmd(0x14);  	// Cursor move to right
      * lcd_send_cmd(0x18);  	// Display move to left
      * lcd_send_cmd(0x1C);  	// Display move to right
      ******************************************************************************
  */
  lcd_init ();
   // lcd_send_string ("FILA 1");
   // lcd_put_cur(1, 0);
   // lcd_send_string ("FILA 2");
    //HAL_Delay(500);
    //lcd_clear ();

  set_reloj();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	char cambiaseg = segundo;
    HAL_Delay(100);
	get_time();
	if(cambiaseg != segundo){display_time();}
	if(Switch1_On){
		Led1_On;
		lcd_clear ();
		lcd_send_string("Hello there");
		while(Switch1_On);
		}
	else {Led1_Off;}
	Row1_On;
	if(Column1_On){lcd_clear ();lcd_send_string("1");while(Column1_On);}
	if(Column2_On){lcd_clear ();lcd_send_string("2");while(Column2_On);}


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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  //DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
  DateToUpdate.Month = RTC_MONTH_JANUARY;
  DateToUpdate.Date = 0x1;
  DateToUpdate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_11|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_15|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA3 PA4 PA5
                           PA11 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_11|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PB15 PB3 PB4
                           PB5 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_15|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB11 PB12 PB13 PB14 */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA10 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void Delay_useg(uint32_t timer)
{
	timer = timer/1.5;
	while(timer--)
	    {
		__ASM volatile ("NOP");
	    }
}

char conv_int_hex (char cih)
{   char x = 0;
	if (cih<16){x = cih;}
	else if (cih<20){x = cih + 6;}
	else if (cih<30){x = cih + 12;}
	else if (cih<40){x = cih + 18;}
	else if (cih<50){x = cih + 24;}
	else if (cih<60){x = cih + 30;};
	return x;
}

void lcd_init (void)
{
	// 4 bit initialisation
	HAL_Delay(50);  			// wait for >40ms
	lcd_send_cmd (0x0F);		// BLINK ON
	HAL_Delay(5);  				// wait for >4.1ms
	lcd_send_cmd (0b10000010);	// 4bit mode
	HAL_Delay(5);  				// wait for >4.1ms
	lcd_send_cmd (0x01);  		// clear display
	HAL_Delay(5);  				// wait for >4.1ms
	lcd_send_cmd (0b00000010);	// Position home
	HAL_Delay(5);  				// wait for >4.1ms
	lcd_send_cmd (0b00001100);	// Cursor->off  Blinking->off
    HAL_Delay(5);  				// wait for >4.1ms
    //lcd_send_cmd (0x06); 		//Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
    lcd_send_cmd (0x28);		// 4 BIT  2 LINES  5X7
    HAL_Delay(5);
}

void lcd_send_cmd (char cmd)
{
    char datatosend;

    /* send upper nibble first */
    datatosend = ((cmd>>4)&0x0f);
    send_to_lcd(datatosend,0);  // RS must be 0 while sending command

    /* send Lower Nibble */
    datatosend = ((cmd)&0x0f);
	send_to_lcd(datatosend, 0);
}

void send_to_lcd (char data, int rs)
{
	HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, rs);  // rs = 1 for data, rs=0 for command

	/* write the data to the respective pin */
	HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, ((data>>3)&0x01));
	HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, ((data>>2)&0x01));
	HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, ((data>>1)&0x01));
	HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, ((data>>0)&0x01));

	/* Toggle EN PIN to send the data
	 * if the HCLK > 100 MHz, use the  20 us delay
	 * if the LCD still doesn't work, increase the delay to 50, 80 or 100..
	 */
	HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, 1);
	//HAL_Delay(1);
	Delay_useg(10);
	HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, 0);
	//HAL_Delay(1);
	Delay_useg(10);
}

void lcd_send_data (char data)
{
	char datatosend;

	/* send higher nibble */
	datatosend = ((data>>4)&0x0f);
	send_to_lcd(datatosend, 1);  // rs =1 for sending data

	/* send Lower nibble */
	datatosend = ((data)&0x0f);
	send_to_lcd(datatosend, 1);
}

void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}

void lcd_put_cur(int row, int col)
{
	char Fil_Col = 128;
	if (col>15) {col = 15;}
	switch (row) {
		case 0: Fil_Col = 128 + col;
			break;
		case 1: Fil_Col = 192 + col;
			break;
		case 2: Fil_Col = 148 + col;
			break;
		case 3: Fil_Col = 212 + col;
			break;
		default: break;
	}
    lcd_send_cmd (Fil_Col);
}

void lcd_clear (void)
{
	lcd_send_cmd(0x01);
	HAL_Delay(2);
}

static void set_reloj(void)
{
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  sTime.Hours = conv_int_hex (hora);
  sTime.Minutes = conv_int_hex (minuto);
  sTime.Seconds = conv_int_hex (segundo);

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  //DateToUpdate.WeekDay = diasem;
  DateToUpdate.Month = conv_int_hex (mes);
  DateToUpdate.Date = conv_int_hex (dia);
  DateToUpdate.Year = conv_int_hex (anio);

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  // Enable the Alarm A

  sAlarm.AlarmTime.Hours = conv_int_hex (Alarma_hora);
  sAlarm.AlarmTime.Minutes = conv_int_hex (Alarma_min);
  sAlarm.AlarmTime.Seconds = conv_int_hex (Alarma_seg);
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
}

void get_time(void)
{
 RTC_DateTypeDef gDate;
 RTC_TimeTypeDef gTime;
/* Get the RTC current Time */
 HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
/* Get the RTC current Date */
 HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);
/* Display time Format: hh:mm:ss */
 hora = gTime.Hours;
 minuto = gTime.Minutes;
 segundo = gTime.Seconds;
 sprintf(time,"%02u:%02u:%02u      ",hora,minuto,segundo);
/* Display date Format: dd-mm-yy */
 diasem = gDate.WeekDay;
 dia = gDate.Date;
 mes = gDate.Month;
 anio = gDate.Year;
 sprintf(date,"%02u/%02u/%02u",dia,mes,anio);
}

void display_time (void)
{
	lcd_put_cur(0, 0);
	switch (diasem) {
		case 0: {lcd_send_string("Domingo ");}	break;
		case 1: {lcd_send_string("Lunes   ");}	break;
		case 2: {lcd_send_string("Martes  ");}	break;
		case 3: {lcd_send_string("Miercol ");}	break;
		case 4: {lcd_send_string("Jueves  ");}	break;
		case 5: {lcd_send_string("Viernes ");}	break;
		case 6: {lcd_send_string("Sabado  ");}	break;
	}
 lcd_put_cur(0, 8);  			// Position  (Fila,Columna)
 lcd_send_string (date);
 lcd_put_cur(1, 0);  			// Position  (Fila,Columna)
 lcd_send_string (time);
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	Alarma_On; // turn on the LED
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
