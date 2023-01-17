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
ADXL345_Error_t ADXL345_EnableLowPower(ADXL345_t& p_Device);

/** @brief          Disable low power operations.
 *  @param p_Device Device instance
 *  @param Enable   Low power operations enable / disable
 *  @return         ADXL345_ERR_OK when no error occurs
 */
ADXL345_Error_t ADXL345_DisableLowPower(ADXL345_t& p_Device);

/** @brief          Enable a given device interrupt configuration.
 *  @param p_Device Device instance
 *  @param p_Config Pointer to interrupt configuration object
 *  @return         ADXL345_ERR_OK when no error occurs
 */
ADXL345_Error_t ADXL345_EnableInterrupt(ADXL345_t& p_Device, ADXL345_IntConfig_t* p_Config);

/** @brief          Disable a given device interrupt configuration.
 *  @param p_Device Device instance
 *  @param p_Config Pointer to interrupt configuration object
 *  @return         ADXL345_ERR_OK when no error occurs
 */
ADXL345_Error_t ADXL345_DisableInterrupt(ADXL345_t& p_Device, ADXL345_IntConfig_t* p_Config);

/** @brief          Enable the tap function of the device.
 *  @param p_Device Device instance
 *  @param p_Config Pointer to tap configuration object
 *  @return         ADXL345_ERR_OK when no error occurs
 */
ADXL345_Error_t ADXL345_EnableTap(ADXL345_t& p_Device, ADXL345_TapConfig_t* p_Config);

/** @brief          Disable the tap function of the device.
 *  @param p_Device Device instance
 *  @return         ADXL345_ERR_OK when no error occurs
 */
ADXL345_Error_t ADXL345_DisableTap(ADXL345_t& p_Device);

/** @brief              Configure and enable the auto sleep mode of the device.
 *  @param p_Device     Device instance
 *  @param Threshold    Inactivity threshold
 *  @param Time         Inactivity time
 *  @return             ADXL345_ERR_OK when no error occurs
 */
ADXL345_Error_t ADXL345_EnableAutoSleep(ADXL345_t& p_Device, uint8_t Threshold, uint8_t Time);

/** @brief          Set the offset values for the offset compensation.
 *  @param p_Device Device instance
 *  @param x        Offset for the x axis
 *  @param y        Offset for the y axis
 *  @param z        Offset for the z axis
 *  @return         ADXL345_ERR_OK when no error occurs
 */
ADXL345_Error_t ADXL345_SetOffset(ADXL345_t& p_Device, int8_t x, int8_t y, int8_t z);

/** @brief              Set the device bandwith.
 *  @param p_Device     Device instance
 *  @param Bandwidth    Measurement bandwidth
 *  @return             ADXL345_ERR_OK when no error occurs
 */
ADXL345_Error_t ADXL345_SetBandwidth(ADXL345_t& p_Device, ADXL345_Bandwidth_t Bandwidth);

/** @brief              Get the device bandwith.
 *  @param p_Device     Device instance
 *  @param p_Bandwidth  Measurement bandwidth
 *  @return             ADXL345_ERR_OK when no error occurs
 */
ADXL345_Error_t ADXL345_GetBandwidth(ADXL345_t& p_Device, ADXL345_Bandwidth_t* p_Bandwidth);

/** @brief          Perform a single shot measurement.
 *  @param p_Device Device instance
 *  @param p_Data   Pointer to measurement data
 *  @return         ADXL345_ERR_OK when no error occurs
 */
ADXL345_Error_t ADXL345_SingleShot(ADXL345_t& p_Device, ADXL345_Data_t* p_Data);

/** @brief          Put the device into continous measurment.
 *  @param p_Device Device instance
 *  @return         ADXL345_ERR_OK when no error occurs
 */
ADXL345_Error_t ADXL345_Start(ADXL345_t& p_Device);

/** @brief          Read the measurement results from the device.
 *  @param p_Device Device instance
 *  @param p_Data   Pointer to measurement data
 *  @return         ADXL345_ERR_OK when no error occurs
 */
ADXL345_Error_t ADXL345_ReadResult(ADXL345_t& p_Device, ADXL345_Data_t* p_Data);

/** @brief          Remove the device from continous measurment.
 *  @param p_Device Device instance
 *  @return         ADXL345_ERR_OK when no error occurs
 */
ADXL345_Error_t ADXL345_Stop(ADXL345_t& p_Device);

/** @brief          Clear the tap, activity, inactivity and free fall interrupt.
 *  @param p_Device Device instance
 *  @return         ADXL345_ERR_OK when no error occurs
 */
ADXL345_Error_t ADXL345_ClearInterrupts(ADXL345_t& p_Device);

/** @brief          Get the tap status for the x, y and z axis.
 *  @param p_Device Device instance
 *  @param p_Mask   Pointer to tab status mask
 *  @return         ADXL345_ERR_OK when no error occurs
 */
ADXL345_Error_t ADXL345_GetTapStatus(ADXL345_t& p_Device, uint8_t* p_Mask);

#endif /* ADXL345_H_ */