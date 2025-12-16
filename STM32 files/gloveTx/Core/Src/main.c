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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "icm20948.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
int16_t P;		// variable for pitch angle declared globally
int16_t Q;		// variable for roll angle declared globally
double thetaHat_acc_rad;
double phiHat_acc_rad;
uint8_t i;
double output_voltage_1;
int32_t CH1_DC=0;
char buffer[16];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */
int _write(int fd,char*ptr,int len)
	  	{
	  		HAL_UART_Transmit(&huart2,(uint8_t*)ptr,len,HAL_MAX_DELAY);
	  		return len;
	  	}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void cs_high();
static void cs_low();
void userbank_select(uint8_t ub);
void icm20948_read(userbank ub,uint8_t reg,uint8_t *data);
void icm20948_write(userbank ub,uint8_t reg,uint8_t data);
void icm20948_init();
void icm20948_accel_data(icm_20948_data * data);
void icm20948_gyro_data(icm_20948_data * data);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	icm_20948_data accel_data; //variable of type icm_20948_data defined for acquiring accelerometer data
	icm_20948_data gyro_data;  //variable of type icm_20948_data defined for acquiring gyroscope data
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
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  icm20948_init(); //Initialization function for ICM20948 called to initialise the IMU
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /*Functions to acquire accelerometer readings and gryoscope readings called in while loop*/
	icm20948_accel_data(&accel_data);
	icm20948_gyro_data(&gyro_data);
	i= HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1);
	//printf("%x\r\n",i);
    i = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);

    if (i == 1)
    {
        // Send: S" "B" "D
        sprintf(buffer, "S%dB%dD\r\n", (int)Q, P);
        HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 100);
    }

    if (i == 0)
    {
        P = 0;
        Q = 0;

        sprintf(buffer, "S%dB%dD\r\n", (int)Q, P);
        HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 100);

        printf("%d\r\n", P);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 38400;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 38400;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PC4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/*Function to SET the GPIO_OUT pin connected to CS pin of icm20948*/
static void cs_high()
{
	HAL_GPIO_WritePin(ICM20948_SPI_CS_PIN_PORT, ICM20948_SPI_CS_PIN_NUMBER, 1);
}

/*Function to RESET the GPIO_OUT pin connected to CS pin of icm20948*/
static void cs_low()
{
	HAL_GPIO_WritePin(ICM20948_SPI_CS_PIN_PORT, ICM20948_SPI_CS_PIN_NUMBER, 0);
}

/*Function to select the user bank to access specify icm20948 register*/
void userbank_select(uint8_t ub)
{
	cs_low(); //cs pin pulled to low state to start SPI Tx/Rx
	uint8_t reg = 0x7f;//USER_BANK_SELECT_REGISTER ADDRESS
	uint8_t user_bank = ub;//SELECTING USER BAK 0
	HAL_SPI_Transmit(ICM20948_SPI, &reg, 1, 100); //SPI transmit function called to write the userbank register
	HAL_SPI_Transmit(ICM20948_SPI, &user_bank, 1, 100);//SPI transmit function called to write the userbank select register
	cs_high();//cs pin pulled to high state to stop SPI Tx/Rx
}

//Function to read specific icm20948 register values and print using USART on serial monitor
void icm20948_read(userbank ub,uint8_t reg, uint8_t * data)
{
	userbank_select(ub); //userbank select function called to select a specific user bank
	cs_low();//cs pin pulled to low state to start SPI Tx/Rx
	reg = READ |reg; // address of the register to be accessed is ORed with 0x80 to read from the register
	uint8_t reg_val; // variable declared to store the values of the register accessed
	HAL_SPI_Transmit(ICM20948_SPI, &reg, 1, 100);//SPI transmit function called to with address of the register from which vales are to be read
	HAL_SPI_Receive(ICM20948_SPI, &reg_val, 1, 100);//SPI receive function called to read the selected register
	*data = reg_val;//pointer initialized acquire data globally from the icm20948_read function
	cs_high();//cs pin pulled to high state to stop SPI Tx/Rx
	//printf("Address:0x%x Register value: 0x%x \n\r",reg,reg_val);
}

