
![Gopher image](/documenation/logo.png)

# A IN-16/IN-8 based nixie clock with WIFI

![test image size](/documenation/pictures/nixie_fin_prev.JPG)



# FEATURES
* burn out protection (cycle all numbers for 10 seconds every 10minutes)
* in8 or in16 tube usable via adapter pcbs
* for 4 (hours:mins) or 6 tubes (hours:mins:secs)
* easy to expand
* simple schematic using THT parts only
* tube holders with simple mount for easy replacement

## [OPTIONAL] FEATURES WIFI EXTENTION
* using esp8266 to add wifi support
* NTP SYNC
* CAPTIVE PORTAL to set wifi settings
* set timezone
* start antiburn-in cycle

# PARTS
* 1 control pcb see `./documenation/nixie_clock_controller_gerber.zip`
* 4 Nixie Clock Adapter board for IN8 tube `./nixie_clock_in8_gerber.zip` or IN16 tube `./documenation/nixie_clock_in16_gerber.zip`
* 4 Nixie tubes IN16 or IN8
* 4 nixie tube drivers SN74141
* 1 Arduino Nano
* 1 RTC Module DS1307
* 4 20k resistors 1/2w
* 1 Nixie power supply 180v
* 3 THT push button
* 2 shift registers 75HC595

* 3 5mm led 20ma
* R1 R5 R6 = 10k pull up resistors
* R2 R3 R4 = 330 (for the leds)

* headers male and female

## PARTS WIFI ADDON
(you can wire a nodemcu directly to the rx/tx of the arduino, so no need for a pcb)

* wifi addon pcb
* esp8266 12-e
* lm1117 (e.g. LM1117IMP-3.3/NOPB)
* C1;C2; C3 =  470uf
* R2, R3,R5, R6 =  10k
* R1 = 1k
* R4 = 2k
* 2x 3x1 2.54 headers + 2 jumpers


resistors used for voltage divider for the tx(arduino)->rx(esp8266)


## PARTS FOR SECOND 6-TUBE VERSION
* seconds addon pcb
* 1 75HC595
* 2 Nixie tubes IN16 or IN8
* 2 nixie tube drivers SN74141
* 2 20k resistors 1/2w


# PARTS FOR THE CASE
* 7 wood pieces 10*20cm @ >8mm
* 4 tube cover pipe holder `./documenation/3d_parts/pipe_holder_bottom.stl`
* 4 tube cover pipe covers `./documenation/3d_parts/pipe_holder_top.stl`
* 2 tube pcb mounting brackets `./documenation/3d_parts/tube_pcb_mounting_bracket.stl`
* 2 tube pcb mounting bracket covers`./documenation/3d_parts/tube_pcb_mounting_bracket_covers.stl`
* 4 PCB spacers for M3
* 4 M3 screws 10mm
* 2 M4 screws 16mm
* woodglue
* 2k glue (mounting 3d parts on glass cover)
* hotglue

# TOOLS
* drill 4mm 6mm 20mm(for tubes)
* saw
* soldering iron
* hotgluegun
* step-drill


# PCB
Images, gerber and schematic can be found in the `./documenation` folder.
You also can use a breadboard or a arduino prototyping shield to build the simple schematic.

# CASE




# DRILLING FOUR TUBE VARIANT
For the top wood plate i have made drill drawing where to drill the 20mm holes for the tubes, if you are using the 10x20cm wood plates.
The PDF can be found at `./documenation/nixie_case_drilling_plan.pdf`
The important thing is that one pair of tubes are 35mm away from each other to match with 3d printed mounting brackets.
See the pictures of the build below!


## THE CONTROL PCB
![test image size](/documenation/pictures/ctrl_photo.png)

## NIXIE ADAPTER FOR IN-8 TUBES
![test image size](/documenation/pictures/in8_photo.png)

You can replace the resistors with wires, except for the `R115 Anode-Current-Resistor 20K`.
The other resitors are for adjust the brightness of each tube segment.

### NOTE
The `SN74141` will be powered from 5V directly from the 5V rail from the Arduino. This works perfectly fine!
Ignore the `9V` marking on the PCB.


## NIXIE ADAPTER FOR IN-16 TUBES [NOT TESTED]
![test image size](/documenation/pictures/in16_photo.png)


## WIFI PCB ADDON

**PLEASE USE THE CORRECT RESISTOR VALUES LISTED IN THE PARTS SECTION**
This wifi addon pcb sits directly under the Arduino Nano, and is soldered to its header pins.
Then this sandwich is plugged into the control pcb. The Arduino-Nano is still required!

### NOTE
To program the `ESP8266`, you have to use a `USB2Serial` cable.
Solder The `RX,TX,GND` pins from the cable to `TX,RX,GND` of the `ESP8266`. 
Remove the two jumpers `EN_UART` from the board!
To reset the `ESP8266` to its flash-mode hold the `BOOT`-Button and toggle the `RESET`-Button. After that, click `Upload` in your Arduino-IDE.
**Use as sketch the `/src/wifi_extention/wifi_extention.ino` for the ESP8266**


![test image size](/documenation/pictures/wifi_pcb.png)




## SEONDS  ADDON PCB

This pcb add 2 more nixie tube outputs, if you want to have seconds.
No software modification needed. it simply adds an other shift register at the end of the chain.

![test image size](/documenation/pictures/sec_pcb.png)


### SOFTWARE
To flash the arduino you can use the Arduino IDE to flash the `./src/nixie_driver/nixie_driver.ino`
Select the Arduino-Nano board and the serial port to upload the program.

### SET TIME
After power up the clock you can use the buttons to change the time.
Select with the `OK-BUTTON` to change between minutes or hours and change them with the `+ / - Buttons`.


### SERIAL COMMANDS
The clock send out the current time string `_t_hours_mins_secs_`.

To set the clock using serial you can use the `st` time command: `_st_hours_mins_`

**Note: The `_` at the end has to be send, with the `\n` new line charater. **

# PICTURES



## SIMPLE BOX
![test image size](/documenation/pictures_build/box_complete.jpg)


## PCB TEST 1
![test image size](/documenation/pictures_build/pcb_test.jpg)

## PCB TEST 2
![test image size](/documenation/pictures_build/pcb_test_2.jpg)


## TUBE MOUNT 1
![test image size](/documenation/pictures_build/tube_mount_2.jpg)


## TUBE MOUNT 2
![test image size](/documenation/pictures_build/tube_mount.jpg)

## WIFI MODULE

![test image size](/documenation/pictures/wifi_module_mounted.jpg)



