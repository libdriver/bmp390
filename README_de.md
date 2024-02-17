[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver BMP390

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/bmp390/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der BMP390 ist ein sehr kleiner, stromsparender und rauscharmer absoluter barometrischer 24-Bit-Drucksensor. Der digitale Hochleistungssensor eignet sich ideal für eine Vielzahl von Höhentracking-Anwendungen. Dieser neue BMP390-Drucksensor bietet eine herausragende Designflexibilität und bietet eine Einzelpaketlösung, die Kunden problemlos in eine Vielzahl bestehender und zukünftiger Geräte wie Smartphones, GPS-Module, Wearables, Hearables und Drohnen integrieren können.

LibDriver BMP390 ist ein voll funktionsfähiger Treiber von BMP390, der von LibDriver gestartet wurde. Er bietet kontinuierliches Lesen von Druck und Temperatur, einzelnes Lesen von Druck und Temperatur, Interrupt-Lesen, FIFO-Erfassung und andere Funktionen. LibDriver ist MISRA-konform.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
    - [example fifo](#example-fifo)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver BMP390-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver BMP390 IIC, SPI.

/test enthält den Testcode des LibDriver BMP390-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver BMP390-Beispielcode.

/doc enthält das LibDriver BMP390-Offlinedokument.

/Datenblatt enthält BMP390-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige IIC, SPI-Schnittstellenvorlage und stellen Sie Ihren Plattform-IIC, SPI-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

```C
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

```C
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

```C
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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/bmp390/index.html](https://www.libdriver.com/docs/bmp390/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.