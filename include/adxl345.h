#ifndef ADXL345_H_
#define ADXL345_H_

#include "adxl345_defs.h"

/** @brief          Initialize the ADXL345 device.
 *  @param p_Device Device instance
 *  @return         ADXL345_ERR_OK when no error occurs
 */
ADXL345_Error_t ADXL345_Init(ADXL345_t& p_Device);

/** @brief          Enable low power operations.
 *  @param p_Device Device instance
 *  @param Enable   Low power operations enable / disable
 *  @return         ADXL345_ERR_OK when no error occurs
 */
ADXL345_Error_t ADXL345_EnableLowPower(ADXL345_t& p_Device, bool Enable);

/** @brief          Perform a single shot measurement.
 *  @param p_Device Device instance
 *  @param p_Data   Pointer to measurement data
 *  @return         ADXL345_ERR_OK when no error occurs
 */
ADXL345_Error_t ADXL345_SingleShot(ADXL345_t& p_Device, ADXL345_Data_t* p_Data);

#endif /* ADXL345_H_ */