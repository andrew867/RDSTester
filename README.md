# RDSTester
I have developed a novel proof-of-concept application by reverse engineering Windows binaries and analyzing ethernet packet captures, in order to gain insight into the software architecture of cost-optimized mass deployed point of sale hardware.

The software I have developed implements the video display protocol used by QSR Automation's video display controllers commonly used in quick service restaurant kitchens. 

Some novel insight gained into binary network protocols utilized by the cost-optimized hardware now allows further support, and end of life extension for the legacy hardware already deployed in the field. Through the development of modern cloud-based point of sale software replacements, implementations of protocols such as this will provide seamless transition to cloud-based and fully supported applications; replacing legacy software without the need for costly hardware replacement and location downtime.

## How to run
Check the Release folder, launch the executable and it will begin listening for messages. Most test routines are hard-coded to unit ID 2 at IP 192.168.11.89. This seems to match the default unit ID/IP assignments for most iQtouch installations. There is currently no support to set unit ID, serial, or IP from this software. Please use QSR's provided RSDConsole to perform these tasks. Upon request, it can be implemented pretty quickly. Windows Firewall *will* cause issues with receiving UDP packets, so please disable it before wasting hours of troubleshooting networking code!

## Supported/Tested Devices
This software was developed and tested with the QSR DE-4000 ePic video controller. It should also work with the DE-2200, DE-3000, and DE-4000. If you have access to these devices and would like to donate, it would be much appreciated!

## Compilation/Modifications
Written in Visual Studio 2022, Windows 10 SDK. Threading is most likely limiting factor to running on Linux or FreeBSD.

## Protocol Details
The video display protocol is based around sending opcodes to one or more devices on a local Ethernet network. Packets are sent as UDP, and either to local broadcast or directly to a video display unit. Since these units have been around for a very long time, they generally use DOS text mode 3 for text, colors and resolution. Fonts are stored as DOS F16 format, there is a font converter for monospaced 8x16 TTF or FON files. QSR's built in font downloader should work, as font and firmware download support is not currently implemented.

## Limitations
There is currently no support for setting ID, IP, serial number, or other device settings outside of the current video mode.
Font and firmware downloading was explored but has yet to be implemented.
Uploading bad F16 fonts may corrupt the firmware on DE4000 ARM based controllers. There is currently no known recovery, but sometimes downloading Default.F16 will correct the unit. 

## Credits
Many thanks to blackjack, your help has been invaluable!
