/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include "icm20948.h"
#include "xor_algo.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
	 axises my_gyro;
	 axises my_accel;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */
void Compression(unsigned char *sizeOut, char *Message_size);

static void cs_high();
static void cs_low();
static void select_user_bank(userbank ub);
static uint8_t read_single_icm20948_reg(userbank ub, uint8_t reg);
static void write_single_icm20948_reg(userbank ub, uint8_t reg, uint8_t val);
static uint8_t *read_multiple_icm20948_reg(userbank ub, uint8_t reg, uint8_t len);
static void write_multiple_icm20948_reg(userbank ub, uint8_t reg, uint8_t *val, uint8_t len);


/* Main Functions */
// sensor init function.
// if sensor id is wrong, it is stuck in while.
void icm20948_init();


// 16 bits ADC value. raw data.
void icm20948_gyro_read(axises *data);
void icm20948_accel_read(axises *data);


// Convert 16 bits ADC value to their unit.
void icm20948_gyro_read_dps(axises *data);
void icm20948_accel_read_g(axises *data);


/* Sub Functions */
bool icm20948_who_am_i();


void icm20948_device_reset();
void ak09916_soft_reset();

void icm20948_wakeup();
void icm20948_sleep();

void icm20948_spi_slave_enable();

void icm20948_i2c_master_reset();
void icm20948_i2c_master_enable();
void icm20948_i2c_master_clk_frq(uint8_t config); // 0 - 15

void icm20948_clock_source(uint8_t source);
void icm20948_odr_align_enable();

void icm20948_gyro_low_pass_filter(uint8_t config);	 // 0 - 7
void icm20948_accel_low_pass_filter(uint8_t config); // 0 - 7

// Output Data Rate = 1.125kHz / (1 + divider)
void icm20948_gyro_sample_rate_divider(uint8_t divider);
void icm20948_accel_sample_rate_divider(uint16_t divider);
void ak09916_operation_mode_setting(operation_mode mode);

// Calibration before select full scale.
void icm20948_gyro_calibration();
void icm20948_accel_calibration();

void icm20948_gyro_full_scale_select(gyro_full_scale full_scale);
void icm20948_accel_full_scale_select(accel_full_scale full_scale);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*  General purpose Function to send a char array
	over the UART and to automatically send a new line
	character after it.
