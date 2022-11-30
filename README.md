![License: CC-BY-SA](https://img.shields.io/badge/license-CC--BY--SA-success)

Repurpose your old nerf-or-similar toy guns that were destined for the trash heap.
Rip their guts out, and replace them with this Open Hardware, Arduino based PCB, an infrared LED, a trigger switch, and optionally some LEDs and/or speakers for feedback.

Print the PCB yourself at a commercial fabrication shop, or hack together your own using a bit of stripboard using the relatively straightforward schematics.

<img src="/docs/images/conversions/02-Blue/right.jpg?raw=true" height="200"
/> <img src="/docs/images/conversions/01-Grey/inside.jpg?raw=true" height="200"
/> <img src="/docs/images/pcbs/v0.1/Tagger-PCB-v0.1-a.jpg?raw=true" height="200"
/> <img src="/docs/images/pcbs/prototype/Tagger-Prototype-top.jpg?raw=true" height="200" />

## See also

* **[@FossTag](https://github.com/FossTag)** - for an overview of the broader project.
* **[@FossTag/TaggerFirmware](https://github.com/FossTag/TaggerFirmware)** - for the latest firmware for this PCB.

## PCB design

Currently, the PCB supports the following:
* Mounting an Arduino Pro Mini.
* Trigger button and IR LED for firing.
* RGB LED and speaker for audio/visual feedback.
* Battery input from a 9V or other appropriate battery.

<img src="/docs/images/pcbs/v0.2/pcb-v0.2.png?raw=true" height="400" />

For details of the latest and all past revisions, check out the [GitHub Releases](https://github.com/FossTag/Tagger/releases) page.
It will include a detailed description of each PCB revision, and also Kicad source files + some Gerber files for specific PCB manufacturers.

### Approx list of materials
* Old nerf gun (and some creativity)
* Arduino Pro Mini
    * 5V version if you wish to drive a piezo speaker for audio feedback
    * 3V otherwise
* 9V battery (or other power source that can connect to the Arduino Pro Mini raw voltage input pin)
* IR LED + resistor for firing
* Momentary switch for trigger
* Optional:
    * 2 x LEDs + resistors for visual feedback of firing and getting hit.
    * 1 x Piezo speaker for audio feedback of firing, getting hit, and dying.
