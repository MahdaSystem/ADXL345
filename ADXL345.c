/**
 **********************************************************************************
 * @file   ADXL345.c
 * @author Hossein.M (https://github.com/Hossein-M98)
 * @brief  ADXL345 driver
 *         Functionalities of the this file:
 *          + Full feature
 *          + Support for I2C communication protocol
 *          + Easy to port
 **********************************************************************************
 *
 * Copyright (c) 2021 Mahda Embedded System (MIT License)
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
 * LIABILITY, WHETHER IN AN Activity OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **********************************************************************************
 */

/* Includes ---------------------------------------------------------------------*/
#include "ADXL345.h"
#include <string.h>



/* Private Constants ------------------------------------------------------------*/
/**
 * @brief  I2C device addresses
 */
#define ADXL345_I2C_ADDRESS_A   0x53
#define ADXL345_I2C_ADDRESS_B   0x1D

/**
 * @brief  ADXL345 registers
 */
#define ADXL345_REG_DEVID             0x00
#define ADXL345_REG_THRESH_TAP        0x1D
#define ADXL345_REG_OFSX              0x1E
#define ADXL345_REG_OFSY              0x1F
#define ADXL345_REG_OFSZ              0x20
#define ADXL345_REG_DUR               0x21
#define ADXL345_REG_LATENT            0x22
#define ADXL345_REG_WINDOW            0x23
#define ADXL345_REG_THRESH_ACT        0x24
#define ADXL345_REG_THRESH_INACT      0x25
#define ADXL345_REG_TIME_INACT        0x26
#define ADXL345_REG_ACT_INACT_CTL     0x27
#define ADXL345_REG_THRESH_FF         0x28
#define ADXL345_REG_TIME_FF           0x29
#define ADXL345_REG_TAP_AXES          0x2A
#define ADXL345_REG_ACT_TAP_STATUS    0x2B
#define ADXL345_REG_BW_RATE           0x2C
#define ADXL345_REG_POWER_CTL         0x2D
#define ADXL345_REG_INT_ENABLE        0x2E
#define ADXL345_REG_INT_MAP           0x2F
#define ADXL345_REG_INT_SOURCE        0x30
#define ADXL345_REG_DATA_FORMAT       0x31
#define ADXL345_REG_DATAX0            0x32
#define ADXL345_REG_DATAX1            0x33
#define ADXL345_REG_DATAY0            0x34
#define ADXL345_REG_DATAY1            0x35
#define ADXL345_REG_DATAZ0            0x36
#define ADXL345_REG_DATAZ1            0x37
#define ADXL345_REG_FIFO_CTL          0x38
#define ADXL345_REG_FIFO_STATUS       0x39

/**
 * @brief  ADXL345 DEVID register value
 */
#define ADXL345_DEVICE_ID   0xE5


/* Private Macro ----------------------------------------------------------------*/
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif



/**
 ==================================================================================
                               ##### Functions #####                               
 ==================================================================================
 */

static ADXL345_Result_t
ADXL345_WriteRegs(ADXL345_Handler_t *Handler,
                  uint8_t StartReg, uint8_t *Data, uint8_t BytesCount)
{
  const uint8_t ADXL345_SEND_BUFFER_SIZE = 9;
  uint8_t Buffer[ADXL345_SEND_BUFFER_SIZE];
  uint8_t Len = 0;

  Buffer[0] = StartReg; // send register address to set RTC pointer
  while (BytesCount)
  {
    Len = MIN(BytesCount, sizeof(Buffer)-1);
    memcpy((void*)(Buffer+1), (const void*)Data, Len);

    if (Handler->PlatformI2CSend(Handler->AddressI2C, Buffer, Len+1) != 0)
      return ADXL345_FAIL;

    Data += Len;
    Buffer[0] += Len;
    BytesCount -= Len;
  }

  return ADXL345_OK;
}

static ADXL345_Result_t
ADXL345_ReadRegs(ADXL345_Handler_t *Handler,
                 uint8_t StartReg, uint8_t *Data, uint8_t BytesCount)
{
  if (Handler->PlatformI2CSend(Handler->AddressI2C, &StartReg, 1) != 0)
    return ADXL345_FAIL;

  if (Handler->PlatformI2CReceive(Handler->AddressI2C, Data, BytesCount) != 0)
    return ADXL345_FAIL;

  return ADXL345_OK;
}



