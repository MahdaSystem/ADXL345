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
#include "sdkconfig.h"
#include "esp_system.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"



/**
 ==================================================================================
                           ##### Private Functions #####                           
 ==================================================================================
 */

static int8_t
Platform_Init(void)
{
  i2c_config_t conf = {0};

  conf.mode = I2C_MODE_MASTER;
  conf.sda_io_num = ADXL345_SDA_GPIO;
  conf.sda_pullup_en = GPIO_PULLUP_DISABLE;
  conf.scl_io_num = ADXL345_SCL_GPIO;
  conf.scl_pullup_en = GPIO_PULLUP_DISABLE;
  conf.master.clk_speed = ADXL345_I2C_RATE;
  if (i2c_param_config(ADXL345_I2C_NUM, &conf) != ESP_OK)
    return -1;

  if (i2c_driver_install(ADXL345_I2C_NUM, conf.mode,
                         0, 0, 0) != ESP_OK)
    return -2;

  return 0;
}


static int8_t
Platform_DeInit(void)
{
  i2c_driver_delete(ADXL345_I2C_NUM);
  gpio_reset_pin(ADXL345_SDA_GPIO);
  gpio_reset_pin(ADXL345_SCL_GPIO);

  return 0;
}


static int8_t
Platform_WriteData(uint8_t Address, uint8_t *Data, uint8_t DataLen)
{
  i2c_cmd_handle_t ADXL345_i2c_cmd_handle = 0;

  Address <<= 1;
  Address &= 0xFE;

  ADXL345_i2c_cmd_handle = i2c_cmd_link_create();
  i2c_master_start(ADXL345_i2c_cmd_handle);
  i2c_master_write(ADXL345_i2c_cmd_handle, &Address, 1, 1);
  i2c_master_write(ADXL345_i2c_cmd_handle, Data, DataLen, 1);
  i2c_master_stop(ADXL345_i2c_cmd_handle);
  if (i2c_master_cmd_begin(ADXL345_I2C_NUM, ADXL345_i2c_cmd_handle,
                           1000 / portTICK_PERIOD_MS) != ESP_OK)
  {
    i2c_cmd_link_delete(ADXL345_i2c_cmd_handle);
    return -1;
  }

  i2c_cmd_link_delete(ADXL345_i2c_cmd_handle);
  return 0;
}


static int8_t
Platform_ReadData(uint8_t Address, uint8_t *Data, uint8_t DataLen)
{
  i2c_cmd_handle_t ADXL345_i2c_cmd_handle = 0;

  Address <<= 1;
  Address |= 0x01;

  ADXL345_i2c_cmd_handle = i2c_cmd_link_create();
  i2c_master_start(ADXL345_i2c_cmd_handle);
  i2c_master_write(ADXL345_i2c_cmd_handle, &Address, 1, 1);
  i2c_master_read(ADXL345_i2c_cmd_handle, Data, DataLen, I2C_MASTER_LAST_NACK);
  i2c_master_stop(ADXL345_i2c_cmd_handle);
  if (i2c_master_cmd_begin(ADXL345_I2C_NUM, ADXL345_i2c_cmd_handle,
                           1000 / portTICK_PERIOD_MS) != ESP_OK)
  {
    i2c_cmd_link_delete(ADXL345_i2c_cmd_handle);
    return -1;
  }

  i2c_cmd_link_delete(ADXL345_i2c_cmd_handle);
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
