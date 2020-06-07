**Tool under construction - Not available yet.**

The goal of this tool is to watch and record the DCC messages going thru the network where the tool is connected.
This setup includes:
  - One Arduino UNO R3
  - HX8357 shield with 3.5" TFT display and one SD card reader.

The Ardnuino is connected to the rails thru opto-coupler and rectifier to get the DCC signal and the power supply.

Dependency Graph
|-- <NmraDcc> 2.0.5
|   |-- <EEPROM> 2.0
|-- <SD>
|   |-- <SPI> 1.0
|-- <SPI> 1.0
|-- <Adafruit GFX Library> 1.8.3
|   |-- <Adafruit BusIO> 1.3.0
|   |   |-- <Wire> 1.0
|   |   |-- <SPI> 1.0
|   |-- <SPI> 1.0
|   |-- <Wire> 1.0
|-- <MCUFRIEND_kbv> 2.9.9-Beta
|   |-- <Adafruit GFX Library> 1.8.3
|   |   |-- <Adafruit BusIO> 1.3.0
|   |   |   |-- <Wire> 1.0
|   |   |   |-- <SPI> 1.0
|   |   |-- <SPI> 1.0
|   |   |-- <Wire> 1.0
|   |-- <SPI> 1.0
|-- <Wire> 1.0