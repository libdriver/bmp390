/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_bmp390.c
 * @brief     driver bmp390 source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-05-15
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/05/15  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_bmp390.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Bosch BMP390"        /**< chip name */
#define MANUFACTURER_NAME         "Bosch"               /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.65f                 /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.6f                  /**< chip max supply voltage */
#define MAX_CURRENT               0.73f                 /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                 /**< chip max operating temperature */
#define DRIVER_VERSION            1000                  /**< driver version */

/**
 * @brief chip register definition
 */
#define BMP390_REG_CMD                 0x7E        /**< command register */
#define BMP390_REG_NVM_PAR_T1_L        0x31        /**< NVM PAR T1 low register */
#define BMP390_REG_NVM_PAR_T1_H        0x32        /**< NVM PAR T1 high register */
#define BMP390_REG_NVM_PAR_T2_L        0x33        /**< NVM PAR T2 low register */
#define BMP390_REG_NVM_PAR_T2_H        0x34        /**< NVM PAR T2 high register */
#define BMP390_REG_NVM_PAR_T3          0x35        /**< NVM PAR T3 register */
#define BMP390_REG_NVM_PAR_P1_L        0x36        /**< NVM PAR P1 low register */
#define BMP390_REG_NVM_PAR_P1_H        0x37        /**< NVM PAR P1 hgih register */
#define BMP390_REG_NVM_PAR_P2_L        0x38        /**< NVM PAR P2 low register */
#define BMP390_REG_NVM_PAR_P2_H        0x39        /**< NVM PAR P2 hgih register */
#define BMP390_REG_NVM_PAR_P3          0x3A        /**< NVM PAR P3 register */
#define BMP390_REG_NVM_PAR_P4          0x3B        /**< NVM PAR P4 register */
#define BMP390_REG_NVM_PAR_P5_L        0x3C        /**< NVM PAR P5 low register */
#define BMP390_REG_NVM_PAR_P5_H        0x3D        /**< NVM PAR P5 hgih register */
#define BMP390_REG_NVM_PAR_P6_L        0x3E        /**< NVM PAR P6 low register */
#define BMP390_REG_NVM_PAR_P6_H        0x3F        /**< NVM PAR P6 hgih register */
#define BMP390_REG_NVM_PAR_P7          0x40        /**< NVM PAR P7 register */
#define BMP390_REG_NVM_PAR_P8          0x41        /**< NVM PAR P8 register */
#define BMP390_REG_NVM_PAR_P9_L        0x42        /**< NVM PAR P9 low register */
#define BMP390_REG_NVM_PAR_P9_H        0x43        /**< NVM PAR P9 hgih register */
#define BMP390_REG_NVM_PAR_P10         0x44        /**< NVM PAR P10 register */
#define BMP390_REG_NVM_PAR_P11         0x45        /**< NVM PAR P11 register */
#define BMP390_REG_CONFIG              0x1F        /**< configure register */
#define BMP390_REG_ODR                 0x1D        /**< odr register */
#define BMP390_REG_OSR                 0x1C        /**< osr register */
#define BMP390_REG_PWR_CTRL            0x1B        /**< power control register */
#define BMP390_REG_IF_CONF             0x1A        /**< if configure register */
#define BMP390_REG_INT_CTRL            0x19        /**< interrupt control register */
#define BMP390_REG_FIFO_CONFIG_2       0x18        /**< fifo configure 2 register */
#define BMP390_REG_FIFO_CONFIG_1       0x17        /**< fifo configure 1 register */
#define BMP390_REG_FIFO_WTM_1          0x16        /**< fifo watermark 1 register */
#define BMP390_REG_FIFO_WTM_0          0x15        /**< fifo watermark 2 register */
#define BMP390_REG_FIFO_DATA           0x14        /**< fifo data register */
#define BMP390_REG_FIFO_LENGTH_1       0x13        /**< fifo length 1 register */
#define BMP390_REG_FIFO_LENGTH_0       0x12        /**< fifo length 0 register */
#define BMP390_REG_INT_STATUS          0x11        /**< interrupt status register */
#define BMP390_REG_EVENT               0x10        /**< event register */
#define BMP390_REG_SENSORTIME_2        0x0E        /**< sensortime 2 register */
#define BMP390_REG_SENSORTIME_1        0x0D        /**< sensortime 1 register */
#define BMP390_REG_SENSORTIME_0        0x0C        /**< sensortime 0 register */
#define BMP390_REG_DATA_5              0x09        /**< data 5 register */
#define BMP390_REG_DATA_4              0x08        /**< data 4 register */
#define BMP390_REG_DATA_3              0x07        /**< data 3 register */
#define BMP390_REG_DATA_2              0x06        /**< data 2 register */
#define BMP390_REG_DATA_1              0x05        /**< data 1 register */
#define BMP390_REG_DATA_0              0x04        /**< data 0 register */
#define BMP390_REG_STATUS              0x03        /**< status register */
#define BMP390_REG_ERR_REG             0x02        /**< error register */
#define BMP390_REG_REV_ID              0x01        /**< rev id register */
#define BMP390_REG_CHIP_ID             0x00        /**< chip id register */

/**
 * @brief      read multiple bytes
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[in]  reg is the register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 iic spi read failed
 * @note       none
 */
static uint8_t a_bmp390_iic_spi_read(bmp390_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_spi == BMP390_INTERFACE_IIC)                                      /* iic interface */
    {
        if (handle->iic_read(handle->iic_addr, reg, buf, len) != 0)                   /* iic read */
        {
            return 1;                                                                 /* return error */
        }
        else
        {
            return 0;                                                                 /* success return 0 */
        }
    }
    else                                                                              /* spi interface */
    {
        reg |= 1 << 7;                                                                /* set read mode */
        if (handle->spi_read(reg, handle->buf, 
                             len > 512 ? (512 + 1) : (len + 1)) != 0)                 /* spi read */
        {
            return 1;                                                                 /* return error */
        }
        memcpy(buf, handle->buf+1, (len > 512) ? 512 : len);                          /* copy data */
        
        return 0;                                                                     /* success return 0 */
    }
}

/**
 * @brief     write multiple bytes
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] reg is the register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 iic spi write failed
 * @note      none
 */
static uint8_t a_bmp390_iic_spi_write(bmp390_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_spi == BMP390_INTERFACE_IIC)                             /* iic interface */
    {
        uint16_t i;
        
        for (i = 0; i < len; i++)                                            /* write data one byte by one byte */
        {
            if (handle->iic_write(handle->iic_addr, 
                                  (uint8_t)(reg + i), buf + i, 1) != 0)      /* iic write */
            {
                return 1;                                                    /* return error */
            }
        }
        
        return 0;                                                            /* success return 0 */
    }
    else
    {
        uint16_t i;
        
        reg &= ~(1 << 7);                                                    /* write mode */
        for (i = 0; i < len; i++)                                            /* write data one byte by one byte */
        {
            if (handle->spi_write((uint8_t)(reg + i), buf + i, 1) != 0)      /* spi write */
            {
                return 1;                                                    /* return error */
            }
        }
        
        return 0;                                                            /* success return 0 */
    }
}

/**
 * @brief  get the calibration data
 * @return status code
 *         - 0 success
 *         - 1 get calibration data failed
 * @note   none
 */
static uint8_t a_bmp390_get_calibration_data(bmp390_handle_t *handle)
{
    uint8_t buf[2];
    
    if (a_bmp390_iic_spi_read(handle, BMP390_REG_NVM_PAR_T1_L, (uint8_t *)buf, 2) != 0)  /* read t1 */
    {
        handle->debug_print("bmp390: get calibration data failed.\n");                   /* get calibration data failed */
       
        return 1;                                                                        /* return error */
    }
    handle->t1 = (uint16_t)buf[1] <<8 | buf[0];                                          /* set t1 */
    if (a_bmp390_iic_spi_read(handle, BMP390_REG_NVM_PAR_T2_L, (uint8_t *)buf, 2) != 0)  /* read t2 */
    {
        handle->debug_print("bmp390: get calibration data failed.\n");                   /* get calibration data failed */
       
        return 1;                                                                        /* return error */
    }
    handle->t2 = (uint16_t)buf[1] << 8 | buf[0];                                         /* set t2 */
    if (a_bmp390_iic_spi_read(handle, BMP390_REG_NVM_PAR_T3, (uint8_t *)buf, 1) != 0)    /* read t3 */
    {
        handle->debug_print("bmp390: get calibration data failed.\n");                   /* get calibration data failed */
       
        return 1;                                                                        /* return error */
    }
    handle->t3 = (int8_t)(buf[0]);                                                       /* set t3 */
    if (a_bmp390_iic_spi_read(handle, BMP390_REG_NVM_PAR_P1_L, (uint8_t *)buf, 2) != 0)  /* read p1 */
    {
        handle->debug_print("bmp390: get calibration data failed.\n");                   /* get calibration data failed */
       
        return 1;                                                                        /* return error */
    }
    handle->p1 = (int16_t)((uint16_t)buf[1] << 8 | buf[0]);                              /* set p1 */
    if (a_bmp390_iic_spi_read(handle, BMP390_REG_NVM_PAR_P2_L, (uint8_t *)buf, 2) != 0)  /* read p2 */
    {
        handle->debug_print("bmp390: get calibration data failed.\n");                   /* get calibration data failed */
       
        return 1;                                                                        /* return error */
    }
    handle->p2 = (int16_t)((uint16_t)buf[1] << 8 | buf[0]);                              /* set p2 */
    if (a_bmp390_iic_spi_read(handle, BMP390_REG_NVM_PAR_P3, (uint8_t *)buf, 1) != 0)    /* read p3 */
    {
        handle->debug_print("bmp390: get calibration data failed.\n");                   /* get calibration data failed */
       
        return 1;                                                                        /* return error */
    }
    handle->p3 = (int8_t)(buf[0]);                                                       /* set p3 */
    if (a_bmp390_iic_spi_read(handle, BMP390_REG_NVM_PAR_P4, (uint8_t *)buf, 1) != 0)    /* read p4 */
    {
        handle->debug_print("bmp390: get calibration data failed.\n");                   /* get calibration data failed */
       
        return 1;                                                                        /* return error */
    }
    handle->p4 = (int8_t)(buf[0]);                                                       /* set p4 */
    if (a_bmp390_iic_spi_read(handle, BMP390_REG_NVM_PAR_P5_L, (uint8_t *)buf, 2) != 0)  /* read p5 */
    {
        handle->debug_print("bmp390: get calibration data failed.\n");                   /* get calibration data failed */
       
        return 1;                                                                        /* return error */
    }
    handle->p5 = (uint16_t)buf[1] << 8 | buf[0];                                         /* set p5 */
    if (a_bmp390_iic_spi_read(handle, BMP390_REG_NVM_PAR_P6_L, (uint8_t *)buf, 2) != 0)  /* read p6l */
    {
        handle->debug_print("bmp390: get calibration data failed.\n");                   /* get calibration data failed */
       
        return 1;                                                                        /* return error */
    }
    handle->p6 = (uint16_t)buf[1] << 8 | buf[0];                                         /* set p6 */
    if (a_bmp390_iic_spi_read(handle, BMP390_REG_NVM_PAR_P7, (uint8_t *)buf, 1) != 0)    /* read p7 */
    {
        handle->debug_print("bmp390: get calibration data failed.\n");                   /* get calibration data failed */
       
        return 1;                                                                        /* return error */
    }
    handle->p7 = (int8_t)(buf[0]);                                                       /* set p7 */
    if (a_bmp390_iic_spi_read(handle, BMP390_REG_NVM_PAR_P8, (uint8_t *)buf, 1) != 0)    /* read p8 */
    {
        handle->debug_print("bmp390: get calibration data failed.\n");                   /* get calibration data failed */
       
        return 1;                                                                        /* return error */
    }
    handle->p8 = (int8_t)(buf[0]);                                                       /* set p8 */
    if (a_bmp390_iic_spi_read(handle, BMP390_REG_NVM_PAR_P9_L, (uint8_t *)buf, 2) != 0)  /* read p9l */
    {
        handle->debug_print("bmp390: get calibration data failed.\n");                   /* get calibration data failed */
       
        return 1;                                                                        /* return error */
    }
    handle->p9 = (int16_t)((uint16_t)buf[1] << 8 | buf[0]);                              /* set p9 */
    if (a_bmp390_iic_spi_read(handle, BMP390_REG_NVM_PAR_P10, (uint8_t *)buf, 1) != 0)   /* read p10 */
    {
        handle->debug_print("bmp390: get calibration data failed.\n");                   /* get calibration data failed */
       
        return 1;                                                                        /* return error */
    }
    handle->p10 = (int8_t)(buf[0]);                                                      /* set p10 */
    if (a_bmp390_iic_spi_read(handle, BMP390_REG_NVM_PAR_P11, (uint8_t *)buf, 1) != 0)   /* read p11 */
    {
        handle->debug_print("bmp390: get calibration data failed.\n");                   /* get calibration data failed */
       
        return 1;                                                                        /* return error */
    }
    handle->p11 = (int8_t)(buf[0]);                                                      /* set p11 */

    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     compensate the temperature
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] data is the raw temperatue
 * @return    compensated temperature
 * @note      none
 */
