# NixieClock
A IN-16/IN-8 based nixie clock





# PARTS
* 1 control pcb see `./documenation/nixie_clock_controller_gerber.zip`
* 4 Nixie Clock Adapter board for IN8 tube `./nixie_clock_in8_gerber.zip` or IN16 tube `./documenation/nixie_clock_in16_gerber.zip`
* 4 Nixie tubes IN16 or IN8
* 4 nixie tube drivers SN74141
* 1 Arduino Nano
* 1 RTC Module DS1307
* 4 20k resistors 1/2w
* 1 Nixie power supply 180v
* 3 tht push button
* 4 shift registers 75HC595
* 3 led resistors 330Ohms at 1/4w
* 3 5mm led 20ma
* 3 10k pull up resistors
* headers male and female


# PARTS FOR THE CASE
* 7 mdf wood 10*20cm @9mm/10mm
* 4 tube cover pipe holder `./documenation/3d_parts/pipe_holder_bottom.stl`
* 4 tube cover pipe covers `./documenation/3d_parts/pipe_holder_top.stl`
* 2 tube pcb mounting brackets `./documenation/3d_parts/tube_pcb_mounting_bracket.stl`
* 2 tube pcb mounting bracket covers`./documenation/3d_parts/tube_pcb_mounting_bracket_covers.stl`
* 4 pcb spacers for M3
* 4 M3 screws 10mm
* 2 M4 screws 16mm
* woodglue
* 2k glue




# PCB
Images and schematic can be found in the `./documenation` folder.

The gerberfiles to manufacture the PCB too. You also can use a breadboard or a arduino prototyping shiled to build the simple schematic.

## THE CONTROL PCB
![test image size](/documenation/pictures/ctrl_photo.png)

## NIXIE ADAPTER FOR IN-8 TUBES
![test image size](/documenation/pictures/in8_photo.png)

You can replace the resistors with wires, except for the `R115 Anode-Current-Resistor 20K`

## NIXIE ADAPTER FOR IN-16 TUBES
![test image size](/documenation/pictures/in16_photo.png)


### SOFTWARE




