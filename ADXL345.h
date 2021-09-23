/**
 **********************************************************************************
 * @file   ADXL345.h
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

/* Define to prevent recursive inclusion ----------------------------------------*/
#ifndef _ADXL345_H_
#define _ADXL345_H_

#ifdef __cplusplus
extern "C"
{
#endif



/* Includes ---------------------------------------------------------------------*/
#include <stdint.h>



/* Exported Data Types ----------------------------------------------------------*/

/**
 * @brief  Library functions result data type
 */
typedef enum ADXL345_Result_e
{
  ADXL345_OK            = 0,
  ADXL345_FAIL          = 1,
  ADXL345_INVALID_PARAM = 3,
} ADXL345_Result_t;

/**
 * @brief  Sample rates data type
 */
typedef enum ADXL345_Rate_e
{
  ADXL345_RATE_0P1            = 0x00,
  ADXL345_RATE_0P2            = 0x01,
  ADXL345_RATE_0P39           = 0x02,
  ADXL345_RATE_0P78           = 0x03,
  ADXL345_RATE_1P56           = 0x04,
  ADXL345_RATE_3P13           = 0x05,
  ADXL345_RATE_6P25           = 0x06,
  ADXL345_RATE_12P5           = 0x07,
  ADXL345_RATE_25             = 0x08,
  ADXL345_RATE_50             = 0x09,
  ADXL345_RATE_100            = 0x0A,
  ADXL345_RATE_200            = 0x0B,
  ADXL345_RATE_400            = 0x0C,
  ADXL345_RATE_800            = 0x0D,
  ADXL345_RATE_1600           = 0x0E,
  ADXL345_RATE_3200           = 0x0F,
  ADXL345_LOW_POWER_RATE_12P5 = 0x17,
  ADXL345_LOW_POWER_RATE_25   = 0x18,
  ADXL345_LOW_POWER_RATE_50   = 0x19,
  ADXL345_LOW_POWER_RATE_100  = 0x1A,
  ADXL345_LOW_POWER_RATE_200  = 0x1B,
  ADXL345_LOW_POWER_RATE_400  = 0x1C,
} ADXL345_Rate_t;

/**
 * @brief  Sensor measurement range data type
 */
typedef enum ADXL345_Range_e
{
  ADXL345_RANGE_2G  = 0x00,
  ADXL345_RANGE_4G  = 0x01,
  ADXL345_RANGE_8G  = 0x02,
  ADXL345_RANGE_16G = 0x03,
} ADXL345_Range_t;

/**
 * @brief  Interrupt pin
 */
typedef enum ADXL345_TriggerPin_e
{
  ADXL345_INTERRUPT_PIN1 = 0x00,
  ADXL345_INTERRUPT_PIN2 = 0x01,
} ADXL345_TriggerPin_t;

/**
 * @brief  FIFO Modes
 */
typedef enum ADXL345_Mode_e
{
  ADXL345_MODE_BYPASS   = 0x00,
  ADXL345_MODE_FIFO     = 0x01,
  ADXL345_MODE_STREAM   = 0x02,
  ADXL345_MODE_TRIGGER  = 0x03,
} ADXL345_Mode_t;

/**
 * @brief  Sleep Frequency
 */
typedef enum ADXL345_SleepFrequency_e
{
  ADXL345_SLEEP_FREQUENCY_8HZ = 0x00,
  ADXL345_SLEEP_FREQUENCY_4HZ = 0x01,
  ADXL345_SLEEP_FREQUENCY_2HZ = 0x02,
  ADXL345_SLEEP_FREQUENCY_1HZ = 0x03,
} ADXL345_SleepFrequency_t;

/**
 * @brief  Interrupt number
 */
typedef enum ADXL345_Interrupt_e
{
  ADXL345_INTERRUPT_OVERRUN     = 0x00,
  ADXL345_INTERRUPT_WATERMARK   = 0x01,
  ADXL345_INTERRUPT_FREE_FALL   = 0x02,
  ADXL345_INTERRUPT_INACTIVITY  = 0x03,
  ADXL345_INTERRUPT_ACTIVITY    = 0x04,
  ADXL345_INTERRUPT_DOUBLE_TAP  = 0x05,
  ADXL345_INTERRUPT_SINGLE_TAP  = 0x06,
  ADXL345_INTERRUPT_DATA_READY  = 0x07,
} ADXL345_Interrupt_t;


/**
 * @brief  Activity and Inactivity Configuration
 */
typedef struct ADXL345_ActivityInactivity_s
{
  uint8_t ActivityThreshold;
  uint8_t InactivityThreshold;
  uint8_t InactivityTime;
  struct ADXL345_ActivityInactivityControl_s
  {
    uint8_t InactivityEnableZ : 1; // Enable inactivity for Z-axis
    uint8_t InactivityEnableY : 1; // Enable inactivity for Y-axis
    uint8_t InactivityEnableX : 1; // Enable inactivity for X-axis
    uint8_t InactivityCoupled : 1; // 0 => DC , 1 => AC
    uint8_t ActivityEnableZ   : 1; // Enable activity for Z-axis
    uint8_t ActivityEnableY   : 1; // Enable activity for Y-axis
    uint8_t ActivityEnableX   : 1; // Enable activity for X-axis
    uint8_t ActivityCoupled   : 1; // 0 => DC , 1 => AC
  } Control;
} ADXL345_ActivityInactivity_t;

/**
 * @brief  Tap Configuration
 */
typedef struct ADXL345_TapConfig_s
{
  uint8_t TapThreshold;
  uint8_t Duration;
  uint8_t Latent;
  uint8_t Window;
  struct ADXL345_TapAxis_s
  {
    uint8_t TapEnableZ  : 1; // Enable Tap detection for Z-axis
    uint8_t TapEnableY  : 1; // Enable Tap detection for Y-axis
    uint8_t TapEnableX  : 1; // Enable Tap detection for X-axis
    uint8_t Suppress    : 1;
  } TapAxis;
} ADXL345_TapConfig_t;

/**
 * @brief  Activity and Tap status data type 
 */
typedef struct ADXL345_ActTapStatus_s
{
  uint8_t TapZ    : 1;
  uint8_t TapY    : 1;
  uint8_t TapX    : 1;
  uint8_t Asleep  : 1;
  uint8_t ActZ    : 1;
  uint8_t ActY    : 1;
  uint8_t ActX    : 1;
} ADXL345_ActTapStatus_t;

/**
 * @brief  Interrupt mask
 */
typedef struct ADXL345_InterruptReg_s
{
  uint8_t Overrun     : 1;
  uint8_t Watermark   : 1;
  uint8_t FreeFall    : 1;
  uint8_t Inactivity  : 1;
  uint8_t Activity    : 1;
  uint8_t DoubleTap   : 1;
  uint8_t SingleTap   : 1;
  uint8_t DataReady   : 1;
} ADXL345_InterruptReg_t;

/**
 * @brief  Interrupt Configuration data type
 */
typedef struct ADXL345_InterruptConfig_s
{
  ADXL345_InterruptReg_t Enable;
  ADXL345_InterruptReg_t Map; // 0 => INT1, 1 => INT2
  uint8_t ActiveLow; // 0 => Active High, 1 => Active Low
} ADXL345_InterruptConfig_t;

/**
 * @brief  Data format Configuration data type
 */
typedef struct ADXL345_DataFormat_s
{
  ADXL345_Range_t Range   : 2;
  uint8_t JustifyLeft     : 1; // 1 => Justify Left
  uint8_t FullResolution  : 1; // 1 => Full Resolution
} ADXL345_DataFormat_t;

/**
 * @brief  FIFO Configuration data type
 */
typedef struct ADXL345_FifoConfig_s
{
  uint8_t WatermarkSamples      : 5;
  ADXL345_TriggerPin_t Trigger  : 1;
  ADXL345_Mode_t Mode           : 2;
} ADXL345_FifoConfig_t;

/**
 * @brief  FIFO Status data type
 */
typedef struct ADXL345_FifoStatus_s
{
  uint8_t Entries   : 6;
  uint8_t Reserved  : 1;
  uint8_t Trigger   : 1;
} ADXL345_FifoStatus_t;

/**
 * @brief  Power Configuration data type
 */
typedef struct ADXL345_PowerControl_s
{
  ADXL345_SleepFrequency_t Wakeup : 2;
  uint8_t Sleep                   : 1;
  uint8_t Measure                 : 1;
  uint8_t AutoSleep               : 1;
  uint8_t Link                    : 1;
} ADXL345_PowerControl_t;

/**
 * @brief  Samples data type
 * @note   The unit of Accelx members (where x is X, Y and Z) is based on g
 */
typedef struct ADXL345_Sample_s
{
  int16_t RawX;
  int16_t RawY;
  int16_t RawZ;
  float AccelX;
  float AccelY;
  float AccelZ;
} ADXL345_Sample_t;

/**
 * @brief  Handler data type
 * @note   User must initialize this this functions before using library:
 *         - PlatformI2CInit
 *         - PlatformI2CDeInit
 *         - PlatformI2CSend
 *         - PlatformI2CReceive
 *         - InterruptCallback
 * @note   If success the functions must return 0 
 */
typedef struct ADXL345_Handler_s
{
  uint8_t AddressI2C;

  // Initializes platform dependent part
  int8_t (*PlatformI2CInit)(void);
  // De-initializes platform dependent part
  int8_t (*PlatformI2CDeInit)(void);
  // Send Data to the slave with the address of Address. (0 <= Address <= 127)
  int8_t (*PlatformI2CSend)(uint8_t Address, uint8_t *Data, uint8_t Len);
  // Receive Data from the slave with the address of Address. (0 <= Address <= 127)
  int8_t (*PlatformI2CReceive)(uint8_t Address, uint8_t *Data, uint8_t Len);

  // Callback For Interrupts. This function will call from ADXL345_IRQ_Handler
  int8_t (*InterruptCallback)(ADXL345_Interrupt_t Interrupt);
} ADXL345_Handler_t;



/**
 ==================================================================================
                               ##### Functions #####                               
 ==================================================================================
 */

/**
 * @brief  Set axis offset
 * @param  Handler: Pointer to handler
 * @param  X: X-axis offset
 * @param  Y: Y-axis offset
 * @param  Z: Z-axis offset
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Set_Offset(ADXL345_Handler_t *Handler, int8_t X, int8_t Y, int8_t Z);

/**
 * @brief  Get axis offset
 * @param  Handler: Pointer to handler
 * @param  X: Pointer to save X-axis offset
 * @param  Y: Pointer to save Y-axis offset
 * @param  Z: Pointer to save Z-axis offset
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Get_Offset(ADXL345_Handler_t *Handler, int8_t *X, int8_t *Y, int8_t *Z);

/**
 * @brief  Convert threshold in g to OFSx register data (where x is X, Y and Z)
 * @param  g: Axis Offset in g
 * @retval OFSx register data
 */
#define ADXL345_ConvToReg_Offset(g) \
  (g / 0.0156f)

/**
 * @brief  Convert OFSx register data (where x is X, Y and Z) to g
 * @param  reg: OFSx register data
 * @retval Axis offset in g
 */
#define ADXL345_ConvToData_Offset(reg) \
  (reg * 0.0156f)


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
                      ADXL345_TapConfig_t *TapConfig);

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
                      ADXL345_TapConfig_t *TapConfig);