*/
void debugPrintln(UART_HandleTypeDef *uart_handle,
				  char _out[])
{
	HAL_UART_Transmit(uart_handle,(uint8_t *) _out,
					  strlen(_out), 60);
	char newline[2] = "\r\n";
	HAL_UART_Transmit(uart_handle, (uint8_t *) newline, 2, 10);
}

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
  MX_USART1_UART_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */


	icm20948_init();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{

			int count = 120;
			char Message[count];

		//reading data
		icm20948_gyro_read_dps(&my_gyro);
		icm20948_accel_read_g(&my_accel);
	  sprintf((char*)Message,"%f, %f, %f, %f, %f, %f",
			  my_accel.x,my_accel.y,my_accel.z,my_gyro.x,my_gyro.y,my_gyro.z);
	  Message[count]='\0';
	  unsigned CompressedSize = sizeof(Message)*7/8;
	  unsigned char CompressedBytes[CompressedSize];

	  //compressing data
//	     Compression(Message, Message);

	  debugPrintln(&huart1,Message);

//	  free(out);

	  //	  uint8_t enc[72];
//	  //encrypting original data
//	  aXor((char *)enc, (char *)buff2);
//	  debugPrintln(&huart1, (char *)enc);

	HAL_Delay(10);


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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

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
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

  /*Configure GPIO pin : PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
// Functions to connect to ICM
static float gyro_scale_factor;
static float accel_scale_factor;

/* Static Functions */

/* Main Functions */
void icm20948_init()
{
	while (!icm20948_who_am_i());

	icm20948_device_reset();
	icm20948_wakeup();

	icm20948_clock_source(1);
	icm20948_odr_align_enable();

	icm20948_spi_slave_enable();

	icm20948_gyro_low_pass_filter(0);
	icm20948_accel_low_pass_filter(0);

	icm20948_gyro_sample_rate_divider(0);
	icm20948_accel_sample_rate_divider(0);

	icm20948_gyro_calibration();
	icm20948_accel_calibration();

	icm20948_gyro_full_scale_select(_2000dps);
	icm20948_accel_full_scale_select(_16g);
}

void ak09916_init()
{
	icm20948_i2c_master_reset();
	icm20948_i2c_master_enable();
	icm20948_i2c_master_clk_frq(7);

//	while (!ak09916_who_am_i())
//		;

	ak09916_soft_reset();
	ak09916_operation_mode_setting(continuous_measurement_100hz);
}

void icm20948_gyro_read(axises *data)
{
	uint8_t *temp = read_multiple_icm20948_reg(ub_0, B0_GYRO_XOUT_H, 6);

	data->x = (int16_t)(temp[0] << 8 | temp[1]);
	data->y = (int16_t)(temp[2] << 8 | temp[3]);
	data->z = (int16_t)(temp[4] << 8 | temp[5]);
}

void icm20948_accel_read(axises *data)
{
	uint8_t *temp = read_multiple_icm20948_reg(ub_0, B0_ACCEL_XOUT_H, 6);

	data->x = (int16_t)(temp[0] << 8 | temp[1]);
	data->y = (int16_t)(temp[2] << 8 | temp[3]);
	data->z = (int16_t)(temp[4] << 8 | temp[5]) + accel_scale_factor;
	// Add scale factor because calibraiton function offset gravity acceleration.
}


void icm20948_gyro_read_dps(axises *data)
{
	icm20948_gyro_read(data);

	data->x /= gyro_scale_factor;
	data->y /= gyro_scale_factor;
	data->z /= gyro_scale_factor;
}

void icm20948_accel_read_g(axises *data)
{
	icm20948_accel_read(data);

	data->x /= accel_scale_factor;
	data->y /= accel_scale_factor;
	data->z /= accel_scale_factor;
}


/* Sub Functions */
bool icm20948_who_am_i()
{
	uint8_t icm20948_id = read_single_icm20948_reg(ub_0, B0_WHO_AM_I);

	if (icm20948_id == ICM20948_ID)
		return true;
	else
		return false;
}



void icm20948_device_reset()
{
	write_single_icm20948_reg(ub_0, B0_PWR_MGMT_1, 0x80 | 0x41);
	HAL_Delay(100);
}



void icm20948_wakeup()
{
	uint8_t new_val = read_single_icm20948_reg(ub_0, B0_PWR_MGMT_1);
	new_val &= 0xBF;

	write_single_icm20948_reg(ub_0, B0_PWR_MGMT_1, new_val);
	HAL_Delay(100);
}

void icm20948_sleep()
{
	uint8_t new_val = read_single_icm20948_reg(ub_0, B0_PWR_MGMT_1);
	new_val |= 0x40;

	write_single_icm20948_reg(ub_0, B0_PWR_MGMT_1, new_val);
	HAL_Delay(100);
}

void icm20948_spi_slave_enable()
{
	uint8_t new_val = read_single_icm20948_reg(ub_0, B0_USER_CTRL);
	new_val |= 0x10;

	write_single_icm20948_reg(ub_0, B0_USER_CTRL, new_val);
}

void icm20948_i2c_master_reset()
{
	uint8_t new_val = read_single_icm20948_reg(ub_0, B0_USER_CTRL);
	new_val |= 0x02;

	write_single_icm20948_reg(ub_0, B0_USER_CTRL, new_val);
}

void icm20948_i2c_master_enable()
{
	uint8_t new_val = read_single_icm20948_reg(ub_0, B0_USER_CTRL);
	new_val |= 0x20;

	write_single_icm20948_reg(ub_0, B0_USER_CTRL, new_val);
	HAL_Delay(100);
}

void icm20948_i2c_master_clk_frq(uint8_t config)
{
	uint8_t new_val = read_single_icm20948_reg(ub_3, B3_I2C_MST_CTRL);
	new_val |= config;

	write_single_icm20948_reg(ub_3, B3_I2C_MST_CTRL, new_val);
}

void icm20948_clock_source(uint8_t source)
{
	uint8_t new_val = read_single_icm20948_reg(ub_0, B0_PWR_MGMT_1);
	new_val |= source;

	write_single_icm20948_reg(ub_0, B0_PWR_MGMT_1, new_val);
}

void icm20948_odr_align_enable()
{
	write_single_icm20948_reg(ub_2, B2_ODR_ALIGN_EN, 0x01);
}

void icm20948_gyro_low_pass_filter(uint8_t config)
{
	uint8_t new_val = read_single_icm20948_reg(ub_2, B2_GYRO_CONFIG_1);
	new_val |= config << 3;

	write_single_icm20948_reg(ub_2, B2_GYRO_CONFIG_1, new_val);
}

void icm20948_accel_low_pass_filter(uint8_t config)
{
	uint8_t new_val = read_single_icm20948_reg(ub_2, B2_ACCEL_CONFIG);
	new_val |= config << 3;

	write_single_icm20948_reg(ub_2, B2_GYRO_CONFIG_1, new_val);
}

void icm20948_gyro_sample_rate_divider(uint8_t divider)
{
	write_single_icm20948_reg(ub_2, B2_GYRO_SMPLRT_DIV, divider);
}

void icm20948_accel_sample_rate_divider(uint16_t divider)
{
	uint8_t divider_1 = (uint8_t)(divider >> 8);
	uint8_t divider_2 = (uint8_t)(0x0F & divider);

	write_single_icm20948_reg(ub_2, B2_ACCEL_SMPLRT_DIV_1, divider_1);
	write_single_icm20948_reg(ub_2, B2_ACCEL_SMPLRT_DIV_2, divider_2);
}



void icm20948_gyro_calibration()
{
	axises temp;
	int32_t gyro_bias[3] = {0};
	uint8_t gyro_offset[6] = {0};

	for (int i = 0; i < 100; i++)
	{
		icm20948_gyro_read(&temp);
		gyro_bias[0] += temp.x;
		gyro_bias[1] += temp.y;
		gyro_bias[2] += temp.z;
	}

	gyro_bias[0] /= 100;
	gyro_bias[1] /= 100;
	gyro_bias[2] /= 100;

	// Construct the gyro biases for push to the hardware gyro bias registers,
	// which are reset to zero upon device startup.
	// Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format.
	// Biases are additive, so change sign on calculated average gyro biases
	gyro_offset[0] = (-gyro_bias[0] / 4 >> 8) & 0xFF;
	gyro_offset[1] = (-gyro_bias[0] / 4) & 0xFF;
	gyro_offset[2] = (-gyro_bias[1] / 4 >> 8) & 0xFF;
	gyro_offset[3] = (-gyro_bias[1] / 4) & 0xFF;
	gyro_offset[4] = (-gyro_bias[2] / 4 >> 8) & 0xFF;
	gyro_offset[5] = (-gyro_bias[2] / 4) & 0xFF;

	write_multiple_icm20948_reg(ub_2, B2_XG_OFFS_USRH, gyro_offset, 6);
}

void icm20948_accel_calibration()
{
	axises temp;
	uint8_t *temp2;
	uint8_t *temp3;
	uint8_t *temp4;

	int32_t accel_bias[3] = {0};
	int32_t accel_bias_reg[3] = {0};
	uint8_t accel_offset[6] = {0};

	for (int i = 0; i < 100; i++)
	{
		icm20948_accel_read(&temp);
		accel_bias[0] += temp.x;
		accel_bias[1] += temp.y;
		accel_bias[2] += temp.z;
	}

	accel_bias[0] /= 100;
	accel_bias[1] /= 100;
	accel_bias[2] /= 100;

	uint8_t mask_bit[3] = {0, 0, 0};

	temp2 = read_multiple_icm20948_reg(ub_1, B1_XA_OFFS_H, 2);
	accel_bias_reg[0] = (int32_t)(temp2[0] << 8 | temp2[1]);
	mask_bit[0] = temp2[1] & 0x01;

	temp3 = read_multiple_icm20948_reg(ub_1, B1_YA_OFFS_H, 2);
	accel_bias_reg[1] = (int32_t)(temp3[0] << 8 | temp3[1]);
	mask_bit[1] = temp3[1] & 0x01;

	temp4 = read_multiple_icm20948_reg(ub_1, B1_ZA_OFFS_H, 2);
	accel_bias_reg[2] = (int32_t)(temp4[0] << 8 | temp4[1]);
	mask_bit[2] = temp4[1] & 0x01;

	accel_bias_reg[0] -= (accel_bias[0] / 8);
	accel_bias_reg[1] -= (accel_bias[1] / 8);
	accel_bias_reg[2] -= (accel_bias[2] / 8);

	accel_offset[0] = (accel_bias_reg[0] >> 8) & 0xFF;
	accel_offset[1] = (accel_bias_reg[0]) & 0xFE;
	accel_offset[1] = accel_offset[1] | mask_bit[0];

	accel_offset[2] = (accel_bias_reg[1] >> 8) & 0xFF;
	accel_offset[3] = (accel_bias_reg[1]) & 0xFE;
	accel_offset[3] = accel_offset[3] | mask_bit[1];

	accel_offset[4] = (accel_bias_reg[2] >> 8) & 0xFF;
	accel_offset[5] = (accel_bias_reg[2]) & 0xFE;
	accel_offset[5] = accel_offset[5] | mask_bit[2];

	write_multiple_icm20948_reg(ub_1, B1_XA_OFFS_H, &accel_offset[0], 2);
	write_multiple_icm20948_reg(ub_1, B1_YA_OFFS_H, &accel_offset[2], 2);
	write_multiple_icm20948_reg(ub_1, B1_ZA_OFFS_H, &accel_offset[4], 2);
}

void icm20948_gyro_full_scale_select(gyro_full_scale full_scale)
{
	uint8_t new_val = read_single_icm20948_reg(ub_2, B2_GYRO_CONFIG_1);

	switch (full_scale)
	{
	case _250dps:
		new_val |= 0x00;
		gyro_scale_factor = 131.0;
		break;
	case _500dps:
		new_val |= 0x02;
		gyro_scale_factor = 65.5;
		break;
	case _1000dps:
		new_val |= 0x04;
		gyro_scale_factor = 32.8;
		break;
	case _2000dps:
		new_val |= 0x06;
		gyro_scale_factor = 16.4;
		break;
	}

	write_single_icm20948_reg(ub_2, B2_GYRO_CONFIG_1, new_val);
}

void icm20948_accel_full_scale_select(accel_full_scale full_scale)
{
	uint8_t new_val = read_single_icm20948_reg(ub_2, B2_ACCEL_CONFIG);

	switch (full_scale)
	{
	case _2g:
		new_val |= 0x00;
		accel_scale_factor = 16384;
		break;
	case _4g:
		new_val |= 0x02;
		accel_scale_factor = 8192;
		break;
	case _8g:
		new_val |= 0x04;
		accel_scale_factor = 4096;
		break;
	case _16g:
		new_val |= 0x06;
		accel_scale_factor = 2048;
		break;
	}

	write_single_icm20948_reg(ub_2, B2_ACCEL_CONFIG, new_val);
}

/* Static Functions */
static void cs_high()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
}

