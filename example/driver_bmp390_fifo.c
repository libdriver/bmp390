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
 * @file      driver_bmp390_fifo.c
 * @brief     driver bmp390 fifo source file
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

#include "driver_bmp390_fifo.h"

static bmp390_handle_t gs_handle;        /**< bmp390 handle */

/**
 * @brief  fifo example irq handler
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t bmp390_fifo_irq_handler(void)
{
    /* run irq handler */
    if (bmp390_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     fifo example init
 * @param[in] interface is the chip interface
 * @param[in] addr_pin is the iic device address
 * @param[in] *fifo_receive_callback points to a fifo receive callback
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t bmp390_fifo_init(bmp390_interface_t interface, bmp390_address_t addr_pin,
                         void (*fifo_receive_callback)(uint8_t type))
{
    uint8_t res;
    
    /* link functions */
    DRIVER_BMP390_LINK_INIT(&gs_handle, bmp390_handle_t);
    DRIVER_BMP390_LINK_IIC_INIT(&gs_handle, bmp390_interface_iic_init);
    DRIVER_BMP390_LINK_IIC_DEINIT(&gs_handle, bmp390_interface_iic_deinit);
    DRIVER_BMP390_LINK_IIC_READ(&gs_handle, bmp390_interface_iic_read);
    DRIVER_BMP390_LINK_IIC_WRITE(&gs_handle, bmp390_interface_iic_write);
    DRIVER_BMP390_LINK_SPI_INIT(&gs_handle, bmp390_interface_spi_init);
    DRIVER_BMP390_LINK_SPI_DEINIT(&gs_handle, bmp390_interface_spi_deinit);
    DRIVER_BMP390_LINK_SPI_READ(&gs_handle, bmp390_interface_spi_read);
    DRIVER_BMP390_LINK_SPI_WRITE(&gs_handle, bmp390_interface_spi_write);
    DRIVER_BMP390_LINK_DELAY_MS(&gs_handle, bmp390_interface_delay_ms);
    DRIVER_BMP390_LINK_DEBUG_PRINT(&gs_handle, bmp390_interface_debug_print);
    DRIVER_BMP390_LINK_RECEIVE_CALLBACK(&gs_handle, fifo_receive_callback);
    
    /* set interface */
    res = bmp390_set_interface(&gs_handle, interface);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interface failed.\n");
       
        return 1;
    }
    
    /* set addr pin */
    res = bmp390_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set addr pin failed.\n");
       
        return 1;
    }
    
    /* bmp390 init */
    res = bmp390_init(&gs_handle);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: init failed.\n");
       
        return 1;
    }
    
    /* set spi wire 4 */
    res = bmp390_set_spi_wire(&gs_handle, BMP390_FIFO_DEFAULT_SPI_WIRE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set spi wire failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default iic watchdog timer */
    res = bmp390_set_iic_watchdog_timer(&gs_handle, BMP390_FIFO_DEFAULT_IIC_WATCHDOG_TIMER);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set iic watchdog timer failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default iic watchdog period */
    res = bmp390_set_iic_watchdog_period(&gs_handle, BMP390_FIFO_DEFAULT_IIC_WATCHDOG_PERIOD);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set iic watchdog period failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable fifo */
    res = bmp390_set_fifo(&gs_handle, BMP390_BOOL_TRUE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default fifo stop on full */
    res = bmp390_set_fifo_stop_on_full(&gs_handle, BMP390_FIFO_DEFAULT_FIFO_STOP_ON_FULL);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo stop on full failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default fifo watermark */
    res = bmp390_set_fifo_watermark(&gs_handle, BMP390_FIFO_DEFAULT_FIFO_WATERMARK);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo watermark failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default fifo sensortime on */
    res = bmp390_set_fifo_sensortime_on(&gs_handle, BMP390_FIFO_DEFAULT_FIFO_SENSORTIME_ON);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo sensortime on failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default fifo pressure on */
    res = bmp390_set_fifo_pressure_on(&gs_handle, BMP390_FIFO_DEFAULT_FIFO_PRESSURE_ON);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo sensortime on failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default fifo temperature on */
    res = bmp390_set_fifo_temperature_on(&gs_handle, BMP390_FIFO_DEFAULT_FIFO_TEMPERATURE_ON);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo temperature on failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default fifo subsampling */
    res = bmp390_set_fifo_subsampling(&gs_handle, BMP390_FIFO_DEFAULT_FIFO_SUBSAMPLING);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo subsampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default fifo data source */
    res = bmp390_set_fifo_data_source(&gs_handle,BMP390_FIFO_DEFAULT_FIFO_DATA_SOURCE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo data source failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default interrupt pin type */
    res = bmp390_set_interrupt_pin_type(&gs_handle, BMP390_FIFO_DEFAULT_INTERRUPT_PIN_TYPE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interrupt pin type failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default interrupt active level */
    res = bmp390_set_interrupt_active_level(&gs_handle, BMP390_FIFO_DEFAULT_INTERRUPT_ACTIVE_LEVEL);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interrupt active level failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default latch interrupt pin and interrupt status */
    res = bmp390_set_latch_interrupt_pin_and_interrupt_status(&gs_handle, BMP390_FIFO_DEFAULT_LATCH_INTERRUPT);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set latch interrupt pin and interrupt status failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default interrupt fifo watermark */
    res = bmp390_set_interrupt_fifo_watermark(&gs_handle, BMP390_FIFO_DEFAULT_INTERRUPT_WATERMARK);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interrupt fifo watermark failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default interrupt fifo full */
    res = bmp390_set_interrupt_fifo_full(&gs_handle, BMP390_FIFO_DEFAULT_INTERRUPT_FIFO_FULL);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interrupt fifo full failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default interrupt data ready */
    res = bmp390_set_interrupt_data_ready(&gs_handle,BMP390_FIFO_DEFAULT_INTERRUPT_DATA_READY);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interrupt data ready failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default pressure */
    res = bmp390_set_pressure(&gs_handle, BMP390_FIFO_DEFAULT_PRESSURE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set pressure failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default temperature */
    res = bmp390_set_temperature(&gs_handle, BMP390_FIFO_DEFAULT_TEMPERATURE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set temperature failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default pressure oversampling */
    res = bmp390_set_pressure_oversampling(&gs_handle, BMP390_FIFO_DEFAULT_PRESSURE_OVERSAMPLING);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set pressure oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default temperature oversampling */
    res = bmp390_set_temperature_oversampling(&gs_handle, BMP390_FIFO_DEFAULT_TEMPERATURE_OVERSAMPLING);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set temperature oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default odr */
    res = bmp390_set_odr(&gs_handle, BMP390_FIFO_DEFAULT_ODR);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default filter coefficient */
    res = bmp390_set_filter_coefficient(&gs_handle, BMP390_FIFO_DEFAULT_FILTER_COEFFICIENT);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set filter coefficient failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set normal mode */
    res = bmp390_set_mode(&gs_handle, BMP390_MODE_NORMAL_MODE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set mode failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    } 
    
    return 0;
}

/**
 * @brief         fifo example read
 * @param[in]     *buf points a data buffer
 * @param[in]     buf_len is the data buffer
 * @param[out]    *frame points a frame structure
 * @param[in,out] *frame_len points a frame data buffer
 * @return        status code
 *                - 0 success
 *                - 1 read failed
 * @note          none
 */
uint8_t bmp390_fifo_read(uint8_t *buf, uint16_t buf_len, bmp390_frame_t *frame, uint16_t *frame_len)
{
    /* read fifo */
    if (bmp390_read_fifo(&gs_handle, (uint8_t *)buf, (uint16_t *)&buf_len) != 0)
    {
        return 1;
    }
    /* parse fifo */
    if (bmp390_fifo_parse(&gs_handle, (uint8_t *)buf, buf_len, (bmp390_frame_t *)frame, (uint16_t *)frame_len) != 0)
    {
        return 1;
    }
   
    return 0;
}

/**
 * @brief  fifo example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t bmp390_fifo_deinit(void)
{
    uint8_t res;
    
    /* set sleep mode */
    res = bmp390_set_mode(&gs_handle, BMP390_MODE_SLEEP_MODE);
    if (res != 0)
    {
        return 1;
    }
    
    if (bmp390_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