/**
 * @brief  Convert threshold in g to THRESH_TAP register data
 * @param  g: Threshold in g
 * @retval THRESH_TAP register data
 */
#define ADXL345_ConvToReg_TapThreshold(g) \
  (g / 0.0625f)

/**
 * @brief  Convert THRESH_TAP register data to g
 * @param  reg: THRESH_TAP register data
 * @retval Tap threshold in g
 */
#define ADXL345_ConvToData_TapThreshold(reg) \
  (reg * 0.0625f)

/**
 * @brief  Convert time in us to DUR register data
 * @param  us: Time in us
 * @retval DUR register data
 */
#define ADXL345_ConvToReg_Duration(us) \
  (us / 625)

/**
 * @brief  Convert DUR register data to us
 * @param  reg: DUR register data
 * @retval DUR time in us
 */
#define ADXL345_ConvToData_Duration(reg) \
  (reg * 625)

/**
 * @brief  Convert time in ms to Latent register data
 * @param  ms: Time in ms
 * @retval Latent register data
 */
#define ADXL345_ConvToReg_Latent(ms) \
  (ms / 1.25f)

/**
 * @brief  Convert Latent register data to ms
 * @param  reg: Latent register data
 * @retval Latent time in ms
 */
#define ADXL345_ConvToData_Latent(reg) \
  (reg * 1.25f)

