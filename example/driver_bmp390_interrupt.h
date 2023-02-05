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
 * @file      driver_bmp390_interrupt.h
 * @brief     driver bmp390 interrupt header file
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

#ifndef DRIVER_BMP390_INTERRUPT_H
#define DRIVER_BMP390_INTERRUPT_H

#include "driver_bmp390_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup bmp390_example_driver
 * @{
 */

/**
 * @brief bmp390 interrupt example default definition
 */
#define BMP390_INTERRUPT_DEFAULT_SPI_WIRE                 BMP390_SPI_WIRE_4                        /**< 4 wire spi */
#define BMP390_INTERRUPT_DEFAULT_IIC_WATCHDOG_TIMER       BMP390_BOOL_FALSE                        /**< disable iic watchdog timer */
#define BMP390_INTERRUPT_DEFAULT_IIC_WATCHDOG_PERIOD      BMP390_IIC_WATCHDOG_PERIOD_1P25_MS       /**< set watchdog timer period 1.25ms */
#define BMP390_INTERRUPT_DEFAULT_INTERRUPT_PIN_TYPE       BMP390_INTERRUPT_PIN_TYPE_PUSH_PULL      /**< interrupt pin type push pull */
#define BMP390_INTERRUPT_DEFAULT_INTERRUPT_ACTIVE_LEVEL   BMP390_INTERRUPT_ACTIVE_LEVEL_HIGHER     /**< interrupt pin active level higher */
#define BMP390_INTERRUPT_DEFAULT_PRESSURE                 BMP390_BOOL_TRUE                         /**< enable pressure */
#define BMP390_INTERRUPT_DEFAULT_TEMPERATURE              BMP390_BOOL_TRUE                         /**< enable temperature */
#define BMP390_INTERRUPT_DEFAULT_PRESSURE_OVERSAMPLING    BMP390_OVERSAMPLING_x32                  /**< pressure oversampling x32 */
#define BMP390_INTERRUPT_DEFAULT_TEMPERATURE_OVERSAMPLING BMP390_OVERSAMPLING_x2                   /**< temperature oversampling x2 */
#define BMP390_INTERRUPT_DEFAULT_ODR                      BMP390_ODR_12P5_HZ                       /**< output data rate 12.5Hz */
#define BMP390_INTERRUPT_DEFAULT_FILTER_COEFFICIENT       BMP390_FILTER_COEFFICIENT_15             /**< set filter coefficient 15 */

/**
 * @brief  interrupt example irq handler
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t bmp390_interrupt_irq_handler(void);

/**
 * @brief     interrupt example init
 * @param[in] interface is the chip interface
 * @param[in] addr_pin is the iic device address
 * @param[in] *interrupt_receive_callback points to a interrupt receive callback
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t bmp390_interrupt_init(bmp390_interface_t interface, bmp390_address_t addr_pin, 
                              void (*interrupt_receive_callback)(uint8_t type));

/**
 * @brief  interrupt example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t bmp390_interrupt_deinit(void);

/**
 * @brief      interrupt example read
 * @param[out] *temperature_c points a converted temperature data buffer
 * @param[out] *pressure_pa points a converted pressure data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t bmp390_interrupt_read(float *temperature_c, float *pressure_pa);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
