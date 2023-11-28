/**
 **********************************************************************************
 * @file   ADXL345_platform.c
 * @author Hossein.M (https://github.com/Hossein-M98)
 * @brief  ADXL345 driver platform dependent part
 **********************************************************************************
 *
 * Copyright (c) 2023 Mahda Embedded System (MIT License)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **********************************************************************************
 */

/* Includes ---------------------------------------------------------------------*/
#include "ADXL345_platform.h"
#include "main.h"


/* Private Constants ------------------------------------------------------------*/
#if defined(ADXL345_PLATFORM_STM32_HAL)
#define ADXL345_TIMEOUT 100
#endif


/**
 ==================================================================================
                           ##### Private Functions #####                           
 ==================================================================================
 */

static int8_t
Platform_Init(void)
{
  return 0;
}


static int8_t
Platform_DeInit(void)
{
  return 0;
}


static int8_t
Platform_WriteData(uint8_t Address, uint8_t *Data, uint8_t DataLen)
{
  extern I2C_HandleTypeDef ADXL345_HI2C;

  Address <<= 1;
  if (HAL_I2C_Master_Transmit(&ADXL345_HI2C, Address,
                              Data, DataLen, ADXL345_TIMEOUT))
    return -1;

  return 0;
}


static int8_t
Platform_ReadData(uint8_t Address, uint8_t *Data, uint8_t DataLen)
{
  extern I2C_HandleTypeDef ADXL345_HI2C;

  Address <<= 1;
  if (HAL_I2C_Master_Receive(&ADXL345_HI2C, Address,
                             Data, DataLen, ADXL345_TIMEOUT))
    return -1;

  return 0;
}



/**
 ==================================================================================
                            ##### Public Functions #####                           
 ==================================================================================
 */

/**
 * @brief  Initialize platform device to communicate ADXL345.
 * @param  Handler: Pointer to handler
 * @retval None
 */
void
ADXL345_Platform_Init(ADXL345_Handler_t *Handler)
{
  Handler->PlatformI2CInit = Platform_Init;
  Handler->PlatformI2CDeInit = Platform_DeInit;
  Handler->PlatformI2CSend = Platform_WriteData;
  Handler->PlatformI2CReceive = Platform_ReadData;
}
