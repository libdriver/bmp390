### 1. Chip

#### 1.1 Chip Info

chip name : STM32F407ZGT6.

extern oscillator : 8MHz.

uart pin: TX/RX PA9/PA10.

iic pin: SCL/SDA PB8/PB9.

spi pin: SCK/MISO/MOSI/CS  PA5/PA6/PA7/PA4.

gpio pin: INT PB0.

### 2. Shell

#### 2.1 Shell Parameter

baud rate: 115200.

data bits : 8.

stop bits: 1.

parity: none.

flow control: none.

### 3. BMP390

#### 3.1 Command Instruction

​          bmp390 is a basic command which can test all bmp390 driver function:

​           -i        show bmp390 chip and driver information.

​           -h       show bmp390 help.

​           -p       show bmp390 pin connections of the current board.

​           -t (reg (-iic | -spi) -a (0 | 1) | read <times> (-iic | -spi) -a (0 | 1)  | int  <times> (-iic | -spi) -a (0 | 1) | fifo  <times> (-iic | -spi) -a (0 | 1))

​           -t reg (-iic | -spi) -a (0 | 1)        run bmp390 register test. 

​           -t read <times> (-iic | -spi) -a (0 | 1)        run bmp390 read test. times means the test times. 

​           -t int <times> (-iic | -spi) -a (0 | 1)        run bmp390 interrupt test. times means the test times. 

​           -t fifo <times> (-iic | -spi) -a (0 | 1)        run bmp390 fifo test. times means the test times. 

​           -c (read <times> (-iic | -spi) -a (0 | 1) | shot <times> (-iic | -spi) -a (0 | 1) | int <times> (-iic | -spi) -a (0 | 1) | fifo <times> (-iic | -spi) -a (0 | 1) )

​           -c read <times> (-iic | -spi) -a (0 | 1)        run bmp390 read function. times means the read times. 

​           -c shot <times> (-iic | -spi) -a (0 | 1)        run bmp390 shot function. times means the read times. 

​           -c int <times> (-iic | -spi) -a (0 | 1)        run bmp390 interrupt function. times means the read times. 

​           -c fifo <times> (-iic | -spi) -a (0 | 1)        run bmp390 fifo function. times means the read times. 

#### 3.2 Command Example

```shell
bmp390 -i

bmp390: chip is Bosch BMP390.
bmp390: manufacturer is Bosch.
bmp390: interface is IIC SPI.
bmp390: driver version is 1.0.
bmp390: min supply voltage is 1.6V.
bmp390: max supply voltage is 3.6V.
bmp390: max current is 0.73mA.
bmp390: max temperature is 85.0C.
bmp390: min temperature is -40.0C.
```

```shell
bmp390 -p

bmp390: SPI interface SCK connected to GPIOA PIN5.
bmp390: SPI interface MISO connected to GPIOA PIN6.
bmp390: SPI interface MOSI connected to GPIOA PIN7.
bmp390: SPI interface CS connected to GPIOA PIN4.
bmp390: IIC interface SCL connected to GPIOB PIN8.
bmp390: IIC interface SDA connected to GPIOB PIN9.
bmp390: INT connected to GPIOB PIN0.
```

