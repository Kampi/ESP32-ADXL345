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