#include <esp_log.h>

#include "adxl345.h"

#define ADXL345_REG_DEVID                       0x00
#define ADXL345_REG_THRESH_TAP                  0x1D
#define ADXL345_REG_OFSX                        0x20
#define ADXL345_REG_DUR                         0x21
#define ADXL345_REG_LATENT                      0x22
#define ADXL345_REG_WINDOW                      0x22
#define ADXL345_REG_THRESH_INACT                0x25
#define ADXL345_REG_TIME_INACT                  0x26
#define ADXL345_REG_TAP_AXES                    0x2A
#define ADXL345_REG_ACT_TAP_STATUS              0x2B
#define ADXL345_REG_BW_RATE                     0x2C
#define ADXL345_REG_POWER_CTL                   0x2D
#define ADXL345_REG_INT_ENABLE                  0x2E
#define ADXL345_REG_INT_MAP                     0x2F
#define ADXL345_REG_INT_SOURCE                  0x30
#define ADXL345_REG_DATA_FORMAT                 0x31
#define ADXL345_REG_DATAX0                      0x32

#define ADXL345_BIT_DATA_READY                  7
#define ADXL345_BIT_LINK                        5
#define ADXL345_BIT_INT_INVERT                  5
#define ADXL345_BIT_LOW_POWER                   4
#define ADXL345_BIT_AUTO_SLEEP                  4
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

    Temp &= ~Mask;
    Temp |= Value;

    ESP_LOGI(TAG, "Write new value %u to register 0x%X...", Temp, Register);

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

    // Disable measurement and sleep mode
    // Disable all interrupt mappings
    // Disable all interrupt
    Temp = 0;
    if((p_Device.I2C.I2C_Write(ADXL345_REG_POWER_CTL, &Temp, 1, &p_Device.I2C.I2C_Address) != 0) ||
       (p_Device.I2C.I2C_Write(ADXL345_REG_INT_MAP, &Temp, 1, &p_Device.I2C.I2C_Address) != 0) ||
       (p_Device.I2C.I2C_Write(ADXL345_REG_INT_ENABLE, &Temp, 1, &p_Device.I2C.I2C_Address) != 0)
      )
    {
        return ADXL345_ERR_INVALID_RESPONSE;
    }

    // Disable low power
    // Set the bandwith to 100 Hz
    Temp = ADXL345_BW_100;
    if((p_Device.I2C.I2C_Write(ADXL345_REG_BW_RATE, &Temp, 1, &p_Device.I2C.I2C_Address) != 0)
      )
    {
        return ADXL345_ERR_INVALID_RESPONSE;
    }

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

ADXL345_Error_t ADXL345_EnableLowPower(ADXL345_t& p_Device)
{
    return ADXL345_ModifyRegister(p_Device, ADXL345_REG_BW_RATE, (0x01 << ADXL345_BIT_LOW_POWER), (true << ADXL345_BIT_LOW_POWER));
}

ADXL345_Error_t ADXL345_DisableLowPower(ADXL345_t& p_Device)
{
    return ADXL345_ModifyRegister(p_Device, ADXL345_REG_BW_RATE, (0x01 << ADXL345_BIT_LOW_POWER), (false << ADXL345_BIT_LOW_POWER));
}

ADXL345_Error_t ADXL345_EnableInterrupt(ADXL345_t& p_Device, ADXL345_IntConfig_t* p_Config)
{
    if(p_Config == NULL)
    {
        return ADXL345_ERR_INVALID_ARG;
    }

    ADXL345_ERROR_CHECK(ADXL345_ModifyRegister(p_Device, ADXL345_REG_INT_ENABLE, p_Config->Mask, p_Config->Mask));

    if(p_Config->Output != ADXL345_INT_OUT_DIS)
    {
        if(p_Config->Output == ADXL345_INT_OUT_INT1)
        {
            ADXL345_ERROR_CHECK(ADXL345_ModifyRegister(p_Device, ADXL345_REG_INT_MAP, p_Config->Mask, 0));
        }
        else if(p_Config->Output == ADXL345_INT_OUT_INT2)
        {
            ADXL345_ERROR_CHECK(ADXL345_ModifyRegister(p_Device, ADXL345_REG_INT_MAP, p_Config->Mask, p_Config->Mask));
        }
    }

    return ADXL345_ModifyRegister(p_Device, ADXL345_REG_DATA_FORMAT, 0x01 << ADXL345_BIT_INT_INVERT, p_Config->Invert << 0x01 << ADXL345_BIT_INT_INVERT);
}