```shell
bmp390 -t reg -spi -a 0

bmp390: chip is Bosch BMP390.
bmp390: manufacturer is Bosch.
bmp390: interface is IIC SPI.
bmp390: driver version is 1.0.
bmp390: min supply voltage is 1.6V.
bmp390: max supply voltage is 3.6V.
bmp390: max current is 0.73mA.
bmp390: max temperature is 85.0C.
bmp390: min temperature is -40.0C.
bmp390: start register test.
bmp390: bmp390_set_interface/bmp390_get_interface test.
bmp390: set interface iic.
bmp390: check interface ok.
bmp390: set interface spi.
bmp390: check interface ok.
bmp390: bmp390_set_addr_pin/bmp390_get_addr_pin test.
bmp390: set addr pin low.
bmp390: check addr pin ok.
bmp390: set addr pin high.
bmp390: check addr pin ok.
bmp390: bmp390_set_fifo_watermark/bmp390_get_fifo_watermark test.
bmp390: set fifo watermark 432.
bmp390: check fifo watermark ok.
bmp390: bmp390_set_fifo/bmp390_get_fifo test.
bmp390: set fifo enable.
bmp390: check fifo ok.
bmp390: set fifo disable.
bmp390: check fifo ok.
bmp390: bmp390_set_fifo_stop_on_full/bmp390_get_fifo_stop_on_full test.
bmp390: set fifo stop on full enable.
bmp390: check fifo stop on full ok.
bmp390: set fifo stop on full disable.
bmp390: check fifo stop on full ok.
bmp390: bmp390_set_fifo_sensortime_on/bmp390_get_fifo_sensortime_on test.
bmp390: set fifo sensortime on enable.
bmp390: check fifo sensortime on ok.
bmp390: set fifo sensortime on disable.
bmp390: check fifo sensortime on ok.
bmp390: bmp390_set_fifo_pressure_on/bmp390_get_fifo_pressure_on test.
bmp390: set fifo pressure on enable.
bmp390: check fifo pressure on ok.
bmp390: set fifo pressure on disable.
bmp390: check fifo pressure on ok.
bmp390: bmp390_set_fifo_temperature_on/bmp390_get_fifo_temperature_on test.
bmp390: set fifo temperature on enable.
bmp390: check fifo temperature on ok.
bmp390: set fifo temperature on disable.
bmp390: check fifo temperature on ok.
bmp390: bmp390_set_fifo_subsampling/bmp390_get_fifo_subsampling test.
bmp390: set fifo subsampling 4.
bmp390: check fifo subsampling on ok.
bmp390: bmp390_set_fifo_data_source/bmp390_get_fifo_data_source test.
bmp390: set fifo data source unfiltered.
bmp390: check fifo data source ok.
bmp390: set fifo data source filtered.
bmp390: check fifo data source ok.
bmp390: bmp390_set_interrupt_pin_type/bmp390_get_interrupt_pin_type test.
bmp390: set interrupt pin type push pull.
bmp390: check interrupt pin type ok.
bmp390: set interrupt pin type open drain.
bmp390: check interrupt pin type ok.
bmp390: bmp390_set_interrupt_active_level/bmp390_get_interrupt_active_level test.
bmp390: set interrupt active level lower.
bmp390: check interrupt active level ok.
bmp390: set interrupt active level higher.
bmp390: check interrupt active level ok.
bmp390: bmp390_set_latch_interrupt_pin_and_interrupt_status/bmp390_get_latch_interrupt_pin_and_interrupt_status test.
bmp390: set latch interrupt pin and interrupt status enable.
bmp390: check latch interrupt pin and interrupt status ok.
bmp390: set latch interrupt pin and interrupt status disable.
bmp390: check latch interrupt pin and interrupt status ok.
bmp390: bmp390_set_interrupt_fifo_watermark/bmp390_get_interrupt_fifo_watermark test.
bmp390: set interrupt fifo watermark enable.
bmp390: check interrupt fifo watermark ok.
bmp390: set interrupt fifo watermark disable.
bmp390: check interrupt fifo watermark ok.
bmp390: bmp390_set_interrupt_fifo_full/bmp390_get_interrupt_fifo_full test.
bmp390: set interrupt fifo full enable.
bmp390: check interrupt fifo full ok.
bmp390: set interrupt fifo full disable.
bmp390: check interrupt fifo full ok.
bmp390: bmp390_set_interrupt_data_ready/bmp390_get_interrupt_fifo_data_ready test.
bmp390: set interrupt data ready enable.
bmp390: check interrupt data ready ok.
bmp390: set interrupt data ready disable.
bmp390: check interrupt data ready ok.
bmp390: bmp390_set_spi_wire/bmp390_get_spi_wire test.
bmp390: set spi 4 wire.
bmp390: check spi wire ok.
bmp390: bmp390_set_iic_watchdog_timer/bmp390_get_iic_watchdog_timer test.
bmp390: set iic watchdog timer enable.
bmp390: check iic watchdog timer ok.
bmp390: set iic watchdog timer disable.
bmp390: check iic watchdog timer ok.
bmp390: bmp390_set_iic_watchdog_period/bmp390_get_iic_watchdog_period test.
bmp390: set iic watchdog period 1.25ms.
bmp390: check iic watchdog period ok.
bmp390: set iic watchdog period 40ms.
bmp390: check iic watchdog period ok.
bmp390: bmp390_set_pressure/bmp390_get_pressure test.
bmp390: set pressure disable.
bmp390: check pressure ok.
bmp390: set pressure enable.
bmp390: check pressure ok.
bmp390: bmp390_set_temperature/bmp390_get_temperature test.
bmp390: set temperature disable.
bmp390: check temperature ok.
bmp390: set temperature enable.
bmp390: check temperature ok.
bmp390: bmp390_set_mode/bmp390_get_mode test.
bmp390: set mode sleep.
bmp390: check mode ok.
bmp390: set mode normal mode.
bmp390: check mode ok.
bmp390: set mode forced mode.
bmp390: check mode ok.
bmp390: bmp390_set_pressure_oversampling/bmp390_get_pressure_oversampling test.
bmp390: set pressure oversampling x1.
bmp390: check pressure oversampling ok.
bmp390: set pressure oversampling x2.
bmp390: check pressure oversampling ok.
bmp390: set pressure oversampling x4.
bmp390: check pressure oversampling ok.
bmp390: set pressure oversampling x8.
bmp390: check pressure oversampling ok.
bmp390: set pressure oversampling x16.
bmp390: check pressure oversampling ok.
bmp390: set pressure oversampling x32.
bmp390: check pressure oversampling ok.
bmp390: bmp390_set_temperature_oversampling/bmp390_get_temperature_oversampling test.
bmp390: set temperature oversampling x1.
bmp390: check temperature oversampling ok.
bmp390: set temperature oversampling x2.
bmp390: check temperature oversampling ok.
bmp390: set temperature oversampling x4.
bmp390: check temperature oversampling ok.
bmp390: set temperature oversampling x8.
bmp390: check temperature oversampling ok.
bmp390: set temperature oversampling x16.
bmp390: check temperature oversampling ok.
bmp390: set temperature oversampling x32.
bmp390: check temperature oversampling ok.
bmp390: bmp390_set_odr/bmp390_get_odr test.
bmp390: set odr 200Hz.
bmp390: check odr ok.
bmp390: set odr 100Hz.
bmp390: check odr ok.
bmp390: set odr 50Hz.
bmp390: check odr ok.
bmp390: set odr 25Hz.
bmp390: check odr ok.
bmp390: set odr 12.5Hz.
bmp390: check odr ok.
bmp390: set odr 6.25Hz.
bmp390: check odr ok.
bmp390: set odr 3.1Hz.
bmp390: check odr ok.
bmp390: set odr 1.5Hz.
bmp390: check odr ok.
bmp390: set odr 0.78Hz.
bmp390: check odr ok.
bmp390: set odr 0.39Hz.
bmp390: check odr ok.
bmp390: set odr 0.2Hz.
bmp390: check odr ok.
bmp390: set odr 0.1Hz.
bmp390: check odr ok.
bmp390: set odr 0.05Hz.
bmp390: check odr ok.
bmp390: set odr 0.02Hz.
bmp390: check odr ok.
bmp390: set odr 0.01Hz.
bmp390: check odr ok.
bmp390: set odr 0.006Hz.
bmp390: check odr ok.
bmp390: set odr 0.003Hz.
bmp390: check odr ok.
bmp390: set odr 0.0015Hz.
bmp390: check odr ok.
bmp390: bmp390_set_filter_coefficient/bmp390_get_filter_coefficient test.
bmp390: set filter coefficient 0.
bmp390: check filter coefficient ok.
bmp390: set filter coefficient 1.
bmp390: check filter coefficient ok.
bmp390: set filter coefficient 3.
bmp390: check filter coefficient ok.
bmp390: set filter coefficient 7.
bmp390: check filter coefficient ok.
bmp390: set filter coefficient 15.
bmp390: check filter coefficient ok.
bmp390: set filter coefficient 31.
bmp390: check filter coefficient ok.
bmp390: set filter coefficient 63.
bmp390: check filter coefficient ok.
bmp390: set filter coefficient 127.
bmp390: check filter coefficient ok.
bmp390: bmp390_get_error.
bmp390: err is 0x00.
bmp390: bmp390_get_status.
bmp390: status is 0x70.
bmp390: bmp390_get_sensortime.
bmp390: sensortime is 0x0025BF.
bmp390: bmp390_get_event.
bmp390: event is power up or softrest.
bmp390: bmp390_get_interrupt_status.
bmp390: interrupt status is 0x08.
bmp390: bmp390_get_fifo_length.
bmp390: get fifo length is 0x0000.
bmp390: bmp390_get_fifo_data.
bmp390: get fifo data is 0x0080.
bmp390: bmp390_flush_fifo.
bmp390: flush fifo ok.
bmp390: bmp390_get_revision_id.
bmp390: revision id is 0x01.
bmp390: bmp390_softreset.
bmp390: softreset ok.
bmp390: finish register test.
```

