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
 * @file      main.c
 * @brief     main source file
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

#include "driver_bmp390_basic.h"
#include "driver_bmp390_shot.h"
#include "driver_bmp390_interrupt.h"
#include "driver_bmp390_fifo.h"
#include "driver_bmp390_register_test.h"
#include "driver_bmp390_read_test.h"
#include "driver_bmp390_interrupt_test.h"
#include "driver_bmp390_fifo_test.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include "gpio.h"
#include "shell.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
static uint8_t gs_data_ready_flag;         /**< data ready flag */
static float gs_temperature_c;             /**< temperature data */
static float gs_pressure_pa;               /**< pressure data */
static uint8_t gs_fifo_full_flag;          /**< fifo full flag */
static uint8_t gs_fifo_watermark_flag;     /**< fifo watermark flag */
static uint8_t gs_buf[512];                /**< buffer */
static bmp390_frame_t gs_frame[256];       /**< frame buffer */
uint8_t g_buf[256];                        /**< uart buffer */
uint16_t g_len;                            /**< uart buffer length */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< irq function address */

/**
 * @brief exti 0 irq
 * @note  none
 */
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
 * @brief     gpio exti callback
 * @param[in] pin is the gpio pin
 * @note      none
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_0)
    {
        if (g_gpio_irq)
        {
            g_gpio_irq();
        }
    }
}

/**
 * @brief     interface interrupt receive callback
 * @param[in] type is the interrupt type
 * @note      none
 */
void bmp390_interface_interrupt_receive_callback(uint8_t type)
{
    switch (type)
    {
        case BMP390_INTERRUPT_STATUS_FIFO_WATERMARK :
        {
            break;
        }
        case BMP390_INTERRUPT_STATUS_FIFO_FULL :
        {
            break;
        }
        case BMP390_INTERRUPT_STATUS_DATA_READY :
        {
            /* read temperature pressure */
            if (bmp390_interrupt_read((float *)&gs_temperature_c, (float *)&gs_pressure_pa) != 0)
            {
                bmp390_interface_debug_print("bmp390: read temperature and pressure failed.\n");
           
                return;
            }
            gs_data_ready_flag  = 1;
            
            break;
        }
        default :
        {
            break;
        }
    }
}

/**
 * @brief     interface fifo receive callback
 * @param[in] type is the interrupt type
 * @note      none
 */
void bmp390_interface_fifo_receive_callback(uint8_t type)
{
    switch (type)
    {
        case BMP390_INTERRUPT_STATUS_FIFO_WATERMARK :
        {
            uint8_t res;
            uint16_t len;
            uint16_t i, frame_len;
            
            len = 512;
            frame_len = 256;
            res = bmp390_fifo_read(gs_buf, len, (bmp390_frame_t *)gs_frame, (uint16_t *)&frame_len);
            if (res != 0)
            {
                bmp390_interface_debug_print("bmp390: fifo read failed.\n");
                
                return;
            }
            for (i = 0; i < frame_len; i++)
            {
                if (gs_frame[i].type == BMP390_FRAME_TYPE_TEMPERATURE)
                {
                    bmp390_interface_debug_print("bmp390: fifo %d/%d.\n", i+1, frame_len);
                    bmp390_interface_debug_print("bmp390: temperature is %0.2fC.\n", gs_frame[i].data);
                }
                else if (gs_frame[i].type == BMP390_FRAME_TYPE_PRESSURE)
                {
                    bmp390_interface_debug_print("bmp390: fifo %d/%d.\n", i+1, frame_len);
                    bmp390_interface_debug_print("bmp390: pressure is %0.2fPa.\n", gs_frame[i].data);
                }
                else if (gs_frame[i].type == BMP390_FRAME_TYPE_SENSORTIME)
                {
                    bmp390_interface_debug_print("bmp390: fifo %d/%d.\n", i+1, frame_len);
                    bmp390_interface_debug_print("bmp390: sensortime is %d.\n", gs_frame[i].raw);
                }
                else
                {
                    bmp390_interface_debug_print("bmp390: fifo %d/%d.\n", i+1, frame_len);
                    bmp390_interface_debug_print("bmp390: unknow type.\n");
                }
            }
            gs_fifo_watermark_flag = 1;
            
            break;
        }
        case BMP390_INTERRUPT_STATUS_FIFO_FULL :
        {
            uint8_t res;
            uint16_t len;
            uint16_t i, frame_len;
            
            len = 512;
            frame_len = 256;
            res = bmp390_fifo_read(gs_buf, len, (bmp390_frame_t *)gs_frame, (uint16_t *)&frame_len);
            if (res != 0)
            {
                bmp390_interface_debug_print("bmp390: fifo read failed.\n");
                
                return;
            }
            for (i = 0; i < frame_len; i++)
            {
                if (gs_frame[i].type == BMP390_FRAME_TYPE_TEMPERATURE)
                {
                    bmp390_interface_debug_print("bmp390: fifo %d/%d.\n", i+1, frame_len);
                    bmp390_interface_debug_print("bmp390: temperature is %0.2fC.\n", gs_frame[i].data);
                }
                else if (gs_frame[i].type == BMP390_FRAME_TYPE_PRESSURE)
                {
                    bmp390_interface_debug_print("bmp390: fifo %d/%d.\n", i+1, frame_len);
                    bmp390_interface_debug_print("bmp390: pressure is %0.2fPa.\n", gs_frame[i].data);
                }
                else if (gs_frame[i].type == BMP390_FRAME_TYPE_SENSORTIME)
                {
                    bmp390_interface_debug_print("bmp390: fifo %d/%d.\n", i+1, frame_len);
                    bmp390_interface_debug_print("bmp390: sensortime is %d.\n", gs_frame[i].raw);
                }
                else
                {
                    bmp390_interface_debug_print("bmp390: fifo %d/%d.\n", i+1, frame_len);
                    bmp390_interface_debug_print("bmp390: unknow type.\n");
                }
            }
            gs_fifo_full_flag = 1;
            
            break;
        }
        case BMP390_INTERRUPT_STATUS_DATA_READY :
        {
            break;
        }
        default :
        {
            break;
        }
    }
}

