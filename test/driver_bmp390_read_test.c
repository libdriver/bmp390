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
 * @file      driver_bmp390_read_test.c
 * @brief     driver bmp390 read test source file
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

#include "driver_bmp390_read_test.h"

static bmp390_handle_t gs_handle;        /**< bmp390 handle */

/**
 * @brief     read test
 * @param[in] interface is the chip interface
 * @param[in] addr_pin is the iic device address
 * @param[in] times is the test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t bmp390_read_test(bmp390_interface_t interface, bmp390_address_t addr_pin, uint32_t times)
{
    uint8_t res;
    uint32_t i;
    bmp390_info_t info;
    
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
    DRIVER_BMP390_LINK_RECEIVE_CALLBACK(&gs_handle, bmp390_interface_receive_callback);
    
    /* bmp390 info */
    res = bmp390_info(&info);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip information */
        bmp390_interface_debug_print("bmp390: chip is %s.\n", info.chip_name);
        bmp390_interface_debug_print("bmp390: manufacturer is %s.\n", info.manufacturer_name);
        bmp390_interface_debug_print("bmp390: interface is %s.\n", info.interface);
        bmp390_interface_debug_print("bmp390: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        bmp390_interface_debug_print("bmp390: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        bmp390_interface_debug_print("bmp390: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        bmp390_interface_debug_print("bmp390: max current is %0.2fmA.\n", info.max_current_ma);
        bmp390_interface_debug_print("bmp390: max temperature is %0.1fC.\n", info.temperature_max);
        bmp390_interface_debug_print("bmp390: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start read test */
    bmp390_interface_debug_print("bmp390: start read test.\n");
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
    res = bmp390_set_spi_wire(&gs_handle, BMP390_SPI_WIRE_4);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set spi wire failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable iic watchdog timer */
    res = bmp390_set_iic_watchdog_timer(&gs_handle, BMP390_BOOL_TRUE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set iic watchdog timer failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set iic watchdog period 40 ms */
    res = bmp390_set_iic_watchdog_period(&gs_handle, BMP390_IIC_WATCHDOG_PERIOD_40_MS);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set iic watchdog period failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable fifo */
    res = bmp390_set_fifo(&gs_handle, BMP390_BOOL_FALSE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable fifo stop on full */
    res = bmp390_set_fifo_stop_on_full(&gs_handle, BMP390_BOOL_FALSE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo stop on full failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set fifo watermark 256 */
    res = bmp390_set_fifo_watermark(&gs_handle, 256);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo watermark failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable fifo sensor time on */
    res = bmp390_set_fifo_sensortime_on(&gs_handle, BMP390_BOOL_TRUE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo sensor time on failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable fifo pressure on */
    res = bmp390_set_fifo_pressure_on(&gs_handle, BMP390_BOOL_TRUE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo sensor time on failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable fifo temperature on */
    res = bmp390_set_fifo_temperature_on(&gs_handle, BMP390_BOOL_TRUE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo temperature on failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set fifo subsampling 0 */
    res = bmp390_set_fifo_subsampling(&gs_handle, 0);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo subsampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set fifo data source filtered */
    res = bmp390_set_fifo_data_source(&gs_handle, BMP390_FIFO_DATA_SOURCE_FILTERED);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo data source failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt pin type push pull */
    res = bmp390_set_interrupt_pin_type(&gs_handle, BMP390_INTERRUPT_PIN_TYPE_PUSH_PULL);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interrupt pin type failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt active level higher */
    res = bmp390_set_interrupt_active_level(&gs_handle, BMP390_INTERRUPT_ACTIVE_LEVEL_HIGHER);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interrupt active level failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable latch interrupt pin and interrupt status */
    res = bmp390_set_latch_interrupt_pin_and_interrupt_status(&gs_handle, BMP390_BOOL_TRUE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set set latch interrupt pin and interrupt status failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable interrupt fifo watermark */
    res = bmp390_set_interrupt_fifo_watermark(&gs_handle, BMP390_BOOL_TRUE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interrupt fifo watermark failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable interrupt fifo full */
    res = bmp390_set_interrupt_fifo_full(&gs_handle, BMP390_BOOL_TRUE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interrupt fifo full failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable interrupt data ready */
    res = bmp390_set_interrupt_data_ready(&gs_handle, BMP390_BOOL_FALSE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interrupt data ready failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable pressure */
    res = bmp390_set_pressure(&gs_handle, BMP390_BOOL_TRUE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set pressure failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable temperature */
    res = bmp390_set_temperature(&gs_handle, BMP390_BOOL_TRUE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set temperature failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set pressure oversampling x32 */
    res = bmp390_set_pressure_oversampling(&gs_handle, BMP390_OVERSAMPLING_x32);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set pressure oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set temperature oversampling x2 */
    res = bmp390_set_temperature_oversampling(&gs_handle, BMP390_OVERSAMPLING_x2);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set temperature oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set odr 12.5Hz */
    res = bmp390_set_odr(&gs_handle, BMP390_ODR_12P5_HZ);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set filter coefficient 15 */
    res = bmp390_set_filter_coefficient(&gs_handle, BMP390_FILTER_COEFFICIENT_15);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set filter coefficient failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start forced mode read test */
    bmp390_interface_debug_print("bmp390: forced mode read test.\n");
    
    /* set forced mode */
    res = bmp390_set_mode(&gs_handle, BMP390_MODE_FORCED_MODE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set mode failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_delay_ms(1000);
    for (i = 0; i < times; i++)
    {
        uint32_t temperature_yaw;
        uint32_t pressure_yaw;
        float temperature_c;
        float pressure_pa;
        
        /* read temperature pressure */
        if (bmp390_read_temperature_pressure(&gs_handle, (uint32_t *)&temperature_yaw, (float *)&temperature_c,
                                            (uint32_t *)&pressure_yaw, (float *)&pressure_pa) != 0)
        {
            bmp390_interface_debug_print("bmp390: read temperature and pressure failed.\n");
            (void)bmp390_deinit(&gs_handle);
            
            return 1;
        }
        bmp390_interface_debug_print("bmp390: temperature is %0.2fC.\n", temperature_c);
        bmp390_interface_debug_print("bmp390: pressure is %0.2fPa.\n", pressure_pa);
        bmp390_interface_delay_ms(1000);
    }
    
    /* start normal mode read */
    bmp390_interface_debug_print("bmp390: normal mode read test.\n");
    res = bmp390_set_mode(&gs_handle, BMP390_MODE_SLEEP_MODE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set mode failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_delay_ms(10);
    
    /* set normal mode */
    res = bmp390_set_mode(&gs_handle, BMP390_MODE_NORMAL_MODE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set mode failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_delay_ms(1000);
    for (i = 0; i < times; i++)
    {
        uint32_t temperature_yaw;
        uint32_t pressure_yaw;
        float temperature_c;
        float pressure_pa;
        
        /* read temperature pressure */
        if (bmp390_read_temperature_pressure(&gs_handle, (uint32_t *)&temperature_yaw, (float *)&temperature_c,
                                            (uint32_t *)&pressure_yaw, (float *)&pressure_pa) != 0)
        {
            bmp390_interface_debug_print("bmp390: read temperature and pressure failed.\n");
            (void)bmp390_deinit(&gs_handle);
            
            return 1;
        }
        bmp390_interface_debug_print("bmp390: temperature is %0.2fC.\n", temperature_c);
        bmp390_interface_debug_print("bmp390: pressure is %0.2fPa.\n", pressure_pa);
        bmp390_interface_delay_ms(1000);
    }
    
    /* finish read test */
    bmp390_interface_debug_print("bmp390: finish read test.\n");
    (void)bmp390_deinit(&gs_handle);
    
    return 0;
}