//Function to write specific icm20948 register values and print using USART on serial monitor
void icm20948_write(userbank ub,uint8_t reg,uint8_t reg_val)
{
	userbank_select(ub);//userbank select function called to select a specific user bank
	cs_low();//cs pin pulled to low state to start SPI Tx/Rx
	reg = WRITE |reg;// address of the register to be accessed is ORed with 0x00 to write to the register
	HAL_SPI_Transmit(ICM20948_SPI, &reg, 1, 100);//SPI transmit function called to with address of the register to which data is to be passed
	HAL_SPI_Transmit(ICM20948_SPI, &reg_val, 1, 100);//SPI transmit function called to with data that is to be transmitted to the register
	cs_high();//cs pin pulled to high state to stop SPI Tx/Rx
	printf("Address:0x%x Register value: 0x%x \n\r\r",reg,reg_val);//values written to the register is printed using USART

}
void icm20948_init()
{
	//uint8_t temp_data;
	icm20948_write(ub_0,B0_PWR_MGMT_1,0xc1); //power management 1 register written to reset all the registers of the icm20948 and select clock
	icm20948_write(ub_0,B0_PWR_MGMT_1,0x01); ////power management 1 register written to wake up icm20948 and select clock
	icm20948_write(ub_0,B0_LP_CONFIG,0x30); //control Duty cycle mode of aceel,gyro and select SPI over I2C
	icm20948_write(ub_0,B0_PWR_MGMT_2,0x00);//enable accelerometer and gyroscope
	icm20948_write(ub_2,B2_ODR_ALIGN_EN,0x01);//ODR register set to sample data from gyroscope and accelerometer simultaneously

	icm20948_write(ub_2,B2_GYRO_SMPLRT_DIV,0x00);//sample rate of gyroscope set to maximum

	icm20948_write(ub_2,B2_GYRO_CONFIG_1,_250dps|0x01);//scale range of gyroscope set to 250dps
	icm20948_write(ub_2,B2_GYRO_CONFIG_2,0x00); //set averaging of the gryscope reading
	icm20948_write(ub_2,B2_ACCEL_SMPLRT_DIV_1,0x00); //set sample rate for accelerometer to maximum 11.2Ghz
	icm20948_write(ub_2,B2_ACCEL_SMPLRT_DIV_2,0x00);//set sample rate for accelerometer to maximum 11.2Ghz

	icm20948_write(ub_2,B2_ACCEL_CONFIG_1,_2g|0x01);//set the scale range of accelerometer to 2g

	//icm20948_read(ub_0,B0_USER_CTRL,&temp_data);
	//temp_data |=0x10;
	icm20948_write(ub_0,B0_USER_CTRL,0xd0);//register to enable disable DMP and FIFO
	//icm20948_read(ub_0,B0_USER_CTRL,&temp_data);
	//userbank_select(ub_0);

}

void icm20948_accel_data(icm_20948_data * data)  //function to read data from the accelerometer

{
	uint8_t data_accel_rx[6];//define an array to store high and low bytes from accelerometer
	icm20948_read(ub_0,B0_ACCEL_XOUT_H,&data_accel_rx[0]); //Read high byte of accelerometer from X axis
	icm20948_read(ub_0,B0_ACCEL_YOUT_H,&data_accel_rx[2]); //Read high byte of accelerometer from Y axis
	icm20948_read(ub_0,B0_ACCEL_ZOUT_H,&data_accel_rx[4]); //Read high byte of accelerometer from Z axis
	icm20948_read(ub_0,B0_ACCEL_XOUT_L,&data_accel_rx[1]); //Read Low byte of accelerometer from X axis
	icm20948_read(ub_0,B0_ACCEL_YOUT_L,&data_accel_rx[3]); //Read Low byte of accelerometer from Y axis
	icm20948_read(ub_0,B0_ACCEL_ZOUT_L,&data_accel_rx[5]); //Read Low byte of accelerometer from Z axis
	data->x_accel = (int16_t)((data_accel_rx[0]<<8)|(data_accel_rx[1]));//combine the 2 byte data acquired from accelerometer at X axis
	data->y_accel = (int16_t)((data_accel_rx[2]<<8)|(data_accel_rx[3]));//combine the 2 byte data acquired from accelerometer at Y axis
	data->z_accel = (int16_t)((data_accel_rx[4]<<8)|(data_accel_rx[5]));//combine the 2 byte data acquired from accelerometer at Z axis

	//convert the RAW DATA value to m/s^2 in terms of g
	data->x_accel = (data->x_accel/65536)*4.0*9.8;
	data->y_accel = (data->y_accel/65536)*4.0*9.8;
	data->z_accel = (data->z_accel/65536)*4.0*9.8;

	//print acceleration in 3-axis
	//printf("Accelerometer : %.2f  %.2f 	%.2f \n\r",data->x_accel,data->y_accel,data->z_accel);

	//estimate angles using accelerometer measurements
	  phiHat_acc_rad = atanf(data->y_accel / data->z_accel)*(180/3.14); //Roll angle
	  thetaHat_acc_rad = asinf(data->x_accel )*(180/3.14); //Pitch angle

	 /*Print translational acceleration of 3-axes from accelerometer*/
	//printf("Linear Accelerations : %.2f  %.2f	%.2f\n\r",data->x_accel,data->y_accel,data->z_accel);

}

