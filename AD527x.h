/*
 * Single-header pure C AD527x driver stub(basic, incomplete) for STM32 with HAL
 *
 * Bits taken from various sources
 * License: MIT(http://opensource.org/licenses/MIT)
 *
 */

#ifndef INC_AD527X_H_
#define INC_AD527X_H_

#include "main.h" // Your project's main should include HAL

#define AD527X_ADDRESS_GND 0x2F	// Default I2C address iff ADDR=GND
#define AD527X_ADDRESS_VDD 0x2C	// I2C address iff ADDR=VDD
#define AD527X_ADDRESS_FLOAT 0x2E // I2C address if ADDR=floating, best not to use

#define AD527X_COMMAND_NOP 0x00

#define AD527X_RDAC_WRITE 0x01 // Must be unlocked first
#define AD527X_RDAC_READ 0x02 // Read the wiper register
#define AD527X_TP_WRITE 0x03 // Store current wiper to 50-write OTP
#define AD527X_RDAC_REFRESH 0x04 // Reset wiper to OTP setting
#define AD527X_TP_WIPER_READ 0x05 // Read 50-write OTP in next frame, data bits 5:0, 0x0 is reserved, 0x01 is first, 0x32 is 50th
#define AD527X_TP_LAST_USED 0x06
#define AD527X_CONTROL_WRITE 0x07 // 2:0 are control
#define AD527X_CONTROL_READ 0x08 // Data = don't care
#define AD527X_SHUTDOWN 0x09 // b0 set = shutdown, unset = normal

#define AD527X_TP_WRITE_ENABLE 0x01 // Only 50 writes possible
#define AD527X_RDAC_WIPER_WRITE_ENABLE 0x02 // Enable writing to wiper
#define AD527X_RDAC_CALIB_DISABLE 0x04 // Disable calibration by clearing C2, default is enabled
#define AD527X_TP_WRITE_SUCCESS 0x08 // Set if OTP is burnt successfully

HAL_StatusTypeDef AD527X_write_data(I2C_HandleTypeDef *i2c, uint8_t addr, uint16_t cmd, uint16_t data)
{
	uint8_t arr[2] =
	{ (cmd << 2) | (0x0003 & (data >> 8)), 0x00FF & data };

	return HAL_I2C_Master_Transmit(i2c, addr << 1, arr, 2, 10);
}

uint16_t AD527X_read_rdac(I2C_HandleTypeDef *i2c, uint8_t addr)
{
	uint8_t ret[2] =
	{ 0 };

	uint8_t arr[2] =
	{ AD527X_RDAC_READ << 2, 0x00 };

	if (HAL_I2C_Master_Transmit(i2c, addr << 1, arr, 2, 10) != HAL_OK)
		return 1025; // Impossible value for 12-bit devices, but possible for 8-bit, so beware

	if (HAL_I2C_Master_Receive(i2c, addr << 1, (uint8_t*) ret, 2, 10) != HAL_OK)
		return 1025;

	return (ret[0] << 8) | ret[1];
}

HAL_StatusTypeDef AD527X_enable_wiper(I2C_HandleTypeDef *i2c, uint8_t addr)
{
	return AD527X_write_data(i2c, addr, AD527X_CONTROL_WRITE, AD527X_RDAC_WIPER_WRITE_ENABLE);
}

HAL_StatusTypeDef AD527X_write_rdac(I2C_HandleTypeDef *i2c, uint8_t addr, uint16_t val)
{
	return AD527X_write_data(i2c, addr, AD527X_RDAC_WRITE, val);
}

#endif /* INC_AD527X_H_ */
