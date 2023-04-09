# Driving parallel RGB LCD (LG Display LD050WV1-SP01) using CPLD

This project is driving LCD(LG Display LD050WV1-SP01) using Altera MAX II EPM240T100C5N CPLD and additional components.

```
CPLD   |  
       |  < (parallel RGB)
       v
    LG Display LD050WV1-SP01
       ^                
       |  < (SPI)
STM32  |  
```

Author: jlywxy (jlywxy@outlook.com)<br>
Document Version: 2.0r1
- --

## Intention of the project
Display RGB color gradient stripes "horizontally".

<img src="demo.png" width="250" ></img>

- --
## Verilog Concepts for RGB Interface

1. VSYNC & HSYNC timing control
* `VSYNC` should be triggered at `HSYNC` negtive edge, not while `HSYNC`==`HTOTAL`.
```verilog
always @(negedge hsync) begin
	 if(v<=VTOTAL-1)begin
	 v<=v+1'd1;
	 end
	 else
	 v<=11'd0;
end
```
* `HSYNC` should be triggered at `PCLK` NEGTIVE edge.
* 'Negedge' information is related to configuration in LCM specification sheet, which varies from different LCM.
2. This project has tested sync configurations below to get best performance(no tearing, no blanking edges) of the LCM.

```verilog
parameter HSYNC = 24,
    HACT=480,
    HBP=36,
    HFP=0,
    HTOTAL=600;

parameter VSYNC = 8,
    VACT=800,
    VBP=16,
    VFP=0,
    VTOTAL=828;
```

3. LCM RGB PCLK overclocking is ok. Therefore CPLD could use 50MHz input clock without PLL to generate RGB signals.

- --
## Overview of the LCD Module 
For LCM interface information, optical information, display workflow(steps to light up), and basic concepts, refer to the document of a similar project rgb-lcd-lg-stm32(https://github.com/jlywxy/rgb-lcd-lg-stm32).

### Document Patch and Errata

* Fixed document format problem: charts.<br>

patch above: jlywxy@2023.2.8<br>