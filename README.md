# ADXL345 Library
ADXL345 Accelerometer sensor driver.
- Full feature
- Support for I2C communication protocol
- Easy to port

## Hardware Support
In the current version, the library uses I2C communication protocol. It is easy to port this library to any platform. But now it is ready for use in:
- STM32 (HAL)
- ESP32 (esp-idf)

## How To Use
1. Add `ADXL345.h` and `ADXL345.c` files to your project.  It is optional to use `ADXL345_platform.h` and `ADXL345_platform.c` files (open and config `ADXL345_platform.h` file).
2. Initialize platform-dependent part of handler.
4. Call `ADXL345_Init()`.
5. Call `ADXL345_SetAddressI2C()`.
6. Call other functions and enjoy.

## Example
<details>
<summary>Using ADXL345_platform files</summary>

```c
#include <stdio.h>
#include "ADXL345.h"
#include "ADXL345_platform.h"

ADXL345_Handler_t Handler;

void ADXL345_INT1_INT2_ISR(void)
{
  ADXL345_IRQ_Handler(&Handler);
}

int8_t InterruptCallback(ADXL345_Interrupt_t Interrupt)
{
  switch (Interrupt)
  {
  case ADXL345_INTERRUPT_OVERRUN:
    printf("INTERRUPT_OVERRUN\r\n");
    break;

  case ADXL345_INTERRUPT_WATERMARK:
    printf("INTERRUPT_WATERMARK\r\n");
    break;

  case ADXL345_INTERRUPT_FREE_FALL:
    printf("INTERRUPT_FREE_FALL\r\n");
    break;

  case ADXL345_INTERRUPT_INACTIVITY:
    printf("INTERRUPT_INACTIVITY\r\n");
    break;

  case ADXL345_INTERRUPT_ACTIVITY:
    printf("INTERRUPT_ACTIVITY\r\n");
    break;

  case ADXL345_INTERRUPT_DOUBLE_TAP:
    printf("INTERRUPT_DOUBLE_TAP\r\n");
    break;

  case ADXL345_INTERRUPT_SINGLE_TAP:
    printf("INTERRUPT_SINGLE_TAP\r\n");
    break;

  case ADXL345_INTERRUPT_DATA_READY:
    printf("INTERRUPT_DATA_READY\r\n");
    break;
  }

  return 0;
}


int main(void)
{
  // Set External Interrupt to call ADXL345_INT1_INT2_ISR function...

  ADXL345_Platform_Init(&Handler);
  Handler.InterruptCallback = InterruptCallback;
  ADXL345_Init(&Handler);
  ADXL345_SetAddressI2C(&Handler, 0);
  // Some Other Configurations

  while (1)
  {
    // Infinite loop codes 
  }

  ADXL345_DeInit(&Handler);
  return 0;
}
```
</details>


<details>
<summary>Without using ADXL345_platform files (esp-idf)</summary>