static int64_t a_bmp390_compensate_temperature(bmp390_handle_t *handle, uint32_t data)
{ 
    uint64_t partial_data1;
    uint64_t partial_data2;
    uint64_t partial_data3;
    int64_t partial_data4;
    int64_t partial_data5;
    int64_t partial_data6;
    int64_t comp_temp;

    /* calculate compensate temperature */
    partial_data1 = (uint64_t)(data - (256 * (uint64_t)(handle->t1)));
    partial_data2 = (uint64_t)(handle->t2 * partial_data1);
    partial_data3 = (uint64_t)(partial_data1 * partial_data1);
    partial_data4 = (int64_t)(((int64_t)partial_data3) * ((int64_t)handle->t3));
    partial_data5 = ((int64_t)(((int64_t)partial_data2) * 262144) + (int64_t)partial_data4);
    partial_data6 = (int64_t)(((int64_t)partial_data5) / 4294967296U);
    handle->t_fine = partial_data6;
    comp_temp = (int64_t)((partial_data6 * 25)  / 16384);
    
    return comp_temp;
}

/**
 * @brief     compensate the pressure
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] data is the raw pressure
 * @return    compensated pressure
 * @note      none
 */
static int64_t a_bmp390_compensate_pressure(bmp390_handle_t *handle, uint32_t data)
{
    int64_t partial_data1;
    int64_t partial_data2;
    int64_t partial_data3;
    int64_t partial_data4;
    int64_t partial_data5;
    int64_t partial_data6;
    int64_t offset;
    int64_t sensitivity;
    uint64_t comp_press;

    /* calculate compensate pressure */
    partial_data1 = handle->t_fine * handle->t_fine;
    partial_data2 = partial_data1 / 64;
    partial_data3 = (partial_data2 * handle->t_fine) / 256;
    partial_data4 = (handle->p8 * partial_data3) / 32;
    partial_data5 = (handle->p7 * partial_data1) * 16;
    partial_data6 = (handle->p6 * handle->t_fine) * 4194304;
    offset = (int64_t)((int64_t)(handle->p5) * (int64_t)140737488355328U) + partial_data4 + partial_data5 + partial_data6;
    partial_data2 = (((int64_t)handle->p4) * partial_data3) / 32;
    partial_data4 = (handle->p3 * partial_data1) * 4;
    partial_data5 = ((int64_t)(handle->p2) - 16384) * ((int64_t)handle->t_fine) * 2097152;
    sensitivity = (((int64_t)(handle->p1) - 16384) * (int64_t)70368744177664U) + partial_data2 + partial_data4 + partial_data5;
    partial_data1 = (sensitivity / 16777216) * data;
    partial_data2 = (int64_t)(handle->p10) * (int64_t)(handle->t_fine);
    partial_data3 = partial_data2 + (65536 * (int64_t)(handle->p9));
    partial_data4 = (partial_data3 * data) / 8192;
    partial_data5 = (partial_data4 * data) / 512;
    partial_data6 = (int64_t)((uint64_t)data * (uint64_t)data);
    partial_data2 = ((int64_t)(handle->p11) * (int64_t)(partial_data6)) / 65536;
    partial_data3 = (partial_data2 * data) / 128;
    partial_data4 = (offset / 4) + partial_data1 + partial_data5 + partial_data3;
    comp_press = (((uint64_t)partial_data4 * 25) / (uint64_t)1099511627776U);
    
    return comp_press;
}

/**
 * @brief      get the revision id
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *id points to an id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get revision id failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_revision_id(bmp390_handle_t *handle, uint8_t *id)
{
    uint8_t res;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_REV_ID, (uint8_t *)id, 1);       /* read config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("bmp390: get revision id register failed.\n");          /* get revision id register failed */
       
        return 1;                                                                   /* return error */
    }

    return 0;                                                                       /* success return 0 */
}

/**
 * @brief      get the error
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *err points to a error buffer
 * @return     status code
 *             - 0 success
 *             - 1 get error failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_error(bmp390_handle_t *handle, uint8_t *err)
{
    uint8_t res;
    
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_ERR_REG, (uint8_t *)err, 1);       /* read config */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("bmp390: get error register failed.\n");                  /* get error register failed */
       
        return 1;                                                                     /* return error */
    }

    return 0;                                                                         /* success return 0 */
}

/**
 * @brief      get the status
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_status(bmp390_handle_t *handle, uint8_t *status)
{
    uint8_t res;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_STATUS, (uint8_t *)status, 1);       /* read status */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("bmp390: get status register failed.\n");                   /* get status register failed */
       
        return 1;                                                                       /* return error */
    }

    return 0;                                                                           /* success return 0 */
}

/**
 * @brief      get the sensortime
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *t points to a timerstamp buffer
 * @return     status code
 *             - 0 success
 *             - 1 get sensortime failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_sensortime(bmp390_handle_t *handle, uint32_t *t)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_SENSORTIME_0, (uint8_t *)buf, 3);       /* read config */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("bmp390: get sensortime register failed.\n");                  /* get sensortime register failed */
       
        return 1;                                                                          /* return error */
    }
    *t = (uint32_t)buf[2] << 16 | (uint32_t)buf[1] << 8 | buf[0];                          /* get time */
    
    return 0;                                                                              /* success return 0 */
}

/**
 * @brief      get the event
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *event points to a event buffer
 * @return     status code
 *             - 0 success
 *             - 1 get event failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_event(bmp390_handle_t *handle, bmp390_event_t *event)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_EVENT, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("bmp390: get event register failed.\n");                  /* get event register failed */
       
        return 1;                                                                     /* return error */
    }
    *event = (bmp390_event_t)(prev & (3 << 0));                                       /* get event */

    return 0;                                                                         /* success return 0 */
}

/**
 * @brief      get the interrupt status
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_interrupt_status(bmp390_handle_t *handle, uint8_t *status)
{
    uint8_t res;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_INT_STATUS, (uint8_t *)status, 1);       /* read status */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("bmp390: get interrupt status register failed.\n");             /* get interrupt status register failed */
       
        return 1;                                                                           /* return error */
    }

    return 0;                                                                               /* success return 0 */
}

/**
 * @brief      get the fifo length
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *length points to a fifo length buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo length failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_fifo_length(bmp390_handle_t *handle, uint16_t *length)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_LENGTH_0, (uint8_t *)buf, 2);       /* read config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("bmp390: get fifo length register failed.\n");                  /* get fifo length register failed */
       
        return 1;                                                                           /* return error */
    }
    *length = ((uint16_t)(buf[1] & 0x01) << 8) | buf[0];                                    /* get data */
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief      get the fifo data
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *data points to a data buffer
 * @param[in]  length is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 get fifo data failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_fifo_data(bmp390_handle_t *handle, uint8_t *data, uint16_t length)
{
    uint8_t res;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_DATA, (uint8_t *)data, length);       /* read data */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("bmp390: get fifo data register failed.\n");                      /* get fifo data register failed */
       
        return 1;                                                                             /* return error */
    }
    
    return 0;                                                                                 /* success return 0 */
}

/**
 * @brief     set the fifo watermark
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] watermark is the fifo watermark
 * @return    status code
 *            - 0 success
 *            - 1 set fifo watermark failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialize
 * @note      none
 */
uint8_t bmp390_set_fifo_watermark(bmp390_handle_t *handle, uint16_t watermark)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    buf[0] = watermark & 0xFF;                                                            /* set low part */
    buf[1] = (watermark >> 8) & 0x01;                                                     /* set high part */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_FIFO_WTM_0, (uint8_t *)buf, 2);       /* write config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("bmp390: set fifo watermark register failed.\n");             /* set fifo watermark register failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the fifo watermark
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *watermark points to a fifo watermark buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo watermark failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_fifo_watermark(bmp390_handle_t *handle, uint16_t *watermark)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_WTM_0, (uint8_t *)buf, 2);       /* read config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("bmp390: get fifo watermark register failed.\n");            /* get fifo watermark register failed */
       
        return 1;                                                                        /* return error */
    }
    *watermark = ((uint16_t)(buf[1] & 0x01) << 8) | buf[0];                              /* get data */
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     enable or disable the fifo
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set fifo failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_fifo(bmp390_handle_t *handle, bmp390_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_CONFIG_1, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("bmp390: get fifo config 1 register failed.\n");                   /* get fifo config 1 register failed */
       
        return 1;                                                                              /* return error */
    }
    prev &= ~(1 << 0);                                                                         /* clear config */
    prev |= enable << 0;                                                                       /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_FIFO_CONFIG_1, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("bmp390: set fifo config 1 register failed.\n");                   /* set fifo config 1 register failed */
       
        return 1;                                                                              /* return error */
    }   
    
    return 0;                                                                                  /* success return 0 */
}

