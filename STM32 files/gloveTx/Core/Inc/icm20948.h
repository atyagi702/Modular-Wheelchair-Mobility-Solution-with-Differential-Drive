/*
 * icm20948.h
 *
 *  Created on: Apr 24, 2023
 *  Author: Anubhav Tyagi
 *  Email : anubhavtyagi702@gmail.com
 */

#ifndef INC_ICM20948_H_
#define INC_ICM20948_H_



#endif /* INC_ICM20948_H_ */
#define READ							0x80
#define WRITE							0x00
#define ICM20948_SPI					(&hspi1)

#define ICM20948_SPI_CS_PIN_PORT		GPIOC
#define ICM20948_SPI_CS_PIN_NUMBER		GPIO_PIN_4

//static float gyro_scale_factor;
//static float accel_scale_factor;

/* Typedefs */
#include "stdio.h"
#include"math.h"
typedef enum
{
	ub_0 = 0 << 4,
	ub_1 = 1 << 4,
	ub_2 = 2 << 4,
	ub_3 = 3 << 4
} userbank;

typedef enum
{
	_250dps,
	_500dps,
	_1000dps,
	_2000dps
} GYRO_RANGE_VALUE;

typedef enum
{
	_2g,
	_4g,
	_8g,
	_16g
} ACCEL_RANGE_VALUE;

typedef struct
{
	int x_accel;
	int y_accel;
	int z_accel;
	int x_gyro;
	int y_gyro;
	int z_gyro;

} icm_20948_data;

typedef enum
{
	power_down_mode = 0,
	single_measurement_mode = 1,
	continuous_measurement_10hz = 2,
	continuous_measurement_20hz = 4,
	continuous_measurement_50hz = 6,
	continuous_measurement_100hz = 8
} operation_mode;

/* ICM-20948 Registers */
#define ICM20948_ID						0xEA
#define REG_BANK_SEL					0x7F

// USER BANK 0
    // Register name          Register address in Hex
#define B0_WHO_AM_I						0x00     		//used to identify the device in use
#define B0_USER_CTRL					0x03 			//control DMP features;I2C mode;SPI only mode;SRAM,DMP&I2C_MST_RST
#define B0_LP_CONFIG					0x05			//control Duty cycle mode of aceel,gyro and I2C
#define B0_PWR_MGMT_1					0x06			//sleep wake function,low power mode function, disable temp sensor,CLK select
#define B0_PWR_MGMT_2					0x07			//enable disable accel,gryo
#define B0_ACCEL_XOUT_H					0x2D			//High Byte of Accelerometer x-axis data.
#define B0_ACCEL_XOUT_L					0x2E			//Low Byte of Accelerometer x-axis data.
#define B0_ACCEL_YOUT_H					0x2F			//High Byte of Accelerometer y-axis data.
#define B0_ACCEL_YOUT_L					0x30			//Low Byte of Accelerometer y-axis data.
#define B0_ACCEL_ZOUT_H					0x31			//High Byte of Accelerometer Z-axis data.
#define B0_ACCEL_ZOUT_L					0x32			//Low Byte of Accelerometer Z-axis data.
#define B0_GYRO_XOUT_H					0x33			//High Byte of Gyroscope x-axis data.
#define B0_GYRO_XOUT_L					0x34			//Low Byte of Gyroscope x-axis data.
#define B0_GYRO_YOUT_H					0x35			//High Byte of Gyroscope y-axis data.
#define B0_GYRO_YOUT_L					0x36			//Low Byte of Gyroscope y-axis data.
#define B0_GYRO_ZOUT_H					0x37			//High Byte of Gyroscope Z-axis data.
#define B0_GYRO_ZOUT_L					0x38			//Low Byte of Gyroscope Z-axis data.

// USER BANK 2
#define B2_GYRO_SMPLRT_DIV				0x00
#define B2_GYRO_CONFIG_1				0x01
#define B2_GYRO_CONFIG_2				0x02
#define B2_ODR_ALIGN_EN					0x09
#define B2_ACCEL_SMPLRT_DIV_1			0x10
#define B2_ACCEL_SMPLRT_DIV_2			0x11
#define B2_ACCEL_INTEL_CTRL				0x12
#define B2_ACCEL_CONFIG_1				0x14
#define B2_ACCEL_CONFIG_2				0x15
#define B2_MOD_CTRL_USR					0X54


#define SAMPLE_TIME_MS_USB  20
#define SAMPLE_TIME_MS_BAR  125
#define SAMPLE_TIME_MS_LED  250
#define SAMPLE_TIME_MS_ATT  10

#define COMP_FILT_ALPHA     0.0500000000  //alpha of complimentary filter
#define RAD_TO_DEG			57.2957795131 //radian to degrees conversion (180/Pi)
#define G				9.8100000000 //accelaration due to gravity
