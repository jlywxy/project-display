# Driving parallel RGB LCD (LG Display LD050WV1-SP01) using STM32H7B0

This project is driving LCD(LG Display LD050WV1-SP01) using STM32H7B0 using STM32H7B0 peripheral via RGB interface.

Author: jlywxy (jlywxy@outlook.com)<br>
Document Version: 2.0r1
- --

## Overview of the LCD Module Interface 

### 1. Display Interface

The display and control interface satisfied with MIPI DPI(aka RGB/LTDC) and MIPI DCS.

|Signal Electrical-Level       | Speed      | Signal Wires|Power Supply
|-----------------------|------------|---------------|-|
|2.9v-TTL | 31MHz | SPI(SDA,SCL), RGB(HSYNC, VSYNC, 24 data)|2.9v|


*Note: This display will not use DE pin input.

### 2. Backlight Interface

This LCD requires 16v single power with 40mA current without internal backlight driver. 

### 3. Connector

The part number of Mating connector is AXT640124 from Panasonic, which has 40pins with 0.4mm interval.
This connector is the only one on the panel includes all control/data/backlight lines.
- --

## Intention of the project
1. Showing pattern below: color gradient ring in the middle with a picture and FPS indicator shown at corner.
![](demo3.png)
2. The performance test is refreshing LTDC peripheral Layer0 with single color gradient animation in 370*670 area. 
![](fps.png)
The refresh rate can up to 101fps with Cortex M7 ICache and DCache enabled.
- --

## Display Workflow(Steps to light up display)

0. Backlight power on.
1. LCD VDD on, Reset.
2. Initialize display with DCS via SPI.
3. Start RGB transmission.

(For DCS commands, refer to LCD specification sheet and the code of this project.)
- --

## STM32 configurations for Display Workflow

Configuration items below are described in STM32CubeMX.
1. SPI interface: `Hardware NSS Signal`, `NSSP Mode` disabled, `CPOL` High, `CPHA` 2 Edge.
2. LTDC interface: `RGB888`, `Pixel Clock Polarity` Inverted Input. Set LTDC Clock to `31 MHz(26.7~36)`. (Overclocking is available up to 52MHz tested.)

LTDC init code should be manually set with:
```
// LG Display - LD050WV1-SP01

hltdc.Init.HorizontalSync = 48;
hltdc.Init.VerticalSync = 10;
hltdc.Init.AccumulatedHBP = 72;
hltdc.Init.AccumulatedVBP = 18;
hltdc.Init.AccumulatedActiveW = 624-70;
hltdc.Init.AccumulatedActiveH = 831-10;
hltdc.Init.TotalWidth = 624;
hltdc.Init.TotalHeigh = 831;
```
LTDC configurations above has been tested to reach best performance but is not consist with specification. 

3. Caution: LTDC peripheral initialization code of STM32 should be executed AFTER SPI DCS command sent.
4. Enabling STM32 Cortex M7 DCache and ICache could significantly promote FPS. However, it could cause tear if not considering the frame buffer write time against screen refresh time.

- --

## Misc

### LCD Module Optical Characteristics

|Pixel-Arrangement      | Panel-Type | Color-Depth
|-----------------------|------------|--------------
RGB horizontal stripes | IPS        | 8bit (16.7M)


|Contrast | Color-Chromaticity | Backlight
|---------|--------------------|--------------------
|700:1    | 50% NTSC           | 500 nits

* Overpowering VDD may promote Color-Chromaticity performance.

### Knowledge Bases of Concepts

1. MIPI DPI & DCS

* The MIPI Alliance defines modern interface of mobile devices like phones, including display, cameras, etc. 
* MIPI-DPI is one of the MIPI display interface series, which is well known as RGB/Parallel/LTDC interface. This interface splits control lines(HSYNC/VSYNC/DE) with data lines(RGB parallel lines). Since it uses single-ended signals(compared to MIPI-DSI), the max speed(clock speed) could be limited, but it can transfer full pixel data in one clock period(compared to serial interfaces). The color depth is configurable as RGB565/RGB666/RGB888 and more, which could also be 'hacked' to leave out some pins or branch some lines(when downsampling color depth, throw away certain LSB; when upsampling, branch certain MSB to LSB or connect certain LSB to GND).
```
RGB888 (typical format of 16.7M color display)
-------------------------------------------------
       RRRRRRRR GGGGGGGG BBBBBBBB (3 bytes)

RGB666 (typical format of 262k color display, and 16.2M color TN panels with FRC)
-------------------------------------------------
             RR RRRRGGGG GGBBBBBB (18 bits)

RGB565
-------------------------------------------------
                RRRRRGGG GGGBBBBB (2 bytes)

RGB101010 (not available in most of the displays, typical format of 1.07B color screen)
-------------------------------------------------
RRRRRR RRRRGGGG GGGGGGBB BBBBBBBB (30 bits)
```
* DCS(Display Command Set) is used to initialize LCD control chips(maybe on TCON), which controlling commands are highly depended on LCD manufacture and its specification sheet.
The same characteristics of different LCD safisfied with MIPI DCS are: (1)share same data transfer format, write register bytes first then write data bytes. Most of them use SPI to transfer. (2) Use the same command to control 'Sleep Out'(0x11), 'Display On'(0x29).



### Document Patch and Errata

* Fixed document format problem: charts.<br>

patch above: jlywxy@2023.2.8<br>