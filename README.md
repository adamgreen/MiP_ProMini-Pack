# MiP ProMini Pack - 16MHz/5V
<img src="https://raw.githubusercontent.com/adamgreen/MiP_ProMini-Pack/master/images/20180416-02.jpg" alt="MiP with ProMini Pack on its back" width="240" height="320"/>

# Attribution
This project is based on the now retired [Sparkfun MiP ProMini-Pack](https://www.sparkfun.com/products/retired/13058).<br>
The original Eagle files can be found on [GitHub](https://github.com/sparkfun/MiP_ProMini-Pack). They were created by
Sparkfun employee [Casey Kuhns](https://github.com/caseykuhns).


# Project Goals
* Produce a functional version of the MiP ProMini-Pack to correct the [MiP communication issue](https://github.com/WowWeeLabs/MiP-BLE-Protocol/issues/18) in the Sparkfun original.
* Run the ATMEGA328 at 16MHz instead of 8MHz. This allows the AVR to properly receive 115200 bps serial output from the
  MiP.
* Increase Vcc from 3.3V to 5V so that the ATMEGA328 can reliably run at 16MHz.
* Modify board layout to make it easier to hand solder the SMD parts.
  * Use larger 0805 passives instead of 0603.
  * Extend the pads on some of the SMD packages to allow for easier transfer of heat from the soldering iron.
* Port my [MiP C API](https://github.com/adamgreen/MiP-Capi) project to Arduino and refactor it to make it more closely resemble other Arduino libraries.
  * This library can be found within the [Arduino/MiP_ProMini_Pack_Library](Arduino/MiP_ProMini_Pack_Library#readme) folder of this GitHub repository.


# Links
* [MiP ProMini-Pack Arduino Library](Arduino/MiP_ProMini_Pack_Library#readme): The Arduino library for this MiP ProMini-Pack.<br>
* [WowWee's Official MiP Protocol Specification](https://github.com/WowWeeLabs/MiP-BLE-Protocol): Describes the serial protocol used by the Arduino library to communicate with the MiP.<br>
* [Sparkfun's MiP ProMini-Pack Installation Tutorial](https://learn.sparkfun.com/tutorials/hacking-the-mip---promini-pack): This retired Sparkfun tutorial is still the best place to learn about opening up your MiP and installing this MiP ProMini Pack.<br>


# Bill of Materials
Description | Quantity | Part Number(s)
------------|----------|---------------
[TS3USB221A USB SWITCH DUAL - 10QFN](https://www.digikey.com/products/en?keywords=296-24019-1-ND) | 1 | U3
[ATMEGA328P - 32TQFP](https://www.digikey.com/products/en?keywords=ATMEGA328P-AURCT-ND) | 1 | U1
[16MHz Ceramic Resonator - 15pF ±0.2% SMD](https://www.digikey.com/products/en?keywords=490-1198-1-ND) | 1 | Q1
[MIC5205 Linear Regulator - 5.0V 150mA SOT23-5](https://www.digikey.com/products/en?keywords=576-1261-1-ND) | 1 | U2
[MIC5205 Linear Regulator - 3.3V 150mA SOT23-5](https://www.digikey.com/products/en?keywords=576-1259-1-ND) | 1 | U4
[10kΩ Resistor - 0805](https://www.adafruit.com/product/441) | 3 | R2, R4, R5
[1kΩ Resistor - 0805](https://www.adafruit.com/product/441) | 6 | R7, R8, R9, R10, R12, R13
[330Ω Resistor - 0805](https://www.adafruit.com/product/441) | 1 | R6
[0.1uF Capacitor - 0805](https://www.adafruit.com/product/441) | 6 | C1, C2, C3, C4, C5, C10
[10uF Capacitor - 1206](https://www.digikey.com/products/en?keywords=478-8235-1-ND) | 4 | C6, C7, C13, C19
[Diode Schottky 10V 570mA SOD323](https://www.digikey.com/products/en?keywords=ZLLS410CT-ND) | 1 | D1
[Green LED - 0603](https://www.digikey.com/products/en?keywords=160-1446-1-ND) | 2 | D3, D4
[PTC Resettable Fuse - 1206](https://www.digikey.com/products/en?keywords=507-1802-1-ND) | 1 | F1
[SPST Tactile SMD Switch - Normally Open](https://www.digikey.com/products/en?keywords=CKN9112CT-ND) | 1 | S1
[JST 4-position Connector - 1.5mm Through-Hole](https://www.digikey.com/products/en?keywords=455-1659-ND) | 1 | JP11
[Male Header for FTDI 1x6 - 0.1" Through-Hole](https://www.digikey.com/products/en?keywords=HDR100IMP40M-G-V-TH-ND) | 1 | JP1
[JST 4-wire Cable]( https://www.digikey.com/product-detail/en/jst-sales-america-inc/A04ZR04ZR28H152A/455-3026-ND/6009402) | 1 |


# Schematic
![Schematic](https://raw.githubusercontent.com/adamgreen/MiP_ProMini-Pack/master/images/20180427-Schematic.png)

# OSHPark Renderings of PCB
<img src="https://raw.githubusercontent.com/adamgreen/MiP_ProMini-Pack/master/images/20180427-PCB-Top.png" alt="PCB Top" width="320" height="492"/>
<img src="https://raw.githubusercontent.com/adamgreen/MiP_ProMini-Pack/master/images/20180427-PCB-Bottom.png" alt="PCB Top" width="320" height="492"/>