```shell
bmp390 -t read 3 -spi -a 0

bmp390: chip is Bosch BMP390.
bmp390: manufacturer is Bosch.
bmp390: interface is IIC SPI.
bmp390: driver version is 1.0.
bmp390: min supply voltage is 1.6V.
bmp390: max supply voltage is 3.6V.
bmp390: max current is 0.73mA.
bmp390: max temperature is 85.0C.
bmp390: min temperature is -40.0C.
bmp390: start read test.
bmp390: forced mode read test.
bmp390: temperature is 27.28c.
bmp390: pressure is 101049.13pa.
bmp390: temperature is 27.28c.
bmp390: pressure is 101049.16pa.
bmp390: temperature is 27.28c.
bmp390: pressure is 101049.13pa.
bmp390: normal mode read test.
bmp390: temperature is 27.36c.
bmp390: pressure is 101040.17pa.
bmp390: temperature is 27.45c.
bmp390: pressure is 101034.65pa.
bmp390: temperature is 27.49c.
bmp390: pressure is 101032.36pa.
bmp390: finish read test.
```

```shell
bmp390 -t int 3 -spi -a 0

bmp390: chip is Bosch BMP390.
bmp390: manufacturer is Bosch.
bmp390: interface is IIC SPI.
bmp390: driver version is 1.0.
bmp390: min supply voltage is 1.6V.
bmp390: max supply voltage is 3.6V.
bmp390: max current is 0.73mA.
bmp390: max temperature is 85.0C.
bmp390: min temperature is -40.0C.
bmp390: start interrupt test.
bmp390: temperature is 27.20c.
bmp390: pressure is 101046.09pa.
bmp390: temperature is 27.36c.
bmp390: pressure is 101035.02pa.
bmp390: temperature is 27.42c.
bmp390: pressure is 101031.46pa.
bmp390: finish interrupt test.
```

