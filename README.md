# Ender3 V3 SE  Community Edition Firmware OnlySD

<div align="center">
   <img src="./repo_assets/fwVersion.gif" width="18%" height="18%"/>      
</div>

<br>

# Table of Contents

- [Background](#background)
- [Special thanks to all the contributors](#special-thanks-to-all-the-contributors)
- [Installation](#installation)
- [After a year](#after-a-year)
- [Features](#features)
  - [Level Menu with Separated functions: Get Auto Z offeset, Level the bed, edit bed leveling (new)](#-level-menu-with-separated-functions-get-auto-z-offeset-level-the-bed-edit-bed-leveling-new)
  - [6x6 GRID](#-6x6-grid)
  - [Fixed Print Information details for SD Print](#-fixed-print-information-details-for-sd-print)
  - [Test Probe UI (CRTouch stow & CRTouch deploy)](#-test-probe-ui-crtouch-stow--crtouch-deploy)
  - [Custom Extrusion Menu](#-custom-extrusion-menu)
  - [Silence the buzzer](#-silence-the-buzzer)
  - [Printer Statistics](#-printer-statistics)
  - [Reset confirmation](#-reset-confirmation)
  - [Software protection against false positive reading of HotBed Termistor](#-software-protection-against-false-positive-reading-of-hotbed-termistor)
  - [Hosts Commands and Support for Runnout Sensor](#-hosts-commands-and-support-for-runnout-sensor)
  - [Raised Max Temperatures Values](#-raised-max-temperatures-values)
  - [Improved M600 feature](#-improved-m600-feature)
  - [Input Shaping via MCode M593 (removed ui menu)](#-input-shaping-via-mcode-m593-removed-ui-menu)
  - [Skew Factor via MCode M852 (removed ui menu)](#-skew-factor-via-mcode-m852-removed-ui-menu)
  - [Linear Advance available via MCode M900 or Slicer setting (removed ui menu)](#-linear-advance-available-via-mcode-m900-or-slicer-setting-removed-ui-menu)
- [Removed features](#removed-features)

----------
<br><br><br>

# Background.
After Creality released the [Ender 3 V3 SE source code](https://github.com/CrealityOfficial/Ender-3V3-SE), many forks started to work on it and suddenly I was involved in this [thread on Creality's Forum](https://forum.creality.com/t/ender-3-v3-se-config-settings-to-roll-our-own-firmware-please/6333) were good folks from everywhere started a conversation about what could be good enhancements for the Firmware. So I thought will be a nice contribution for the community to merge the most common forks into One Repo and start from there new features for the firmware.

<br><br>

## Special thanks to all the contributors:
* [@aschmitt1909](https://github.com/aschmitt1909/Ender-3V3-SE)
* [@queeup-Forks](https://github.com/queeup-Forks/Ender-3V3-SE)
* [@rtorchia](https://github.com/rtorchia/Ender-3V3-SE/commits/main/)
* [@eduard-sukharev](https://github.com/eduard-sukharev)
* And the folks from Creality Forum because they are making continuos tests and feature requests.

<br><br>

# Installation:
> [!TIP]
>
>First you need to flash or have at least(or superior) the creality firmware version 1.0.6. and the TFT files for the display.
>If your printer is already in that version(or sperior) you can do it directly.
>
>From: [Latest Release of Printer's Firmware for SD](https://github.com/navaismo/Ender-3V3-SE) download the ZIP file.
>
>Unzip and:
>
> 1. Turn Off your printer.
> 2. Format you SD to FAT32 recommended to use MiniTool Partition or Gparted.
> 3. Rename the file to something random, i.E. “nashav110.bin” and copy to the SD.
> 4. Put the SD on your Printer SD-Card Reader(Not the LCD).
> 5. Turn On your printer.
> 6. Wait for the update to finish - it needs ~10-15 seconds.
> 7. Reset your printer. 
> 8. Run a new Autolevel.
>

<br><br>


> [!CAUTION]
> 
> **Disclaimer**
>
> I'm not responsable of the damage or brick that may happen to your printer if you don't know what are you doing.
> I'm provided this fork tested on my own printer without warranties.** 


<br>

# After a year...
Of people's feedback, testing, many features and some improvements seems like the **C13 hardware cannot handle all** the goodies that [@eduard-sukharev](https://github.com/eduard-sukharev) and I tried to implement on it.
So after many bug reports we decided to make this repo as the firmware of the modify stock firmware for ONLY SD support, keepig the repo [GCode Preview](https://github.com/navaismo/Ender-3V3-SE-Gcode-Preview) as the main one firmware to use with Octoprint.

<br>

> [!CAUTION]
> 
> **IMPORTANT**
>
> I'm dropping the development of the firmware since we reached the memory capabilities of the printer, this is the last release for the hardware version C13, the branches and files are going to still be available,
> but no more development, bug reports or features requests are going to be taken.
> 
> I'm migrating the work to the new Marlin upstream in the repo: [Marlin 2.1 for Ender 3 V3 SE](https://github.com/navaismo/Marlin_bugfix_2.1_E3V3SE) which is under progress. 

<br>

So this is what we have now:

# Features:
 
## * Level Menu with Separated functions: Get Auto Z offeset, Level the bed, edit bed leveling *(new)*.
You spoke and we've heard, we have splitted the LEVEL Menu.


<div align="center">
   <img src="./repo_assets/LevelMenu.gif" width="20%" height="20%"/>      
</div>

<br>


## * 6x6 GRID
Based on the fork of [@aschmitt1909](https://github.com/aschmitt1909/Ender-3V3-SE), and merged with the [PR#18](https://github.com/navaismo/Ender-3V3-SE/pull/18) from [@eduard-sukharev](https://github.com/eduard-sukharev)

<div align="center">
   <img src="./repo_assets/Grid6x6.gif" width="20%" height="20%"/>      
</div>

<br>


## * Fixed Print Information details for SD Print :
Merged the fix of [PR#39](https://github.com/navaismo/Ender-3V3-SE/pull/39) from [@eduard-sukharev](https://github.com/eduard-sukharev) to get the print details like filament, Estimated time and layer height, when printing from SD Card for CURA and Orca Slicer Files.


<div align="center">
   <img src="./repo_assets/GcodeInfo.gif" width="20%" height="20%"/>      
</div>

<br>


## * Test Probe UI (CRTouch stow & CRTouch deploy).

<div align="center">
   <img src="./repo_assets/Probe.gif" width="20%" height="20%"/>      
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
   <img src="./repo_assets/cextrude.gif" width="20%" height="20%"/>      
</div>


<br>


## * Silence the buzzer.
* Option to mute or unmute the LCD when you press the Knob. From feature request in: [Issue #27](https://github.com/navaismo/Ender-3V3-SE/issues/27). Sometimes you want to be quiet. 
To preseve the state of the feature go to Menu Control --> Save settings.


<div align="center">
   <img src="./repo_assets/buzzer.gif" width="20%" height="20%"/>      
</div>


<br>


## * Printer Statistics:
Added Submenu under Control Menu to show the Print Stats gather by Marlin. From feature request on [Issue #48](https://github.com/navaismo/Ender-3V3-SE/issues/48)

<div align="center">
   <img src="./repo_assets/statistics.gif" width="20%" height="20%"/>      
</div>


<br>


## * Reset confirmation.
To avoid undesired resets we added a confirmation menu to ensure not to loose our settings by mistake.


<div align="center">
   <img src="./repo_assets/Reset.gif" width="20%" height="20%"/>      
</div>


<br>

## * Software protection against false positive reading of HotBed Termistor.
Starting last month(2025-06-01) my printer sometimes had a bad reading on the bed termistor causing crash on prints with several hours running, and these readings were randomly and during a short period but the printer still cancelled a job.
So beacuse of that I added in the Firmware a protection for small readings to ignore it unless we have confirmation and get the same temp several times.

<div align="center">

![BedTooLow](./repo_assets/BedtooLow.png)
</div>



## * Hosts Commands and Support for Runnout Sensor:
Based on the fork of [@rtorchia](https://github.com/rtorchia/Ender-3V3-SE/commits/main/) 
If you need to connec through Serial Port use the 115200 baudrate.
<br>


## * Raised Max Temperatures Values.
Based in the fork of [TomasekJ](https://github.com/TomasekJ)
* For **BED 110°C**
* For **Nozzle 300°C**

_Be careful with the Nozzle Temperature because the PTFE Tube from extruder will deform startint at 260°C_

<br>


## * Improved M600 feature.
When using M600 code to change filament now the head will park and raise Z to a decent distant to handle the purges. Also was increased the purge lenght to have a better extrusion clean.

<br>

## * Input Shaping via MCode M593 *(removed ui menu*).
Due to the memory issues we kept the feature available by usig the Mcode.

## * Skew Factor via MCode M852 *(removed ui menu)*.
Due to the memory issues we kept the feature available by usig the Mcode.

## * Linear Advance available via MCode M900 or Slicer setting *(removed ui menu)*.
Due to the memory issues we kept the feature available by usig the Mcode.
 


# Removed features
 * Bed level visualizer.
 * QR help.
 * Advanced Help messages (Evaluation of the GRID)
 * Custom probe height after homing.
 * Extrusion Menu during Tune.
 * Preheat Alert.
 * M112 support removed.
 * Creality G212 code removed.


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