ADXL345_Error_t ADXL345_DisableInterrupt(ADXL345_t& p_Device, ADXL345_IntConfig_t* p_Config)
{
    return ADXL345_ModifyRegister(p_Device, ADXL345_REG_INT_ENABLE, p_Config->Mask, 0);
}

ADXL345_Error_t ADXL345_EnableTap(ADXL345_t& p_Device, ADXL345_TapConfig_t* p_Config)
{
    if(p_Config == NULL)
    {
        return ADXL345_ERR_INVALID_ARG;
    }

    ADXL345_ERROR_CHECK(ADXL345_ModifyRegister(p_Device, ADXL345_REG_THRESH_TAP, 0xFF, p_Config->Thresh));
    ADXL345_ERROR_CHECK(ADXL345_ModifyRegister(p_Device, ADXL345_REG_DUR, 0xFF, p_Config->Duration));

    if(p_Config->isDouble)
    {
        ADXL345_ERROR_CHECK(ADXL345_ModifyRegister(p_Device, ADXL345_REG_WINDOW, 0xFF, p_Config->Latent));
        ADXL345_ERROR_CHECK(ADXL345_ModifyRegister(p_Device, ADXL345_REG_LATENT, 0xFF, p_Config->Window));
    }

    return ADXL345_ModifyRegister(p_Device, ADXL345_REG_TAP_AXES, 0x0F, (p_Config->useSupress << 0x03) | p_Config->AxisMask);
}

ADXL345_Error_t ADXL345_DisableTap(ADXL345_t& p_Device)
{
    ADXL345_ERROR_CHECK(ADXL345_ModifyRegister(p_Device, ADXL345_REG_DUR, 0xFF, 0));
}

ADXL345_Error_t ADXL345_EnableAutoSleep(ADXL345_t& p_Device, uint8_t Threshold, uint8_t Time)
{
    ADXL345_ERROR_CHECK(ADXL345_ModifyRegister(p_Device, ADXL345_REG_THRESH_INACT, 0xFF, Threshold));
    ADXL345_ERROR_CHECK(ADXL345_ModifyRegister(p_Device, ADXL345_REG_TIME_INACT, 0xFF, Time));
    return ADXL345_ModifyRegister(p_Device, ADXL345_REG_POWER_CTL, (0x01 << ADXL345_BIT_LINK) | (0x01 << ADXL345_BIT_AUTO_SLEEP), (0x01 << ADXL345_BIT_LINK) | (0x01 << ADXL345_BIT_AUTO_SLEEP));
}

ADXL345_Error_t ADXL345_SetOffset(ADXL345_t& p_Device, int8_t x, int8_t y, int8_t z)
{
    uint8_t Temp[3];

    if(p_Device.Internal.isInitialized)
    {
        return ADXL345_ERR_OK;
    }

    Temp[0] = x;
    Temp[1] = y;
    Temp[2] = z;

    if(p_Device.I2C.I2C_Write(ADXL345_REG_OFSX, Temp, 3, &p_Device.I2C.I2C_Address) != 0)
    {
        return ADXL345_ERR_INVALID_RESPONSE;
    }

    return ADXL345_ERR_OK;
}

ADXL345_Error_t ADXL345_SetBandwidth(ADXL345_t& p_Device, ADXL345_Bandwidth_t Bandwidth)
{
    return ADXL345_ModifyRegister(p_Device, ADXL345_REG_BW_RATE, 0x0F, Bandwidth);
}

