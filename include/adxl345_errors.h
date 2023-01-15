#ifndef ADXL345_ERRORS_H_
#define ADXL345_ERRORS_H_

typedef int ADXL345_Error_t;

/** @brief      Generic error check macro.
 *  @param Func Function that should be checked
 */
#define ADXL345_ERROR_CHECK(Func)                               do                                                                                                              \
                                                                {                                                                                                               \
                                                                    ADXL345_Error_t Error = Func;                                                                              \
                                                                    if(Error != ADXL345_ERR_OK)                                                                                \
                                                                    {                                                                                                           \
                                                                        ESP_LOGE("ADXL345", "Error check failed in (%s) at line (%u): 0x%X!", __FUNCTION__, __LINE__, Error);  \
                                                                        return Error;                                                                                           \
                                                                    }                                                                                                           \
                                                                } while(0);

/** @brief ADXL345 error base.
 */
#define ADXL345_BASE                                            0x7000

/** @brief Everything okay.
 */
#define ADXL345_ERR_OK                                          (ADXL345_BASE + 0)

/** @brief Invalid function parameter.
 */
#define ADXL345_ERR_INVALID_ARG                                 (ADXL345_BASE + 1)

/** @brief Invalid response from sensor.
 */
#define ADXL345_ERR_INVALID_RESPONSE                            (ADXL345_BASE + 2)

/** @brief No device found.
 */
#define ADXL345_ERR_NO_DEVICE                                   (ADXL345_BASE + 3)

/** @brief Device not initialized.
 */
#define ADXL345_ERR_NOT_INITIALIZED                             (ADXL345_BASE + 4)

#endif /* ADXL345_ERRORS_H_ */