/**
 * @brief      get the fifo status
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *enable points to a bool buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_fifo(bmp390_handle_t *handle, bmp390_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_CONFIG_1, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("bmp390: get fifo config 1 register failed.\n");                  /* get fifo config 1 register failed */
       
        return 1;                                                                             /* return error */
    }
    *enable = (bmp390_bool_t)(prev & 0x01);                                                   /* get config */
    
    return 0;                                                                                 /* success return 0 */
}

/**
 * @brief     enable or disable the fifo stopping on full
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set fifo stop on full failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_fifo_stop_on_full(bmp390_handle_t *handle, bmp390_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_CONFIG_1, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("bmp390: get fifo config 1 register failed.\n");                   /* get fifo config 1 register failed */
       
        return 1;                                                                              /* return error */
    }
    prev &= ~(1 << 1);                                                                         /* clear config */
    prev |= enable << 1;                                                                       /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_FIFO_CONFIG_1, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("bmp390: set fifo config 1 register failed.\n");                   /* set fifo config 1 register failed */
       
        return 1;                                                                              /* return error */
    }   
    
    return 0;                                                                                  /* success return 0 */
}

/**
 * @brief      get the fifo stopping on full status
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *enable points to a bool buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo stop on full failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_fifo_stop_on_full(bmp390_handle_t *handle, bmp390_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_CONFIG_1, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("bmp390: get fifo config 1 register failed.\n");                  /* get fifo config 1 register failed */
        
        return 1;                                                                             /* return error */
    }
    *enable = (bmp390_bool_t)((prev >> 1) & 0x01);                                            /* get config */
    
    return 0;                                                                                 /* success return 0 */
}

/**
 * @brief     enable or disable the fifo sensortime on
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set fifo sensortime on failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_fifo_sensortime_on(bmp390_handle_t *handle, bmp390_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_CONFIG_1, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("bmp390: get fifo config 1 register failed.\n");                   /* get fifo config 1 register failed */
       
        return 1;                                                                              /* return error */
    }
    prev &= ~(1 << 2);                                                                         /* clear config */
    prev |= enable << 2;                                                                       /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_FIFO_CONFIG_1, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("bmp390: set fifo config 1 register failed.\n");                   /* set fifo config 1 register failed */
       
        return 1;                                                                              /* return error */
    }   
    
    return 0;                                                                                  /* success return 0 */
}

/**
 * @brief      get the fifo sensortime on status
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *enable points to a bool buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo sensortime on failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_fifo_sensortime_on(bmp390_handle_t *handle, bmp390_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_CONFIG_1, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("bmp390: get fifo config 1 register failed.\n");                  /* get fifo config 1 register failed */
       
        return 1;                                                                             /* return error */
    }
    *enable = (bmp390_bool_t)((prev >> 2) & 0x01);                                            /* get config */
    
    return 0;                                                                                 /* success return 0 */
}

/**
 * @brief     enable or disable the fifo pressure on
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set fifo pressure on failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_fifo_pressure_on(bmp390_handle_t *handle, bmp390_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_CONFIG_1, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("bmp390: get fifo config 1 register failed.\n");                   /* get fifo config 1 register failed */
       
        return 1;                                                                              /* return error */
    }
    prev &= ~(1 << 3);                                                                         /* clear config */
    prev |= enable << 3;                                                                       /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_FIFO_CONFIG_1, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("bmp390: set fifo config 1 register failed.\n");                   /* set fifo config 1 register failed */
       
        return 1;                                                                              /* return error */
    }
    
    return 0;                                                                                  /* success return 0 */
}

/**
 * @brief      get the fifo pressure on status
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *enable points to a bool buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo pressure on failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_fifo_pressure_on(bmp390_handle_t *handle, bmp390_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_CONFIG_1, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("bmp390: get fifo config 1 register failed.\n");                  /* get fifo config 1 register failed */
       
        return 1;                                                                             /* return error */
    }
    *enable = (bmp390_bool_t)((prev >> 3) & 0x01);                                            /* get config */
    
    return 0;                                                                                 /* success return 0 */
}

/**
 * @brief     enable or disable the fifo temperature on
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set fifo temperature on failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_fifo_temperature_on(bmp390_handle_t *handle, bmp390_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_CONFIG_1, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("bmp390: get fifo config 1 register failed.\n");                   /* get fifo config 1 register failed */
       
        return 1;                                                                              /* return error */
    }
    prev &= ~(1 << 4);                                                                         /* clear config */
    prev |= enable << 4;                                                                       /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_FIFO_CONFIG_1, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("bmp390: set fifo config 1 register failed.\n");                   /* set fifo config 1 register failed */
       
        return 1;                                                                              /* return error */
    }   
    
    return 0;                                                                                  /* success return 0 */
}

/**
 * @brief      get the fifo temperature on status
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *enable points to a bool buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo temperature on failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_fifo_temperature_on(bmp390_handle_t *handle, bmp390_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_CONFIG_1, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("bmp390: get fifo config 1 register failed.\n");                  /* get fifo config 1 register failed */
       
        return 1;                                                                             /* return error */
    }
    *enable = (bmp390_bool_t)((prev >> 4) & 0x01);                                            /* get config */
    
    return 0;                                                                                 /* success return 0 */
}

/**
 * @brief     set the fifo subsampling
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] subsample is the fifo subsample
 * @return    status code
 *            - 0 success
 *            - 1 set fifo subsampling failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      subsample <= 7
 */
uint8_t bmp390_set_fifo_subsampling(bmp390_handle_t *handle, uint8_t subsample)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    
    if (subsample > 7)                                                                         /* check subsample */
    {
        handle->debug_print("bmp390: subsample is invalid.\n");                                /* subsample is invalid */
       
        return 4;                                                                              /* return error */
    }
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_CONFIG_2, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("bmp390: get fifo config 2 register failed.\n");                   /* get fifo config 2 register failed */
       
        return 1;                                                                              /* return error */
    }
    prev &= ~(7 << 0);                                                                         /* clear config */
    prev |= subsample << 0;                                                                    /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_FIFO_CONFIG_2, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("bmp390: set fifo config 2 register failed.\n");                   /* set fifo config 2 register failed */
       
        return 1;                                                                              /* return error */
    }   
    
    return 0;                                                                                  /* success return 0 */
}

/**
 * @brief      get the fifo subsampling
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *subsample points to subsample buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo subsampling failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_fifo_subsampling(bmp390_handle_t *handle, uint8_t *subsample)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_CONFIG_2, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("bmp390: get fifo config 2 register failed.\n");                  /* get fifo config 2 register failed */
       
        return 1;                                                                             /* return error */
    }
    *subsample = (bmp390_bool_t)((prev >> 0) & 0x07);                                         /* get config */
    
    return 0;                                                                                 /* success return 0 */
}

/**
 * @brief     set the fifo data source
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] source is the fifo data source
 * @return    status code
 *            - 0 success
 *            - 1 set fifo data source failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_fifo_data_source(bmp390_handle_t *handle, bmp390_fifo_data_source_t source)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_CONFIG_2, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("bmp390: get fifo config 2 register failed.\n");                   /* get fifo config 2 register failed */
       
        return 1;                                                                              /* return error */
    }   
    prev &= ~(3 << 3);                                                                         /* clear config */
    prev |= source << 3;                                                                       /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_FIFO_CONFIG_2, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("bmp390: set fifo config 2 register failed.\n");                   /* set fifo config 2 register failed */
       
        return 1;                                                                              /* return error */
    }   
    
    return 0;                                                                                  /* success return 0 */
}

/**
 * @brief      get the fifo data source
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *source points to fifo data source buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo data source failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_fifo_data_source(bmp390_handle_t *handle, bmp390_fifo_data_source_t *source)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_CONFIG_2, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("bmp390: get fifo config 2 register failed.\n");                  /* get fifo config 2 register failed */
       
        return 1;                                                                             /* return error */
    }
    *source = (bmp390_fifo_data_source_t)((prev >> 3) & 0x01);                                /* get config */

    return 0;                                                                                 /* success return 0 */
}