void icm20948_gyro_data(icm_20948_data * data)

{
	uint8_t data_gyro_rx[6];//define an array to store high and low bytes from gyroscope
	icm20948_read(ub_0,B0_GYRO_XOUT_H,&data_gyro_rx[0]);//Read high byte of gyroscope from X axis
	icm20948_read(ub_0,B0_GYRO_YOUT_H,&data_gyro_rx[2]);//Read high byte of gyroscope from Y axis
	icm20948_read(ub_0,B0_GYRO_ZOUT_H,&data_gyro_rx[4]);//Read high byte of gyroscope from Z axis
	icm20948_read(ub_0,B0_GYRO_XOUT_L,&data_gyro_rx[1]);//Read low byte of gyroscope from X axis
	icm20948_read(ub_0,B0_GYRO_YOUT_L,&data_gyro_rx[3]);//Read low byte of gyroscope from Y axis
	icm20948_read(ub_0,B0_GYRO_ZOUT_L,&data_gyro_rx[5]);//Read low byte of gyroscope from Z axis
	data->x_gyro = (int16_t)((data_gyro_rx[0]<<8)|(data_gyro_rx[1]));//combine the 2 byte data acquired from gyroscope at X axis
	data->y_gyro = (int16_t)((data_gyro_rx[2]<<8)|(data_gyro_rx[3]));//combine the 2 byte data acquired from gyroscope at Y axis
	data->z_gyro = (int16_t)((data_gyro_rx[4]<<8)|(data_gyro_rx[5]));//combine the 2 byte data acquired from gyroscope at Z axis

	/*convert the RAW DATA value to rad/s from dps*/
	data->x_gyro = ((data->x_gyro)/131)*0.01744444f;
	data->y_gyro = ((data->y_gyro)/131)*0.01744444f;
	data->z_gyro = ((data->z_gyro)/131)*0.01744444f;

	/*transform body rates to euler rates*/
	double phiHat_rad = 0.0f; //variable declared to acquire roll angle
	double thetaHat_rad = 0.0f; //variable declared to acquire pitch angle
	double a=0.0f,b=0.0f; //variables to take mean of the euler angle value

	/*Acquire the derivatives of the euler angles from the gryoscope*/
	double phiDot_rps = data->x_gyro + tanf(thetaHat_rad) * (sinf(phiHat_rad) * data->y_gyro + cosf(phiHat_rad) * data->z_gyro);
	double thetaDot_rps = cosf(phiHat_rad) * data->y_gyro - sinf(phiHat_rad) * data->z_gyro;

	/*combine accelerometer estimates with integral of gyroscope readings*/
	for(int i=0;i<100;i++)
	{
	phiHat_rad = COMP_FILT_ALPHA * phiHat_acc_rad + (1.0f - COMP_FILT_ALPHA) * (phiHat_rad + (SAMPLE_TIME_MS_USB / 1000.0f) * phiDot_rps);

	thetaHat_rad = COMP_FILT_ALPHA * thetaHat_acc_rad + (1.0f - COMP_FILT_ALPHA) * (thetaHat_rad + (SAMPLE_TIME_MS_USB / 1000.0f) * thetaDot_rps);
	a=a+phiHat_rad;
	b=b+thetaHat_rad;
	}

	/*Variables defined to print pitch and roll*/
	P = (int16_t)(a/100);
	Q = (int16_t)(b/100);

	/*Print Angular velocity of 3-axes from gyroscope*/
	//printf("Angular Velocity : %.2f  %.2f	%.2f \n\r",data->x_gyro,data->y_gyro,data->z_gyro);

	/*Print Euler angles from complementary filter*/
	//printf("roll : %.2f pitch: %.2f \n\n\r\r",P,Q);

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
