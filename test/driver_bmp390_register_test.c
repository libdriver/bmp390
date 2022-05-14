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
 * @file      driver_bmp390_register_test.c
 * @brief     driver bmp390 register test source file
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

#include "driver_bmp390_register_test.h"
#include <stdlib.h>

static bmp390_handle_t gs_handle;        /**< bmp390 handle */

/**
 * @brief     register test
 * @param[in] interface is the chip interface
 * @param[in] addr_pin is the iic device address
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t bmp390_register_test(bmp390_interface_t interface, bmp390_address_t addr_pin)
{
    uint8_t res;
    uint16_t fifo_watermark_in;
    uint16_t fifo_watermark_out;
    uint8_t subsampling_in;
    uint8_t subsampling_out;
    uint8_t err;
    uint8_t status;
    uint8_t data;
    uint8_t id;
    uint16_t length;
    uint32_t sensortime;
    bmp390_event_t event;
    bmp390_info_t info;
    bmp390_interface_t interface_test;
    bmp390_address_t addr_pin_test;
    bmp390_bool_t enable;
    bmp390_fifo_data_source_t source;
    bmp390_interrupt_pin_type_t pin_type;
    bmp390_interrupt_active_level_t level;
    bmp390_spi_wire_t wire;
    bmp390_iic_watchdog_period_t period;
    bmp390_mode_t mode;
    bmp390_oversampling_t oversampling;
    bmp390_odr_t odr;
    bmp390_filter_coefficient_t coef;
    
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
        bmp390_interface_debug_print("bmp390: driver version is %d.%d.\n", info.driver_version/1000, (info.driver_version%1000)/100);
        bmp390_interface_debug_print("bmp390: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        bmp390_interface_debug_print("bmp390: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        bmp390_interface_debug_print("bmp390: max current is %0.2fmA.\n", info.max_current_ma);
        bmp390_interface_debug_print("bmp390: max temperature is %0.1fC.\n", info.temperature_max);
        bmp390_interface_debug_print("bmp390: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start register test */
    bmp390_interface_debug_print("bmp390: start register test.\n");
    
    /* bmp390_set_interface/bmp390_get_interface test */
    bmp390_interface_debug_print("bmp390: bmp390_set_interface/bmp390_get_interface test.\n");
    
    /* set iic */
    res = bmp390_set_interface(&gs_handle, BMP390_INTERFACE_IIC);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interface failed.\n");
       
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set interface iic.\n");
    res = bmp390_get_interface(&gs_handle, &interface_test);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get interface failed.\n");
       
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check interface %s.\n", interface_test==BMP390_INTERFACE_IIC?"ok":"error");
    
    /* set spi */
    res = bmp390_set_interface(&gs_handle, BMP390_INTERFACE_SPI);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interface failed.\n");
       
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set interface spi.\n");
    res = bmp390_get_interface(&gs_handle, &interface_test);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get interface failed.\n");
       
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check interface %s.\n", interface_test==BMP390_INTERFACE_SPI?"ok":"error");
    
    /* bmp390_set_addr_pin/bmp390_get_addr_pin test */
    bmp390_interface_debug_print("bmp390: bmp390_set_addr_pin/bmp390_get_addr_pin test.\n");
    
    /* set low */
    res = bmp390_set_addr_pin(&gs_handle, BMP390_ADDRESS_ADO_LOW);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set addr pin failed.\n");
       
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set addr pin low.\n");
    res = bmp390_get_addr_pin(&gs_handle, &addr_pin_test);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get addr pin failed.\n");
       
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check addr pin %s.\n", addr_pin_test==BMP390_ADDRESS_ADO_LOW?"ok":"error");
    
    /* set high */
    res = bmp390_set_addr_pin(&gs_handle, BMP390_ADDRESS_ADO_HIGH);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set addr pin failed.\n");
       
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set addr pin high.\n");
    res = bmp390_get_addr_pin(&gs_handle, &addr_pin_test);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get addr pin failed.\n");
       
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check addr pin %s.\n", addr_pin_test==BMP390_ADDRESS_ADO_HIGH?"ok":"error");
    
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
    
    /* bmp390_set_fifo_watermark/bmp390_get_fifo_watermark test */
    bmp390_interface_debug_print("bmp390: bmp390_set_fifo_watermark/bmp390_get_fifo_watermark test.\n");
    fifo_watermark_in = rand()%256 + 256;
    res = bmp390_set_fifo_watermark(&gs_handle, fifo_watermark_in);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo watermark failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set fifo watermark %d.\n", fifo_watermark_in);
    res = bmp390_get_fifo_watermark(&gs_handle, (uint16_t *)&fifo_watermark_out);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get fifo watermark failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check fifo watermark %s.\n", fifo_watermark_out==fifo_watermark_in?"ok":"error");
    
    /* bmp390_set_fifo/bmp390_get_fifo test */
    bmp390_interface_debug_print("bmp390: bmp390_set_fifo/bmp390_get_fifo test.\n");
    
    /* enable */
    res = bmp390_set_fifo(&gs_handle, BMP390_BOOL_TRUE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set fifo enable.\n");
    res = bmp390_get_fifo(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get fifo failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check fifo %s.\n", enable==BMP390_BOOL_TRUE?"ok":"error");
    
    /* disable */
    res = bmp390_set_fifo(&gs_handle, BMP390_BOOL_FALSE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set fifo disable.\n");
    res = bmp390_get_fifo(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get fifo failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check fifo %s.\n", enable==BMP390_BOOL_FALSE?"ok":"error");
    
    /* bmp390_set_fifo_stop_on_full/bmp390_get_fifo_stop_on_full test */
    bmp390_interface_debug_print("bmp390: bmp390_set_fifo_stop_on_full/bmp390_get_fifo_stop_on_full test.\n");
    
    /* enable */
    res = bmp390_set_fifo_stop_on_full(&gs_handle, BMP390_BOOL_TRUE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo stop on full failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set fifo stop on full enable.\n");
    res = bmp390_get_fifo_stop_on_full(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get fifo stop on full failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check fifo stop on full %s.\n", enable==BMP390_BOOL_TRUE?"ok":"error");
    
    /* disable */
    res = bmp390_set_fifo_stop_on_full(&gs_handle, BMP390_BOOL_FALSE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo stop on full failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set fifo stop on full disable.\n");
    res = bmp390_get_fifo_stop_on_full(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get fifo stop on full failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check fifo stop on full %s.\n", enable==BMP390_BOOL_FALSE?"ok":"error");
    
    /* bmp390_set_fifo_sensortime_on/bmp390_get_fifo_sensortime_on test */
    bmp390_interface_debug_print("bmp390: bmp390_set_fifo_sensortime_on/bmp390_get_fifo_sensortime_on test.\n");
    
    /* enable */
    res = bmp390_set_fifo_sensortime_on(&gs_handle, BMP390_BOOL_TRUE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo sensortime on failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set fifo sensortime on enable.\n");
    res = bmp390_get_fifo_sensortime_on(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get fifo sensortime on failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check fifo sensortime on %s.\n", enable==BMP390_BOOL_TRUE?"ok":"error");
    
    /* disable */
    res = bmp390_set_fifo_sensortime_on(&gs_handle, BMP390_BOOL_FALSE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo sensortime on failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set fifo sensortime on disable.\n");
    res = bmp390_get_fifo_sensortime_on(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get fifo sensortime on failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check fifo sensortime on %s.\n", enable==BMP390_BOOL_FALSE?"ok":"error");
    
    /* bmp390_set_fifo_pressure_on/bmp390_get_fifo_pressure_on test */
    bmp390_interface_debug_print("bmp390: bmp390_set_fifo_pressure_on/bmp390_get_fifo_pressure_on test.\n");
    
    /* enable */
    res = bmp390_set_fifo_pressure_on(&gs_handle, BMP390_BOOL_TRUE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo pressure on failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set fifo pressure on enable.\n");
    res = bmp390_get_fifo_pressure_on(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get fifo pressure on failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check fifo pressure on %s.\n", enable==BMP390_BOOL_TRUE?"ok":"error");
    
    /* disable */
    res = bmp390_set_fifo_pressure_on(&gs_handle, BMP390_BOOL_FALSE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo pressure on failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set fifo pressure on disable.\n");
    res = bmp390_get_fifo_pressure_on(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get fifo pressure on failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check fifo pressure on %s.\n", enable==BMP390_BOOL_FALSE?"ok":"error");
    
    /* bmp390_set_fifo_temperature_on/bmp390_get_fifo_temperature_on test */
    bmp390_interface_debug_print("bmp390: bmp390_set_fifo_temperature_on/bmp390_get_fifo_temperature_on test.\n");
    
    /* enable */
    res = bmp390_set_fifo_temperature_on(&gs_handle, BMP390_BOOL_TRUE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo temperature on failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set fifo temperature on enable.\n");
    res = bmp390_get_fifo_temperature_on(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get fifo temperature on failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check fifo temperature on %s.\n", enable==BMP390_BOOL_TRUE?"ok":"error");
    
    /* disable */
    res = bmp390_set_fifo_temperature_on(&gs_handle, BMP390_BOOL_FALSE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo temperature on failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set fifo temperature on disable.\n");
    res = bmp390_get_fifo_temperature_on(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get fifo temperature on failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check fifo temperature on %s.\n", enable==BMP390_BOOL_FALSE?"ok":"error");
    
    /* bmp390_set_fifo_subsampling/bmp390_get_fifo_subsampling test */
    bmp390_interface_debug_print("bmp390: bmp390_set_fifo_subsampling/bmp390_get_fifo_subsampling test.\n");
    subsampling_in = rand()%7;
    res = bmp390_set_fifo_subsampling(&gs_handle, subsampling_in);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo subsampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set fifo subsampling %d.\n", subsampling_in);
    res = bmp390_get_fifo_subsampling(&gs_handle, (uint8_t *)&subsampling_out);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get fifo subsampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check fifo subsampling on %s.\n", subsampling_in==subsampling_out?"ok":"error");
    
    /* bmp390_set_fifo_data_source/bmp390_get_fifo_data_source test */
    bmp390_interface_debug_print("bmp390: bmp390_set_fifo_data_source/bmp390_get_fifo_data_source test.\n");
    
    /* set unfiltered */
    res = bmp390_set_fifo_data_source(&gs_handle, BMP390_FIFO_DATA_SOURCE_UNFILTERED);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo data source failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set fifo data source unfiltered.\n");
    res = bmp390_get_fifo_data_source(&gs_handle, (bmp390_fifo_data_source_t *)&source);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get fifo data source failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check fifo data source %s.\n", source==BMP390_FIFO_DATA_SOURCE_UNFILTERED?"ok":"error");
    
    /* set filtered */
    res = bmp390_set_fifo_data_source(&gs_handle, BMP390_FIFO_DATA_SOURCE_FILTERED);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set fifo data source failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set fifo data source filtered.\n");
    res = bmp390_get_fifo_data_source(&gs_handle, (bmp390_fifo_data_source_t *)&source);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get fifo data source failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check fifo data source %s.\n", source==BMP390_FIFO_DATA_SOURCE_FILTERED?"ok":"error");
    
    /* bmp390_set_interrupt_pin_type/bmp390_get_interrupt_pin_type test */
    bmp390_interface_debug_print("bmp390: bmp390_set_interrupt_pin_type/bmp390_get_interrupt_pin_type test.\n");
    
    /* set push pull */
    res = bmp390_set_interrupt_pin_type(&gs_handle, BMP390_INTERRUPT_PIN_TYPE_PUSH_PULL);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interrupt pin type failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set interrupt pin type push pull.\n");
    res = bmp390_get_interrupt_pin_type(&gs_handle, (bmp390_interrupt_pin_type_t *)&pin_type);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get interrupt pin type failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check interrupt pin type %s.\n", pin_type==BMP390_INTERRUPT_PIN_TYPE_PUSH_PULL?"ok":"error");
    
    /* set open drain */
    res = bmp390_set_interrupt_pin_type(&gs_handle, BMP390_INTERRUPT_PIN_TYPE_OPEN_DRAIN);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interrupt pin type failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set interrupt pin type open drain.\n");
    res = bmp390_get_interrupt_pin_type(&gs_handle, (bmp390_interrupt_pin_type_t *)&pin_type);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get interrupt pin type failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check interrupt pin type %s.\n", pin_type==BMP390_INTERRUPT_PIN_TYPE_OPEN_DRAIN?"ok":"error");
    
    /* bmp390_set_interrupt_active_level/bmp390_get_interrupt_active_level test */
    bmp390_interface_debug_print("bmp390: bmp390_set_interrupt_active_level/bmp390_get_interrupt_active_level test.\n");
    
    /* set lower */
    res = bmp390_set_interrupt_active_level(&gs_handle, BMP390_INTERRUPT_ACTIVE_LEVEL_LOWER);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interrupt active level failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set interrupt active level lower.\n");
    res = bmp390_get_interrupt_active_level(&gs_handle, (bmp390_interrupt_active_level_t *)&level);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get interrupt active level failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check interrupt active level %s.\n", level==BMP390_INTERRUPT_ACTIVE_LEVEL_LOWER?"ok":"error");
    
    /* set higher */
    res = bmp390_set_interrupt_active_level(&gs_handle, BMP390_INTERRUPT_ACTIVE_LEVEL_HIGHER);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interrupt active level failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set interrupt active level higher.\n");
    res = bmp390_get_interrupt_active_level(&gs_handle, (bmp390_interrupt_active_level_t *)&level);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get interrupt active level failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check interrupt active level %s.\n", level==BMP390_INTERRUPT_ACTIVE_LEVEL_HIGHER?"ok":"error");
    
    /* bmp390_set_latch_interrupt_pin_and_interrupt_status/bmp390_get_latch_interrupt_pin_and_interrupt_status test */
    bmp390_interface_debug_print("bmp390: bmp390_set_latch_interrupt_pin_and_interrupt_status/bmp390_get_latch_interrupt_pin_and_interrupt_status test.\n");
    
    /* enable */
    res = bmp390_set_latch_interrupt_pin_and_interrupt_status(&gs_handle, BMP390_BOOL_TRUE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set latch interrupt pin and interrupt status failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set latch interrupt pin and interrupt status enable.\n");
    res = bmp390_get_latch_interrupt_pin_and_interrupt_status(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get latch interrupt pin and interrupt status failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check latch interrupt pin and interrupt status %s.\n", enable==BMP390_BOOL_TRUE?"ok":"error");
    
    /* disable */
    res = bmp390_set_latch_interrupt_pin_and_interrupt_status(&gs_handle, BMP390_BOOL_FALSE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set latch interrupt pin and interrupt status failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set latch interrupt pin and interrupt status disable.\n");
    res = bmp390_get_latch_interrupt_pin_and_interrupt_status(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get latch interrupt pin and interrupt status failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check latch interrupt pin and interrupt status %s.\n", enable==BMP390_BOOL_FALSE?"ok":"error");
    
    /* bmp390_set_interrupt_fifo_watermark/bmp390_get_interrupt_fifo_watermark test */
    bmp390_interface_debug_print("bmp390: bmp390_set_interrupt_fifo_watermark/bmp390_get_interrupt_fifo_watermark test.\n");
    
    /* enable */
    res = bmp390_set_interrupt_fifo_watermark(&gs_handle, BMP390_BOOL_TRUE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interrupt fifo watermark failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set interrupt fifo watermark enable.\n");
    res = bmp390_get_interrupt_fifo_watermark(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get interrupt fifo watermark failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check interrupt fifo watermark %s.\n", enable==BMP390_BOOL_TRUE?"ok":"error");
    
    /* disable */
    res = bmp390_set_interrupt_fifo_watermark(&gs_handle, BMP390_BOOL_FALSE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interrupt fifo watermark failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set interrupt fifo watermark disable.\n");
    res = bmp390_get_interrupt_fifo_watermark(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get interrupt fifo watermark failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check interrupt fifo watermark %s.\n", enable==BMP390_BOOL_FALSE?"ok":"error");
    
    /* bmp390_set_interrupt_fifo_full/bmp390_get_interrupt_fifo_full test */
    bmp390_interface_debug_print("bmp390: bmp390_set_interrupt_fifo_full/bmp390_get_interrupt_fifo_full test.\n");
    
    /* enable */
    res = bmp390_set_interrupt_fifo_full(&gs_handle, BMP390_BOOL_TRUE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interrupt fifo full failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set interrupt fifo full enable.\n");
    res = bmp390_get_interrupt_fifo_full(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get interrupt fifo full failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check interrupt fifo full %s.\n", enable==BMP390_BOOL_TRUE?"ok":"error");
    
    /* disable */
    res = bmp390_set_interrupt_fifo_full(&gs_handle, BMP390_BOOL_FALSE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interrupt fifo full failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set interrupt fifo full disable.\n");
    res = bmp390_get_interrupt_fifo_full(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get interrupt fifo full failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check interrupt fifo full %s.\n", enable==BMP390_BOOL_FALSE?"ok":"error");
    
    /* bmp390_set_interrupt_data_ready/bmp390_get_interrupt_fifo_data_ready test */
    bmp390_interface_debug_print("bmp390: bmp390_set_interrupt_data_ready/bmp390_get_interrupt_fifo_data_ready test.\n");
    
    /* enable */
    res = bmp390_set_interrupt_data_ready(&gs_handle, BMP390_BOOL_TRUE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interrupt data ready failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set interrupt data ready enable.\n");
    res = bmp390_get_interrupt_data_ready(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get interrupt data ready failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check interrupt data ready %s.\n", enable==BMP390_BOOL_TRUE?"ok":"error");
    
    /* disable */
    res = bmp390_set_interrupt_data_ready(&gs_handle, BMP390_BOOL_FALSE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set interrupt data ready failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set interrupt data ready disable.\n");
    res = bmp390_get_interrupt_data_ready(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get interrupt data ready failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check interrupt data ready %s.\n", enable==BMP390_BOOL_FALSE?"ok":"error");
    
    /* bmp390_set_spi_wire/bmp390_get_spi_wire test */
    bmp390_interface_debug_print("bmp390: bmp390_set_spi_wire/bmp390_get_spi_wire test.\n");
    
    /* set 4 wire */
    res = bmp390_set_spi_wire(&gs_handle, BMP390_SPI_WIRE_4);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set spi wire failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set spi 4 wire.\n");
    res = bmp390_get_spi_wire(&gs_handle, (bmp390_spi_wire_t *)&wire);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get spi wire failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check spi wire %s.\n", wire==BMP390_SPI_WIRE_4?"ok":"error");
    if (interface == BMP390_INTERFACE_IIC)
    {
        /* set 3 wire */
        res = bmp390_set_spi_wire(&gs_handle, BMP390_SPI_WIRE_3);
        if (res != 0)
        {
            bmp390_interface_debug_print("bmp390: set spi wire failed.\n");
            (void)bmp390_deinit(&gs_handle);
            
            return 1;
        }
        bmp390_interface_debug_print("bmp390: set spi 3 wire.\n");
        res = bmp390_get_spi_wire(&gs_handle, (bmp390_spi_wire_t *)&wire);
        if (res != 0)
        {
            bmp390_interface_debug_print("bmp390: get spi wire failed.\n");
            (void)bmp390_deinit(&gs_handle);
            
            return 1;
        }
        bmp390_interface_debug_print("bmp390: check spi wire %s.\n", wire==BMP390_SPI_WIRE_3?"ok":"error");
    }
    
    /* bmp390_set_iic_watchdog_timer/bmp390_get_iic_watchdog_timer test */
    bmp390_interface_debug_print("bmp390: bmp390_set_iic_watchdog_timer/bmp390_get_iic_watchdog_timer test.\n");
    
    /* enable */
    res = bmp390_set_iic_watchdog_timer(&gs_handle, BMP390_BOOL_TRUE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set iic watchdog timer failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set iic watchdog timer enable.\n");
    res = bmp390_get_iic_watchdog_timer(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get iic watchdog timer failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check iic watchdog timer %s.\n", enable==BMP390_BOOL_TRUE?"ok":"error");
    
    /* disable */
    res = bmp390_set_iic_watchdog_timer(&gs_handle, BMP390_BOOL_FALSE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set iic watchdog timer failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set iic watchdog timer disable.\n");
    res = bmp390_get_iic_watchdog_timer(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get iic watchdog timer failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check iic watchdog timer %s.\n", enable==BMP390_BOOL_FALSE?"ok":"error");
    
    /* bmp390_set_iic_watchdog_period/bmp390_get_iic_watchdog_period test */
    bmp390_interface_debug_print("bmp390: bmp390_set_iic_watchdog_period/bmp390_get_iic_watchdog_period test.\n");
    
    /* set 1.25ms period */
    res = bmp390_set_iic_watchdog_period(&gs_handle, BMP390_IIC_WATCHDOG_PERIOD_1P25_MS);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set iic watchdog period failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set iic watchdog period 1.25ms.\n");
    res = bmp390_get_iic_watchdog_period(&gs_handle, (bmp390_iic_watchdog_period_t *)&period);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get iic watchdog period failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check iic watchdog period %s.\n", period==BMP390_IIC_WATCHDOG_PERIOD_1P25_MS?"ok":"error");
    
    /* set 40ms period */
    res = bmp390_set_iic_watchdog_period(&gs_handle, BMP390_IIC_WATCHDOG_PERIOD_40_MS);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set iic watchdog period failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set iic watchdog period 40ms.\n");
    res = bmp390_get_iic_watchdog_period(&gs_handle, (bmp390_iic_watchdog_period_t *)&period);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get iic watchdog period failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check iic watchdog period %s.\n", period==BMP390_IIC_WATCHDOG_PERIOD_40_MS?"ok":"error");
    
    /* bmp390_set_pressure/bmp390_get_pressure test */
    bmp390_interface_debug_print("bmp390: bmp390_set_pressure/bmp390_get_pressure test.\n");
    
    /* disable */
    res = bmp390_set_pressure(&gs_handle, BMP390_BOOL_FALSE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set pressure failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set pressure disable.\n");
    res = bmp390_get_pressure(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get pressure failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check pressure %s.\n", enable==BMP390_BOOL_FALSE?"ok":"error");
    
    /* enable */
    res = bmp390_set_pressure(&gs_handle, BMP390_BOOL_TRUE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set pressure failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set pressure enable.\n");
    res = bmp390_get_pressure(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get pressure failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check pressure %s.\n", enable==BMP390_BOOL_TRUE?"ok":"error");
    
    /* bmp390_set_temperature/bmp390_get_temperature test */
    bmp390_interface_debug_print("bmp390: bmp390_set_temperature/bmp390_get_temperature test.\n");
    
    /* disable */
    res = bmp390_set_temperature(&gs_handle, BMP390_BOOL_FALSE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set temperature failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set temperature disable.\n");
    res = bmp390_get_temperature(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get temperature failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check temperature %s.\n", enable==BMP390_BOOL_FALSE?"ok":"error");
    
    /* enable */
    res = bmp390_set_temperature(&gs_handle, BMP390_BOOL_TRUE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set temperature failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set temperature enable.\n");
    res = bmp390_get_temperature(&gs_handle, (bmp390_bool_t *)&enable);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get temperature failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check temperature %s.\n", enable==BMP390_BOOL_TRUE?"ok":"error");
    
    /* bmp390_set_mode/bmp390_get_mode test */
    bmp390_interface_debug_print("bmp390: bmp390_set_mode/bmp390_get_mode test.\n");
    
    /* sleep */
    res = bmp390_set_mode(&gs_handle, BMP390_MODE_SLEEP_MODE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set mode failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set mode sleep.\n");
    res = bmp390_get_mode(&gs_handle, (bmp390_mode_t *)&mode);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get mode failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check mode %s.\n", mode==BMP390_MODE_SLEEP_MODE?"ok":"error");
    
    /* normal mode */
    res = bmp390_set_mode(&gs_handle, BMP390_MODE_NORMAL_MODE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set mode failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set mode normal mode.\n");
    res = bmp390_get_mode(&gs_handle, (bmp390_mode_t *)&mode);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get mode failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check mode %s.\n", mode==BMP390_MODE_NORMAL_MODE?"ok":"error");
    
    /* set forced mode */
    res = bmp390_set_mode(&gs_handle, BMP390_MODE_SLEEP_MODE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set mode failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_delay_ms(50);
    res = bmp390_set_mode(&gs_handle, BMP390_MODE_FORCED_MODE);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set mode failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set mode forced mode.\n");
    res = bmp390_get_mode(&gs_handle, (bmp390_mode_t *)&mode);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get mode failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check mode %s.\n", mode==BMP390_MODE_FORCED_MODE?"ok":"error"); 
    
    /* bmp390_set_pressure_oversampling/bmp390_get_pressure_oversampling test */
    bmp390_interface_debug_print("bmp390: bmp390_set_pressure_oversampling/bmp390_get_pressure_oversampling test.\n");
    
    /* set oversampling x1 */
    res = bmp390_set_pressure_oversampling(&gs_handle, BMP390_OVERSAMPLING_x1);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set pressure oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set pressure oversampling x1.\n");
    res = bmp390_get_pressure_oversampling(&gs_handle, (bmp390_oversampling_t *)&oversampling);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get pressure oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check pressure oversampling %s.\n", oversampling==BMP390_OVERSAMPLING_x1?"ok":"error");
    
    /* set oversampling x2 */
    res = bmp390_set_pressure_oversampling(&gs_handle, BMP390_OVERSAMPLING_x2);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set pressure oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set pressure oversampling x2.\n");
    res = bmp390_get_pressure_oversampling(&gs_handle, (bmp390_oversampling_t *)&oversampling);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get pressure oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check pressure oversampling %s.\n", oversampling==BMP390_OVERSAMPLING_x2?"ok":"error");
    
    /* set oversampling x4 */
    res = bmp390_set_pressure_oversampling(&gs_handle, BMP390_OVERSAMPLING_x4);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set pressure oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set pressure oversampling x4.\n");
    res = bmp390_get_pressure_oversampling(&gs_handle, (bmp390_oversampling_t *)&oversampling);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get pressure oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check pressure oversampling %s.\n", oversampling==BMP390_OVERSAMPLING_x4?"ok":"error");
    
    /* set oversampling x8 */
    res = bmp390_set_pressure_oversampling(&gs_handle, BMP390_OVERSAMPLING_x8);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set pressure oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set pressure oversampling x8.\n");
    res = bmp390_get_pressure_oversampling(&gs_handle, (bmp390_oversampling_t *)&oversampling);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get pressure oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check pressure oversampling %s.\n", oversampling==BMP390_OVERSAMPLING_x8?"ok":"error");
    
    /* set oversampling x16 */
    res = bmp390_set_pressure_oversampling(&gs_handle, BMP390_OVERSAMPLING_x16);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set pressure oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set pressure oversampling x16.\n");
    res = bmp390_get_pressure_oversampling(&gs_handle, (bmp390_oversampling_t *)&oversampling);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get pressure oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check pressure oversampling %s.\n", oversampling==BMP390_OVERSAMPLING_x16?"ok":"error");
    
    /* set oversampling x32 */
    res = bmp390_set_pressure_oversampling(&gs_handle, BMP390_OVERSAMPLING_x32);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set pressure oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set pressure oversampling x32.\n");
    res = bmp390_get_pressure_oversampling(&gs_handle, (bmp390_oversampling_t *)&oversampling);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get pressure oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check pressure oversampling %s.\n", oversampling==BMP390_OVERSAMPLING_x32?"ok":"error");
    
    /* bmp390_set_temperature_oversampling/bmp390_get_temperature_oversampling test */
    bmp390_interface_debug_print("bmp390: bmp390_set_temperature_oversampling/bmp390_get_temperature_oversampling test.\n");
    
    /* oversampling x1 */
    res = bmp390_set_temperature_oversampling(&gs_handle, BMP390_OVERSAMPLING_x1);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set temperature oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set temperature oversampling x1.\n");
    res = bmp390_get_temperature_oversampling(&gs_handle, (bmp390_oversampling_t *)&oversampling);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get temperature oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check temperature oversampling %s.\n", oversampling==BMP390_OVERSAMPLING_x1?"ok":"error");  
    
    /* set oversampling x2 */
    res = bmp390_set_temperature_oversampling(&gs_handle, BMP390_OVERSAMPLING_x2);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set temperature oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set temperature oversampling x2.\n");
    res = bmp390_get_temperature_oversampling(&gs_handle, (bmp390_oversampling_t *)&oversampling);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get temperature oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check temperature oversampling %s.\n", oversampling==BMP390_OVERSAMPLING_x2?"ok":"error");
    
    /* set oversampling x4 */
    res = bmp390_set_temperature_oversampling(&gs_handle, BMP390_OVERSAMPLING_x4);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set temperature oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set temperature oversampling x4.\n");
    res = bmp390_get_temperature_oversampling(&gs_handle, (bmp390_oversampling_t *)&oversampling);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get temperature oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check temperature oversampling %s.\n", oversampling==BMP390_OVERSAMPLING_x4?"ok":"error");
    
    /* set oversampling x8 */
    res = bmp390_set_temperature_oversampling(&gs_handle, BMP390_OVERSAMPLING_x8);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set temperature oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set temperature oversampling x8.\n");
    res = bmp390_get_temperature_oversampling(&gs_handle, (bmp390_oversampling_t *)&oversampling);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get temperature oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check temperature oversampling %s.\n", oversampling==BMP390_OVERSAMPLING_x8?"ok":"error");
    
    /* set oversampling x16 */
    res = bmp390_set_temperature_oversampling(&gs_handle, BMP390_OVERSAMPLING_x16);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set temperature oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set temperature oversampling x16.\n");
    res = bmp390_get_temperature_oversampling(&gs_handle, (bmp390_oversampling_t *)&oversampling);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get temperature oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check temperature oversampling %s.\n", oversampling==BMP390_OVERSAMPLING_x16?"ok":"error");
    
    /* set oversampling x32 */
    res = bmp390_set_temperature_oversampling(&gs_handle, BMP390_OVERSAMPLING_x32);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set temperature oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set temperature oversampling x32.\n");
    res = bmp390_get_temperature_oversampling(&gs_handle, (bmp390_oversampling_t *)&oversampling);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get temperature oversampling failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check temperature oversampling %s.\n", oversampling==BMP390_OVERSAMPLING_x32?"ok":"error");
    
    /* bmp390_set_odr/bmp390_get_odr test */
    bmp390_interface_debug_print("bmp390: bmp390_set_odr/bmp390_get_odr test.\n");
    
    /* set odr 200Hz */
    res = bmp390_set_odr(&gs_handle, BMP390_ODR_200_HZ);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set odr 200Hz.\n");
    res = bmp390_get_odr(&gs_handle, (bmp390_odr_t *)&odr);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check odr %s.\n", odr==BMP390_ODR_200_HZ?"ok":"error");
    
    /* set odr 100Hz */
    res = bmp390_set_odr(&gs_handle, BMP390_ODR_100_HZ);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set odr 100Hz.\n");
    res = bmp390_get_odr(&gs_handle, (bmp390_odr_t *)&odr);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check odr %s.\n", odr==BMP390_ODR_100_HZ?"ok":"error");
    
    /* set odr 50Hz */
    res = bmp390_set_odr(&gs_handle, BMP390_ODR_50_HZ);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set odr 50Hz.\n");
    res = bmp390_get_odr(&gs_handle, (bmp390_odr_t *)&odr);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check odr %s.\n", odr==BMP390_ODR_50_HZ?"ok":"error");
    
    /* set odr 25Hz */
    res = bmp390_set_odr(&gs_handle, BMP390_ODR_25_HZ);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set odr 25Hz.\n");
    res = bmp390_get_odr(&gs_handle, (bmp390_odr_t *)&odr);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check odr %s.\n", odr==BMP390_ODR_25_HZ?"ok":"error");
    
    /* set odr 12.5Hz */
    res = bmp390_set_odr(&gs_handle, BMP390_ODR_12P5_HZ);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set odr 12.5Hz.\n");
    res = bmp390_get_odr(&gs_handle, (bmp390_odr_t *)&odr);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check odr %s.\n", odr==BMP390_ODR_12P5_HZ?"ok":"error");
    
    /* set odr 6.25Hz */
    res = bmp390_set_odr(&gs_handle, BMP390_ODR_6P25_HZ);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set odr 6.25Hz.\n");
    res = bmp390_get_odr(&gs_handle, (bmp390_odr_t *)&odr);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check odr %s.\n", odr==BMP390_ODR_6P25_HZ?"ok":"error");
    
    /* set odr 3.1Hz */
    res = bmp390_set_odr(&gs_handle, BMP390_ODR_3P1_HZ);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set odr 3.1Hz.\n");
    res = bmp390_get_odr(&gs_handle, (bmp390_odr_t *)&odr);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check odr %s.\n", odr==BMP390_ODR_3P1_HZ?"ok":"error");
    
    /* set odr 1.5Hz */
    res = bmp390_set_odr(&gs_handle, BMP390_ODR_1P5_HZ);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set odr 1.5Hz.\n");
    res = bmp390_get_odr(&gs_handle, (bmp390_odr_t *)&odr);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check odr %s.\n", odr==BMP390_ODR_1P5_HZ?"ok":"error");
    
    /* set odr 0.78Hz */
    res = bmp390_set_odr(&gs_handle, BMP390_ODR_0P78_HZ);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set odr 0.78Hz.\n");
    res = bmp390_get_odr(&gs_handle, (bmp390_odr_t *)&odr);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check odr %s.\n", odr==BMP390_ODR_0P78_HZ?"ok":"error");
    
    /* set odr 0.39Hz */
    res = bmp390_set_odr(&gs_handle, BMP390_ODR_0P39_HZ);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set odr 0.39Hz.\n");
    res = bmp390_get_odr(&gs_handle, (bmp390_odr_t *)&odr);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check odr %s.\n", odr==BMP390_ODR_0P39_HZ?"ok":"error");
    
    /* set odr 0.2Hz */
    res = bmp390_set_odr(&gs_handle, BMP390_ODR_0P2_HZ);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set odr 0.2Hz.\n");
    res = bmp390_get_odr(&gs_handle, (bmp390_odr_t *)&odr);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check odr %s.\n", odr==BMP390_ODR_0P2_HZ?"ok":"error");
    
    /* set odr 0.1Hz */
    res = bmp390_set_odr(&gs_handle, BMP390_ODR_0P1_HZ);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set odr 0.1Hz.\n");
    res = bmp390_get_odr(&gs_handle, (bmp390_odr_t *)&odr);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check odr %s.\n", odr==BMP390_ODR_0P1_HZ?"ok":"error");
    
    /* set odr 0.05Hz */
    res = bmp390_set_odr(&gs_handle, BMP390_ODR_0P05_HZ);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set odr 0.05Hz.\n");
    res = bmp390_get_odr(&gs_handle, (bmp390_odr_t *)&odr);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check odr %s.\n", odr==BMP390_ODR_0P05_HZ?"ok":"error");
    
    /* set odr 0.02Hz */
    res = bmp390_set_odr(&gs_handle, BMP390_ODR_0P02_HZ);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set odr 0.02Hz.\n");
    res = bmp390_get_odr(&gs_handle, (bmp390_odr_t *)&odr);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check odr %s.\n", odr==BMP390_ODR_0P02_HZ?"ok":"error");
    
    /* set odr 0.01Hz */
    res = bmp390_set_odr(&gs_handle, BMP390_ODR_0P01_HZ);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set odr 0.01Hz.\n");
    res = bmp390_get_odr(&gs_handle, (bmp390_odr_t *)&odr);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check odr %s.\n", odr==BMP390_ODR_0P01_HZ?"ok":"error");
    
    /* set odr 0.006Hz */
    res = bmp390_set_odr(&gs_handle, BMP390_ODR_0P006_HZ);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set odr 0.006Hz.\n");
    res = bmp390_get_odr(&gs_handle, (bmp390_odr_t *)&odr);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check odr %s.\n", odr==BMP390_ODR_0P006_HZ?"ok":"error");
    
    /* set odr 0.003Hz */
    res = bmp390_set_odr(&gs_handle, BMP390_ODR_0P003_HZ);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set odr 0.003Hz.\n");
    res = bmp390_get_odr(&gs_handle, (bmp390_odr_t *)&odr);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check odr %s.\n", odr==BMP390_ODR_0P003_HZ?"ok":"error");
    
    /* set odr 0.0015Hz */
    res = bmp390_set_odr(&gs_handle, BMP390_ODR_0P0015_HZ);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set odr 0.0015Hz.\n");
    res = bmp390_get_odr(&gs_handle, (bmp390_odr_t *)&odr);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get odr failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check odr %s.\n", odr==BMP390_ODR_0P0015_HZ?"ok":"error");
    
    /* bmp390_set_filter_coefficient/bmp390_get_filter_coefficient test */
    bmp390_interface_debug_print("bmp390: bmp390_set_filter_coefficient/bmp390_get_filter_coefficient test.\n");
    
    /* set coefficient 0 */
    res = bmp390_set_filter_coefficient(&gs_handle, BMP390_FILTER_COEFFICIENT_0);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set filter coefficient failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set filter coefficient 0.\n");
    res = bmp390_get_filter_coefficient(&gs_handle, (bmp390_filter_coefficient_t *)&coef);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get filter coefficient failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check filter coefficient %s.\n", coef==BMP390_FILTER_COEFFICIENT_0?"ok":"error");
    
    /* set coefficient 1 */
    res = bmp390_set_filter_coefficient(&gs_handle, BMP390_FILTER_COEFFICIENT_1);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set filter coefficient failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set filter coefficient 1.\n");
    res = bmp390_get_filter_coefficient(&gs_handle, (bmp390_filter_coefficient_t *)&coef);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get filter coefficient failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check filter coefficient %s.\n", coef==BMP390_FILTER_COEFFICIENT_1?"ok":"error");
    
    /* set coefficient 3 */
    res = bmp390_set_filter_coefficient(&gs_handle, BMP390_FILTER_COEFFICIENT_3);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set filter coefficient failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set filter coefficient 3.\n");
    res = bmp390_get_filter_coefficient(&gs_handle, (bmp390_filter_coefficient_t *)&coef);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get filter coefficient failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check filter coefficient %s.\n", coef==BMP390_FILTER_COEFFICIENT_3?"ok":"error");
    
    /* set coefficient 7 */
    res = bmp390_set_filter_coefficient(&gs_handle, BMP390_FILTER_COEFFICIENT_7);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set filter coefficient failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set filter coefficient 7.\n");
    res = bmp390_get_filter_coefficient(&gs_handle, (bmp390_filter_coefficient_t *)&coef);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get filter coefficient failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check filter coefficient %s.\n", coef==BMP390_FILTER_COEFFICIENT_7?"ok":"error");
    
    /* set coefficient 15 */
    res = bmp390_set_filter_coefficient(&gs_handle, BMP390_FILTER_COEFFICIENT_15);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set filter coefficient failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set filter coefficient 15.\n");
    res = bmp390_get_filter_coefficient(&gs_handle, (bmp390_filter_coefficient_t *)&coef);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get filter coefficient failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check filter coefficient %s.\n", coef==BMP390_FILTER_COEFFICIENT_15?"ok":"error");
    
    /* set coefficient 31 */
    res = bmp390_set_filter_coefficient(&gs_handle, BMP390_FILTER_COEFFICIENT_31);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set filter coefficient failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set filter coefficient 31.\n");
    res = bmp390_get_filter_coefficient(&gs_handle, (bmp390_filter_coefficient_t *)&coef);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get filter coefficient failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check filter coefficient %s.\n", coef==BMP390_FILTER_COEFFICIENT_31?"ok":"error");
    
    /* set coefficient 63 */
    res = bmp390_set_filter_coefficient(&gs_handle, BMP390_FILTER_COEFFICIENT_63);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set filter coefficient failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set filter coefficient 63.\n");
    res = bmp390_get_filter_coefficient(&gs_handle, (bmp390_filter_coefficient_t *)&coef);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get filter coefficient failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check filter coefficient %s.\n", coef==BMP390_FILTER_COEFFICIENT_63?"ok":"error");
    
    /* set coefficient 127 */
    res = bmp390_set_filter_coefficient(&gs_handle, BMP390_FILTER_COEFFICIENT_127);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: set filter coefficient failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: set filter coefficient 127.\n");
    res = bmp390_get_filter_coefficient(&gs_handle, (bmp390_filter_coefficient_t *)&coef);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get filter coefficient failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: check filter coefficient %s.\n", coef==BMP390_FILTER_COEFFICIENT_127?"ok":"error");
    
    /* bmp390_get_error */
    bmp390_interface_debug_print("bmp390: bmp390_get_error.\n");
    res = bmp390_get_error(&gs_handle, (uint8_t *)&err);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get err failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: err is 0x%02X.\n", (uint8_t)err);
    
    /* bmp390_get_status */
    bmp390_interface_debug_print("bmp390: bmp390_get_status.\n");
    res = bmp390_get_status(&gs_handle, (uint8_t *)&status);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get status failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: status is 0x%02X.\n", (uint8_t)status);
    
    /* bmp390_get_sensortime */
    bmp390_interface_debug_print("bmp390: bmp390_get_sensortime.\n");
    res = bmp390_get_sensortime(&gs_handle, (uint32_t *)&sensortime);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get sensortime failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: sensortime is 0x%06X.\n", (uint32_t)sensortime);
    
    /* bmp390_get_event */
    bmp390_interface_debug_print("bmp390: bmp390_get_event.\n");
    res = bmp390_get_event(&gs_handle, &event);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get event failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: event is %s.\n", event==BMP390_EVENT_NONE?"none":"power up or softrest");
    
    /* bmp390_get_interrupt_status */
    bmp390_interface_debug_print("bmp390: bmp390_get_interrupt_status.\n");
    res = bmp390_get_interrupt_status(&gs_handle, (uint8_t *)&status);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get interrupt status failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: interrupt status is 0x%02X.\n", (uint8_t)status);
    
    /* bmp390_get_fifo_length */
    bmp390_interface_debug_print("bmp390: bmp390_get_fifo_length.\n");
    res = bmp390_get_fifo_length(&gs_handle, (uint16_t *)&length);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get fifo length failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: get fifo length is 0x%04X.\n", (uint16_t)length);
    
    /* bmp390_get_fifo_data */
    bmp390_interface_debug_print("bmp390: bmp390_get_fifo_data.\n");
    res = bmp390_get_fifo_data(&gs_handle, (uint8_t *)&data, 1);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get fifo data failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: get fifo data is 0x%04X.\n", (uint8_t)data);
    
    /* bmp390_flush_fifo */
    bmp390_interface_debug_print("bmp390: bmp390_flush_fifo.\n");
    res = bmp390_flush_fifo(&gs_handle);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: flush fifo failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: flush fifo %s.\n", res?"error":"ok");
    
    /* bmp390_get_revision_id */
    bmp390_interface_debug_print("bmp390: bmp390_get_revision_id.\n");
    res = bmp390_get_revision_id(&gs_handle, &id);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: get revision id failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: revision id is 0x%02X.\n", id);
    
    /* bmp390_softreset */
    bmp390_interface_debug_print("bmp390: bmp390_softreset.\n");
    res = bmp390_softreset(&gs_handle);
    if (res != 0)
    {
        bmp390_interface_debug_print("bmp390: bmp390 softreset failed.\n");
        (void)bmp390_deinit(&gs_handle);
        
        return 1;
    }
    bmp390_interface_debug_print("bmp390: softreset %s.\n", res?"error":"ok");
    
    /* finish register test */
    bmp390_interface_debug_print("bmp390: finish register test.\n");
    (void)bmp390_deinit(&gs_handle);
    
    return 0;
}
