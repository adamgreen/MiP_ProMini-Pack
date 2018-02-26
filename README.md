# Attribution
This project is based on the now retired [Sparkfun MiP ProMini-Pack](https://www.sparkfun.com/products/retired/13058).<br>
The original Eagle files can be found on [GitHub](https://github.com/sparkfun/MiP_ProMini-Pack). They were created by 
Sparkfun employee [Casey Kuhns](https://github.com/caseykuhns).


# Project Goals
* Run the ATMEGA328 at 16MHz instead of 8MHz. This allows the AVR to properly receive 115200 bps serial output from the
  MiP.
* Increase Vcc from 3.3V to 5V so that the ATMEGA328 can reliably run at 16MHz.
* Add AVR ISP programming header to back of the board so that a person building their own board can perform the one
  time upload of the bootloader required for subsequent Arduino programming.
* Modify board layout to make it easier to hand solder the SMD parts.


# Bill of Materials
Description | Quantity | Part Number(s) 
------------|----------|---------------
[10uF Capacitor - 1206](https://www.digikey.com/products/en?keywords=478-8235-1-ND) | 4 | C6, C7, C13, C19
[0.1uF Capacitor - 0603](https://www.adafruit.com/product/442) | 6 | C1, C2, C3, C4, C5, C10
[10kΩ Resistor - 0603](https://www.adafruit.com/product/442) | 3 | R2, R4, R5
[1kΩ Resistor - 0603](https://www.adafruit.com/product/442) | 5 | R7, R8, R10, R12, R13
[330Ω Resistor - 0603](https://www.adafruit.com/product/442) | 1 | R6
[Green LED - 0603](https://www.digikey.com/products/en?keywords=160-1446-1-ND) | 2 | D3, D4
[PTC Resettable Fuse - 1206](https://www.digikey.com/products/en?keywords=507-1802-1-ND) | 1 | F1
[SPST Tactile SMD Switch - Normally Open](https://www.digikey.com/products/en?keywords=CKN9112CT-ND) | 1 | S1
[Male Header for FTDI 1x6 - 0.1" Through-Hole](https://www.digikey.com/products/en?keywords=HDR100IMP40M-G-V-TH-ND) | 1 | JP1
[JST 4-position Connector - 1.5mm Through-Hole](https://www.digikey.com/products/en?keywords=455-1659-ND) | 1 | JP11
[ATMEGA328P - 32TQFP](https://www.digikey.com/products/en?keywords=ATMEGA328P-AURCT-ND) | 1 | U1
[MIC5205 Linear Regulator - 3.3V 150mA SOT23-5](https://www.digikey.com/products/en?keywords=576-1259-1-ND) | 1 | U4
[MIC5205 Linear Regulator - 5.0V 150mA SOT23-5](https://www.digikey.com/products/en?keywords=576-1261-1-ND) | 1 | U2
[TS3USB221A USB SWITCH DUAL - 10QFN](https://www.digikey.com/products/en?keywords=296-24019-1-ND) | 1 | U3
[16MHz Ceramic Resonator - 15pF ±0.2% SMD](https://www.digikey.com/products/en?keywords=490-1198-1-ND) | 1 | Q1
[Diode Schottky 10V 570mA SOD323](https://www.digikey.com/products/en?keywords=ZLLS410CT-ND) | 1 | D1
