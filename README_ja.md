[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver BMP390

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/bmp390/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

BMP390は、非常に小型、低電力、低ノイズの24ビット絶対気圧センサーです。 デジタルの高性能センサーは、幅広い高度追跡アプリケーションに最適です。 この新しいBMP390圧力センサーは、卓越した設計の柔軟性を提供し、スマートフォン、GPSモジュール、ウェアラブル、ヒアラブル、ドローンなど、既存および今後の多数のデバイスに顧客が簡単に統合できる単一のパッケージソリューションを提供します。

LibDriver BMP390は、LibDriverによって起動されたBMP390の全機能ドライバーであり、圧力と温度の連続読み取り、圧力と温度の単一読み取り、割り込み読み取り、FIFO取得およびその他の機能を提供します。 LibDriverはMISRAに準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
    - [example fifo](#example-fifo)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver BMP390のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver BMP390用のプラットフォームに依存しないIIC, SPIバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver BMP390ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver BMP390プログラミング例が含まれています。

/ docディレクトリには、LibDriver BMP390オフラインドキュメントが含まれています。

/ datasheetディレクトリには、BMP390データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないIIC, SPIバステンプレートを参照して、指定したプラットフォームのIIC, SPIバスドライバを完成させます。

/ srcディレクトリ、/ interfaceディレクトリ、および/exampleディレクトリをプロジェクトに追加します。

### 使用

#### example basic

```C
#include "driver_bmp390_basic.h"

uint8_t res;
uint8_t i;
float temperature_c;
float pressure_pa;

res = bmp390_basic_init(BMP390_INTERFACE_IIC, BMP390_ADDRESS_ADO_LOW);
if (res != 0)
{
    return 1;
}

...

for (i = 0; i < 3; i++)
{
    bmp390_interface_delay_ms(1000);
    res = bmp390_basic_read((float *)&temperature_c, (float *)&pressure_pa);
    if (res != 0)
    {
        (void)bmp390_basic_deinit();

        return 1;
    }
    bmp390_interface_debug_print("bmp390: temperature is %0.2fC.\n", temperature_c);
    bmp390_interface_debug_print("bmp390: pressure is %0.2fPa.\n", pressure_pa);

    ...
        
}

...

(void)bmp390_basic_deinit();

return 0;
```

#### example shot

```c
#include "driver_bmp390_shot.h"

uint8_t res;
uint8_t i;
float temperature_c;
float pressure_pa;

res = bmp390_shot_init(BMP390_INTERFACE_IIC, BMP390_ADDRESS_ADO_LOW);
if (res != 0)
{
    return 1;
}

...

for (i = 0; i < 3; i++)
{
    bmp390_interface_delay_ms(1000);
    res = bmp390_shot_read((float *)&temperature_c, (float *)&pressure_pa);
    if (res != 0)
    {
        (void)bmp390_shot_deinit();

        return 1;
    }
    bmp390_interface_debug_print("bmp390: temperature is %0.2fC.\n", temperature_c);
    bmp390_interface_debug_print("bmp390: pressure is %0.2fPa.\n", pressure_pa);
    
    ...
    
}

...

(void)bmp390_shot_deinit();

return 0;
```

#### example interrupt

```c
#include "driver_bmp390_interrupt.h"

uint8_t res;
uint16_t i, timeout;
uint8_t gs_data_ready_flag;
float gs_temperature_c;
float gs_pressure_pa;

void bmp390_interrupt_receive_callback(uint8_t type)
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

res = gpio_interrupt_init();
if (res != 0)
{
    return 1;
}
res = bmp390_interrupt_init(BMP390_INTERFACE_IIC, BMP390_ADDRESS_ADO_LOW, bmp390_interrupt_receive_callback);
if (res != 0)
{
    (void)gpio_interrupt_deinit();

    return 1;
}

...

gs_data_ready_flag = 0;
timeout = 5000;
for (i = 0; i < 3; i++)
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
            (void)gpio_interrupt_deinit();
            (void)bmp390_interrupt_deinit();

            return 1;
        }
    }
    gs_data_ready_flag = 0;
    timeout = 5000;
    bmp390_interface_debug_print("bmp390: temperature is %0.2fC.\n", gs_temperature_c);
    bmp390_interface_debug_print("bmp390: pressure is %0.2fPa.\n", gs_pressure_pa);
    
    ...
    
}

...

(void)gpio_interrupt_deinit();
(void)bmp390_interrupt_deinit();

return 0;
```

#### example fifo

```c
#include "driver_bmp390_fifo.h"

uint8_t gs_fifo_full_flag;
uint8_t gs_fifo_watermark_flag;
uint16_t i, timeout;
uint8_t gs_buf[512];
bmp390_frame_t gs_frame[256];

void bmp390_fifo_receive_callback(uint8_t type)
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

res = gpio_interrupt_init();
if (res != 0)
{
    return 1;
}
res = bmp390_fifo_init(BMP390_INTERFACE_IIC, BMP390_ADDRESS_ADO_LOW, bmp390_fifo_receive_callback);
if (res != 0)
{
    (void)gpio_interrupt_deinit();

    return 1;
}

...

gs_fifo_watermark_flag = 0;
gs_fifo_full_flag = 0;
timeout = 5000;
for (i = 0; i < 3; i++)
{
    while (timeout != 0)
    {
        bmp390_interface_delay_ms(100);
        timeout--;
        if ((gs_fifo_watermark_flag != 0) || (gs_fifo_full_flag != 0))
        {
            break;
        }
        if (timeout == 0)
        {
            (void)gpio_interrupt_deinit();
            (void)bmp390_fifo_deinit();

            return 1;
        }
    }
    gs_fifo_watermark_flag = 0;
    gs_fifo_full_flag = 0;
    timeout = 5000;
    
    ...
    
}
(void)gpio_interrupt_deinit();
(void)bmp390_fifo_deinit();

...

return 0;
```

### ドキュメント

オンラインドキュメント: https://www.libdriver.com/docs/bmp390/index.html

オフラインドキュメント: /doc/html/index.html

### 貢献

お問い合わせくださいlishifenging@outlook.com

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com