ADXL345_Error_t ADXL345_GetBandwidth(ADXL345_t& p_Device, ADXL345_Bandwidth_t* p_Bandwidth)
{
    uint8_t Temp;

    if(p_Bandwidth == NULL)
    {
        return ADXL345_ERR_INVALID_ARG;
    }

    if(p_Device.I2C.I2C_Read(ADXL345_REG_BW_RATE, &Temp, 1, &p_Device.I2C.I2C_Address) != 0)
    {
        return ADXL345_ERR_INVALID_RESPONSE;
    }

    *p_Bandwidth = static_cast<ADXL345_Bandwidth_t>(Temp & 0x0F);

    return ADXL345_ERR_OK;
}

ADXL345_Error_t ADXL345_SingleShot(ADXL345_t& p_Device, ADXL345_Data_t* p_Data)
{
    uint8_t Temp;

    ADXL345_ERROR_CHECK(ADXL345_Start(p_Device));

    do
    {
        if(p_Device.I2C.I2C_Read(ADXL345_REG_INT_SOURCE, &Temp, 1, &p_Device.I2C.I2C_Address) != 0)
        {
            return ADXL345_ERR_INVALID_RESPONSE;
        }
    } while((Temp & (0x01 << ADXL345_BIT_DATA_READY)) == false);

    ADXL345_ERROR_CHECK(ADXL345_ReadResult(p_Device, p_Data));

    return ADXL345_Stop(p_Device);
}

ADXL345_Error_t ADXL345_Start(ADXL345_t& p_Device)
{
    return ADXL345_ModifyRegister(p_Device, ADXL345_REG_POWER_CTL, (0x01 << ADXL345_BIT_MODE_MEASURE), (0x01 << ADXL345_BIT_MODE_MEASURE));
}

ADXL345_Error_t ADXL345_ReadResult(ADXL345_t& p_Device, ADXL345_Data_t* p_Data)
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

    ESP_LOGD(TAG, "Data x: %i", p_Data->Vec.x);
    ESP_LOGD(TAG, "Data y: %i", p_Data->Vec.y);
    ESP_LOGD(TAG, "Data z: %i", p_Data->Vec.z);

    return ADXL345_ERR_OK;
}

ADXL345_Error_t ADXL345_Stop(ADXL345_t& p_Device)
{
    return ADXL345_ModifyRegister(p_Device, ADXL345_REG_POWER_CTL, (0x01 << ADXL345_BIT_MODE_MEASURE), (0x00 << ADXL345_BIT_MODE_MEASURE));
}

ADXL345_Error_t ADXL345_ClearInterrupts(ADXL345_t& p_Device)
{
    uint8_t Temp;

    if(p_Device.Internal.isInitialized == false)
    {
        return ADXL345_ERR_NOT_INITIALIZED;
    }

    if(p_Device.I2C.I2C_Read(ADXL345_REG_INT_SOURCE, &Temp, 1, &p_Device.I2C.I2C_Address) != 0)
    {
        return ADXL345_ERR_INVALID_RESPONSE;
    }

    return ADXL345_ERR_OK;
}

ADXL345_Error_t ADXL345_GetTapStatus(ADXL345_t& p_Device, uint8_t* p_Mask)
{
    uint8_t Temp;

    if(p_Mask == NULL)
    {
        return ADXL345_ERR_INVALID_ARG;
    }
    else if(p_Device.Internal.isInitialized == false)
    {
        return ADXL345_ERR_NOT_INITIALIZED;
    }

    if(p_Device.I2C.I2C_Read(ADXL345_REG_ACT_TAP_STATUS, &Temp, 1, &p_Device.I2C.I2C_Address) != 0)
    {
        return ADXL345_ERR_INVALID_RESPONSE;
    }

    *p_Mask = Temp;

    return ADXL345_ERR_OK;
}