#include <esp_log.h>
#include <esp_err.h>

#include "ADXL345/adxl345.h"

#define ADXL345_REG_DEVID                       0x00
#define ADXL345_REG_BW_RATE                     0x2C
#define ADXL345_REG_POWER_CTL                   0x2D
#define ADXL345_REG_INT_SOURCE                  0x30
#define ADXL345_REG_DATA_FORMAT                 0x31
#define ADXL345_REG_DATAX0                      0x32

#define ADXL345_BIT_DATA_READY                  7
#define ADXL345_BIT_LOW_POWER                   4
#define ADXL345_BIT_MODE_MEASURE                3
#define ADXL345_BIT_FULL_RES                    3
#define ADXL345_BIT_JUSTIFY                     2

#define ADXL345_DEV_ID                          0xE5

static const char* TAG 							= "ADXL345";

/** @brief          Modify the content of a given register.
 *  @param Register Register address
 *  @param Mask     Bit mask for the bits to modify
 *  @param Value    New value
 *  @return         ADXL345_ERR_OK when no error occurs
 */
static ADXL345_Error_t ADXL345_ModifyRegister(ADXL345_t& p_Device, uint8_t Register, uint8_t Mask, uint8_t Value)
{
    uint8_t Temp;

    if(p_Device.Internal.isInitialized == false)
    {
        return ADXL345_ERR_NOT_INITIALIZED;
    }

    if(p_Device.I2C.I2C_Read(Register, &Temp, 1, &p_Device.I2C.I2C_Address) != 0)
    {
        return ADXL345_ERR_INVALID_RESPONSE;
    }

    Temp &= Mask;
    Temp ^= Value;

    if(p_Device.I2C.I2C_Write(Register, &Temp, 1, &p_Device.I2C.I2C_Address) != 0)
    {
        return ADXL345_ERR_INVALID_RESPONSE;
    }

    return ADXL345_ERR_OK;
}

ADXL345_Error_t ADXL345_Init(ADXL345_t& p_Device)
{
    uint8_t Temp;

    if((p_Device.I2C.I2C_Read == NULL) || (p_Device.I2C.I2C_Write == NULL) || (p_Device.I2C.I2C_Address == 0))
    {
        return ADXL345_ERR_INVALID_ARG;
    }
    else if(p_Device.Internal.isInitialized)
    {
        return ADXL345_ERR_OK;
    }

    if(p_Device.I2C.I2C_Read(ADXL345_REG_DEVID, &Temp, 1, &p_Device.I2C.I2C_Address) != 0)
    {
        return ADXL345_ERR_INVALID_RESPONSE;
    }

    ESP_LOGI(TAG, "ID: %u", Temp);

    if(Temp != ADXL345_DEV_ID)
    {
        return ADXL345_ERR_NO_DEVICE;
    }

    p_Device.ID = Temp;

    if(p_Device.I2C.I2C_Read(ADXL345_REG_DATA_FORMAT, &Temp, 1, &p_Device.I2C.I2C_Address) != 0)
    {
        return ADXL345_ERR_INVALID_RESPONSE;
    }

    p_Device.Internal.isFullRes = (Temp & (0x01 << ADXL345_BIT_FULL_RES)) >> ADXL345_BIT_FULL_RES;
    p_Device.Internal.isLeftJustified = (Temp & (0x01 << ADXL345_BIT_JUSTIFY)) >> ADXL345_BIT_JUSTIFY;
    p_Device.Internal.Range = static_cast<ADXL345_Range_t>(Temp & 0x03);
    p_Device.Internal.isInitialized = true;

    ESP_LOGI(TAG, "Full resolution mode: %u", p_Device.Internal.isFullRes);
    ESP_LOGI(TAG, "Left justified: %u", p_Device.Internal.isLeftJustified);
    ESP_LOGI(TAG, "Range: %u", p_Device.Internal.Range);

    return ADXL345_ERR_OK;
}

ADXL345_Error_t ADXL345_EnableLowPower(ADXL345_t& p_Device, bool Enable)
{
    return ADXL345_ModifyRegister(p_Device, ADXL345_REG_BW_RATE, (0x01 << ADXL345_BIT_LOW_POWER), (Enable << ADXL345_BIT_LOW_POWER));
}

ADXL345_Error_t ADXL345_SingleShotRaw(ADXL345_t& p_Device, ADXL345_Data_t* p_Data)
{
    uint8_t Temp[6];

    if(p_Data == NULL)
    {
        return ADXL345_ERR_INVALID_ARG;
    }
    else if(p_Device.Internal.isInitialized == false)
    {
        return ADXL345_ERR_NOT_INITIALIZED;
    }

    ADXL345_ERROR_CHECK(ADXL345_ModifyRegister(p_Device, ADXL345_REG_POWER_CTL, (0x01 << ADXL345_BIT_MODE_MEASURE), (0x01 << ADXL345_BIT_MODE_MEASURE)));

    do
    {
        if(p_Device.I2C.I2C_Read(ADXL345_REG_INT_SOURCE, &Temp[0], 1, &p_Device.I2C.I2C_Address) != 0)
        {
            return ADXL345_ERR_INVALID_RESPONSE;
        }
    } while((Temp[0] & (0x01 << ADXL345_BIT_DATA_READY)) == false);

    if(p_Device.I2C.I2C_Read(ADXL345_REG_DATAX0, &Temp[0], 6, &p_Device.I2C.I2C_Address) != 0)
    {
        return ADXL345_ERR_INVALID_RESPONSE;
    }

    if(p_Device.Internal.isLeftJustified)
    {
        p_Data->Vec.x = (static_cast<int16_t>(Temp[0]) << 8) | Temp[1];
        p_Data->Vec.y = (static_cast<int16_t>(Temp[2]) << 8) | Temp[3];
        p_Data->Vec.z = (static_cast<int16_t>(Temp[4]) << 8) | Temp[5];
    }
    else
    {
        p_Data->Vec.x = (static_cast<int16_t>(Temp[1]) << 8) | Temp[0];
        p_Data->Vec.y = (static_cast<int16_t>(Temp[3]) << 8) | Temp[2];
        p_Data->Vec.z = (static_cast<int16_t>(Temp[5]) << 8) | Temp[4];
    }

    ESP_LOGI(TAG, "Data x: %i", p_Data->Vec.x);
    ESP_LOGI(TAG, "Data y: %i", p_Data->Vec.y);
    ESP_LOGI(TAG, "Data z: %i", p_Data->Vec.z);

    return ADXL345_ModifyRegister(p_Device, ADXL345_REG_POWER_CTL, (0x01 << ADXL345_BIT_MODE_MEASURE), (0x00 << ADXL345_BIT_MODE_MEASURE));
}