/**
 * @brief     set the interrupt pin type
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] pin_type is the interrupt pin type
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt pin type failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_interrupt_pin_type(bmp390_handle_t *handle, bmp390_interrupt_pin_type_t pin_type)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_INT_CTRL, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("bmp390: get int ctrl register failed.\n");                   /* get int ctrl register failed */
       
        return 1;                                                                         /* return error */
    }   
    prev &= ~(1 << 0);                                                                    /* clear config */
    prev |= pin_type << 0;                                                                /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_INT_CTRL, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("bmp390: set int ctrl register failed.\n");                   /* set int ctrl register failed */
       
        return 1;                                                                         /* return error */
    }   
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the interrupt pin type
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *pin_type points to a interrupt pin type buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt pin type failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_interrupt_pin_type(bmp390_handle_t *handle, bmp390_interrupt_pin_type_t *pin_type)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_INT_CTRL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("bmp390: get int ctrl register failed.\n");                  /* get int ctrl register failed */
       
        return 1;                                                                        /* return error */
    }
    *pin_type = (bmp390_interrupt_pin_type_t)(prev & 0x01);                              /* get interrupt pin type */
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     set the interrupt active level
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] level is the interrupt active level
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt active level failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_interrupt_active_level(bmp390_handle_t *handle, bmp390_interrupt_active_level_t level)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_INT_CTRL, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("bmp390: get int ctrl register failed.\n");                   /* get int ctrl register failed */
       
        return 1;                                                                         /* return error */
    }   
    prev &= ~(1 << 1);                                                                    /* clear config */
    prev |= level << 1;                                                                   /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_INT_CTRL, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("bmp390: set int ctrl register failed.\n");                   /* set int ctrl register failed */
       
        return 1;                                                                         /* return error */
    }   
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the interrupt active level
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *level points to a interrupt active level buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt active level failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_interrupt_active_level(bmp390_handle_t *handle, bmp390_interrupt_active_level_t *level)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_INT_CTRL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("bmp390: get int ctrl register failed.\n");                  /* get int ctrl register failed */
       
        return 1;                                                                        /* return error */
    }
    *level = (bmp390_interrupt_active_level_t)((prev >> 1) & 0x01);                      /* get config */
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     enable or disable latching interrupt pin and interrupt status
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set latch interrupt pin and interrupt status failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_latch_interrupt_pin_and_interrupt_status(bmp390_handle_t *handle, bmp390_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_INT_CTRL, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("bmp390: get int ctrl register failed.\n");                   /* get int ctrl register failed */
       
        return 1;                                                                         /* return error */
    }   
    prev &= ~(1 << 2);                                                                    /* clear config */
    prev |= enable << 2;                                                                  /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_INT_CTRL, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("bmp390: set int ctrl register failed.\n");                   /* set int ctrl register failed */
       
        return 1;                                                                         /* return error */
    }   
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get latching interrupt pin and interrupt status
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *enable points to a bool buffer
 * @return     status code
 *             - 0 success
 *             - 1 get latch interrupt pin and interrupt status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_latch_interrupt_pin_and_interrupt_status(bmp390_handle_t *handle, bmp390_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_INT_CTRL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("bmp390: get int ctrl register failed.\n");                  /* get int ctrl register failed */
       
        return 1;                                                                        /* return error */
    }   
    *enable = (bmp390_bool_t)((prev >> 2) & 0x01);                                       /* get config */
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     enable or disable the fifo watermark interrupt
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt fifo watermark failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_interrupt_fifo_watermark(bmp390_handle_t *handle, bmp390_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_INT_CTRL, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("bmp390: get int ctrl register failed.\n");                   /* get int ctrl register failed */
       
        return 1;                                                                         /* return error */
    }   
    prev &= ~(1 << 3);                                                                    /* clear config */
    prev |= enable << 3;                                                                  /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_INT_CTRL, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("bmp390: set int ctrl register failed.\n");                   /* set int ctrl register failed */
       
        return 1;                                                                         /* return error */
    }   
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the interrupt fifo watermark
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *enable points to a bool buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt fifo watermark failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_interrupt_fifo_watermark(bmp390_handle_t *handle, bmp390_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_INT_CTRL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("bmp390: get int ctrl register failed.\n");                  /* get int ctrl register failed */
       
        return 1;                                                                        /* return error */
    }   
    *enable = (bmp390_bool_t)((prev >> 3) & 0x01);                                       /* get config */
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     enable or disable the fifo full interrupt
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt fifo full failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_interrupt_fifo_full(bmp390_handle_t *handle, bmp390_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_INT_CTRL, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("bmp390: get int ctrl register failed.\n");                   /* get int ctrl register failed */
       
        return 1;                                                                         /* return error */
    }   
    prev &= ~(1 << 4);                                                                    /* clear config */
    prev |= enable << 4;                                                                  /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_INT_CTRL, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("bmp390: set int ctrl register failed.\n");                   /* set int ctrl register failed */
       
        return 1;                                                                         /* return error */
    }   
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the interrupt fifo full
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *enable points to a bool buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt fifo full failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_interrupt_fifo_full(bmp390_handle_t *handle, bmp390_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_INT_CTRL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("bmp390: get int ctrl register failed.\n");                  /* get int ctrl register failed */
       
        return 1;                                                                        /* return error */
    }   
    *enable = (bmp390_bool_t)((prev >> 4) & 0x01);                                       /* get config */
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     enable or disable the data ready interrupt
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt data ready failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_interrupt_data_ready(bmp390_handle_t *handle, bmp390_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_INT_CTRL, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("bmp390: get int ctrl register failed.\n");                   /* get int ctrl register failed */
       
        return 1;                                                                         /* return error */
    }   
    prev &= ~(1 << 6);                                                                    /* clear config */
    prev |= enable << 6;                                                                  /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_INT_CTRL, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("bmp390: set int ctrl register failed.\n");                   /* set int ctrl register failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the interrupt data ready status
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *enable points to a bool buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt data ready failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_interrupt_data_ready(bmp390_handle_t *handle, bmp390_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_INT_CTRL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("bmp390: get int ctrl register failed.\n");                  /* get int ctrl register failed */
       
        return 1;                                                                        /* return error */
    }
    *enable = (bmp390_bool_t)((prev >> 6) & 0x01);                                       /* get config */
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     set the spi wire
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] wire is the spi wire
 * @return    status code
 *            - 0 success
 *            - 1 set spi wire failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_spi_wire(bmp390_handle_t *handle, bmp390_spi_wire_t wire)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_IF_CONF, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("bmp390: get if conf register failed.\n");                   /* get if conf register failed */
       
        return 1;                                                                        /* return error */
    }
    prev &= ~(1 << 0);                                                                   /* clear config */
    prev |= wire << 0;                                                                   /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_IF_CONF, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("bmp390: set if conf register failed.\n");                   /* set if conf register failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      get the spi wire
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *wire points to a spi wire buffer
 * @return     status code
 *             - 0 success
 *             - 1 get spi wire failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_spi_wire(bmp390_handle_t *handle, bmp390_spi_wire_t *wire)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_IF_CONF, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("bmp390: get if conf register failed.\n");                  /* get if conf register failed */
       
        return 1;                                                                       /* return error */
    } 
    *wire = (bmp390_spi_wire_t)(prev & 0x01);                                           /* get config */
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief     enable or disable the iic watchdog timer
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set iic watchdog timer failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_iic_watchdog_timer(bmp390_handle_t *handle, bmp390_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_IF_CONF, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("bmp390: get if conf register failed.\n");                   /* get if conf register failed */
       
        return 1;                                                                        /* return error */
    }
    prev &= ~(1 << 1);                                                                   /* clear config */
    prev |= enable << 1;                                                                 /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_IF_CONF, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("bmp390: set if conf register failed.\n");                   /* set if conf register failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      get the iic watchdog timer status
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *enable points to a bool buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic watchdog timer failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_iic_watchdog_timer(bmp390_handle_t *handle, bmp390_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_IF_CONF, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("bmp390: get if conf register failed.\n");                  /* get if conf register failed */
       
        return 1;                                                                       /* return error */
    } 
    *enable = (bmp390_bool_t)((prev >> 1) & 0x01);                                      /* get config */
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief     set the iic watchdog period
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] period is the iic watchdog period
 * @return    status code
 *            - 0 success
 *            - 1 set iic watchdog period failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_iic_watchdog_period(bmp390_handle_t *handle, bmp390_iic_watchdog_period_t period)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_IF_CONF, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("bmp390: get if conf register failed.\n");                   /* get if conf register failed */
       
        return 1;                                                                        /* return error */
    }
    prev &= ~(1 << 2);                                                                   /* clear config */
    prev |= period << 2;                                                                 /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_IF_CONF, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("bmp390: set if conf register failed.\n");                   /* set if conf register failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      get the iic watchdog period
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *period points to a iic watchdog period buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic watchdog period failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_iic_watchdog_period(bmp390_handle_t *handle, bmp390_iic_watchdog_period_t *period)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_IF_CONF, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("bmp390: get if conf register failed.\n");                  /* get if conf register failed */
       
        return 1;                                                                       /* return error */
    } 
    *period = (bmp390_iic_watchdog_period_t)((prev >> 2) & 0x01);                       /* get config */
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief     enable or disable the pressrue
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set pressure failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_pressure(bmp390_handle_t *handle, bmp390_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_PWR_CTRL, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("bmp390: get pwr ctrl register failed.\n");                   /* get pwr ctrl register failed */
       
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 0);                                                                    /* clear config */
    prev |= enable << 0;                                                                  /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_PWR_CTRL, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("bmp390: set pwr ctrl register failed.\n");                   /* set pwr ctrl register failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the pressure status
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *enable points to a bool buffer
 * @return     status code
 *             - 0 success
 *             - 1 get pressure failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_pressure(bmp390_handle_t *handle, bmp390_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_PWR_CTRL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("bmp390: get pwr ctrl register failed.\n");                  /* get pwr ctrl register failed */
       
        return 1;                                                                        /* return error */
    }  
    *enable = (bmp390_bool_t)((prev >> 0) & 0x01);                                       /* get config */
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     enable or disable the temperature
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set temperature failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_temperature(bmp390_handle_t *handle, bmp390_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_PWR_CTRL, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("bmp390: get pwr ctrl register failed.\n");                   /* get pwr ctrl register failed */
       
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 1);                                                                    /* clear config */
    prev |= enable << 1;                                                                  /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_PWR_CTRL, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("bmp390: set pwr ctrl register failed.\n");                   /* set pwr ctrl register failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the temperature status
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *enable points to a bool buffer
 * @return     status code
 *             - 0 success
 *             - 1 get temperature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_temperature(bmp390_handle_t *handle, bmp390_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_PWR_CTRL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("bmp390: get pwr ctrl register failed.\n");                  /* get pwr ctrl register failed */
       
        return 1;                                                                        /* return error */
    }  
    *enable = (bmp390_bool_t)((prev >> 1) & 0x01);                                       /* get config */
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     set the chip mode
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] mode is the chip mode
 * @return    status code
 *            - 0 success
 *            - 1 set mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_mode(bmp390_handle_t *handle, bmp390_mode_t mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_PWR_CTRL, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("bmp390: get pwr ctrl register failed.\n");                   /* get pwr ctrl register failed */
       
        return 1;                                                                         /* return error */
    }
    prev &= ~(3 << 4);                                                                    /* clear config */
    prev |= mode << 4;                                                                    /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_PWR_CTRL, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("bmp390: set pwr ctrl register failed.\n");                   /* set pwr ctrl register failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the chip mode
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *mode points to a mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_mode(bmp390_handle_t *handle, bmp390_mode_t *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_PWR_CTRL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("bmp390: get pwr ctrl register failed.\n");                  /* get pwr ctrl register failed */
       
        return 1;                                                                        /* return error */
    }
    *mode = (bmp390_mode_t)((prev >> 4) & 0x03);                                         /* get config */
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     set the pressure oversampling
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] oversampling is the pressure oversampling
 * @return    status code
 *            - 0 success
 *            - 1 set pressure oversampling failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_pressure_oversampling(bmp390_handle_t *handle, bmp390_oversampling_t oversampling)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_OSR, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("bmp390: get osr register failed.\n");                   /* get osr register failed */
       
        return 1;                                                                    /* return error */
    }
    prev &= ~(7 << 0);                                                               /* clear config */
    prev |= oversampling << 0;                                                       /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_OSR, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("bmp390: set osr register failed.\n");                   /* set osr register failed */
       
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief      get the pressure oversampling
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *oversampling points to a oversampling buffer
 * @return     status code
 *             - 0 success
 *             - 1 get pressure oversampling failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_pressure_oversampling(bmp390_handle_t *handle, bmp390_oversampling_t *oversampling)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_OSR, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("bmp390: get osr register failed.\n");                  /* get osr register failed */
       
        return 1;                                                                   /* return error */
    }
    *oversampling = (bmp390_oversampling_t)((prev >> 0) & 0x07);                    /* get config */
    
    return 0;                                                                       /* success return 0 */
}

