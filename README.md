## Changes to [@navaismo](https://github.com/navaismo/Ender-3V3-SE) fork of [v1.0.9.7_7](https://github.com/navaismo/Ender-3V3-SE/releases/tag/v1.0.9.7_7):
- Increased the baudrate from 115200 to 128000
- Increased the prevent cold extrusion extrusion_temp from 180c to 240c
- Changed the bed mesh levelling from 5x5 to 7x7

## Changes to Creality's Firmware
- Included the Autolevel-Grid to a 7x7-Mesh based on the fork of [@aschmitt1909](https://github.com/aschmitt1909/Ender-3V3-SE)
- Included the Linear Advance functions based     on the fork of [@queeup-Forks](https://github.com/queeup-Forks/Ender-3V3-SE)
- Included the Support of Hosts Commands based    on the fork of [@rtorchia](https://github.com/rtorchia/Ender-3V3-SE/commits/main/) 
- [PR#18](https://github.com/navaismo/Ender-3V3-SE/pull/18) for Adaptative Mesh for the Bed Level Grid from [@eduard-sukharev](https://github.com/eduard-sukharev).
- [PR#22](https://github.com/navaismo/Ender-3V3-SE/pull/22) to Enable Input Shaping LCD Menu under Control Settings from [@eduard-sukharev](https://github.com/eduard-sukharev). 


> [!IMPORTANT]
>
> **My fork changes includes:**
>
>  - [x] The 7x7 Mesh to 5x5
>  - [x] Increase the Z distance when M600(change filament) is invoked to provide enough space to change and purge the filament.
>  - [x] Added M117 Support to show Messages in the LCD Screen.
>  - [x] Added the detailed page and controls of a Print Job Coming from Octoprint to be shown in the LCD.
>  - [x] Added custom comand O9000 to receive the data to render in the Print Job Page from Octo print.
>  - [x] Increased Buffers for the Serial Communication.
>  - [x] Increased the BaudRate from 115200 to 128000. 
>  - [x] Added a new Item to the TUNE Menu to control the Extrusion/Flow Rate from 0 to 200%.
>  - [x] Added @eduard-sukharev Adaptative Mesh for the Bed Level Grid.
>  - [x] Added @eduard-sukharev Input Shaping LCD Menu.


<br />

 - [x] ***This firmware needs the [OctoPrint Custom Plugin to send 09000 commands](https://github.com/navaismo/OctoPrint-E3v3seprintjobdetails) in order to render the job in the LCD.***

<br />


  ![Octorpint Print Job Detai](https://i.imgur.com/HDJQjH8_d.webp?maxwidth=1520&fidelity=grand)


<br />




<br />


  ![Octorpint Print Job Detai](https://i.imgur.com/sWYtlSG.jpeg)


<br />



  ![Octorpint Print Job Detai](https://i.imgur.com/xGjRIQl.png)


<br />



  ![Octorpint Print Job Detai](https://i.imgur.com/mOqacZJ.png)


<br />



  ![Octorpint Print Job Detai](https://i.imgur.com/HGceUAl.jpeg)



----

<br />



> [!TIP]
>
> Under Firmware folder you can find a precompiled .bin inside a zip file, which you can just flash on your printer, if youre running 1.0.6 already.
> 1. Put the firmware on your Printer SD-Card(Not the LCD), format you SD to fat32 recommended to use MiniTool Partition or Gparted
> 2. Rename it to something random, i.E. "hiPrinterPleaseFlashThisFirmware123.bin"
> 3. Plug the SD Card in your printer and turn it on
> 4. Wait for the update to finish - it needs ~10 seconds.
> 5. Run a new Autolevel.
> 6. In Octoprint enable the 128000 baud rate.



----

<br />




> [!CAUTION]
> 
> **Disclaimer**
>
> I'm not responsable of the damage or brick that may happen to your printer if you don't know what are you doing.
> I'm provided this fork tested on my own printer without warranties.** 

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
