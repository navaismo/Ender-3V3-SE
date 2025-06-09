# Ender3 V3 SE  Community Edition Firmware for Octoprint


<div align="center">

<a href=""><img src="https://i.imgur.com/HDJQjH8_d.webp?maxwidth=1520&fidelity=grand" align="center" height="776" width="640"  ></a>

</div>

<br>



# Table of Contents

- [Background](#background)
- [Special thanks to all the contributors](#special-thanks-to-all-the-contributors)
- [Installation](#installation)
- [Common Features for the Stand Alone Printer & for Octoprint](#common-features-for-the-stand-alone-printer--for-octoprint)
  - [The 7x7-Mesh](#the-7x7-mesh)
  - [Linear Advance](#linear-advance)
  - [Hosts Commands and Support for Runnout Sensor](#hosts-commands-and-support-for-runnout-sensor)
  - [Input Shaping Menu](#input-shaping-menu)
  - [Added Extrusion Flow Item to the Tune Menu](#added-extrusion-flow-item-to-the-tune-menu)
  - [Added Mute/Unmute LCD Buzzer to the Prepare Menu](#added-muteunmute-lcd-buzzer-to-the-prepare-menu)
  - [Improved M600 feature](#improved-m600-feature)
  - [Raised Max Temperatures Values](#raised-max-temperatures-values)
  - [Fixed Print Information details for SD Print](#fixed-print-information-details-for-sd-print)
  - [Linear Advance quick Menu](#linear-advance-quick-menu)
  - [Custom Extrusion Menu](#custom-extrusion-menu)
- [Features for Octoprint](#features-for-octoprint)
  - [Configure the plugin](#configure-the-plugin)
    - [Install Plugin's Dependencies](#install-plugins-dependencies)
    - [Set Correct Baud Rate](#set-correct-baud-rate)
    - [Add GCODE SCRIPTS](#add-gcode-scripts)
    - [Configure O9000 commands](#configure-o9000-commands)
    - [Configure The GCODE Preview transmission](#configure-the-gcode-preview-transmission)
    - [Select the Based Progress Type for the Percentage](#select-the-based-progress-type-for-the-percentage)
- [FAQS](#faqs)
----------
<br><br><br>

# Background.
After Creality released the [Ender 3 V3 SE source code](https://github.com/CrealityOfficial/Ender-3V3-SE), many forks started to work on it and suddenly I was involved in this [thread on Creality's Forum](https://forum.creality.com/t/ender-3-v3-se-config-settings-to-roll-our-own-firmware-please/6333) were good folks from everywhere started a conversation about what could be good enhancements for the Firmware. So I thought will be a nice contribution for the community to merge the most common forks into One Repo and start from there new features for the firmware.

Also I wanted to go a little bit further, since I'm using [Octoprint](https://octoprint.org/), so I started to focusing on the integration of the Firmware with it, specially to make my LCD alive again.

## Special thanks to all the contributors:
* [@aschmitt1909](https://github.com/aschmitt1909/Ender-3V3-SE)
* [@queeup-Forks](https://github.com/queeup-Forks/Ender-3V3-SE)
* [@rtorchia](https://github.com/rtorchia/Ender-3V3-SE/commits/main/)
* [@eduard-sukharev](https://github.com/eduard-sukharev)
* And the folks from Creality Forum because they are making continuos tests and feature requests.

<br>

> [!CAUTION]
> 
> **Disclaimer**
>
> I'm not responsable of the damage or brick that may happen to your printer if you don't know what are you doing.
> I'm provided this fork tested on my own printer without warranties.** 


<br><br>


# Installation:
> [!TIP]
>
>First you need to flash creality firmware version 1.0.6. and the TFT files for the display.
>If your printer is already in that version you can do it directly.
>
>From: [Latest Release of Printer's Firmware without Gcode Preview](https://github.com/navaismo/Ender-3V3-SE/releases) download the ZIP that fits your Octoprint configuration, recommended 150000 baud rate.
>
> or
>From: [Latest Release of Printer's Firmware with Gcode Preview](https://github.com/navaismo/Ender-3V3-SE-Gcode-Preview/releases) download the ZIP that fits your Octoprint configuration, recommended 150000 baud rate.
>
>Unzip and:
>
> 1. Turn Off your printer.
> 2. Format you SD to FAT32 recommended to use MiniTool Partition or Gparted.
> 3. Rename the file to something random, i.E. “OC198B.bin” and copy to the SD.
> 4. Put the SD on your Printer SD-Card Reader(Not the LCD).
> 5. Turn On your printer.
> 6. Wait for the update to finish - it needs ~10-15 seconds.
> 7. Run a new Autolevel.
>
>For Octoprint:
>
> 1. Download the Latest Release plugin: navaismo/OctoPrint-E3v3seprintjobdetails
> 2. Install it manually using the Plugin Manager.
> 3. Follow the below section of Octoprint to configure it.
>

<br><br>

# Common Features for the Stand Alone Printer & for Octoprint.
## * The  7x7-Mesh:
Based on the fork of [@aschmitt1909](https://github.com/aschmitt1909/Ender-3V3-SE), and merged with the [PR#18](https://github.com/navaismo/Ender-3V3-SE/pull/18) from [@eduard-sukharev](https://github.com/eduard-sukharev) 


<div align="center">

<a href=""><img src="https://i.imgur.com/RZ7Foci.jpeg" align="center" height="576" width="340" ></a>

</div>



<br>

## * Linear Advance:
Based on the fork of [@queeup-Forks](https://github.com/queeup-Forks/Ender-3V3-SE), for this version follow these instructions to enable it:

  - https://github.com/navaismo/Ender-3V3-SE/issues/30
  - https://forum.creality.com/t/ender-3-v3-se-config-settings-to-roll-our-own-firmware-please/6333/278


<br>


## * Hosts Commands and Support for Runnout Sensor:
Based on the fork of [@rtorchia](https://github.com/rtorchia/Ender-3V3-SE/commits/main/) 

<br>


## * Raised Max Temperatures Values.
Based in the fork of [TomasekJ](https://github.com/TomasekJ)
* For **BED 110°C**
* For **Nozzle 300°C**

_Be careful with the Nozzle Temperature because the PTFE Tube from extruder will deform startint at 260°C_

<br>

## * Input Shaping Menu:
Merged the [PR#22](https://github.com/navaismo/Ender-3V3-SE/pull/22) from [@eduard-sukharev](https://github.com/eduard-sukharev) to Enable the Input Shaping Menu in the LCD.

<div align="center">

<a href=""><img src="https://i.imgur.com/Ocak0dE.jpeg" align="center" height="576" width="340" ></a>

<br>
<a href=""><img src="https://i.imgur.com/mH1uAC5.jpeg" align="center" height="576" width="340" ></a>


</div>

You can automate the configuration of your Input Shaping with the [Octoprint-Pinput_Shaping plugin for Marlin here](https://github.com/navaismo/Octoprint-Pinput_Shaping)


<br>
<div align="center">
   <img src="https://i.imgur.com/l2Y03AS.png" width="60%" height="60%"/>      
</div>
<br>

<br>

## * Added Extrusion Flow Item to the Tune Menu:
Enabled the option in Tune menu to modify the extrusion flow/rate of the printer during the print job. 


<div align="center">

<a href=""><img src="https://i.imgur.com/sWYtlSG.jpeg"  align="center" height="576" width="440" ></a>

</div>


<br>

## * Added a Display Settings subMenu to the Prepare Menu:
The submenu contains:

* Option to mute or unmute the LCD when you press the Knob. From feature request in: [Issue #27](https://github.com/navaismo/Ender-3V3-SE/issues/27). Sometimes you want to be quiet. 
To preseve the state of the feature go to Menu Control --> Save settings.

* Option to set the Maximum Brightness during current runtime. From feature request in [Issue #46](https://github.com/navaismo/Ender-3V3-SE/issues/46).

* Option to set the (Dimm)Brightness when the printer became inactive.

* Option to set the Time Before dimming the screen.

* Option to save settings in EEPROM.


<div align="center">

<a href=""><img src="https://i.imgur.com/YUpHKl2.jpeg" align="center" height="676" width="440" ></a>

</div>

<br>

## * Improved M600 feature.
When using M600 code to change filament now the head will park and raise Z to a decent distant to handle the purges. Also was increased the purge lenght to have a better extrusion clean.

> [!IMPORTANT]
>
> - [x] When using standalone printer, the Knob will control the Pause and Continue processes. If you connect the host(Octoprint) then the Knob became useless and you need to control the Pause and Continue from the Octoprint GUI until you disconnect or restart the printer.
>


<br>

## * Fixed Print Information details for SD Print :
Merged the fix of [PR#39](https://github.com/navaismo/Ender-3V3-SE/pull/39) from [@eduard-sukharev](https://github.com/eduard-sukharev) to get the print details like filament, Estimated time and layer height, when printing from SD Card.

<div align="center">

<a href=""><img src="https://i.imgur.com/Q6LLgo0.jpeg" align="center" height="576" width="440" ></a>

</div>

<br>

## * Linear Advance quick Menu:
Added a quick Linear Advance Menu under Motion Menu from feature request on [Issue#26](https://github.com/navaismo/Ender-3V3-SE/issues/36)

<div align="center">

<a href=""><img src="https://i.imgur.com/MjZFWX0.jpeg" align="center" height="576" width="940" ></a>

</div>

<br>

## * Custom Extrusion Menu:
Added a custom Extrusion Menu under Prepare Menu from feature request on [Issue#27](https://github.com/navaismo/Ender-3V3-SE/issues/27). You can Extrude based on different tempearatures and lengths to match your material.

#### Usage:
> [!TIP]
>
> 1. Set your desired temperature above 195°C, else an error will come out.
> 2. Set your desired extrusion length above 10mm, else an error will come out.
> 3. Click on proceed.
> 
> After that the Noozzle will heat till +-3 Degrees of the desired temp and will extrude the desired length automatically.
>
> Finally just click on the confirm button to go out the menu.
>


<div align="center">

<a href=""><img src="https://i.imgur.com/fY8wWJA.jpeg" align="center" height="976" width="1140" ></a>

</div>


<br>

## * Printer Statistics:
Added Submenu under Control Menu to show the Print Stats gather by Marlin. From feature request on [Issue #48](https://github.com/navaismo/Ender-3V3-SE/issues/48)

<div align="center">

<a href=""><img src="https://i.imgur.com/3irzF4E.jpeg" align="center" height="576" width="940" ></a>

</div>

<br>

## * PETG/ABS Preheat Settings:
Added options for preheat new Materials: PETG and ABS under Control and Pepare Menus. From feature request on [Issue #49](https://github.com/navaismo/Ender-3V3-SE/issues/49)

<div align="center">

<a href=""><img src="https://i.imgur.com/Q051ckp.jpeg" align="center" height="976" width="940" ></a>

</div>

<br>


## * Custom Height after Homing:
Added option to edit the height of the nozzle after homing. From feature request on [Issue #49](https://github.com/navaismo/Ender-3V3-SE/issues/80)

<div align="center">

<a href=""><img src="https://i.imgur.com/WR7rl8V.jpeg" align="center" height="620" width="440" ></a>

</div>

<br>


## * CRTouch Probe tests:
Added option to test the probe to deploay and stow inside Move Menu. From feature request on [Issue #105](https://github.com/navaismo/Ender-3V3-SE/issues/105)

<div align="center">

<a href=""><img src="https://i.imgur.com/0qEBfUl.jpeg" align="center" height="620" width="440" ></a>

</div>

<br>

----------

<br><br>

# Features for Octoprint.
To have the most of Octoprint and this firmware you will need to install the [Custom Octoprint Plugin](https://github.com/navaismo/OctoPrint-E3v3seprintjobdetails) for the printer.

<br>

## Configure the plugin:

### * Install Plugin's Dependencies:
To work correctly the plugin needs the following Plugins to be installed already:
 * [Print Time Genius](https://github.com/eyal0/OctoPrint-PrintTimeGenius)
 * [Dashboard](https://github.com/j7126/OctoPrint-Dashboard)

 Disable or Unistalled:
 * Disable Display Layer Progress

<br>

### * Set Correct Baud Rate.
Depending on the Firmware version that you've downloaded add the Baud Rate in the additional baud rates box.

_Note: If youe are using OCTO4A the only baud rate that will work is 115200 due to an Android or Octo4A limitation._

<div align="center">

<a href=""><img src="https://i.imgur.com/12DrB0x.png" align="center" height="400" width="640" ></a>

</div>


<br>

### * Add GCODE SCRIPTS.
To properly show the Ready connection string at Menu's header add the following in the
 * After Serial Connection to printer established
 * Before Serial Connection to printer is closed


<div align="center">

<a href=""><img src="https://i.imgur.com/8ovZgO3.png" align="center" height="400" width="840" ></a>

</div>

<br>

<div align="center">

<a href=""><img src="https://i.imgur.com/JsTXKEl.jpeg" align="center" height="576" width="440" ></a>

</div>

 <br>
 <br>

* For After Print Job Completes:
>
>```bash
>G1 X0 Y220
>```

 <br>



* For After Print Job is Cancelled:

_In this section is **important to add the last M117 command** it will help to clear some variables before going to Main Menu_

>
>```bash
>; relative moving
>G91
>; move head 10mm up
>G1 Z10 F800
>; absolute moving
>G90
>
>; move print head out of the way
>G1 X0 Y220 F3000
>
>; disable motors
>M84
>
>; disable all heaters
>{% snippet 'disable_hotends' %}
>M104 S0 ; Set Hotend to 0
>
>{% snippet 'disable_bed' %}
>M140 S0 ; Set Bed to 0
>
>;disable fan
>M106 S0
>
>; send message to printer.
>M117 Print was cancelled
>```


<br>

* For After Print Job is Paused:

>
>```bash
>{% if pause_position.x is not none %}
>; relative XYZE
>G91
>M83
>
>; retract filament of 0.8 mm up, move Z slightly upwards and 
>G1 Z+5 E-0.8 F4500
>
>; absolute XYZE
>M82
>G90
>
>; move to a safe rest position, adjust as necessary
>G1 X0 Y220
>{% endif %}
>
>```
>

<br>

* For After Print Job is Resumed
>
>```bash
>{% if pause_position.x is not none %}
>; relative extruder
>M83
>
>; prime nozzle
>G1 E-0.8 F4500
>G1 E0.8 F4500
>G1 E0.8 F4500
>
>; absolute E
>M82
>
>; absolute XYZ
>G90
>
>; reset E
>G92 E{{ pause_position.e }}
>
>; WARNING!!! - use M83 or M82(extruder absolute mode) according what your slicer generates
>M83 ; extruder relative mode
>
>; move back to pause position XYZ
>G1 X{{ pause_position.x }} Y{{ pause_position.y }} Z{{ pause_position.z }} F4500
>
>; reset to feed rate before pause if available
>{% if pause_position.f is not none %}G1 F{{ pause_position.f }}{% endif %}
>{% endif %}
>```

Of course you can change it to your desired behaviour above are just working examples in my setup.

<br>

### * Configure O9000 commands:
Enable the option to Turn On the Job details on the LCD. Otherwise left M117 and any other plugin will send information to it. 

_M177 has a basic implementation and I'm not planning on improve M117 messages_ 


<div align="center">

<a href=""><img src="https://i.imgur.com/M4K7023.png" align="center"  ></a>

</div>

<br>

### * Configure The GCODE Preview transmission.
If you installed the firmware from the [Repo that Enable the Gcode Preview Using Octoprint](https://github.com/navaismo/Ender-3V3-SE-Gcode-Preview.git), enble the slicer so the plugin can send it to the LCD.

> [!IMPORTANT]
>
> - [x] The supported size for the GCODE Preview is a size of 96x96.
> - [x] Supported Slicers are Cura and Orca.
> - [x] In Orca use the following **96x96/JPG**.
> - [x] In Cura default **96x96** setting is enough.
> - [x] In your slicer use a filament color other than black or the Image will be barely visible.
> - [x] **Previews will only work after enable those settings and for the new Files that you upload, any old file will not work**
> - [x] **Transmitting and Rendering the preview will take up to 2 minutes**.
>

<br>

<div align="center">

<a href=""><img src="https://i.imgur.com/f5zKGgK.png" align="center"  ></a>

</div>

<br>



### * Select the Based Progress Type for the Percentage  .
* **Time Pogress**: Will render the circle percent progress based on the estimation of the Time Printer or PTG plugin. Is a known issue that Printer or PTG arent accurate at all so this is not the best option.

* **Layer Progress**: Will render the circle progress based on the calculation of the percent bewteen the Curren Layer and the Total Layers.

* **M73 Progress**: Will render the circle progress based on the information that the Gcode file send trhough M73 command. This is the recommended one.

 _You need to enable your slicer to send this information is not by default and must follow the standar command like: **M73 P10 R60**_


<br>

<div align="center">

<a href=""><img src="https://i.imgur.com/q7oevYd.png" align="center"  ></a>

</div>

<br>

## After configuring all the above you will be able to see the New Layout of the screen:


### Octoprint GUI while transmitting the Thumbnail.
  
<div align="center">

<a href=""><img src="https://i.imgur.com/B5pbWeH.png" align="center" height="476" width="940" ></a>

</div>

<br>


### Receiving Thumbnail.

<div align="center">

<a href=""><img src="https://i.imgur.com/OG4TiIl.jpeg" align="center" height="576" width="440" ></a>

</div>

<br>

### Rendering Thumbnail.

<div align="center">

<a href=""><img src="https://i.imgur.com/XQXIhTd.jpeg" align="center" height="576" width="440"  ></a>

</div>

<br>

### Loaded Thumbnail.

<div align="center">

<a href=""><img src="https://i.imgur.com/HrwsfDM.jpeg" align="center" height="576" width="440"  ></a>

</div>

<br>




### New Tune Layout.

<br>

<div align="center">

<a href=""><img src="https://i.imgur.com/Q5CJWSB.jpeg" align="center" height="576" width="440" ></a>

</div>

<br>


<div align="center">

<a href=""><img src="https://i.imgur.com/J2r8VFr.jpeg" align="center" height="576" width="440" ></a>

</div>

<br>


#### If you disable the Thumbnail in the plugin's configuration or using the [Firmware from this REPO](https://github.com/navaismo/Ender-3V3-SE), the default Creality Image will be rendered:

<div align="center">

<a href=""><img src="https://i.imgur.com/HDJQjH8_d.webp?maxwidth=1520&fidelity=grand" align="center" height="576" width="440" ></a>

</div>

<br>


# FAQS
Most of the questions has been answered in the Issue section of the Repo but here are the most common:

```diff
+ 1. Why I don't see the updates in real time on the LCD?
```
 TL;DR: Because rendering the LCD will affect your print quality.

 Take a look on the [Issue #7](https://github.com/navaismo/OctoPrint-E3v3seprintjobdetails/issues/7) to get the full backgorund of why the Real Time response was removed.

<br>

```diff
+ 2. Do I need Octoprint to work with this?
```
No, the common feeatures works for both, you just will see the stock LCD Layout. The new layout with Gcode Preview, Layer count etc. Will be render only using Octoprint.

<br>

```diff
+ 3. Why you don't Increase the connection Baud Rate?
```
TL;DR: Because printer cannot handle communication above 150000.

Take a look on the [Issue #5](https://github.com/navaismo/Ender-3V3-SE/issues/5) to get the full backgorund.

<br>

```diff
+ 4. Got a leveling error?
```
This error is only present when using Octoprint and is expected since you flashed a new Grid with no values. So after flashing the new firmware start a new fresh level procedure and then connect Octoprint again.


<br>

```diff
+ 5. Why the Render of GCcode Preview takes so much time? 
+ 5a Why there is no Gcode Preview without Octoprint?
```
TL;DR: Because LCD is closed and we haven't found it it has a SRAM to store images. An thats why is only supported by Octoprint and not stand alone. To process the image and send it.

Take a look on [this Discussion](https://github.com/navaismo/Ender-3V3-SE/discussions/28) to see the efforts. Eduard and I spent a lot of time trying to find an Address with no luck.

<br>

```diff
+ 6. Sometimes when Thumbnail Enabled the printer pauses and has weird movement behaviour before the print starts?
```
This is because the plugin programaticaly pauses the print job until the Thumbnail is rendered, to avoid affect the print quality.

It is recommended to Load the file first and then click on Print.
If is a direct print, is recommended to preheat the filament so the transmission will start faster, else it will wait till nozzle reach the temperature.

<br>

```diff
+ 7. Sometimes when the thumb is disabled an loaded the Job in LCD I see the Default image dissapearing slowly while the other is rendering. Can you just clean the LCD?
```
This is a personal decision, I like a lot to see how the Creality Man dissapear slowly, it brings me peace.

<br>

```diff
+ 8. Sometimes in the Tune Menu appears the Nozzle and Bed Icons, Why?
```
This is beacuse the encoder/Knob is moved too fast and enter in an unknow state that by default try to render the lower info area. If you move it slowly this will no appear.



<br><br><br>
____________________________________________________________________________________________


# Marlin 3D Printer Firmware

![GitHub](https://img.shields.io/github/license/marlinfirmware/marlin.svg)
![GitHub contributors](https://img.shields.io/github/contributors/marlinfirmware/marlin.svg)
![GitHub Release Date](https://img.shields.io/github/release-date/marlinfirmware/marlin.svg)
[![Build Status](https://github.com/MarlinFirmware/Marlin/workflows/CI/badge.svg?branch=bugfix-2.0.x)](https://github.com/MarlinFirmware/Marlin/actions)

<img align="right" width=175 src="buildroot/share/pixmaps/logo/marlin-250.png" />

Additional documentation can be found at the [Marlin Home Page](https://marlinfw.org/).
Please test this firmware and let us know if it misbehaves in any way. Volunteers are standing by!

## Marlin 2.0

Marlin 2.0 takes this popular RepRap firmware to the next level by adding support for much faster 32-bit and ARM-based boards while improving support for 8-bit AVR boards. Read about Marlin's decision to use a "Hardware Abstraction Layer" below.

Download earlier versions of Marlin on the [Releases page](https://github.com/MarlinFirmware/Marlin/releases).

## Example Configurations

Before building Marlin you'll need to configure it for your specific hardware. Your vendor should have already provided source code with configurations for the installed firmware, but if you ever decide to upgrade you'll need updated configuration files. Marlin users have contributed dozens of tested example configurations to get you started. Visit the [MarlinFirmware/Configurations](https://github.com/MarlinFirmware/Configurations) repository to find the right configuration for your hardware.

## Building Marlin 2.0

To build Marlin 2.0 you'll need [Arduino IDE 1.8.8 or newer](https://www.arduino.cc/en/main/software) or [PlatformIO](http://docs.platformio.org/en/latest/ide.html#platformio-ide). Detailed build and install instructions are posted at:

  - [Installing Marlin (Arduino)](http://marlinfw.org/docs/basics/install_arduino.html)
  - [Installing Marlin (VSCode)](http://marlinfw.org/docs/basics/install_platformio_vscode.html).

### Supported Platforms

  Platform|MCU|Example Boards
  --------|---|-------
  [Arduino AVR](https://www.arduino.cc/)|ATmega|RAMPS, Melzi, RAMBo
  [Teensy++ 2.0](http://www.microchip.com/wwwproducts/en/AT90USB1286)|AT90USB1286|Printrboard
  [Arduino Due](https://www.arduino.cc/en/Guide/ArduinoDue)|SAM3X8E|RAMPS-FD, RADDS, RAMPS4DUE
  [ESP32](https://github.com/espressif/arduino-esp32)|ESP32|FYSETC E4, E4d@BOX, MRR
  [LPC1768](http://www.nxp.com/products/microcontrollers-and-processors/arm-based-processors-and-mcus/lpc-cortex-m-mcus/lpc1700-cortex-m3/512kb-flash-64kb-sram-ethernet-usb-lqfp100-package:LPC1768FBD100)|ARM® Cortex-M3|MKS SBASE, Re-ARM, Selena Compact
  [LPC1769](https://www.nxp.com/products/processors-and-microcontrollers/arm-microcontrollers/general-purpose-mcus/lpc1700-cortex-m3/512kb-flash-64kb-sram-ethernet-usb-lqfp100-package:LPC1769FBD100)|ARM® Cortex-M3|Smoothieboard, Azteeg X5 mini, TH3D EZBoard
  [STM32F103](https://www.st.com/en/microcontrollers-microprocessors/stm32f103.html)|ARM® Cortex-M3|Malyan M200, GTM32 Pro, MKS Robin, BTT SKR Mini
  [STM32F401](https://www.st.com/en/microcontrollers-microprocessors/stm32f401.html)|ARM® Cortex-M4|ARMED, Rumba32, SKR Pro, Lerdge, FYSETC S6
  [STM32F7x6](https://www.st.com/en/microcontrollers-microprocessors/stm32f7x6.html)|ARM® Cortex-M7|The Borg, RemRam V1
  [SAMD51P20A](https://www.adafruit.com/product/4064)|ARM® Cortex-M4|Adafruit Grand Central M4
  [Teensy 3.5](https://www.pjrc.com/store/teensy35.html)|ARM® Cortex-M4|
  [Teensy 3.6](https://www.pjrc.com/store/teensy36.html)|ARM® Cortex-M4|
  [Teensy 4.0](https://www.pjrc.com/store/teensy40.html)|ARM® Cortex-M7|
  [Teensy 4.1](https://www.pjrc.com/store/teensy41.html)|ARM® Cortex-M7|
  Linux Native|x86/ARM/etc.|Raspberry Pi

## Submitting Changes

- Submit **Bug Fixes** as Pull Requests to the ([bugfix-2.0.x](https://github.com/MarlinFirmware/Marlin/tree/bugfix-2.0.x)) branch.
- Follow the [Coding Standards](http://marlinfw.org/docs/development/coding_standards.html) to gain points with the maintainers.
- Please submit your questions and concerns to the [Issue Queue](https://github.com/MarlinFirmware/Marlin/issues).

## Marlin Support

The Issue Queue is reserved for Bug Reports and Feature Requests. To get help with configuration and troubleshooting, please use the following resources:

- [Marlin Documentation](http://marlinfw.org) - Official Marlin documentation
- [Marlin Discord](https://discord.gg/n5NJ59y) - Discuss issues with Marlin users and developers
- Facebook Group ["Marlin Firmware"](https://www.facebook.com/groups/1049718498464482/)
- RepRap.org [Marlin Forum](http://forums.reprap.org/list.php?415)
- [Tom's 3D Forums](https://forum.toms3d.org/)
- Facebook Group ["Marlin Firmware for 3D Printers"](https://www.facebook.com/groups/3Dtechtalk/)
- [Marlin Configuration](https://www.youtube.com/results?search_query=marlin+configuration) on YouTube

## Contributors

Marlin is constantly improving thanks to a huge number of contributors from all over the world bringing their specialties and talents. Huge thanks are due to [all the contributors](https://github.com/MarlinFirmware/Marlin/graphs/contributors) who regularly patch up bugs, help direct traffic, and basically keep Marlin from falling apart. Marlin's continued existence would not be possible without them.

## Administration

Regular users can open and close their own issues, but only the administrators can do project-related things like add labels, merge changes, set milestones, and kick trolls. The current Marlin admin team consists of:

 - Scott Lahteine [[@thinkyhead](https://github.com/thinkyhead)] - USA - Project Maintainer &nbsp; [![Donate](https://api.flattr.com/button/flattr-badge-large.png)](http://www.thinkyhead.com/donate-to-marlin)
 - Roxanne Neufeld [[@Roxy-3D](https://github.com/Roxy-3D)] - USA
 - Keith Bennett [[@thisiskeithb](https://github.com/thisiskeithb)] - USA
 - Victor Oliveira [[@rhapsodyv](https://github.com/rhapsodyv)] - Brazil
 - Chris Pepper [[@p3p](https://github.com/p3p)] - UK
 - Jason Smith [[@sjasonsmith](https://github.com/sjasonsmith)] - USA
 - Luu Lac [[@sjasonsmith](https://github.com/sjasonsmith)] - USA
 - Bob Kuhn [[@Bob-the-Kuhn](https://github.com/Bob-the-Kuhn)] - USA
 - Erik van der Zalm [[@ErikZalm](https://github.com/ErikZalm)] - Netherlands &nbsp; [![Flattr Erik](https://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=ErikZalm&url=https://github.com/MarlinFirmware/Marlin&title=Marlin&language=&tags=github&category=software)

## License

Marlin is published under the [GPL license](/LICENSE) because we believe in open development. The GPL comes with both rights and obligations. Whether you use Marlin firmware as the driver for your open or closed-source product, you must keep Marlin open, and you must provide your compatible Marlin source code to end users upon request. The most straightforward way to comply with the Marlin license is to make a fork of Marlin on Github, perform your modifications, and direct users to your modified fork.

While we can't prevent the use of this code in products (3D printers, CNC, etc.) that are closed source or crippled by a patent, we would prefer that you choose another firmware or, better yet, make your own.