/**
 * @brief     bmp390 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t bmp390(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
        goto help;
    }
    else if (argc == 2)
    {
        if (strcmp("-i", argv[1]) == 0)
        {
            bmp390_info_t info;
            
            /* print bmp390 info */
            bmp390_info(&info);
            bmp390_interface_debug_print("bmp390: chip is %s.\n", info.chip_name);
            bmp390_interface_debug_print("bmp390: manufacturer is %s.\n", info.manufacturer_name);
            bmp390_interface_debug_print("bmp390: interface is %s.\n", info.interface);
            bmp390_interface_debug_print("bmp390: driver version is %d.%d.\n", info.driver_version/1000, (info.driver_version%1000)/100);
            bmp390_interface_debug_print("bmp390: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
            bmp390_interface_debug_print("bmp390: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
            bmp390_interface_debug_print("bmp390: max current is %0.2fmA.\n", info.max_current_ma);
            bmp390_interface_debug_print("bmp390: max temperature is %0.1fC.\n", info.temperature_max);
            bmp390_interface_debug_print("bmp390: min temperature is %0.1fC.\n", info.temperature_min);
            
            return 0;
        }
        else if (strcmp("-p", argv[1]) == 0)
        {
            /* print pin connection */
            bmp390_interface_debug_print("bmp390: SPI interface SCK connected to GPIOA PIN5.\n");
            bmp390_interface_debug_print("bmp390: SPI interface MISO connected to GPIOA PIN6.\n");
            bmp390_interface_debug_print("bmp390: SPI interface MOSI connected to GPIOA PIN7.\n");
            bmp390_interface_debug_print("bmp390: SPI interface CS connected to GPIOA PIN4.\n");
            bmp390_interface_debug_print("bmp390: IIC interface SCL connected to GPIOB PIN8.\n");
            bmp390_interface_debug_print("bmp390: IIC interface SDA connected to GPIOB PIN9.\n");
            bmp390_interface_debug_print("bmp390: INT connected to GPIOB PIN0.\n");
            
            return 0;
        }
        else if (strcmp("-h", argv[1]) == 0)
        {
            /* show bmp390 help */
            
            help:
            
            bmp390_interface_debug_print("bmp390 -i\n\tshow bmp390 chip and driver information.\n");
            bmp390_interface_debug_print("bmp390 -h\n\tshow bmp390 help.\n");
            bmp390_interface_debug_print("bmp390 -p\n\tshow bmp390 pin connections of the current board.\n");
            bmp390_interface_debug_print("bmp390 -t reg (-iic | -spi) -a (0 | 1)\n\trun bmp390 register test.\n");
            bmp390_interface_debug_print("bmp390 -t read <times> (-iic | -spi) -a (0 | 1)\n\trun bmp390 read test.times means the test times.\n");
            bmp390_interface_debug_print("bmp390 -t int <times> (-iic | -spi) -a (0 | 1)\n\trun bmp390 interrupt test.times means the test times.\n");
            bmp390_interface_debug_print("bmp390 -t fifo <times> (-iic | -spi) -a (0 | 1)\n\trun bmp390 fifo test.times means the test times.\n");
            bmp390_interface_debug_print("bmp390 -c read <times> (-iic | -spi) -a (0 | 1)\n\trun bmp390 read function.times means the read times.\n");
            bmp390_interface_debug_print("bmp390 -c shot <times> (-iic | -spi) -a (0 | 1)\n\trun bmp390 shot function.times means the read times.\n");
            bmp390_interface_debug_print("bmp390 -c int <times> (-iic | -spi) -a (0 | 1)\n\trun bmp390 interrupt function.times means the read times.\n");
            bmp390_interface_debug_print("bmp390 -c fifo <times> (-iic | -spi) -a (0 | 1)\n\trun bmp390 fifo function.times means the read times.\n");
            
            return 0;
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 6)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
            /* reg test */
            if (strcmp("reg", argv[2]) == 0)
            {
                uint8_t res;
                bmp390_address_t addr_pin;
                bmp390_interface_t interface;
                
                if (strcmp("-iic", argv[3]) == 0)
                {
                    interface = BMP390_INTERFACE_IIC;
                }
                else if (strcmp("-spi", argv[3]) == 0)
                {
                    interface = BMP390_INTERFACE_SPI;
                }
                else
                {
                    bmp390_interface_debug_print("bmp390: interface is invalid.\n");
                    
                    return 5;
                }
                if (strcmp("-a", argv[4]) != 0)
                {
                    return 1;
                }
                if (strcmp("0", argv[5]) == 0)
                {
                    addr_pin = BMP390_ADDRESS_ADO_LOW;
                }
                else if (strcmp("1", argv[5]) == 0)
                {
                    addr_pin = BMP390_ADDRESS_ADO_HIGH;
                }
                else
                {
                    bmp390_interface_debug_print("bmp390: address is invalid.\n");
                    
                    return 5;
                }
                res = bmp390_register_test(interface, addr_pin);
                if (res != 0)
                {
                    return 1;
                }
                
                return 0;
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 7)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
            /* read test */
            if (strcmp("read", argv[2]) == 0)
            {
                uint8_t res;
                bmp390_address_t addr_pin;
                bmp390_interface_t interface;
                
                if (strcmp("-iic", argv[4]) == 0)
                {
                    interface = BMP390_INTERFACE_IIC;
                }
                else if (strcmp("-spi", argv[4]) == 0)
                {
                    interface = BMP390_INTERFACE_SPI;
                }
                else
                {
                    bmp390_interface_debug_print("bmp390: interface is invalid.\n");
                    
                    return 5;
                }
                if (strcmp("-a", argv[5]) != 0)
                {
                    return 5;
                }
                if (strcmp("0", argv[6]) == 0)
                {
                    addr_pin = BMP390_ADDRESS_ADO_LOW;
                }
                else if (strcmp("1", argv[6]) == 0)
                {
                    addr_pin = BMP390_ADDRESS_ADO_HIGH;
                }
                else
                {
                    bmp390_interface_debug_print("bmp390: address is invalid.\n");
                    
                    return 5;
                }
                res = bmp390_read_test(interface, addr_pin, atoi(argv[3]));
                if (res != 0)
                {
                    return 1;
                }
                
                return 0;
            }
            /* interrupt test */
            else if (strcmp("int", argv[2]) == 0)
            {
                uint8_t res;
                bmp390_address_t addr_pin;
                bmp390_interface_t interface;
                
                if (strcmp("-iic", argv[4]) == 0)
                {
                    interface = BMP390_INTERFACE_IIC;
                }
                else if (strcmp("-spi", argv[4]) == 0)
                {
                    interface = BMP390_INTERFACE_SPI;
                }
                else
                {
                    bmp390_interface_debug_print("bmp390: interface is invalid.\n");
                    
                    return 5;
                }
                if (strcmp("-a", argv[5]) != 0)
                {
                    return 5;
                }
                if (strcmp("0", argv[6]) == 0)
                {
                    addr_pin = BMP390_ADDRESS_ADO_LOW;
                }
                else if (strcmp("1", argv[6]) == 0)
                {
                    addr_pin = BMP390_ADDRESS_ADO_HIGH;
                }
                else
                {
                    bmp390_interface_debug_print("bmp390: address is invalid.\n");
                    
                    return 5;
                }
                res = gpio_interrupt_init();
                if (res != 0)
                {
                    return 1;
                }
                g_gpio_irq = bmp390_interrupt_test_irq_handler;
                res = bmp390_interrupt_test(interface, addr_pin, atoi(argv[3]));
                if (res != 0)
                {
                    g_gpio_irq = NULL;
                    (void)gpio_interrupt_deinit();
                    
                    return 1;
                }
                g_gpio_irq = NULL;
                (void)gpio_interrupt_deinit();
                
                return 0;
            }
            /* fifo test */
            else if (strcmp("fifo", argv[2]) == 0)
            {
                uint8_t res;
                bmp390_address_t addr_pin;
                bmp390_interface_t interface;
                
                if (strcmp("-iic", argv[4]) == 0)
                {
                    interface = BMP390_INTERFACE_IIC;
                }
                else if (strcmp("-spi", argv[4]) == 0)
                {
                    interface = BMP390_INTERFACE_SPI;
                }
                else
                {
                    bmp390_interface_debug_print("bmp390: interface is invalid.\n");
                    
                    return 5;
                }
                if (strcmp("-a", argv[5]) != 0)
                {
                    return 5;
                }
                if (strcmp("0", argv[6]) == 0)
                {
                    addr_pin = BMP390_ADDRESS_ADO_LOW;
                }
                else if (strcmp("1", argv[6]) == 0)
                {
                    addr_pin = BMP390_ADDRESS_ADO_HIGH;
                }
                else
                {
                    bmp390_interface_debug_print("bmp390: address is invalid.\n");
                    
                    return 5;
                }
                res = gpio_interrupt_init();
                if (res != 0)
                {
                    return 1;
                }
                g_gpio_irq = bmp390_fifo_test_irq_handler;
                res = bmp390_fifo_test(interface, addr_pin, atoi(argv[3]));
                if (res != 0)
                {
                    g_gpio_irq = NULL;
                    (void)gpio_interrupt_deinit();
                    
                    return 1;
                }
                g_gpio_irq = NULL;
                (void)gpio_interrupt_deinit();
                
                return 0;
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* run function */
        if (strcmp("-c", argv[1]) == 0)
        {
            /* read function */
            if (strcmp("read", argv[2]) == 0)
            {
                uint8_t res;
                uint32_t times, i;
                float temperature_c;
                float pressure_pa;
                bmp390_address_t addr_pin;
                bmp390_interface_t interface;
                
                if (strcmp("-iic", argv[4]) == 0)
                {
                    interface = BMP390_INTERFACE_IIC;
                }
                else if (strcmp("-spi", argv[4]) == 0)
                {
                    interface = BMP390_INTERFACE_SPI;
                }
                else
                {
                    bmp390_interface_debug_print("bmp390: interface is invalid.\n");
                    
                    return 5;
                }
                if (strcmp("-a", argv[5]) != 0)
                {
                    return 5;
                }
                if (strcmp("0", argv[6]) == 0)
                {
                    addr_pin = BMP390_ADDRESS_ADO_LOW;
                }
                else if (strcmp("1", argv[6]) == 0)
                {
                    addr_pin = BMP390_ADDRESS_ADO_HIGH;
                }
                else
                {
                    bmp390_interface_debug_print("bmp390: address is invalid.\n");
                    
                    return 5;
                }
                res = bmp390_basic_init(interface, addr_pin);
                if (res != 0)
                {
                    return 1;
                }
                times = atoi(argv[3]);
                for (i = 0; i < times; i++)
                {
                    bmp390_interface_delay_ms(1000);
                    res = bmp390_basic_read((float *)&temperature_c, (float *)&pressure_pa);
                    if (res != 0)
                    {
                        (void)bmp390_basic_deinit();
                        
                        return 1;
                    }
                    bmp390_interface_debug_print("bmp390: %d/%d.\n", i+1, times);
                    bmp390_interface_debug_print("bmp390: temperature is %0.2fC.\n", temperature_c);
                    bmp390_interface_debug_print("bmp390: pressure is %0.2fPa.\n", pressure_pa);
                }
                (void)bmp390_basic_deinit();
                
                return 0;
            }
            /* shot function */
            else if (strcmp("shot", argv[2]) == 0)
            {
                uint8_t res;
                uint32_t times, i;
                float temperature_c;
                float pressure_pa;
                bmp390_address_t addr_pin;
                bmp390_interface_t interface;
                
                if (strcmp("-iic", argv[4]) == 0)
                {
                    interface = BMP390_INTERFACE_IIC;
                }
                else if (strcmp("-spi", argv[4]) == 0)
                {
                    interface = BMP390_INTERFACE_SPI;
                }
                else
                {
                    bmp390_interface_debug_print("bmp390: interface is invalid.\n");
                    
                    return 5;
                }
                if (strcmp("-a", argv[5]) != 0)
                {
                    return 5;
                }
                if (strcmp("0", argv[6]) == 0)
                {
                    addr_pin = BMP390_ADDRESS_ADO_LOW;
                }
                else if (strcmp("1", argv[6]) == 0)
                {
                    addr_pin = BMP390_ADDRESS_ADO_HIGH;
                }
                else
                {
                    bmp390_interface_debug_print("bmp390: address is invalid.\n");
                    
                    return 5;
                }
                res = bmp390_shot_init(interface, addr_pin);
                if (res != 0)
                {
                    return 1;
                }
                times = atoi(argv[3]);
                for (i = 0; i < times; i++)
                {
                    bmp390_interface_delay_ms(1000);
                    res = bmp390_shot_read((float *)&temperature_c, (float *)&pressure_pa);
                    if (res != 0)
                    {
                        (void)bmp390_shot_deinit();
                        
                        return 1;
                    }
                    bmp390_interface_debug_print("bmp390: %d/%d.\n", i+1, times);
                    bmp390_interface_debug_print("bmp390: temperature is %0.2fC.\n", temperature_c);
                    bmp390_interface_debug_print("bmp390: pressure is %0.2fPa.\n", pressure_pa);
                }
                (void)bmp390_shot_deinit();
               
                return 0;
            }
            /* interrupt function */
            else if (strcmp("int", argv[2]) == 0)
            {
                uint8_t res;
                uint32_t times, i, timeout;;
                bmp390_address_t addr_pin;
                bmp390_interface_t interface;
                
                if (strcmp("-iic", argv[4]) == 0)
                {
                    interface = BMP390_INTERFACE_IIC;
                }
                else if (strcmp("-spi", argv[4]) == 0)
                {
                    interface = BMP390_INTERFACE_SPI;
                }
                else
                {
                    bmp390_interface_debug_print("bmp390: interface is invalid.\n");
                    
                    return 5;
                }
                if (strcmp("-a", argv[5]) != 0)
                {
                    return 5;
                }
                if (strcmp("0", argv[6]) == 0)
                {
                    addr_pin = BMP390_ADDRESS_ADO_LOW;
                }
                else if (strcmp("1", argv[6]) == 0)
                {
                    addr_pin = BMP390_ADDRESS_ADO_HIGH;
                }
                else
                {
                    bmp390_interface_debug_print("bmp390: address is invalid.\n");
                    
                    return 5;
                }
                g_gpio_irq = bmp390_interrupt_irq_handler;
                res = gpio_interrupt_init();
                if (res != 0)
                {
                    return 1;
                }
                res = bmp390_interrupt_init(interface, addr_pin, bmp390_interface_interrupt_receive_callback);
                if (res != 0)
                {
                    g_gpio_irq = NULL;
                    (void)gpio_interrupt_deinit();
                    
                    return 1;
                }
                times = atoi(argv[3]);
                gs_data_ready_flag = 0;
                timeout = 5000;
                for (i = 0; i < times; i++)
                {
                    while (timeout != 0)
                    {
                        bmp390_interface_delay_ms(100);
                        timeout--;
                        if (gs_data_ready_flag != 0)
                        {
                            break;
                        }
                        if (timeout == 0)
                        {
                            g_gpio_irq = NULL;
                            (void)gpio_interrupt_deinit();
                            (void)bmp390_interrupt_deinit();
                            
                            return 1;
                        }
                    }
                    gs_data_ready_flag = 0;
                    timeout = 5000;
                    bmp390_interface_debug_print("bmp390: %d/%d.\n", i+1, times);
                    bmp390_interface_debug_print("bmp390: temperature is %0.2fC.\n", gs_temperature_c);
                    bmp390_interface_debug_print("bmp390: pressure is %0.2fPa.\n", gs_pressure_pa);
                }
                g_gpio_irq = NULL;
                (void)gpio_interrupt_deinit();
                (void)bmp390_interrupt_deinit();
               
                return 0;
            }
            /* fifo function */
            else if (strcmp("fifo", argv[2]) == 0)
            {
                uint8_t res;
                uint32_t times, i, timeout;;
                bmp390_address_t addr_pin;
                bmp390_interface_t interface;
                
                if (strcmp("-iic", argv[4]) == 0)
                {
                    interface = BMP390_INTERFACE_IIC;
                }
                else if (strcmp("-spi", argv[4]) == 0)
                {
                    interface = BMP390_INTERFACE_SPI;
                }
                else
                {
                    bmp390_interface_debug_print("bmp390: interface is invalid.\n");
                    
                    return 5;
                }
                if (strcmp("-a", argv[5]) != 0)
                {
                    return 5;
                }
                if (strcmp("0", argv[6]) == 0)
                {
                    addr_pin = BMP390_ADDRESS_ADO_LOW;
                }
                else if (strcmp("1", argv[6]) == 0)
                {
                    addr_pin = BMP390_ADDRESS_ADO_HIGH;
                }
                else
                {
                    bmp390_interface_debug_print("bmp390: address is invalid.\n");
                    
                    return 5;
                }
                g_gpio_irq = bmp390_fifo_irq_handler;
                res = gpio_interrupt_init();
                if (res != 0)
                {
                    return 1;
                }
                res = bmp390_fifo_init(interface, addr_pin, bmp390_interface_fifo_receive_callback);
                if (res != 0)
                {
                    g_gpio_irq = NULL;
                    (void)gpio_interrupt_deinit();
                    
                    return 1;
                }
                times = atoi(argv[3]);
                gs_fifo_watermark_flag = 0;
                gs_fifo_full_flag = 0;
                timeout = 5000;
                for (i = 0; i < times; i++)
                {
                    while (timeout != 0)
                    {
                        bmp390_interface_delay_ms(100);
                        timeout--;
                        if (gs_fifo_watermark_flag != 0 || gs_fifo_full_flag != 0)
                        {
                            break;
                        }
                        if (timeout == 0)
                        {
                            g_gpio_irq = NULL;
                            (void)gpio_interrupt_deinit();
                            (void)bmp390_fifo_deinit();
                            
                            return 1;
                        }
                    }
                    gs_fifo_watermark_flag = 0;
                    gs_fifo_full_flag = 0;
                    timeout = 5000;
                }
                bmp390_interface_debug_print("bmp390: finish fifo read.\n");
                g_gpio_irq = NULL;
                (void)gpio_interrupt_deinit();
                (void)bmp390_fifo_deinit();
               
                return 0;
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    /* param is invalid */
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart1 init */
    uart1_init(115200);
    
    /* shell init && register bmp390 fuction */
    shell_init();
    shell_register("bmp390", bmp390);
    uart1_print("bmp390: welcome to libdriver bmp390.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart1_read(g_buf, 256);
        if (g_len)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart1_print("bmp390: run failed.\n");
            }
            else if (res == 2)
            {
                uart1_print("bmp390: unknow command.\n");
            }
            else if (res == 3)
            {
                uart1_print("bmp390: length is too long.\n");
            }
            else if (res == 4)
            {
                uart1_print("bmp390: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart1_print("bmp390: param is invalid.\n");
            }
            else
            {
                uart1_print("bmp390: unknow status code.\n");
            }
            uart1_flush();
        }
        delay_ms(100);
    }
}