/**
 * @brief     set the temperature oversampling
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] oversampling is the temperature oversampling
 * @return    status code
 *            - 0 success
 *            - 1 set temperature oversampling failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_temperature_oversampling(bmp390_handle_t *handle, bmp390_oversampling_t oversampling)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_OSR, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("bmp390: get osr register failed.\n");                   /* get osr register failed */
       
        return 1;                                                                    /* return error */
    }
    prev &= ~(7 << 3);                                                               /* clear config */
    prev |= oversampling << 3;                                                       /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_OSR, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("bmp390: set osr register failed.\n");                   /* set osr register failed */
       
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief      get the temperature oversampling
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *oversampling points to a oversampling buffer
 * @return     status code
 *             - 0 success
 *             - 1 get temperature oversampling failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_temperature_oversampling(bmp390_handle_t *handle, bmp390_oversampling_t *oversampling)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_OSR, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("bmp390: get osr register failed.\n");                  /* get osr register failed */
       
        return 1;                                                                   /* return error */
    }
    *oversampling = (bmp390_oversampling_t)((prev >> 3) & 0x07);                    /* get config */
    
    return 0;                                                                       /* success return 0 */
}

/**
 * @brief     set the output data rate
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] odr is the output data rate
 * @return    status code
 *            - 0 success
 *            - 1 set odr failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_odr(bmp390_handle_t *handle, bmp390_odr_t odr)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_ODR, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("bmp390: get odr register failed.\n");                   /* get odr register failed */
       
        return 1;                                                                    /* return error */
    }
    
    prev &= ~(31 << 0);                                                              /* clear config */
    prev |= odr << 0;                                                                /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_ODR, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("bmp390: set odr register failed.\n");                   /* set odr register failed */
       
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief      get the output data rate
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *odr points to a output data rate buffer
 * @return     status code
 *             - 0 success
 *             - 1 get odr failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_odr(bmp390_handle_t *handle, bmp390_odr_t *odr)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_ODR, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("bmp390: get odr register failed.\n");                  /* get odr register failed */
       
        return 1;                                                                   /* return error */
    }
    *odr = (bmp390_odr_t)((prev >> 0) & 31);                                        /* get config */
    
    return 0;                                                                       /* success return 0 */
}

/**
 * @brief     set the filter coefficient
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] coefficient is the filter coefficient
 * @return    status code
 *            - 0 success
 *            - 1 set filter coefficient failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_filter_coefficient(bmp390_handle_t *handle, bmp390_filter_coefficient_t coefficient)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_CONFIG, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("bmp390: get config register failed.\n");                   /* get config register failed */
       
        return 1;                                                                       /* return error */
    }
    
    prev &= ~(0x7 << 1);                                                                /* clear config */
    prev |= coefficient << 1;                                                           /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_CONFIG, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("bmp390: set config register failed.\n");                   /* set config register failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief      get the filter coefficient
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *coefficient points to a filter coefficient buffer
 * @return     status code
 *             - 0 success
 *             - 1 get filter coefficient failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_filter_coefficient(bmp390_handle_t *handle, bmp390_filter_coefficient_t *coefficient)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_CONFIG, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("bmp390: get config register failed.\n");                  /* return error */
       
        return 1;                                                                      /* return error */
    }
    *coefficient = (bmp390_filter_coefficient_t)((prev >> 1) & 0x07);                  /* get coefficient */
    
    return 0;                                                                          /* success return 0 */
}

/**
 * @brief     flush the fifo
 * @param[in] *handle points to a bmp390 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 flush fifo failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_flush_fifo(bmp390_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    prev = 0xB0;                                                                     /* command */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_CMD, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("bmp390: set cmd register failed.\n");                   /* set cmd register failed */
       
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief     soft reset
 * @param[in] *handle points to a bmp390 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 soft reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_softreset(bmp390_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    prev = 0xB6;                                                                     /* command */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_CMD, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("bmp390: set cmd register failed.\n");                   /* set cmd register failed */
       
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief     close the chip
 * @param[in] *handle points to a bmp390 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 close failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
static uint8_t a_bmp390_close(bmp390_handle_t *handle)
{
    if (handle->iic_spi == BMP390_INTERFACE_IIC)                        /* if iic interface */
    {
        if (handle->iic_deinit() != 0)                                  /* close iic */
        {
            handle->debug_print("bmp390: iic deinit failed.\n");        /* iic deinit failed */
       
            return 1;                                                   /* return error */
        }
        else
        {
            return 0;                                                   /* success return 0 */
        }
    }
    else
    {
        if (handle->spi_deinit() != 0)                                  /* close spi */
        {
            handle->debug_print("bmp390: spi deinit failed.\n");        /* spi deinit failed */
       
            return 1;                                                   /* return error */
        }
        else
        {
            return 0;                                                   /* success return 0 */
        }
    }
}

/**
 * @brief     initialize the chip
 * @param[in] *handle points to a bmp390 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic or spi initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 id is invalid
 *            - 5 reset failed
 *            - 6 get calibration failed
 * @note      none
 */
uint8_t bmp390_init(bmp390_handle_t *handle)
{
    uint8_t id;
    uint8_t reg;
  
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->debug_print == NULL)                                                 /* check debug_print */
    {
        return 3;                                                                    /* return error */
    }
    if (handle->iic_init == NULL)                                                    /* check iic_init */
    {
        handle->debug_print("bmp390: iic_init is null.\n");                          /* iic_init is null */
       
        return 3;                                                                    /* return error */
    }
    if (handle->iic_deinit == NULL)                                                  /* check iic_init */
    {
        handle->debug_print("bmp390: iic_deinit is null.\n");                        /* iic_deinit is null */
       
        return 3;                                                                    /* return error */
    }
    if (handle->iic_read == NULL)                                                    /* check iic_read */
    {
        handle->debug_print("bmp390: iic_read is null.\n");                          /* iic_read is null */
       
        return 3;                                                                    /* return error */
    }
    if (handle->iic_write == NULL)                                                   /* check iic_write */
    {
        handle->debug_print("bmp390: iic_write is null.\n");                         /* iic_write is null */
       
        return 3;                                                                    /* return error */
    }
    if (handle->spi_init == NULL)                                                    /* check spi_init */
    {
        handle->debug_print("bmp390: spi_init is null.\n");                          /* spi_init is null */
       
        return 3;                                                                    /* return error */
    }
    if (handle->spi_deinit == NULL)                                                  /* check spi_deinit */
    {
        handle->debug_print("bmp390: spi_deinit is null.\n");                        /* spi_deinit is null */
       
        return 3;                                                                    /* return error */
    }
    if (handle->spi_read == NULL)                                                    /* check spi_read */
    {
        handle->debug_print("bmp390: spi_read is null.\n");                          /* spi_read is null */
       
        return 3;                                                                    /* return error */
    }
    if (handle->spi_write == NULL)                                                   /* check spi_write */
    {
        handle->debug_print("bmp390: spi_write is null.\n");                         /* spi_write is null */
       
        return 3;                                                                    /* return error */
    }
    if (handle->delay_ms == NULL)                                                    /* check delay_ms */
    {
        handle->debug_print("bmp390: delay_ms is null.\n");                          /* delay_ms is null */
       
        return 3;                                                                    /* return error */
    }
    
    if (handle->iic_spi == BMP390_INTERFACE_IIC)                                     /* if iic interface */
    {
        if (handle->iic_init() != 0)                                                 /* initialize iic bus */
        {
            handle->debug_print("bmp390: iic init failed.\n");                       /* iic init failed */
            
            return 1;                                                                /* return error */
        }
    }
    else
    {
        if (handle->spi_init() != 0)                                                 /* initialize spi bus */
        {
            handle->debug_print("bmp390: spi init failed.\n");                       /* spi init failed */
           
            return 1;                                                                /* return error */
        }
    }
    if (a_bmp390_iic_spi_read(handle, BMP390_REG_CHIP_ID, (uint8_t *)&id, 1) != 0)   /* read chip id */
    {
        handle->debug_print("bmp390: read chip id failed.\n");                       /* read chip id failed */
        (void)a_bmp390_close(handle);                                                /* close bmp390 */
        
        return 4;                                                                    /* return error */
    }
    if (id != 0x60)                                                                  /* check chip id */
    {
        handle->debug_print("bmp390: id is invalid.\n");                             /* id is invalid */
        (void)a_bmp390_close(handle);                                                /* close bmp390 */
        
        return 4;         
    }                                                                                /* return error */
    reg = 0xB6;                                                                      /* set command */
    if (a_bmp390_iic_spi_write(handle, BMP390_REG_CMD, (uint8_t *)&reg, 1) != 0)     /* write command */
    {
        handle->debug_print("bmp390: soft rest failed.\n");                          /* soft rest failed */
        (void)a_bmp390_close(handle);                                                /* close bmp390 */
        
        return 5;                                                                    /* return error */
    }
    handle->delay_ms(10);                                                            /* delay 10 ms */
    if (a_bmp390_iic_spi_read(handle, BMP390_REG_ERR_REG, (uint8_t *)&reg, 1) != 0)  /* read reg */
    {
        handle->debug_print("bmp390: get err reg failed.\n");                        /* return error */
        (void)a_bmp390_close(handle);                                                /* close bmp390 */
        
        return 5;                                                                    /* return error */
    }
    if ((reg & 0x07) != 0)                                                           /* check running status */
    {
        handle->debug_print("bmp390: find running error.\n");                        /* find running error */
        (void)a_bmp390_close(handle);                                                /* close bmp390 */
        
        return 5;                                                                    /* return error */
    }
    if (a_bmp390_get_calibration_data(handle) != 0)                                  /* get calibration data */
    {
        handle->debug_print("bmp390: get calibration data error.\n");                /* get calibration data error */
        (void)a_bmp390_close(handle);                                                /* close bmp390 */
        
        return 6;                                                                    /* return error */
    }
    handle->inited = 1;                                                              /* flag finish initialization */
     
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief     close the chip
 * @param[in] *handle points to a bmp390 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic or spi deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 power down failed
 * @note      none
 */
uint8_t bmp390_deinit(bmp390_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_PWR_CTRL, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("bmp390: get pwr ctrl register failed.\n");                   /* get pwr ctrl register failed */
       
        return 4;                                                                         /* return error */
    }
    prev &= ~(3 << 0);                                                                    /* clear config */
    prev &= ~(3 << 4);                                                                    /* set config */
    res = a_bmp390_iic_spi_write(handle, BMP390_REG_PWR_CTRL, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("bmp390: set pwr ctrl register failed.\n");                   /* set pwr ctrl register failed */
       
        return 4;                                                                         /* return error */
    }
    if (a_bmp390_close(handle) != 0)                                                      /* close bmp390 */
    {
        return 1;                                                                         /* return error */
    }
    else
    {
        handle->inited = 0;                                                               /* flag close */
    
        return 0;                                                                         /* success return 0 */
    }
}