/**
 * @brief  Convert time in ms to Window register data
 * @param  ms: Time in ms
 * @retval Window register data
 */
#define ADXL345_ConvToReg_Window(ms) \
  (ms / 1.25f)

/**
 * @brief  Convert Window register data to ms
 * @param  reg: Window register data
 * @retval Window time in ms
 */
#define ADXL345_ConvToData_Window(reg) \
  (reg * 1.25f)

/**
 * @brief  Get activity and tap status
 * @param  Handler: Pointer to handler
 * @param  Status: Pointer to activity and tap status
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Get_ActTapStatus(ADXL345_Handler_t *Handler,
                         ADXL345_ActTapStatus_t *Status);


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
                               ADXL345_ActivityInactivity_t *ActivityInactivity);


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
                               ADXL345_ActivityInactivity_t *ActivityInactivity);

/**
 * @brief  Convert threshold in g to THRESH_ACT register data
 * @param  g: Threshold in g
 * @retval THRESH_ACT register data
 */
#define ADXL345_ConvToReg_ActivityThreshold(g) \
  (g / 0.0625f)

/**
 * @brief  Convert THRESH_ACT register data to g
 * @param  reg: THRESH_ACT register data
 * @retval Inactivity threshold in g
 */
#define ADXL345_ConvToData_ActivityThreshold(reg) \
  (reg * 0.0625f)