```c
#include <stdio.h>
#include <stdint.h>
#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_err.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "ADXL345.h"

#define ADXL345_I2C_NUM   I2C_NUM_1
#define ADXL345_I2C_RATE  100000
#define ADXL345_SCL_GPIO  GPIO_NUM_13
#define ADXL345_SDA_GPIO  GPIO_NUM_14

ADXL345_Handler_t Handler;

int8_t
ADXL345_Platform_Init(void)
{
  i2c_config_t conf;
  conf.mode = I2C_MODE_MASTER;
  conf.sda_io_num = ADXL345_SDA_GPIO;
  conf.sda_pullup_en = GPIO_PULLUP_DISABLE;
  conf.scl_io_num = ADXL345_SCL_GPIO;
  conf.scl_pullup_en = GPIO_PULLUP_DISABLE;
  conf.master.clk_speed = ADXL345_I2C_RATE;
  if (i2c_param_config(ADXL345_I2C_NUM, &conf) != ESP_OK)
    return -1;
  if (i2c_driver_install(ADXL345_I2C_NUM, conf.mode, 0, 0, 0) != ESP_OK)
    return -1;
  return 0;
}

int8_t
ADXL345_Platform_DeInit(void)
{
  i2c_driver_delete(ADXL345_I2C_NUM);
  gpio_reset_pin(ADXL345_SDA_GPIO);
  gpio_reset_pin(ADXL345_SCL_GPIO);
  return 0;
}

int8_t
ADXL345_Platform_Send(uint8_t Address, uint8_t *Data, uint8_t DataLen)
{
  i2c_cmd_handle_t ADXL345_i2c_cmd_handle = 0;
  Address <<= 1;
  Address &= 0xFE;

  ADXL345_i2c_cmd_handle = i2c_cmd_link_create();
  i2c_master_start(ADXL345_i2c_cmd_handle);
  i2c_master_write(ADXL345_i2c_cmd_handle, &Address, 1, 1);
  i2c_master_write(ADXL345_i2c_cmd_handle, Data, DataLen, 1);
  i2c_master_stop(ADXL345_i2c_cmd_handle);
  if (i2c_master_cmd_begin(ADXL345_I2C_NUM, ADXL345_i2c_cmd_handle, 1000 / portTICK_RATE_MS) != ESP_OK)
  {
    i2c_cmd_link_delete(ADXL345_i2c_cmd_handle);
    return -1;
  }
  i2c_cmd_link_delete(ADXL345_i2c_cmd_handle);
  return 0;
}

int8_t
ADXL345_Platform_Receive(uint8_t Address, uint8_t *Data, uint8_t DataLen)
{
  i2c_cmd_handle_t ADXL345_i2c_cmd_handle = 0;
  Address <<= 1;
  Address |= 0x01;

  ADXL345_i2c_cmd_handle = i2c_cmd_link_create();
  i2c_master_start(ADXL345_i2c_cmd_handle);
  i2c_master_write(ADXL345_i2c_cmd_handle, &Address, 1, 1);
  i2c_master_read(ADXL345_i2c_cmd_handle, Data, DataLen, I2C_MASTER_LAST_NACK);
  i2c_master_stop(ADXL345_i2c_cmd_handle);
  if (i2c_master_cmd_begin(ADXL345_I2C_NUM, ADXL345_i2c_cmd_handle, 1000 / portTICK_RATE_MS) != ESP_OK)
  {
    i2c_cmd_link_delete(ADXL345_i2c_cmd_handle);
    return -1;
  }
  i2c_cmd_link_delete(ADXL345_i2c_cmd_handle);
  return 0;
}

void ADXL345_INT1_INT2_ISR(void)
{
  ADXL345_IRQ_Handler(&Handler);
}

int8_t InterruptCallback(ADXL345_Interrupt_t Interrupt)
{
  switch (Interrupt)
  {
  case ADXL345_INTERRUPT_OVERRUN:
    printf("INTERRUPT_OVERRUN\r\n");
    break;

  case ADXL345_INTERRUPT_WATERMARK:
    printf("INTERRUPT_WATERMARK\r\n");
    break;

  case ADXL345_INTERRUPT_FREE_FALL:
    printf("INTERRUPT_FREE_FALL\r\n");
    break;

  case ADXL345_INTERRUPT_INACTIVITY:
    printf("INTERRUPT_INACTIVITY\r\n");
    break;

  case ADXL345_INTERRUPT_ACTIVITY:
    printf("INTERRUPT_ACTIVITY\r\n");
    break;

  case ADXL345_INTERRUPT_DOUBLE_TAP:
    printf("INTERRUPT_DOUBLE_TAP\r\n");
    break;

  case ADXL345_INTERRUPT_SINGLE_TAP:
    printf("INTERRUPT_SINGLE_TAP\r\n");
    break;

  case ADXL345_INTERRUPT_DATA_READY:
    printf("INTERRUPT_DATA_READY\r\n");
    break;
  }

  return 0;
}

int main(void)
{
  // Set External Interrupt to call ADXL345_INT1_INT2_ISR function...

  Handler.PlatformI2CInit    = ADXL345_Platform_Init;
  Handler.PlatformI2CDeInit  = ADXL345_Platform_DeInit;
  Handler.PlatformI2CSend    = ADXL345_Platform_Send;
  Handler.PlatformI2CReceive = ADXL345_Platform_Receive;
  Handler.InterruptCallback  = InterruptCallback;
  ADXL345_Init(&Handler);
  // Some Other Configurations

  while (1)
  {
    // Infinite loop codes 
  }


  ADXL345_DeInit(&Handler);
  return 0;
}
```
</details>