/**
 * @brief      read the temperature
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *raw points to a raw temperature buffer
 * @param[out] *c points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read temperature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_read_temperature(bmp390_handle_t *handle, uint32_t *raw, float *c)
{
    uint8_t res;
    uint8_t prev;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_CONFIG_1, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("bmp390: get fifo config 1 register failed.\n");                  /* get fifo config 1 register failed */
       
        return 1;                                                                             /* return error */
    }
    if ((prev & 0x01) != 0)                                                                   /* check mode */
    {
        handle->debug_print("bmp390: fifo mode can't use this fuction.\n");                   /* fifo mode can't use this fuction */
       
        return 1;                                                                             /* return error */
    }
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_PWR_CTRL, (uint8_t *)&prev, 1);            /* read pwr ctrl */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("bmp390: get pwr ctrl register failed.\n");                       /* get pwr ctrl register failed */
       
        return 1;                                                                             /* return error */
    } 
    if (((prev >> 4) & 0x03) == 0x03)                                                         /* normal mode */
    {
        res = a_bmp390_iic_spi_read(handle, BMP390_REG_STATUS, (uint8_t *)&prev, 1);          /* read config */
        if (res != 0)                                                                         /* check result */
        {
            handle->debug_print("bmp390: get status register failed.\n");                     /* get status register failed */
           
            return 1;                                                                         /* return error */
        }
        if ((prev & (1 << 6)) != 0)                                                           /* data is ready */
        {
            int64_t output;
            
            res = a_bmp390_iic_spi_read(handle, BMP390_REG_DATA_3, (uint8_t *)buf, 3);        /* read raw data */
            if (res != 0)                                                                     /* check result */
            {
                handle->debug_print("bmp390: get data register failed.\n");                   /* get data register failed */
               
                return 1;                                                                     /* return error */
            }
            *raw = (uint32_t)buf[2] << 16| (uint32_t)buf[1] << 8 | buf[0];                    /* get data */
            output = a_bmp390_compensate_temperature(handle, *raw);                           /* compensate temperature */
            *c = (float)((double)output / 100.0);                                             /* get converted temperature */
            
            return 0;                                                                         /* success return 0 */
            
        }
        else
        {
            handle->debug_print("bmp390: temperature data is not ready.\n");                  /* temperature data is not ready */
           
            return 1;                                                                         /* return error */
        }            
    }
    else if (((prev >> 4) & 0x03) == 0x00)                                                    /* force mode */
    {
        uint16_t cnt = 5000;
        
        res = a_bmp390_iic_spi_read(handle, BMP390_REG_PWR_CTRL, (uint8_t *)&prev, 1);        /* read config */
        if (res != 0)                                                                         /* check result */
        {
            handle->debug_print("bmp390: get pwr ctrl register failed.\n");                   /* get pwr ctrl register failed */
           
            return 1;                                                                         /* return error */
        }
        prev &= ~(0x03 << 4);                                                                 /* clear 4-5 bits */
        prev |= 0x01 << 4;                                                                    /* set bit 4 */
        res = a_bmp390_iic_spi_write(handle, BMP390_REG_PWR_CTRL, (uint8_t *)&prev, 1);       /* read config */
        if (res != 0)                                                                         /* check result */
        {
            handle->debug_print("bmp390: set pwr ctrl register failed.\n");                   /* set pwr ctrl register failed */
           
            return 1;                                                                         /* return error */
        }
        
        while (1)                                                                             /* loop */
        {
            res = a_bmp390_iic_spi_read(handle, BMP390_REG_STATUS, (uint8_t *)&prev, 1);      /* read status */
            if (res != 0)                                                                     /* check result */
            {
                handle->debug_print("bmp390: get status register failed.\n");                 /* get status register failed */
               
                return 1;                                                                     /* return error */
            }
            if ((prev & (1 << 6)) != 0)                                                       /* data is ready */
            {
                int64_t output;
                
                res = a_bmp390_iic_spi_read(handle, BMP390_REG_DATA_3, (uint8_t *)buf, 3);    /* read raw data */
                if (res != 0)                                                                 /* check result */
                {
                    handle->debug_print("bmp390: get data register failed.\n");               /* get data register failed */
                   
                    return 1;                                                                 /* return error */
                }
                *raw = (uint32_t)buf[2] << 16| (uint32_t)buf[1] << 8 | buf[0];                /* get data */
                output = a_bmp390_compensate_temperature(handle, *raw);                       /* compensate temperature */
                *c = (float)((double)output / 100.0);                                         /* get converted temperature */
                
                return 0;                                                                     /* success return 0 */
                
            }
            else
            {
                if (cnt != 0)                                                                 /* check cnt */
                {
                    cnt--;                                                                    /* cnt-- */
                    handle->delay_ms(1);                                                      /* delay 1 ms */
                    
                    continue;                                                                 /* continue */
                }
                handle->debug_print("bmp390: temperature data is not ready.\n");              /* temperature data is not ready */
               
                return 1;                                                                     /* return error */
            }
        }
    }
    else
    {
        handle->debug_print("bmp390: mode is invalid.\n");                                    /* mode is invalid */
           
        return 1;                                                                             /* retun error */
    }
}

/**
 * @brief      read the pressure
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *raw points to a raw pressure buffer
 * @param[out] *pa points to a converted pressure buffer
 * @return     status code
 *             - 0 success
 *             - 1 read pressure failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_read_pressure(bmp390_handle_t *handle, uint32_t *raw, float *pa)
{
    uint8_t res;
    uint8_t prev;
    uint8_t buf[3];
    uint32_t temperature_yaw;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_CONFIG_1, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("bmp390: get fifo config 1 register failed.\n");                  /* get fifo config 1 register failed */
       
        return 1;                                                                             /* return error */
    }
    if ((prev & 0x01) != 0)                                                                   /* check mode */
    {
        handle->debug_print("bmp390: fifo mode can't use this fuction.\n");                   /* fifo mode can't use this fuction */
       
        return 1;                                                                             /* return error */
    }
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_PWR_CTRL, (uint8_t *)&prev, 1);            /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("bmp390: get pwr ctrl register failed.\n");                       /* get pwr ctrl register failed */
       
        return 1;                                                                             /* return error */
    }
    if (((prev >> 4) & 0x03) == 0x03)                                                         /* normal mode */
    {
        res = a_bmp390_iic_spi_read(handle, BMP390_REG_STATUS, (uint8_t *)&prev, 1);          /* read status */
        if (res != 0)                                                                         /* check result */
        {
            handle->debug_print("bmp390: get status register failed.\n");                     /* get status register failed */
           
            return 1;                                                                         /* return error */
        }
        if ((prev & (1 << 6)) != 0)                                                           /* data is ready */
        {
            res = a_bmp390_iic_spi_read(handle, BMP390_REG_DATA_3, (uint8_t *)buf, 3);        /* read config */
            if (res != 0)                                                                     /* check result */
            {
                handle->debug_print("bmp390: get data register failed.\n");                   /* get data register failed */
               
                return 1;                                                                     /* return error */
            }
            temperature_yaw= (uint32_t)buf[2] << 16| (uint32_t)buf[1] << 8 | buf[0];          /* get data */
            (void)a_bmp390_compensate_temperature(handle, temperature_yaw);                   /* compensate temperature */
        }
        else
        {
            handle->debug_print("bmp390: temperature data is not ready.\n");                  /* temperature data is not ready */
           
            return 1;                                                                         /* return error */
        }
        if ((prev & (1 << 5)) != 0)                                                           /* data is ready */
        {
            int64_t output;
            
            res = a_bmp390_iic_spi_read(handle, BMP390_REG_DATA_0, (uint8_t *)buf, 3);        /* read config */
            if (res != 0)                                                                     /* check result */
            {
                handle->debug_print("bmp390: get data register failed.\n");                   /* get data register failed */
               
                return 1;                                                                     /* return error */
            }
            *raw = (uint32_t)buf[2] << 16| (uint32_t)buf[1] << 8 | buf[0];                    /* get data */
            output = a_bmp390_compensate_pressure(handle, *raw);                              /* compensate pressure */
            *pa = (float)((double)output / 100.0);                                            /* get converted pressure */
            
            return 0;                                                                         /* success return 0 */
        }
        else
        {
            handle->debug_print("bmp390: pressure data is not ready.\n");                     /* pressure data is not ready */
           
            return 1;                                                                         /* return error */
        }
    }
    else if (((prev >> 4) & 0x03) == 0x00)                                                    /* force mode */
    {
        uint16_t cnt = 5000;
        
        res = a_bmp390_iic_spi_read(handle, BMP390_REG_PWR_CTRL, (uint8_t *)&prev, 1);        /* read config */
        if (res != 0)                                                                         /* check result */
        {
            handle->debug_print("bmp390: get pwr ctrl register failed.\n");                   /* get pwr ctrl register failed */
           
            return 1;                                                                         /* return error */
        }
        prev &= ~(0x03 << 4);                                                                 /* clear 4-5 bits */
        prev |= 0x01 << 4;                                                                    /* set 4 bit */
        res = a_bmp390_iic_spi_write(handle, BMP390_REG_PWR_CTRL, (uint8_t *)&prev, 1);       /* read config */
        if (res != 0)                                                                         /* check result */
        {
            handle->debug_print("bmp390: set pwr ctrl register failed.\n");                   /* set pwr ctrl register failed */
           
            return 1;                                                                         /* return error */
        }
        
        while (1)                                                                             /* loop */
        {
            res = a_bmp390_iic_spi_read(handle, BMP390_REG_STATUS, (uint8_t *)&prev, 1);      /* read config */
            if (res != 0)                                                                     /* check result */
            {
                handle->debug_print("bmp390: get status register failed.\n");                 /* get status register failed */
               
                return 1;                                                                     /* return error */
            }
            if ((prev & (1 << 6)) != 0)                                                       /* data is ready */
            {
                res = a_bmp390_iic_spi_read(handle, BMP390_REG_DATA_3, (uint8_t *)buf, 3);    /* read raw data */
                if (res != 0)                                                                 /* check result */
                {
                    handle->debug_print("bmp390: get data register failed.\n");               /* get data register failed */
                   
                    return 1;                                                                 /* return error */
                }
                temperature_yaw = (uint32_t)buf[2] << 16| (uint32_t)buf[1] << 8 | buf[0];     /* get data */
                (void)a_bmp390_compensate_temperature(handle, temperature_yaw);               /* compensate temperature */
                
                goto press;                                                                   /* goto press */
            }
            else
            {
                if (cnt != 0)                                                                 /* check cnt */
                {
                    cnt--;                                                                    /* cnt-- */
                    handle->delay_ms(1);                                                      /* delay 1 ms */
                    
                    continue;                                                                 /* continue */
                }
                handle->debug_print("bmp390: temperature data is not ready.\n");              /* temperature data is not ready */
               
                return 1;                                                                     /* return error */
            }
        
            press:
            
            cnt = 5000;                                                                       /* set cnt 5000 */
            if ((prev & (1 << 5)) != 0)                                                       /* data is ready */
            {
                int64_t output;
                
                res = a_bmp390_iic_spi_read(handle, BMP390_REG_DATA_0, (uint8_t *)buf, 3);    /* read config */
                if (res != 0)                                                                 /* check result */
                {
                    handle->debug_print("bmp390: get data register failed.\n");               /* get data register failed */
                   
                    return 1;                                                                 /* return error */
                }
                *raw = (uint32_t)buf[2] << 16| (uint32_t)buf[1] << 8 | buf[0];                /* get data */
                output = a_bmp390_compensate_pressure(handle, *raw);                          /* compensate pressure */
                *pa = (float)((double)output / 100.0);                                        /* get converted pressure */
                
                return 0;                                                                     /* success return 0 */
            }
            else
            {
                if (cnt != 0)                                                                 /* check cnt */
                {
                    cnt--;                                                                    /* cnt-- */
                    handle->delay_ms(1);                                                      /* delay 1 ms */
                    
                    continue;                                                                 /* continue */
                }
                handle->debug_print("bmp390: temperature data is not ready.\n");              /* temperature data is not ready */
               
                return 1;                                                                     /* return error */
            }
        }
    }
    else
    {
        handle->debug_print("bmp390: mode is invalid.\n");                                    /* mode is invalid */
        
        return 1;                                                                             /* return error */
    }
}