```shell
bmp390 -t fifo 3 -spi -a 0

bmp390: chip is Bosch BMP390.
bmp390: manufacturer is Bosch.
bmp390: interface is IIC SPI.
bmp390: driver version is 1.0.
bmp390: min supply voltage is 1.6V.
bmp390: max supply voltage is 3.6V.
bmp390: max current is 0.73mA.
bmp390: max temperature is 85.0C.
bmp390: min temperature is -40.0C.
bmp390: start fifo test.
bmp390: fifo watermark test.
bmp390: read fifo with length 265.
bmp390: read fifo with length 263.
bmp390: read fifo with length 263.
bmp390: fifo full test.
bmp390: clear fifo with length 508.
bmp390: fifo parse success and total frame is 145.
bmp390: clear fifo with length 508.
bmp390: fifo parse success and total frame is 145.
bmp390: clear fifo with length 508.
bmp390: fifo parse success and total frame is 145.
bmp390: finish fifo test.
```

```shell
bmp390 -c read 3 -spi -a 0

bmp390: 1/3.
bmp390: temperature is 26.90C.
bmp390: pressure is 101041.40Pa.
bmp390: 2/3.
bmp390: temperature is 26.99C.
bmp390: pressure is 101035.43Pa.
bmp390: 3/3.
bmp390: temperature is 27.04C.
bmp390: pressure is 101032.73Pa.
```