static void cs_low()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
}

static void select_user_bank(userbank ub)
{
	uint8_t write_reg[2];
	write_reg[0] = WRITE | REG_BANK_SEL;
	write_reg[1] = ub;

	cs_low();
	HAL_SPI_Transmit(&hspi2, write_reg, 2, 10);
	cs_high();
}

static uint8_t read_single_icm20948_reg(userbank ub, uint8_t reg)
{
	uint8_t read_reg = READ | reg;
	uint8_t reg_val;
	select_user_bank(ub);

	cs_low();
	HAL_SPI_Transmit(&hspi2, &read_reg, 1, 1000);
	HAL_SPI_Receive(&hspi2, &reg_val, 1, 1000);
	cs_high();

	return reg_val;
}

static void write_single_icm20948_reg(userbank ub, uint8_t reg, uint8_t val)
{
	uint8_t write_reg[2];
	write_reg[0] = WRITE | reg;
	write_reg[1] = val;

	select_user_bank(ub);

	cs_low();
	HAL_SPI_Transmit(&hspi2, write_reg, 2, 1000);
	cs_high();
}

static uint8_t *read_multiple_icm20948_reg(userbank ub, uint8_t reg, uint8_t len)
{
	uint8_t read_reg = READ | reg;
	static uint8_t reg_val[6];
	select_user_bank(ub);

	cs_low();
	HAL_SPI_Transmit(&hspi2, &read_reg, 1, 1000);
	HAL_SPI_Receive(&hspi2, reg_val, len, 1000);
	cs_high();

	return reg_val;
}

static void write_multiple_icm20948_reg(userbank ub, uint8_t reg, uint8_t *val, uint8_t len)
{
	uint8_t write_reg = WRITE | reg;
	select_user_bank(ub);

	cs_low();
	HAL_SPI_Transmit(&hspi2, &write_reg, 1, 1000);
	HAL_SPI_Transmit(&hspi2, val, len, 1000);
	cs_high();
}


//---------------new compression---------------
void Compression(unsigned char *sizeOut, char *Message_size) {

	unsigned long long Buffer = 0;
	char Bits = 0;

	while (*Message_size != 0) {
		Buffer |= (unsigned long long)(*Message_size++) << Bits;
		Bits += 7;
		if (Bits == 7 * 8) {
			while (Bits > 0) {
				*sizeOut++ = Buffer;
				Buffer >>= 8;
				Bits -= 8;
			}
			Bits = 0;
			Buffer = 0;
		}
	}
	char cbits = Bits;
	while (cbits > 0) {
		*sizeOut++ = Buffer;
		Buffer >>= 8;
		if(Bits<8){
			cbits =0;
		}
		else{
		Bits -= 8;
		cbits = Bits;
		}
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
	/* User can add his own implementation to report the HAL error return statae */
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