/**
 ==================================================================================
                  ##### Public Measurement Functions #####                         
 ==================================================================================
 */

ADXL345_Result_t
ADXL345_Set_Offset(ADXL345_Handler_t *Handler, int8_t X, int8_t Y, int8_t Z)
{
  int8_t Buffer[3];

  Buffer[0] = X;
  Buffer[1] = Y;
  Buffer[2] = Z;
  if (ADXL345_WriteRegs(Handler, ADXL345_REG_OFSX, (uint8_t*)Buffer, 3) != ADXL345_OK)
    return ADXL345_FAIL;

  return ADXL345_OK;
}

ADXL345_Result_t
ADXL345_Get_Offset(ADXL345_Handler_t *Handler, int8_t *X, int8_t *Y, int8_t *Z)
{
  int8_t Buffer[3];

  if (ADXL345_ReadRegs(Handler, ADXL345_REG_OFSX, (uint8_t*)Buffer, 3) != ADXL345_OK)
    return ADXL345_FAIL;

  *X = Buffer[0];
  *Y = Buffer[1];
  *Z = Buffer[2];

  return ADXL345_OK;
}


/**
 * @brief  Set tap configuration
 * @param  Handler: Pointer to handler
 * @param  TapConfig: Pointer to tap status
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Set_TapConfig(ADXL345_Handler_t *Handler,
                      ADXL345_TapConfig_t *TapConfig)
{
  uint8_t Buffer[3];

  Buffer[0] = TapConfig->TapThreshold;

  if (ADXL345_WriteRegs(Handler, ADXL345_REG_THRESH_TAP, Buffer, 1) != ADXL345_OK)
    return ADXL345_FAIL;

  Buffer[0] = TapConfig->Duration;
  Buffer[1] = TapConfig->Latent;
  Buffer[2] = TapConfig->Window;

  if (ADXL345_WriteRegs(Handler, ADXL345_REG_DUR, Buffer, 3) != ADXL345_OK)
    return ADXL345_FAIL;

  if (TapConfig->TapAxis.TapEnableZ)
    Buffer[0] |= 0x01;
  if (TapConfig->TapAxis.TapEnableY)
    Buffer[0] |= 0x02;
  if (TapConfig->TapAxis.TapEnableX)
    Buffer[0] |= 0x04;
  if (TapConfig->TapAxis.Suppress)
    Buffer[0] |= 0x08;

  return ADXL345_WriteRegs(Handler, ADXL345_REG_TAP_AXES, Buffer, 1);
}

/**
 * @brief  Get tap configuration
 * @param  Handler: Pointer to handler
 * @param  TapConfig: Pointer to tap status
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Get_TapConfig(ADXL345_Handler_t *Handler,
                      ADXL345_TapConfig_t *TapConfig)
{
  uint8_t Buffer[3];

  memset(TapConfig, 0, sizeof(ADXL345_TapConfig_t));

  if (ADXL345_ReadRegs(Handler, ADXL345_REG_THRESH_TAP, Buffer, 1) != ADXL345_OK)
    return ADXL345_FAIL;

  TapConfig->TapThreshold = Buffer[0];

  if (ADXL345_ReadRegs(Handler, ADXL345_REG_DUR, Buffer, 3) != ADXL345_OK)
    return ADXL345_FAIL;

  TapConfig->Duration = Buffer[0];
  TapConfig->Latent = Buffer[1];
  TapConfig->Window = Buffer[2];

  if (ADXL345_ReadRegs(Handler, ADXL345_REG_TAP_AXES, Buffer, 1) != ADXL345_OK)
    return ADXL345_FAIL;

  if (Buffer[0] & 0x01)
    TapConfig->TapAxis.TapEnableZ = 1;
  if (Buffer[0] & 0x02)
    TapConfig->TapAxis.TapEnableY = 1;
  if (Buffer[0] & 0x04)
    TapConfig->TapAxis.TapEnableX = 1;
  if (Buffer[0] & 0x08)
    TapConfig->TapAxis.Suppress = 1;

  return ADXL345_OK;
}

/**
 * @brief  Get tap status
 * @param  Handler: Pointer to handler
 * @param  Status: Pointer to tap status
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Get_TapStatus(ADXL345_Handler_t *Handler,
                      ADXL345_TapStatus_t *Status)
{
  uint8_t Reg = 0;

  if (ADXL345_ReadRegs(Handler, ADXL345_REG_ACT_TAP_STATUS, &Reg, 1) != ADXL345_OK)
    return ADXL345_FAIL;

  if (Reg & 0x01)
    Status->TapZ = 1;
  if (Reg & 0x02)
    Status->TapY = 1;
  if (Reg & 0x04)
    Status->TapX = 1;

  if (Reg & 0x08)
    Status->Asleep = 1;

  if (Reg & 0x10)
    Status->ActZ = 1;
  if (Reg & 0x20)
    Status->ActY = 1;
  if (Reg & 0x40)
    Status->ActX = 1;

  return ADXL345_OK;
}


/**
 * @brief  Set Activity and Inactivity Settings
 * @param  Handler: Pointer to handler
 * @param  ActivityInactivity: Pointer to Activity and Inactivity Configuration
 *                             structure
 * 
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Set_ActivityInactivity(ADXL345_Handler_t *Handler,
                               ADXL345_ActivityInactivity_t *ActivityInactivity)
{
  uint8_t Buffer[4] = {0};

  Buffer[0] = ActivityInactivity->ActivityThreshold;
  Buffer[1] = ActivityInactivity->InactivityThreshold;
  Buffer[2] = ActivityInactivity->InactivityTime;

  if (ActivityInactivity->Control.InactivityEnableZ)
    Buffer[3] |= 0x01;
  if (ActivityInactivity->Control.InactivityEnableY)
    Buffer[3] |= 0x02;
  if (ActivityInactivity->Control.InactivityEnableX)
    Buffer[3] |= 0x04;
  if (ActivityInactivity->Control.InactivityCoupled)
    Buffer[3] |= 0x08;

  if (ActivityInactivity->Control.ActivityEnableZ)
    Buffer[3] |= 0x10;
  if (ActivityInactivity->Control.ActivityEnableY)
    Buffer[3] |= 0x20;
  if (ActivityInactivity->Control.ActivityEnableX)
    Buffer[3] |= 0x40;
  if (ActivityInactivity->Control.ActivityCoupled)
    Buffer[3] |= 0x80;
  
  return ADXL345_WriteRegs(Handler, ADXL345_REG_ACT_INACT_CTL, Buffer, 4);
}

/**
 * @brief  Get Activity and Inactivity Settings
 * @param  Handler: Pointer to handler
 * @param  ActivityInactivity: Pointer to Activity and Inactivity Configuration
 *                             structure
 * 
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Get_ActivityInactivity(ADXL345_Handler_t *Handler,
                               ADXL345_ActivityInactivity_t *ActivityInactivity)
{
  uint8_t Buffer[4] = {0};

  if (ADXL345_ReadRegs(Handler, ADXL345_REG_THRESH_ACT, Buffer, 4) != ADXL345_OK)
    return ADXL345_FAIL;

  memset(ActivityInactivity, 0, sizeof(ADXL345_ActivityInactivity_t));

  ActivityInactivity->ActivityThreshold = Buffer[0];
  ActivityInactivity->InactivityThreshold = Buffer[1];
  ActivityInactivity->InactivityTime = Buffer[2];

  if (Buffer[3] & 0x01)
    ActivityInactivity->Control.InactivityEnableZ = 1;
  if (Buffer[3] & 0x02)
    ActivityInactivity->Control.InactivityEnableY = 1;
  if (Buffer[3] & 0x04)
    ActivityInactivity->Control.InactivityEnableX = 1;
  if (Buffer[3] & 0x08)
    ActivityInactivity->Control.InactivityCoupled = 1;

  if (Buffer[3] & 0x10)
    ActivityInactivity->Control.ActivityEnableZ = 1;
  if (Buffer[3] & 0x20)
    ActivityInactivity->Control.ActivityEnableY = 1;
  if (Buffer[3] & 0x40)
    ActivityInactivity->Control.ActivityEnableX = 1;
  if (Buffer[3] & 0x80)
    ActivityInactivity->Control.ActivityCoupled = 1;
  
  return ADXL345_OK;
}


/**
 * @brief  Set free-fall Threshold and Time
 * @param  Handler: Pointer to handler
 * @param  Threshold: Threshold
 * @param  Time: Time
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Set_FreeFall(ADXL345_Handler_t *Handler,
                     uint8_t Threshold, uint8_t Time)
{
  uint8_t Buffer[2] = {0};

  Buffer[0] = Threshold;
  Buffer[1] = Time;

  return ADXL345_WriteRegs(Handler, ADXL345_REG_THRESH_FF, Buffer, 2);
}

/**
 * @brief  Get free-fall Threshold and Time
 * @param  Handler: Pointer to handler
 * @param  Threshold: Pointer to save Threshold
 * @param  Time: Pointer to save Time
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Get_FreeFall(ADXL345_Handler_t *Handler,
                     uint8_t *Threshold, uint8_t *Time)
{
  uint8_t Buffer[2] = {0};

  if (ADXL345_ReadRegs(Handler,
                       ADXL345_REG_THRESH_FF, Buffer, 2) != ADXL345_OK)
    return ADXL345_FAIL;

  *Threshold = Buffer[0];
  *Time = Buffer[1];

  return ADXL345_OK;
}


/**
 * @brief  Set Data Rate
 * @param  Handler: Pointer to handler
 * @param  Rate: Data Rate
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Set_Rate(ADXL345_Handler_t *Handler, ADXL345_Rate_t Rate)
{
  uint8_t Reg = 0;

  if (ADXL345_ReadRegs(Handler,
                       ADXL345_REG_BW_RATE, &Reg, 1) != ADXL345_OK)
    return ADXL345_FAIL;

  Reg &= ~(0x1F);
  Reg |= Rate;

  return ADXL345_WriteRegs(Handler, ADXL345_REG_BW_RATE, &Reg, 1);
}

/**
 * @brief  Get Data Rate
 * @param  Handler: Pointer to handler
 * @param  Rate: Pointer to save Data Rate
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Get_Rate(ADXL345_Handler_t *Handler, ADXL345_Rate_t *Rate)
{
  uint8_t Reg = 0;

  if (ADXL345_ReadRegs(Handler,
                       ADXL345_REG_BW_RATE, &Reg, 1) != ADXL345_OK)
    return ADXL345_FAIL;

  Reg &= 0x1F;
  *Rate = (ADXL345_Rate_t)(Reg);

  return ADXL345_OK;
}


/**
 * @brief  Set Interrupt Configuration
 * @param  Handler: Pointer to handler
 * @param  Config: Pointer to Interrupt Configuration structure
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Set_InterruptConfig(ADXL345_Handler_t *Handler,
                            ADXL345_InterruptConfig_t *Config)
{
  uint8_t Buffer[2] = {0};

  if (Config->Enable.Overrun)
    Buffer[0] |= 0x01;
  if (Config->Enable.Watermark)
    Buffer[0] |= 0x02;
  if (Config->Enable.FreeFall)
    Buffer[0] |= 0x04;
  if (Config->Enable.Inactivity)
    Buffer[0] |= 0x08;
  if (Config->Enable.Activity)
    Buffer[0] |= 0x10;
  if (Config->Enable.DoubleTap)
    Buffer[0] |= 0x20;
  if (Config->Enable.SingleTap)
    Buffer[0] |= 0x40;
  if (Config->Enable.DataReady)
    Buffer[0] |= 0x80;

  if (Config->Map.Overrun)
    Buffer[1] |= 0x01;
  if (Config->Map.Watermark)
    Buffer[1] |= 0x02;
  if (Config->Map.FreeFall)
    Buffer[1] |= 0x04;
  if (Config->Map.Inactivity)
    Buffer[1] |= 0x08;
  if (Config->Map.Activity)
    Buffer[1] |= 0x10;
  if (Config->Map.DoubleTap)
    Buffer[1] |= 0x20;
  if (Config->Map.SingleTap)
    Buffer[1] |= 0x40;
  if (Config->Map.DataReady)
    Buffer[1] |= 0x80;

  if (ADXL345_WriteRegs(Handler,
                        ADXL345_REG_INT_ENABLE, Buffer, 2) != ADXL345_OK)
    return ADXL345_FAIL;

  if (ADXL345_ReadRegs(Handler,
                       ADXL345_REG_DATA_FORMAT, Buffer, 1) != ADXL345_OK)
    return ADXL345_FAIL;

  if (Config->ActiveLow)
    Buffer[0] |= 0x20;

  return ADXL345_WriteRegs(Handler, ADXL345_REG_DATA_FORMAT, Buffer, 1);
}

/**
 * @brief  Get Interrupt Configuration
 * @param  Handler: Pointer to handler
 * @param  Config: Pointer to Interrupt Configuration structure
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Get_InterruptConfig(ADXL345_Handler_t *Handler,
                            ADXL345_InterruptConfig_t *Config)
{
  uint8_t Buffer[2] = {0};

  if (ADXL345_ReadRegs(Handler,
                       ADXL345_REG_INT_ENABLE, Buffer, 2) != ADXL345_OK)
    return ADXL345_FAIL;

  memset(Config, 0, sizeof(ADXL345_InterruptConfig_t));

  if (Buffer[0] & 0x01)
    Config->Enable.Overrun = 1;
  if (Buffer[0] & 0x02)
    Config->Enable.Watermark = 1;
  if (Buffer[0] & 0x04)
    Config->Enable.FreeFall = 1;
  if (Buffer[0] & 0x08)
    Config->Enable.Inactivity = 1;
  if (Buffer[0] & 0x10)
    Config->Enable.Activity = 1;
  if (Buffer[0] & 0x20)
    Config->Enable.DoubleTap = 1;
  if (Buffer[0] & 0x40)
    Config->Enable.SingleTap = 1;
  if (Buffer[0] & 0x80)
    Config->Enable.DataReady = 1;

  if (Buffer[1] & 0x01)
    Config->Map.Overrun = 1;
  if (Buffer[1] & 0x02)
    Config->Map.Watermark = 1;
  if (Buffer[1] & 0x04)
    Config->Map.FreeFall = 1;
  if (Buffer[1] & 0x08)
    Config->Map.Inactivity = 1;
  if (Buffer[1] & 0x10)
    Config->Map.Activity = 1;
  if (Buffer[1] & 0x20)
    Config->Map.DoubleTap = 1;
  if (Buffer[1] & 0x40)
    Config->Map.SingleTap = 1;
  if (Buffer[1] & 0x80)
    Config->Map.DataReady = 1;

  return ADXL345_OK;
}

/**
 * @brief  Get Interrupt Source
 * @param  Handler: Pointer to handler
 * @param  Source: Pointer to Interrupt Source structure
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Get_InterruptSource(ADXL345_Handler_t *Handler,
                            ADXL345_InterruptReg_t *Source)
{
  uint8_t Reg = 0;

  if (ADXL345_ReadRegs(Handler,
                       ADXL345_REG_INT_SOURCE, &Reg, 1) != ADXL345_OK)
    return ADXL345_FAIL;

  memset(Source, 0, sizeof(ADXL345_InterruptReg_t));

  if (Reg & 0x01)
    Source->Overrun = 1;
  if (Reg & 0x02)
    Source->Watermark = 1;
  if (Reg & 0x04)
    Source->FreeFall = 1;
  if (Reg & 0x08)
    Source->Inactivity = 1;
  if (Reg & 0x10)
    Source->Activity = 1;
  if (Reg & 0x20)
    Source->DoubleTap = 1;
  if (Reg & 0x40)
    Source->SingleTap = 1;
  if (Reg & 0x80)
    Source->DataReady = 1;

  return ADXL345_OK;
}


/**
 * @brief  Set Data Format settings
 * @param  Handler: Pointer to handler
 * @param  DataFormat: Pointer to Data Format settings structure
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Set_DataFormat(ADXL345_Handler_t *Handler,
                       ADXL345_DataFormat_t *DataFormat)
{
  uint8_t Reg = 0;

  if (ADXL345_ReadRegs(Handler,
                       ADXL345_REG_DATA_FORMAT, &Reg, 1) != ADXL345_OK)
    return ADXL345_FAIL;

  Reg &= 0xF0;

  Reg |= DataFormat->Range;

  if (DataFormat->JustifyLeft)
    Reg |= 0x04;

  if (DataFormat->FullResolution)
    Reg |= 0x08;

  return ADXL345_WriteRegs(Handler, ADXL345_REG_DATA_FORMAT, &Reg, 1);
}

/**
 * @brief  Get Data Format settings
 * @param  Handler: Pointer to handler
 * @param  DataFormat: Pointer to Data Format settings structure
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Get_DataFormat(ADXL345_Handler_t *Handler,
                       ADXL345_DataFormat_t *DataFormat)
{
  uint8_t Reg = 0;

  if (ADXL345_ReadRegs(Handler,
                       ADXL345_REG_DATA_FORMAT, &Reg, 1) != ADXL345_OK)
    return ADXL345_FAIL;

  DataFormat->Range = (ADXL345_Range_t) (Reg & 0x03);

  if (Reg & 0x04)
    DataFormat->JustifyLeft = 1;

  if (Reg & 0x08)
    DataFormat->FullResolution = 1;

  return ADXL345_OK;
}


/**
 * @brief  Set FIFO Configurations
 * @param  Handler: Pointer to handler
 * @param  Config: Pointer to FIFO Configurations structure
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Set_FifoConfig(ADXL345_Handler_t *Handler,
                       ADXL345_FifoConfig_t *Config)
{
  uint8_t Reg = 0;

  Reg |= Config->WatermarkSamples & 0x1F;

  if (Config->Trigger)
    Reg |= 0x20;

  Reg |= (Config->Mode) << 6;

  return ADXL345_WriteRegs(Handler, ADXL345_REG_FIFO_CTL, &Reg, 1);
}

/**
 * @brief  Get FIFO Configurations
 * @param  Handler: Pointer to handler
 * @param  Config: Pointer to FIFO Configurations structure
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Get_FifoConfig(ADXL345_Handler_t *Handler,
                       ADXL345_FifoConfig_t *Config)
{
  uint8_t Reg = 0;

  if (ADXL345_ReadRegs(Handler,
                       ADXL345_REG_FIFO_CTL, &Reg, 1) != ADXL345_OK)
    return ADXL345_FAIL;

  memset(Config, 0, sizeof(ADXL345_FifoConfig_t));

  Config->WatermarkSamples = (Reg & 0x1F);

  if (Reg & 0x20)
    Config->Trigger = 1;

  Config->Mode = Reg >> 6;

  return ADXL345_OK;
}

/**
 * @brief  Get FIFO status
 * @param  Handler: Pointer to handler
 * @param  Status: Pointer to FIFO status structure
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Get_FifoStatus(ADXL345_Handler_t *Handler,
                       ADXL345_FifoStatus_t *Status)
{
  uint8_t Reg = 0;

  if (ADXL345_ReadRegs(Handler,
                       ADXL345_REG_FIFO_STATUS, &Reg, 1) != ADXL345_OK)
    return ADXL345_FAIL;

  memset(Status, 0, sizeof(ADXL345_FifoStatus_t));

  Status->Entries = (Reg & 0x3F);

  if (Reg & 0x80)
    Status->Trigger = 1;

  return ADXL345_OK;
}


/**
 * @brief  Set Power Settings
 * @param  Handler: Pointer to handler
 * @param  PowerControl: Pointer to Power Settings structure
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Set_PowerControl(ADXL345_Handler_t *Handler,
                         ADXL345_PowerControl_t *PowerControl)
{
  uint8_t Reg = 0;

  Reg |= PowerControl->Wakeup & 0x03;

  if (PowerControl->Sleep)
    Reg |= 0x04;
  if (PowerControl->Measure)
    Reg |= 0x08;
  if (PowerControl->AutoSleep)
    Reg |= 0x10;
  if (PowerControl->Link)
    Reg |= 0x20;

  return ADXL345_WriteRegs(Handler, ADXL345_REG_POWER_CTL, &Reg, 1);
}

/**
 * @brief  Get Power Settings
 * @param  Handler: Pointer to handler
 * @param  PowerControl: Pointer to Power Settings structure
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Get_PowerControl(ADXL345_Handler_t *Handler,
                         ADXL345_PowerControl_t *PowerControl)
{
  uint8_t Reg = 0;

  if (ADXL345_ReadRegs(Handler,
                       ADXL345_REG_POWER_CTL, &Reg, 1) != ADXL345_OK)
    return ADXL345_FAIL;

  memset(PowerControl, 0, sizeof(ADXL345_PowerControl_t));

  PowerControl->Wakeup = Reg & 0x03;

  if (Reg & 0x04)
    PowerControl->Sleep = 1;
  if (Reg & 0x08)
    PowerControl->Measure = 1;
  if (Reg & 0x10)
    PowerControl->AutoSleep = 1;
  if (Reg & 0x20)
    PowerControl->Link = 1;

  return ADXL345_OK;
}


/**
 * @brief  
 * @param  Handler: Pointer to handler
 * @param  Samples: Pointer to Samples array
 * @param  SamplesBufferLen: Sample buffer capacity in terms of number of samples
 * @param  ReadSamples: Number of samples read
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_ReadSamples(ADXL345_Handler_t *Handler,
                    ADXL345_Sample_t *Samples,
                    uint8_t SamplesBufferLen, uint8_t *ReadSamples)
{
  ADXL345_FifoConfig_t FifoConfig;
  ADXL345_DataFormat_t DataFormat;
  uint8_t Buffer[32 * 6];
  float Factor = 0.0f;

  ADXL345_Get_FifoConfig(Handler, &FifoConfig);
  ADXL345_Get_DataFormat(Handler, &DataFormat);

  if (FifoConfig.Mode == ADXL345_MODE_BYPASS)
    *ReadSamples = 1;
  else
  {
    ADXL345_FifoStatus_t FifoStatus;
    ADXL345_Get_FifoStatus(Handler, &FifoStatus);

    *ReadSamples = MIN(SamplesBufferLen, FifoStatus.Entries);
  }

  if (ADXL345_ReadRegs(Handler,
                       ADXL345_REG_DATAX0, Buffer,
                       6 * (*ReadSamples)) != ADXL345_OK)
    return ADXL345_FAIL;

  for (uint8_t i = 0; i < (*ReadSamples); i++)
  {
    Samples[i].RawX = (int16_t)(Buffer[1+i*6] << 8) | Buffer[0+i*6];
    Samples[i].RawY = (int16_t)(Buffer[3+i*6] << 8) | Buffer[2+i*6];
    Samples[i].RawZ = (int16_t)(Buffer[5+i*6] << 8) | Buffer[4+i*6];

    if (DataFormat.FullResolution)
    {
      if (DataFormat.JustifyLeft)
      {
        Samples[i].RawX = (Samples[i].RawX & 0x80) |
                           ((Samples[i].RawX & 0x7F) >> (6 - DataFormat.Range));
        Samples[i].RawY = (Samples[i].RawY & 0x80) |
                           ((Samples[i].RawY & 0x7F) >> (6 - DataFormat.Range));
        Samples[i].RawZ = (Samples[i].RawZ & 0x80) |
                           ((Samples[i].RawZ & 0x7F) >> (6 - DataFormat.Range));
      }

      Factor = 0.004f;
    }
    else
    {
      if (DataFormat.JustifyLeft)
      {
        Samples[i].RawX = (Samples[i].RawX & 0x80) |
                           ((Samples[i].RawX & 0x7F) >> 6);
        Samples[i].RawY = (Samples[i].RawY & 0x80) |
                           ((Samples[i].RawY & 0x7F) >> 6);
        Samples[i].RawZ = (Samples[i].RawZ & 0x80) |
                           ((Samples[i].RawZ & 0x7F) >> 6);
      }

      switch (DataFormat.Range)
      {
      case ADXL345_RANGE_2G:
        Factor = 0.0039f;
        break;

      case ADXL345_RANGE_4G:
        Factor = 0.0078f;
        break;

      case ADXL345_RANGE_8G:
        Factor = 0.0156f;
        break;

      case ADXL345_RANGE_16G:
        Factor = 0.0312f;
        break;
      }
    }

    Samples[i].AccelX = (float)(Samples[i].RawX) * Factor;
    Samples[i].AccelY = (float)(Samples[i].RawY) * Factor;
    Samples[i].AccelZ = (float)(Samples[i].RawZ) * Factor;
  }

  return ADXL345_OK;
}

/**
 * @brief  IRQ Handler
 * @note   Put this function in ISR. This function will call
 *         Handler->InterruptCallback Function.
 * 
 * @param  Handler: Pointer to handler
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_IRQ_Handler(ADXL345_Handler_t *Handler)
{
  ADXL345_InterruptReg_t Interrupt;

  if ((Handler->InterruptCallback) == NULL)
    return ADXL345_FAIL;

  ADXL345_Get_InterruptSource(Handler, &Interrupt);

  if (Interrupt.Overrun)
    Handler->InterruptCallback(ADXL345_INTERRUPT_OVERRUN);

  if (Interrupt.Watermark)
    Handler->InterruptCallback(ADXL345_INTERRUPT_WATERMARK);

  if (Interrupt.FreeFall)
    Handler->InterruptCallback(ADXL345_INTERRUPT_FREE_FALL);

  if (Interrupt.Inactivity)
    Handler->InterruptCallback(ADXL345_INTERRUPT_INACTIVITY);

  if (Interrupt.Activity)
    Handler->InterruptCallback(ADXL345_INTERRUPT_ACTIVITY);

  if (Interrupt.DoubleTap)
    Handler->InterruptCallback(ADXL345_INTERRUPT_DOUBLE_TAP);

  if (Interrupt.SingleTap)
    Handler->InterruptCallback(ADXL345_INTERRUPT_SINGLE_TAP);

  if (Interrupt.DataReady)
    Handler->InterruptCallback(ADXL345_INTERRUPT_DATA_READY);

  return ADXL345_OK;
}


/**
 * @brief  Initializer function
 * @param  Handler: Pointer to handler
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Init(ADXL345_Handler_t *Handler)
{
  if ((Handler->PlatformI2CInit == NULL) ||
      (Handler->PlatformI2CDeInit == NULL) ||
      (Handler->PlatformI2CSend == NULL) ||
      (Handler->PlatformI2CReceive == NULL))
    return ADXL345_FAIL;

  ADXL345_SetAddressI2C(Handler, 0);

  if (Handler->PlatformI2CInit() != 0)
    return ADXL345_FAIL;

  return ADXL345_OK;
}

/**
 * @brief  Deinitialize function
 * @param  Handler: Pointer to handler
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_DeInit(ADXL345_Handler_t *Handler)
{
  ADXL345_PowerControl_t PowerControl;

  if (ADXL345_Get_PowerControl(Handler, &PowerControl) != ADXL345_OK)
    return ADXL345_FAIL;
  PowerControl.Sleep = 1;
  PowerControl.Measure = 0;
  if (ADXL345_Set_PowerControl(Handler, &PowerControl) != ADXL345_OK)
    return ADXL345_FAIL;

  if (Handler->PlatformI2CDeInit() != 0)
    return ADXL345_FAIL;
  return ADXL345_OK;
}


/**
 * @brief  Initializer function
 * @param  Handler: Pointer to handler
 * @param  Address: The address depends on ADDR pin state. You should use one of
 *                  this options:
 *         - 0: This address used when ADDR is connected VSS
 *         - 1: This address used when ADDR is connected VDD
 * 
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 */
ADXL345_Result_t
ADXL345_SetAddressI2C(ADXL345_Handler_t *Handler, uint8_t Address)
{
  if (Address == 0)
    Handler->AddressI2C = ADXL345_I2C_ADDRESS_A;
  else
    Handler->AddressI2C = ADXL345_I2C_ADDRESS_B;

  return ADXL345_OK;
}


/**
 * @brief  Check the DEVID register to ensure the correctness of the connection
 * @param  Handler: Pointer to handler
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_CheckDeviceID(ADXL345_Handler_t *Handler)
{
  uint8_t RegisterDeviceID = 0;

  if (ADXL345_ReadRegs(Handler,
                       ADXL345_REG_DEVID, &RegisterDeviceID, 1) != ADXL345_OK)
    return ADXL345_FAIL;

  if (RegisterDeviceID != ADXL345_DEVICE_ID)
    return ADXL345_FAIL;

  return ADXL345_OK;
}
