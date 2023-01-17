#ifndef ADXL345_DEFS_H_
#define ADXL345_DEFS_H_

#include <cstddef>
#include <stdint.h>
#include <stdbool.h>

#include "adxl345_errors.h"

/** @brief              I2C master write function definition.
 *  @param Address      Device address
 *  @param p_Data       Pointer to data buffer
 *  @param Size         Data length
 *  @param p_Instance   Pointer to interface instance
 *  @return             Status code
 */
typedef int8_t (*ADXL345_Write_t)(uint8_t Address, const uint8_t* p_Data, size_t Size, const void* p_Instance);

/** @brief              I2C master read function definition.
 *  @param Address      Device address
 *  @param p_Data       Pointer to data buffer
 *  @param Size         Data length
 *  @param p_Instance   Pointer to interface instance
 *  @return             Status code
 */
typedef int8_t (*ADXL345_Read_t)(uint8_t Address, uint8_t* p_Data, size_t Size, const void* p_Instance);

/** @brief ADXL345 bandwidth settings definition.
 */
typedef enum
{
    ADXL345_BW_010      = 0,                                /**< 0.1 Hz output data rate. */
    ADXL345_BW_020,                                         /**< 0.2 Hz output data rate. */
    ADXL345_BW_039,                                         /**< 0.39 Hz output data rate. */
    ADXL345_BW_078,                                         /**< 0.78 Hz output data rate. */
    ADXL345_BW_156,                                         /**< 1.56 Hz output data rate. */
    ADXL345_BW_313,                                         /**< 3.13 Hz output data rate. */
    ADXL345_BW_625,                                         /**< 6.25 Hz output data rate. */
    ADXL345_BW_125,                                         /**< 12.5 Hz output data rate. */
    ADXL345_BW_25,                                          /**< 25 Hz output data rate. */
    ADXL345_BW_50,                                          /**< 50 Hz output data rate. */
    ADXL345_BW_100,                                         /**< 100 Hz output data rate. */
    ADXL345_BW_200,                                         /**< 200 Hz output data rate. */
    ADXL345_BW_400,                                         /**< 400 Hz output data rate. */
    ADXL345_BW_800,                                         /**< 800 Hz output data rate. */
    ADXL345_BW_1600,                                        /**< 1600 Hz output data rate. */
    ADXL345_BW_3200,                                        /**< 3200 Hz output data rate. */
} ADXL345_Bandwidth_t;

/** @brief ADXL345 axis settings definition.
 */
typedef enum
{
    ADXL345_AXIS_Z      = 1,                                /**< z axis. */
    ADXL345_AXIS_Y      = 2,                                /**< y axis. */
    ADXL345_AXIS_X      = 4,                                /**< X axis. */
} ADXL345_Axis_t;

/** @brief ADXL345 range settings definition.
 */
typedef enum
{
    ADXL345_RANGE_2G    = 0,                                /**< +-2g. */
    ADXL345_RANGE_4G,                                       /**< +-4g. */
    ADXL345_RANGE_8G,                                       /**< +-8g. */
    ADXL345_RANGE_16G,                                      /**< +-16g. */
} ADXL345_Range_t;

/** @brief ADXL345 sleep frequency settings definition.
 */
typedef enum
{
    ADXL345_SLEEPFREQ_8 = 0,                                /**< 8 Hz. */
    ADXL345_SLEEPFREQ_4,                                    /**< 4 Hz. */
    ADXL345_SLEEPFREQ_2,                                    /**< 2 Hz. */
    ADXL345_SLEEPFREQ_1,                                    /**< 1 Hz. */
} ADXL345_SleepFreq_t;

/** @brief ADXL345 interrupt output definition.
 */
typedef enum
{
    ADXL345_INT_OUT_INT1,                                   /**< Enable INT 1 as output. */
    ADXL345_INT_OUT_INT2,                                   /**< Enable INT 2 as output. */
    ADXL345_INT_OUT_DIS = 2,                                /**< No interrupt output. */
} ADXL345_IntOut_t;

/** @brief ADXL345 interrupt source definition.
 */
typedef enum
{
    ADXL345_INT_OVERRUN = 0,                                /**< Overrun interrupt source. */
    ADXL345_INT_WATERMARK,                                  /**< Watermark interrupt source. */
    ADXL345_INT_FREE_FALL,                                  /**< Free fall interrupt source. */
    ADXL345_INT_INACTIVITY,                                 /**< Inactivity interrupt source. */
    ADXL345_INT_ACTIVITY,                                   /**< Activity interrupt source. */
    ADXL345_INT_DOUBLE_TAP,                                 /**< Double tap interrupt source. */
    ADXL345_INT_SINGLE_TAP,                                 /**< Single tap interrupt source. */
    ADXL345_INT_DATA_READY,                                 /**< Data ready interrupt source. */
} ADXL345_IntSource_t;

/** @brief ADXL345 data point object definition.
 */
typedef union
{
    struct
    {
        int16_t x;                                          /**< Data for x axis. */
        int16_t y;                                          /**< Data for y axis. */
        int16_t z;                                          /**< Data for z axis. */
    } Vec;
    uint8_t Raw[6];                                         /**< Raw data. */
} ADXL345_Data_t;

/** @brief ADXL345 tap configuration object definition.
 */
typedef struct
{
    uint8_t Thresh;                                         /**< Tap threshold value. */
    uint8_t Duration;                                       /**< Tap duration. */
    uint8_t AxisMask;                                       /**< Axis selection mask. */
    bool isDouble;                                          /**< Set to #true to enable double tap. */
    uint8_t Latent;                                         /**< Latency for a double tap.
                                                                 NOTE: Only needed when \ref ADXL345_TapConfig_t.isDouble set to #true. */
    uint8_t Window;                                         /**< Double tap window.
                                                                 NOTE: Only needed when \ref ADXL345_TapConfig_t.isDouble set to #true. */
    bool useSupress;                                        /**< Set to #true to supress double tap detection if acceleration is greater than the value in THRESH_TAP between taps.
                                                                 NOTE: Only needed when \ref ADXL345_TapConfig_t.isDouble set to #true. */
} ADXL345_TapConfig_t;

/** @brief ADXL345 interrupt configuration object definition.
 */
typedef struct
{
    uint8_t Mask;                                           /**< Interrupt source mask. */
    ADXL345_IntOut_t Output;                                /**< Interrupt output configuration. */
    bool Invert;                                            /**< Set to #true to invert the output of the interrupt output. */
} ADXL345_IntConfig_t;

/** @brief ADXL345 device object definition.
 */
typedef struct
{
    uint8_t ID;                                             /**< Device ID. */
    struct
    {
        ADXL345_Write_t I2C_Write;                          /**< Platform specific I2C write function. */
        ADXL345_Read_t I2C_Read;                            /**< Platform specific I2C read function. */
        uint8_t I2C_Address;                                /**< I2C address of the device. */
    } I2C;
    struct
    {
        ADXL345_Range_t Range;                              /**< Current measurement range.
                                                                 NOTE: Managed by the driver. */
        bool isFullRes;                                     /**< #true when full resolution mode is enabled.
                                                                 NOTE: Managed by the driver. */
        bool isLeftJustified;                               /**< #true when the data are left-justified.
                                                                 NOTE: Managed by the driver. */
        bool isInitialized;                                 /**< Device was initialized successfully.
                                                                 NOTE: Managed by the driver. */
    } Internal;
} ADXL345_t;

#endif /* ADXL345_DEFS_H_ */