/**
 * @brief  Convert threshold in g to THRESH_INACT register data
 * @param  g: Threshold in g
 * @retval THRESH_INACT register data
 */
#define ADXL345_ConvToReg_InactivityThreshold(g) \
  (g / 0.0625f)

/**
 * @brief  Convert THRESH_INACT register data to g
 * @param  reg: THRESH_INACT register data
 * @retval Inactivity threshold in g
 */
#define ADXL345_ConvToData_InactivityThreshold(reg) \
  (reg * 0.0625f)

/**
 * @brief  Convert time in s to TIME_INACT register data
 * @param  s: Time in s
 * @retval TIME_INACT register data
 */
#define ADXL345_ConvToReg_InactivityTime(s) \
  (s)

/**
 * @brief  Convert TIME_INACT register data to s
 * @param  reg: TIME_INACT register data
 * @retval Inactivity time in s
 */
#define ADXL345_ConvToData_InactivityTime(reg) \
  (reg)


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
                     uint8_t Threshold, uint8_t Time);

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
                     uint8_t *Threshold, uint8_t *Time);

/**
 * @brief  Convert Acceleration in g to THRESH_FF register data
 * @param  g: Acceleration in g
 * @retval THRESH_FF register data
 */
#define ADXL345_ConvToReg_FreeFallThreshold(g) \
  ((uint8_t)(g / 0.0625f))

/**
 * @brief  Convert THRESH_FF register data to g
 * @param  reg: THRESH_FF register data
 * @retval Free-fall Threshold in g
 */
#define ADXL345_ConvToData_FreeFallThreshold(reg) \
  (reg * 0.0625f)

/**
 * @brief  Convert time in ms to TIME_FF register data
 * @param  ms: Time in ms
 * @retval TIME_FF register data
 */
#define ADXL345_ConvToReg_FreeFallTime(ms) \
  ((uint8_t)(ms / 5))

/**
 * @brief  Convert TIME_FF register data to ms
 * @param  reg: TIME_FF register data
 * @retval Free-fall time in ms
 */
#define ADXL345_ConvToData_FreeFallTime(reg) \
  (reg * 5)


/**
 * @brief  Set Data Rate
 * @param  Handler: Pointer to handler
 * @param  Rate: Data Rate
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Set_Rate(ADXL345_Handler_t *Handler, ADXL345_Rate_t Rate);

/**
 * @brief  Get Data Rate
 * @param  Handler: Pointer to handler
 * @param  Rate: Pointer to save Data Rate
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Get_Rate(ADXL345_Handler_t *Handler, ADXL345_Rate_t *Rate);


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
                            ADXL345_InterruptConfig_t *Config);

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
                            ADXL345_InterruptConfig_t *Config);

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
                            ADXL345_InterruptReg_t *Source);


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
                       ADXL345_DataFormat_t *DataFormat);

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
                       ADXL345_DataFormat_t *DataFormat);


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
                       ADXL345_FifoConfig_t *Config);

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
                       ADXL345_FifoConfig_t *Config);


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
                       ADXL345_FifoStatus_t *Status);


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
                         ADXL345_PowerControl_t *PowerControl);

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
                         ADXL345_PowerControl_t *PowerControl);


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
                    uint8_t SamplesBufferLen, uint8_t *ReadSamples);


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
ADXL345_IRQ_Handler(ADXL345_Handler_t *Handler);


/**
 * @brief  Initializer function
 * @param  Handler: Pointer to handler
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_Init(ADXL345_Handler_t *Handler);

/**
 * @brief  Deinitialize function
 * @param  Handler: Pointer to handler
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 */
ADXL345_Result_t
ADXL345_DeInit(ADXL345_Handler_t *Handler);

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
ADXL345_SetAddressI2C(ADXL345_Handler_t *Handler, uint8_t Address);


/**
 * @brief  
 * @param  Handler: Pointer to handler
 * @retval ADXL345_Result_t
 *         - ADXL345_OK: Operation was successful.
 *         - ADXL345_FAIL: Failed to send or receive data.
 */
ADXL345_Result_t
ADXL345_CheckDeviceID(ADXL345_Handler_t *Handler);



#ifdef __cplusplus
}
#endif


#endif //! _ADXL345_H_