/**
 * @brief      read the temperature and pressure
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *temperature_raw points to a raw temperature buffer
 * @param[out] *temperature_c points to a converted temperature buffer
 * @param[out] *pressure_raw points to a raw pressure buffer
 * @param[out] *pressure_pa points to a converted pressure buffer
 * @return     status code
 *             - 0 success
 *             - 1 read temperature pressure failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_read_temperature_pressure(bmp390_handle_t *handle, uint32_t *temperature_raw, float *temperature_c, 
                                         uint32_t *pressure_raw, float *pressure_pa)
{
    uint8_t res;
    uint8_t prev;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_CONFIG_1, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("bmp390: get fifo config 1 register failed.\n");                  /* get fifo config 1 register failed */
       
        return 1;                                                                             /* return error */
    }
    if ((prev & 0x01) != 0)                                                                   /* check fifo mode */
    {
        handle->debug_print("bmp390: fifo mode can't use this fuction.\n");                   /* fifo mode can't use this fuction */
       
        return 1;                                                                             /* return error */
    }
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_PWR_CTRL, (uint8_t *)&prev, 1);            /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("bmp390: get pwr ctrl register failed.\n");                       /* get pwr ctrl register failed */
       
        return 1;                                                                             /* return error */
    }
    if (((prev >> 4) & 0x03) == 0x03)                                                         /* normal mode */
    {
        res = a_bmp390_iic_spi_read(handle, BMP390_REG_STATUS, (uint8_t *)&prev, 1);          /* read config */
        if (res != 0)                                                                         /* check result */
        {
            handle->debug_print("bmp390: get status register failed.\n");                     /* get status register failed */
           
            return 1;                                                                         /* return error */
        }
        if ((prev & (1 << 6)) != 0)                                                           /* data is ready */
        {
            int64_t output;
            
            res = a_bmp390_iic_spi_read(handle, BMP390_REG_DATA_3, (uint8_t *)buf, 3);        /* read raw data */
            if (res != 0)                                                                     /* check result */
            {
                handle->debug_print("bmp390: get data register failed.\n");                   /* get data register failed */
               
                return 1;                                                                     /* return error */
            }
            *temperature_raw= (uint32_t)buf[2] << 16| (uint32_t)buf[1] << 8 | buf[0];         /* get data */
            output = a_bmp390_compensate_temperature(handle, *temperature_raw);               /* compensate temperature */
            *temperature_c = (float)((double)output / 100.0);                                 /* get converted temperature */
        }
        else
        {
            handle->debug_print("bmp390: temperature data is not ready.\n");                  /* temperature data is not ready */
           
            return 1;                                                                         /* return error */
        }
        if ((prev & (1 << 5)) != 0)                                                           /* data is ready */
        {
            int64_t output;
            
            res = a_bmp390_iic_spi_read(handle, BMP390_REG_DATA_0, (uint8_t *)buf, 3);        /* read data */
            if (res != 0)                                                                     /* check result */
            {
                handle->debug_print("bmp390: get data register failed.\n");                   /* get data register failed */
               
                return 1;                                                                     /* return error */
            }
            *pressure_raw = (uint32_t)buf[2] << 16| (uint32_t)buf[1] << 8 | buf[0];           /* get data */
            output = a_bmp390_compensate_pressure(handle, *pressure_raw);                     /* compensate pressure */
            *pressure_pa = (float)((double)output / 100.0);                                   /* get converted pressure */
            
            return 0;                                                                         /* success return 0 */
        }
        else
        {
            handle->debug_print("bmp390: pressure data is not ready.\n");                     /* pressure data is not ready */
           
            return 1;                                                                         /* return error */
        }
    }
    else if (((prev >> 4) & 0x03) == 0x00)                                                    /* force mode */
    {
        uint16_t cnt = 5000;
        
        res = a_bmp390_iic_spi_read(handle, BMP390_REG_PWR_CTRL, (uint8_t *)&prev, 1);        /* read config */
        if (res != 0)                                                                         /* check result */
        {
            handle->debug_print("bmp390: get pwr ctrl register failed.\n");                   /* get pwr ctrl register failed */
           
            return 1;                                                                         /* return error */
        }
        prev &= ~(0x03 << 4);                                                                 /* clear 4-5 bits */
        prev |= 0x01 << 4;                                                                    /* set bit 4 */
        res = a_bmp390_iic_spi_write(handle, BMP390_REG_PWR_CTRL, (uint8_t *)&prev, 1);       /* write config */
        if (res != 0)                                                                         /* check result */
        {
            handle->debug_print("bmp390: set pwr ctrl register failed.\n");                   /* set pwr ctrl register failed */
           
            return 1;                                                                         /* return error */
        }
        
        while (1)                                                                             /* loop */
        {
            res = a_bmp390_iic_spi_read(handle, BMP390_REG_STATUS, (uint8_t *)&prev, 1);      /* read config */
            if (res != 0)                                                                     /* check result */
            {
                handle->debug_print("bmp390: get status register failed.\n");                 /* get status register failed */
               
                return 1;                                                                     /* return error */
            }
            if ((prev & (1 << 6)) != 0)                                                       /* data is ready */
            {
                int64_t output;
                
                res = a_bmp390_iic_spi_read(handle, BMP390_REG_DATA_3, (uint8_t *)buf, 3);    /* read raw data */
                if (res != 0)                                                                 /* check result */
                {
                    handle->debug_print("bmp390: get data register failed.\n");               /* get data register failed */
                   
                    return 1;                                                                 /* return error */
                }
                *temperature_raw = (uint32_t)buf[2] << 16| (uint32_t)buf[1] << 8 | buf[0];    /* get data */
                output = a_bmp390_compensate_temperature(handle, *temperature_raw);           /* compensate temperature */
                *temperature_c = (float)((double)output / 100.0);                             /* get converted temperature */
                
                break;                                                                        /* break */
            }
            else
            {
                if (cnt != 0)                                                                 /* check cnt */
                {
                    cnt--;                                                                    /* cnt-- */
                    handle->delay_ms(1);                                                      /* delay 1 ms */
                    
                    continue;                                                                 /* continue */
                }
                handle->debug_print("bmp390: temperature data is not ready.\n");              /* temperature data is not ready */
               
                return 1;                                                                     /* return error */
            }
        }
        cnt = 5000;                                                                           /* set cnt */
        
        while (1)                                                                             /* loop */
        {
            if ((prev & (1 << 5)) != 0)                                                       /* data is ready */
            {
                int64_t output;
                
                res = a_bmp390_iic_spi_read(handle, BMP390_REG_DATA_0, (uint8_t *)buf, 3);    /* read raw data */
                if (res != 0)                                                                 /* check result */
                {
                    handle->debug_print("bmp390: get data register failed.\n");               /* get data register failed */
                   
                    return 1;                                                                 /* return error */
                }
                *pressure_raw = (uint32_t)buf[2] << 16| (uint32_t)buf[1] << 8 | buf[0];       /* get data */
                output = a_bmp390_compensate_pressure(handle, *pressure_raw);                 /* compensate pressure */
                *pressure_pa = (float)((double)output / 100.0);                               /* get converted pressure */
                
                return 0;                                                                     /* success return 0 */
            }
            else
            {
                if (cnt != 0)                                                                 /* check cnt */
                {
                    cnt--;                                                                    /* cnt-- */
                    handle->delay_ms(1);                                                      /* delay 1 ms */
                    
                    continue;                                                                 /* continue */
                }
                handle->debug_print("bmp390: temperature data is not ready.\n");              /* temperature data is not ready */
               
                return 1;                                                                     /* return error */
            }
        }
    }
    else
    {
        handle->debug_print("bmp390: mode is invalid.\n");                                    /* mode is invalid */
           
        return 1;                                                                             /* return error */
    }
}

/**
 * @brief     irq handler
 * @param[in] *handle points to a bmp390 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_irq_handler(bmp390_handle_t *handle)
{
    uint8_t res;
    uint8_t status;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_INT_STATUS, (uint8_t *)&status, 1);       /* read config */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("bmp390: get interrupt status register failed.\n");              /* get interrupt status register failed */
       
        return 1;                                                                            /* return error */
    }
    if ((status & (1 << 1)) != 0)                                                            /* if fifo full */
    {
        if(handle->receive_callback != NULL)                                                 /* if receive callback is valid */
        {
            handle->receive_callback(BMP390_INTERRUPT_STATUS_FIFO_FULL);                     /* run receive callback */
        }
    }
    if ((status & (1 << 0)) != 0)                                                            /* if fifo watermark */
    {
        if(handle->receive_callback != NULL)                                                 /* if receive callback is valid */
        {
            handle->receive_callback(BMP390_INTERRUPT_STATUS_FIFO_WATERMARK);                /* run receive callback */
        }
    }
    if ((status & (1 << 3)) != 0)                                                            /* if data ready */
    {
        if (handle->receive_callback != NULL)                                                /* if receive callback is valid */
        {
            handle->receive_callback(BMP390_INTERRUPT_STATUS_DATA_READY);                    /* run receive callback */
        }
    }
    
    return 0;                                                                                /* success return 0 */
}