```shell
bmp390 -c shot 3 -spi -a 0

bmp390: 1/3.
bmp390: temperature is 26.74C.
bmp390: pressure is 101051.39Pa.
bmp390: 2/3.
bmp390: temperature is 26.74C.
bmp390: pressure is 101051.37Pa.
bmp390: 3/3.
bmp390: temperature is 26.74C.
bmp390: pressure is 101051.33Pa.
```

```shell
bmp390 -c int 3 -spi -a 0

bmp390: 1/3.
bmp390: temperature is 26.68C.
bmp390: pressure is 101049.81Pa.
bmp390: 2/3.
bmp390: temperature is 26.68C.
bmp390: pressure is 101049.34Pa.
bmp390: 3/3.
bmp390: temperature is 26.71C.
bmp390: pressure is 101047.74Pa.
```

```shell
bmp390 -c fifo 3 -spi -a 0

bmp390: fifo 1/1.
bmp390: sensortime is 74023.
bmp390: fifo 1/75.
bmp390: temperature is 26.92C.
bmp390: fifo 2/75.
bmp390: pressure is 101032.72Pa.
bmp390: fifo 3/75.
bmp390: temperature is 26.92C.
bmp390: fifo 4/75.
bmp390: pressure is 101032.54Pa.
bmp390: fifo 5/75.
bmp390: temperature is 26.92C.
bmp390: fifo 6/75.
bmp390: pressure is 101032.49Pa.
bmp390: fifo 7/75.
bmp390: temperature is 26.93C.
bmp390: fifo 8/75.
bmp390: pressure is 101032.49Pa.
bmp390: fifo 9/75.
bmp390: temperature is 26.93C.
bmp390: fifo 10/75.
bmp390: pressure is 101032.32Pa.
bmp390: fifo 11/75.
bmp390: temperature is 26.93C.
bmp390: fifo 12/75.
bmp390: pressure is 101032.18Pa.
bmp390: fifo 13/75.
bmp390: temperature is 26.93C.
bmp390: fifo 14/75.
bmp390: pressure is 101032.22Pa.
bmp390: fifo 15/75.
bmp390: temperature is 26.93C.
bmp390: fifo 16/75.
bmp390: pressure is 101032.21Pa.
bmp390: fifo 17/75.
bmp390: temperature is 26.94C.
bmp390: fifo 18/75.
bmp390: pressure is 101032.15Pa.
bmp390: fifo 19/75.
bmp390: temperature is 26.94C.
bmp390: fifo 20/75.
bmp390: pressure is 101031.99Pa.
bmp390: fifo 21/75.
bmp390: temperature is 26.94C.
bmp390: fifo 22/75.
bmp390: pressure is 101031.88Pa.
bmp390: fifo 23/75.
bmp390: temperature is 26.94C.
bmp390: fifo 24/75.
bmp390: pressure is 101031.78Pa.
bmp390: fifo 25/75.
bmp390: temperature is 26.94C.
bmp390: fifo 26/75.
bmp390: pressure is 101031.87Pa.
bmp390: fifo 27/75.
bmp390: temperature is 26.94C.
bmp390: fifo 28/75.
bmp390: pressure is 101031.78Pa.
bmp390: fifo 29/75.
bmp390: temperature is 26.95C.
bmp390: fifo 30/75.
bmp390: pressure is 101031.76Pa.
bmp390: fifo 31/75.
bmp390: temperature is 26.95C.
bmp390: fifo 32/75.
bmp390: pressure is 101031.67Pa.
bmp390: fifo 33/75.
bmp390: temperature is 26.95C.
bmp390: fifo 34/75.
bmp390: pressure is 101031.66Pa.
bmp390: fifo 35/75.
bmp390: temperature is 26.95C.
bmp390: fifo 36/75.
bmp390: pressure is 101031.56Pa.
bmp390: fifo 37/75.
bmp390: temperature is 26.95C.
bmp390: fifo 38/75.
bmp390: pressure is 101031.46Pa.
bmp390: fifo 39/75.
bmp390: temperature is 26.95C.
bmp390: fifo 40/75.
bmp390: pressure is 101031.44Pa.
bmp390: fifo 41/75.
bmp390: temperature is 26.95C.
bmp390: fifo 42/75.
bmp390: pressure is 101031.30Pa.
bmp390: fifo 43/75.
bmp390: temperature is 26.96C.
bmp390: fifo 44/75.
bmp390: pressure is 101031.29Pa.
bmp390: fifo 45/75.
bmp390: temperature is 26.96C.
bmp390: fifo 46/75.
bmp390: pressure is 101031.23Pa.
bmp390: fifo 47/75.
bmp390: temperature is 26.96C.
bmp390: fifo 48/75.
bmp390: pressure is 101031.11Pa.
bmp390: fifo 49/75.
bmp390: temperature is 26.96C.
bmp390: fifo 50/75.
bmp390: pressure is 101031.14Pa.
bmp390: fifo 51/75.
bmp390: temperature is 26.96C.
bmp390: fifo 52/75.
bmp390: pressure is 101031.13Pa.
bmp390: fifo 53/75.
bmp390: temperature is 26.96C.
bmp390: fifo 54/75.
bmp390: pressure is 101031.08Pa.
bmp390: fifo 55/75.
bmp390: temperature is 26.96C.
bmp390: fifo 56/75.
bmp390: pressure is 101031.13Pa.
bmp390: fifo 57/75.
bmp390: temperature is 26.96C.
bmp390: fifo 58/75.
bmp390: pressure is 101031.06Pa.
bmp390: fifo 59/75.
bmp390: temperature is 26.96C.
bmp390: fifo 60/75.
bmp390: pressure is 101031.06Pa.
bmp390: fifo 61/75.
bmp390: temperature is 26.97C.
bmp390: fifo 62/75.
bmp390: pressure is 101031.04Pa.
bmp390: fifo 63/75.
bmp390: temperature is 26.97C.
bmp390: fifo 64/75.
bmp390: pressure is 101031.04Pa.
bmp390: fifo 65/75.
bmp390: temperature is 26.97C.
bmp390: fifo 66/75.
bmp390: pressure is 101031.12Pa.
bmp390: fifo 67/75.
bmp390: temperature is 26.97C.
bmp390: fifo 68/75.
bmp390: pressure is 101031.13Pa.
bmp390: fifo 69/75.
bmp390: temperature is 26.97C.
bmp390: fifo 70/75.
bmp390: pressure is 101031.08Pa.
bmp390: fifo 71/75.
bmp390: temperature is 26.97C.
bmp390: fifo 72/75.
bmp390: pressure is 101031.05Pa.
bmp390: fifo 73/75.
bmp390: temperature is 26.97C.
bmp390: fifo 74/75.
bmp390: pressure is 101030.88Pa.
bmp390: fifo 75/75.
bmp390: sensortime is 149595.
bmp390: fifo 1/11.
bmp390: temperature is 26.97C.
bmp390: fifo 2/11.
bmp390: pressure is 101030.92Pa.
bmp390: fifo 3/11.
bmp390: temperature is 26.97C.
bmp390: fifo 4/11.
bmp390: pressure is 101030.93Pa.
bmp390: fifo 5/11.
bmp390: temperature is 26.97C.
bmp390: fifo 6/11.
bmp390: pressure is 101030.93Pa.
bmp390: fifo 7/11.
bmp390: temperature is 26.97C.
bmp390: fifo 8/11.
bmp390: pressure is 101030.94Pa.
bmp390: fifo 9/11.
bmp390: temperature is 26.98C.
bmp390: fifo 10/11.
bmp390: pressure is 101030.90Pa.
bmp390: fifo 11/11.
bmp390: sensortime is 161255.
bmp390: fifo 1/75.
bmp390: temperature is 26.98C.
bmp390: fifo 2/75.
bmp390: pressure is 101030.95Pa.
bmp390: fifo 3/75.
bmp390: temperature is 26.98C.
bmp390: fifo 4/75.
bmp390: pressure is 101030.89Pa.
bmp390: fifo 5/75.
bmp390: temperature is 26.98C.
bmp390: fifo 6/75.
bmp390: pressure is 101030.98Pa.
bmp390: fifo 7/75.
bmp390: temperature is 26.98C.
bmp390: fifo 8/75.
bmp390: pressure is 101031.03Pa.
bmp390: fifo 9/75.
bmp390: temperature is 26.98C.
bmp390: fifo 10/75.
bmp390: pressure is 101030.92Pa.
bmp390: fifo 11/75.
bmp390: temperature is 26.98C.
bmp390: fifo 12/75.
bmp390: pressure is 101030.88Pa.
bmp390: fifo 13/75.
bmp390: temperature is 26.98C.
bmp390: fifo 14/75.
bmp390: pressure is 101030.83Pa.
bmp390: fifo 15/75.
bmp390: temperature is 26.98C.
bmp390: fifo 16/75.
bmp390: pressure is 101030.87Pa.
bmp390: fifo 17/75.
bmp390: temperature is 26.98C.
bmp390: fifo 18/75.
bmp390: pressure is 101030.81Pa.
bmp390: fifo 19/75.
bmp390: temperature is 26.98C.
bmp390: fifo 20/75.
bmp390: pressure is 101030.77Pa.
bmp390: fifo 21/75.
bmp390: temperature is 26.98C.
bmp390: fifo 22/75.
bmp390: pressure is 101030.82Pa.
bmp390: fifo 23/75.
bmp390: temperature is 26.98C.
bmp390: fifo 24/75.
bmp390: pressure is 101030.83Pa.
bmp390: fifo 25/75.
bmp390: temperature is 26.98C.
bmp390: fifo 26/75.
bmp390: pressure is 101030.90Pa.
bmp390: fifo 27/75.
bmp390: temperature is 26.98C.
bmp390: fifo 28/75.
bmp390: pressure is 101030.98Pa.
bmp390: fifo 29/75.
bmp390: temperature is 26.98C.
bmp390: fifo 30/75.
bmp390: pressure is 101031.00Pa.
bmp390: fifo 31/75.
bmp390: temperature is 26.99C.
bmp390: fifo 32/75.
bmp390: pressure is 101030.91Pa.
bmp390: fifo 33/75.
bmp390: temperature is 26.99C.
bmp390: fifo 34/75.
bmp390: pressure is 101030.88Pa.
bmp390: fifo 35/75.
bmp390: temperature is 26.99C.
bmp390: fifo 36/75.
bmp390: pressure is 101030.83Pa.
bmp390: fifo 37/75.
bmp390: temperature is 26.99C.
bmp390: fifo 38/75.
bmp390: pressure is 101030.77Pa.
bmp390: fifo 39/75.
bmp390: temperature is 26.99C.
bmp390: fifo 40/75.
bmp390: pressure is 101030.72Pa.
bmp390: fifo 41/75.
bmp390: temperature is 26.99C.
bmp390: fifo 42/75.
bmp390: pressure is 101030.75Pa.
bmp390: fifo 43/75.
bmp390: temperature is 26.99C.
bmp390: fifo 44/75.
bmp390: pressure is 101030.86Pa.
bmp390: fifo 45/75.
bmp390: temperature is 26.99C.
bmp390: fifo 46/75.
bmp390: pressure is 101030.85Pa.
bmp390: fifo 47/75.
bmp390: temperature is 26.99C.
bmp390: fifo 48/75.
bmp390: pressure is 101030.87Pa.
bmp390: fifo 49/75.
bmp390: temperature is 26.99C.
bmp390: fifo 50/75.
bmp390: pressure is 101030.76Pa.
bmp390: fifo 51/75.
bmp390: temperature is 26.99C.
bmp390: fifo 52/75.
bmp390: pressure is 101030.79Pa.
bmp390: fifo 53/75.
bmp390: temperature is 26.99C.
bmp390: fifo 54/75.
bmp390: pressure is 101030.94Pa.
bmp390: fifo 55/75.
bmp390: temperature is 26.99C.
bmp390: fifo 56/75.
bmp390: pressure is 101030.91Pa.
bmp390: fifo 57/75.
bmp390: temperature is 26.99C.
bmp390: fifo 58/75.
bmp390: pressure is 101030.97Pa.
bmp390: fifo 59/75.
bmp390: temperature is 26.99C.
bmp390: fifo 60/75.
bmp390: pressure is 101030.96Pa.
bmp390: fifo 61/75.
bmp390: temperature is 26.99C.
bmp390: fifo 62/75.
bmp390: pressure is 101030.94Pa.
bmp390: fifo 63/75.
bmp390: temperature is 26.99C.
bmp390: fifo 64/75.
bmp390: pressure is 101030.92Pa.
bmp390: fifo 65/75.
bmp390: temperature is 26.99C.
bmp390: fifo 66/75.
bmp390: pressure is 101031.01Pa.
bmp390: fifo 67/75.
bmp390: temperature is 26.99C.
bmp390: fifo 68/75.
bmp390: pressure is 101030.92Pa.
bmp390: fifo 69/75.
bmp390: temperature is 26.99C.
bmp390: fifo 70/75.
bmp390: pressure is 101030.93Pa.
bmp390: fifo 71/75.
bmp390: temperature is 26.99C.
bmp390: fifo 72/75.
bmp390: pressure is 101030.90Pa.
bmp390: fifo 73/75.
bmp390: temperature is 26.99C.
bmp390: fifo 74/75.
bmp390: pressure is 101030.93Pa.
bmp390: fifo 75/75.
bmp390: sensortime is 235611.
bmp390: fifo 1/11.
bmp390: temperature is 26.99C.
bmp390: fifo 2/11.
bmp390: pressure is 101030.99Pa.
bmp390: fifo 3/11.
bmp390: temperature is 27.00C.
bmp390: fifo 4/11.
bmp390: pressure is 101031.00Pa.
bmp390: fifo 5/11.
bmp390: temperature is 27.00C.
bmp390: fifo 6/11.
bmp390: pressure is 101031.06Pa.
bmp390: fifo 7/11.
bmp390: temperature is 27.00C.
bmp390: fifo 8/11.
bmp390: pressure is 101030.99Pa.
bmp390: fifo 9/11.
bmp390: temperature is 27.00C.
bmp390: fifo 10/11.
bmp390: pressure is 101031.01Pa.
bmp390: fifo 11/11.
bmp390: sensortime is 247303.
bmp390: finish fifo read.
```

```shell
bmp390 -h

bmp390 -i
	show bmp390 chip and driver information.
bmp390 -h
	show bmp390 help.
bmp390 -p
	show bmp390 pin connections of the current board.
bmp390 -t reg (-iic | -spi) -a (0 | 1)
	run bmp390 register test.
bmp390 -t read <times> (-iic | -spi) -a (0 | 1)
	run bmp390 read test.times means the test times.
bmp390 -t int <times> (-iic | -spi) -a (0 | 1)
	run bmp390 interrupt test.times means the test times.
bmp390 -t fifo <times> (-iic | -spi) -a (0 | 1)
	run bmp390 fifo test.times means the test times.
bmp390 -c read <times> (-iic | -spi) -a (0 | 1)
	run bmp390 read function.times means the read times.
bmp390 -c shot <times> (-iic | -spi) -a (0 | 1)
	run bmp390 shot function.times means the read times.
bmp390 -c int <times> (-iic | -spi) -a (0 | 1)
	run bmp390 interrupt function.times means the read times.
bmp390 -c fifo <times> (-iic | -spi) -a (0 | 1)
	run bmp390 fifo function.times means the read times.
```