/**
 * @brief     set the iic address pin
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] addr_pin is the iic address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t bmp390_set_addr_pin(bmp390_handle_t *handle, bmp390_address_t addr_pin)
{
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }
    
    handle->iic_addr = (uint8_t)addr_pin;        /* set iic address */
    
    return 0;                                    /* success return 0 */
}

/**
 * @brief      get the iic address pin
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *addr_pin points to a iic address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t bmp390_get_addr_pin(bmp390_handle_t *handle, bmp390_address_t *addr_pin)
{
    if (handle == NULL)                                    /* check handle */
    {
        return 2;                                          /* return error */
    }
    
    *addr_pin = (bmp390_address_t)handle->iic_addr;        /* get iic address */
    
    return 0;                                              /* success return 0 */
}

/**
 * @brief     set the interface
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t bmp390_set_interface(bmp390_handle_t *handle, bmp390_interface_t interface) 
{
    if (handle == NULL)                        /* check handle */
    {
        return 2;                              /* return error */
    }
    
    handle->iic_spi = (uint8_t)interface;      /* set interface */
    
    return 0;                                  /* success return 0 */
}

/**
 * @brief      get the interface
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[out] *interface points to a chip interface buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t bmp390_get_interface(bmp390_handle_t *handle, bmp390_interface_t *interface)
{
    if (handle == NULL)                                        /* check handle */
    {
        return 2;                                              /* return error */
    }
    
    *interface = (bmp390_interface_t)(handle->iic_spi);        /* get interface */
    
    return 0;                                                  /* success return 0 */
}

/**
 * @brief         read the fifo
 * @param[in]     *handle points to a bmp390 handle structure
 * @param[in]     *buf points to a data buffer
 * @param[in,out] *len points to a data length buffer
 * @return        status code
 *                - 0 success
 *                - 1 read fifo failed
 *                - 2 handle is NULL
 *                - 3 handle is not initialized
 * @note          none
 */
uint8_t bmp390_read_fifo(bmp390_handle_t *handle, uint8_t *buf, uint16_t *len)
{
    uint8_t res;
    uint8_t prev;
    uint8_t tmp_buf[2];
    uint16_t length;
    
    if (handle == NULL)                                                                             /* check handle */
    {
        return 2;                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                        /* check handle initialization */
    {
        return 3;                                                                                   /* return error */
    }
    
    res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_CONFIG_1, (uint8_t *)&prev, 1);             /* read config */
    if (res != 0)                                                                                   /* check result */
    {
        handle->debug_print("bmp390: get fifo config 1 register failed.\n");                        /* get fifo config 1 register failed */
       
        return 1;                                                                                   /* return error */
    }
    if ((prev & 0x01) != 0)                                                                         /* check mode */
    {
        res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_LENGTH_0, (uint8_t *)tmp_buf, 2);       /* read config */
        if (res != 0)                                                                               /* check result */
        {
            handle->debug_print("bmp390: get fifo length register failed.\n");                      /* get fifo length register failed */
           
            return 1;                                                                               /* return error */
        }
        length = ((uint16_t)(tmp_buf[1] & 0x01) << 8) | tmp_buf[0];                                 /* get data */
        if ((prev & (1 << 2)) != 0)                                                                 /* if include sensorttime */
        {
            length += 4;                                                                            /* add sensortime length */
        }            
        *len = (*len) < length ? (*len) :length;                                                    /* get real length */
        res = a_bmp390_iic_spi_read(handle, BMP390_REG_FIFO_DATA, (uint8_t *)buf, *len);            /* read config */
        if (res != 0)                                                                               /* check result */
        {
            handle->debug_print("bmp390: get fifo data failed.\n");                                 /* get fifo data failed */
           
            return 1;                                                                               /* return error */
        }
        
        return 0;                                                                                   /* success return 0 */
    }
    else
    {
        handle->debug_print("bmp390: normal mode or forced mode can't use this fuction.\n");        /* normal mode or forced mode can't use this fuction */
           
        return 1;                                                                                   /* return error */
    }
}

/**
 * @brief         parse the fifo data
 * @param[in]     *handle points to a bmp390 handle structure
 * @param[in]     *buf is the fifo data
 * @param[in]     buf_len is the fifo data length
 * @param[out]    *frame points to a frame buffer
 * @param[in,out] *frame_len points to a frame length buffer
 * @return        status code
 *                - 0 success
 *                - 1 fifo parse failed
 *                - 2 handle is NULL
 *                - 3 handle is not initialized
 * @note          none
 */
uint8_t bmp390_fifo_parse(bmp390_handle_t *handle, uint8_t *buf, uint16_t buf_len, bmp390_frame_t *frame, uint16_t *frame_len)
{
    uint8_t res;
    uint16_t i;
    uint16_t frame_total;
    
    if (handle == NULL)                                                                                                                   /* check handle */
    {
        return 2;                                                                                                                         /* return error */
    }
    if (handle->inited != 1)                                                                                                              /* check handle initialization */
    {
        return 3;                                                                                                                         /* return error */
    }
    
    if (buf_len == 0)                                                                                                                     /* check buffer length */
    {
        handle->debug_print("bmp390: buffer length is invalid.\n");                                                                       /* buffer length is invalid */
       
        return 1;                                                                                                                         /* return error */
    } 
    frame_total = 0;                                                                                                                      /* clear total frame */
    res = 0;                                                                                                                              /* set 0 */
    i = 0;                                                                                                                                /* set 0 */
    while (i < buf_len)                                                                                                                   /* loop */
    {
        switch ((uint8_t)buf[i])
        {
            case 0x90 :
            {
                if (frame_total > ((*frame_len)-1))                                                                                       /* check length */
                {
                    return 0;                                                                                                             /* return success */
                }
                frame[frame_total].type =  BMP390_FRAME_TYPE_TEMPERATURE;                                                                 /* set temperature type */
                frame[frame_total].raw = (uint32_t)buf[i + 2 + 1] << 16 | (uint32_t)buf[i + 1 + 1] << 8 | buf[i + 0 + 1];                 /* set raw */
                frame[frame_total].data = (float)((double)a_bmp390_compensate_temperature(handle, frame[frame_total].raw) / 100.0);       /* set compensate temerature */
                frame_total++;                                                                                                            /* frame++ */
                i += 4;                                                                                                                   /* index + 4 */
                
                break;                                                                                                                    /* break */
            }
            case 0x94 :
            {
                if ((frame_total) > ((*frame_len)-1))                                                                                     /* check length */
                {
                    return 0;                                                                                                             /* return success */
                }
                frame[frame_total].type =  BMP390_FRAME_TYPE_TEMPERATURE;                                                                 /* set temperature type */
                frame[frame_total].raw = (uint32_t)buf[i + 2 + 1] << 16| (uint32_t)buf[i + 1 + 1] << 8 | buf[i + 0 + 1];                  /* set raw */
                frame[frame_total].data = (float)((double)a_bmp390_compensate_temperature(handle, frame[frame_total].raw) / 100.0);       /* set compensate temerature */
                frame_total++;                                                                                                            /* frame++ */
                if (frame_total > ((*frame_len)-1))                                                                                       /* check length */
                {
                    return 0;                                                                                                             /* return success */
                }
                frame[frame_total].type =  BMP390_FRAME_TYPE_PRESSURE;                                                                    /* set pressure type */
                frame[frame_total].raw = (uint32_t)buf[i + 5 + 1] << 16| (uint32_t)buf[i + 4 + 1] << 8 | buf[i + 3 + 1];                  /* set raw */
                frame[frame_total].data = (float)((double)a_bmp390_compensate_pressure(handle, frame[frame_total].raw) / 100.0);          /* set compensate pressure */
                frame_total++;                                                                                                            /* frame++ */
                i += 7;                                                                                                                   /* index + 7 */
                
                break;                                                                                                                    /* break */
            }
            case 0xA0 :
            {
                if (frame_total > ((*frame_len)-1))                                                                                       /* check length */
                {
                    return 0;                                                                                                             /* return success */
                }
                frame[frame_total].type =  BMP390_FRAME_TYPE_SENSORTIME;                                                                  /* set sensortime type */
                frame[frame_total].raw = (uint32_t)buf[i + 2 + 1] << 16 | (uint32_t)buf[i + 1 + 1] << 8 | buf[i + 0 + 1];                 /* set raw */
                frame[frame_total].data = 0;                                                                                              /* set data */
                frame_total++;                                                                                                            /* frame++ */
                i += 4;                                                                                                                   /* index+4 */
                
                break;                                                                                                                    /* break */
            }
            default :
            {
                handle->debug_print("bmp390: header is invalid.\n");                                                                      /* header is invalid */
                res = 1;                                                                                                                  /* set 1 */
                
                break;                                                                                                                    /* break */
            }
        }
        if (res == 1)                                                                                                                     /* check the result */
        {
            return 1;                                                                                                                     /* return error */
        }
    }
    *frame_len = frame_total;                                                                                                             /* set frame length */
    
    return 0;                                                                                                                             /* success return 0 */
}

/**
 * @brief     set the chip register
 * @param[in] *handle points to a bmp390 handle structure
 * @param[in] reg is the iic register address
 * @param[in] value is the data written to the register
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t bmp390_set_reg(bmp390_handle_t *handle, uint8_t reg, uint8_t value)
{
    if (handle == NULL)                                          /* check handle */
    {
        return 2;                                                /* return error */
    }
    if (handle->inited != 1)                                     /* check handle initialization */
    {
        return 3;                                                /* return error */
    } 
    
    return a_bmp390_iic_spi_write(handle, reg, &value, 1);       /* write register */
}

/**
 * @brief      get the chip register
 * @param[in]  *handle points to a bmp390 handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *value points to a read data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t bmp390_get_reg(bmp390_handle_t *handle, uint8_t reg, uint8_t *value)
{
    if (handle == NULL)                                        /* check handle */
    {
        return 2;                                              /* return error */
    }
    if (handle->inited != 1)                                   /* check handle initialization */
    {
        return 3;                                              /* return error */
    } 
    
    return a_bmp390_iic_spi_read(handle, reg, value, 1);       /* read register */
}

/**
 * @brief      get chip's information
 * @param[out] *info points to a bmp390 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t bmp390_info(bmp390_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(bmp390_info_t));                         /* initialize bmp390 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC SPI", 8);                         /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver verison */
    
    return 0;                                                       /* success return 0 */
}
