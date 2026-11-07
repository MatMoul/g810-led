# g810-led</br>

Linux led controller for Logitech G213, G410, G413, G512, G513, G610, G810, G815, G910 and GPRO Keyboards.</br>

## Compatible keyboards :</br>
- **G213 Prodigy**</br>
- **G410 Atlas Spectrum**</br>
- **G413 Carbon**</br>
- **G512 Carbon**</br>
- **G513 Carbon**</br>
- **G515 TKL** (wired USB, `046d:c358`; static lighting)</br>
- **G610 Orion Brown**</br>
- **G610 Orion Red**</br>
- **G810 Orion Spectrum**</br>
- **G815 LIGHTSYNC**</br>
- **G910 Orion Spark**</br>
- **G910 Orion Spectrum**</br>
- **GPRO**</br>

## Contribute and evolution :</br>
* [CONTRIBUTING.md](https://github.com/MatMoul/g810-led/blob/master/CONTRIBUTING.md)

## Install :</br>
* [INSTALL.md](https://github.com/MatMoul/g810-led/blob/master/INSTALL.md)

## Profiles :<br>
You can load predefined configurations on startup! 
* [PROFILES.md](https://github.com/MatMoul/g810-led/blob/master/PROFILES.md)

## Help :</br>
`g213-led --help`</br>
`g410-led --help`</br>
`g413-led --help`</br>
`g512-led --help`</br>
`g513-led --help`</br>
`g610-led --help`</br>
`g810-led --help`</br>
`g815-led --help`</br>
`g910-led --help`</br>
`gpro-led --help`</br>

`g810-led --help-keys`</br>
`g810-led --help-effects`</br>
`g810-led --help-samples`</br>

## Samples :</br>
`g810-led -p /etc/g810-led/profile # Load a profile`</br>
`g810-led -k logo ff0000 # Set color of a key`</br>
`g810-led -a 00ff00 # Set color of all keys`</br>
`g810-led -g fkeys ff00ff # Set color of a group of keys`</br>
`g810-led -s color # Set keyboard power on effect`</br>
`g810-led -fx color keys 00ff00 # Set fixed color effect`</br>
`g810-led -fx breathing logo 00ff00 0a # Set breathing effect`</br>
`g810-led -fx cycle all 0a # Set color cycle effect`</br>
`g810-led -fx hwave keys 0a # Set horizontal wave effect`</br>
`g810-led -fx vwave keys 0a # Set vertical wave effect`</br>
`g810-led -fx cwave keys 0a # Set center wave effect`</br>

## Samples with no commit :</br>
`g810-led -an 000000 # Set color of all key with no action`</br>
`g810-led -gn modifiers ff0000 # Set color of a group with no action`</br>
`g810-led -kn w ff0000 # Set color of a key with no action`</br>
`g810-led -kn a ff0000 # Set color of a key with no action`</br>
`g810-led -kn s ff0000 # Set color of a key with no action`</br>
`g810-led -kn d ff0000 # Set color of a key with no action`</br>
`g810-led -c # Commit all changes`</br>

## Samples for G610 :</br>
`g610-led -a 60 # Set intensity of all keys`</br>
`g610-led -k logo ff # Set intensity of a key`</br>
`g610-led -g fkeys aa # Set intensity of a group of keys`</br>

## Samples for G213 :</br>
`g213-led -a 00ff00 # Set all keys green`</br>
`g213-led -r 1 ff0000 # Set region 1 red`</br>

## Samples with pipe (for effects) :</br>
`g810-led -pp < profilefile # Load a profile`</br>
`echo -e "k w ff0000\nk a ff0000\nk s ff0000\nk d ff0000\nc" | g810-led -pp # Set multiple keys`</br>

## Testing unsupported keyboards :</br>
Start by retrieving the VendorID and the ProductID of your keyboard using lsusb.</br>
`lsusb`</br>
Sample return :<br>
`Bus 001 Device 001: ID 046d:c331 Logitech, Inc.`</br>
In this sample VendorID is 046d and ProductID is c331. Now test your keyboard with all supported protocol (for 2019 keyboard start with -tuk 4):</br>
`g810-led -dv 046d -dp c331 -tuk 1 -a 000000`</br>
If your keyboard set all key to off you have found the protocol (1), if not continue.</br>
`g810-led -dv 046d -dp c331 -tuk 2 -a 000000`</br>
If your keyboard set all key to off you have found the protocol (2), if not continue.</br>
`g810-led -dv 046d -dp c331 -tuk 3 -a 000000`</br>
If your keyboard set all key to off you have found the protocol (2), if not continue.</br>
`g810-led -dv 046d -dp c331 -tuk 4 -a 000000`</br>
If your keyboard set all key to off you have found the protocol (3), if not, need new dump.</br>

## Building and linking against the libg810-led library :</br>
Include in implementing source files.</br>
```cpp
#include <g810-led/Keyboard.h>
```
To link, simply provide `-lg810-led` to the build flags.</br>

To build the g810-led application as a dynamically-linked variant, run the target:</br>
`make bin-linked`</br>

## Dumps :
Dumps of keyboards are now stored in a separate project to preserve a small download size of this project.
You can find them here : [https://github.com/MatMoul/g810-led-resources](https://github.com/MatMoul/g810-led-resources)

## G515 TKL (wired USB)

The G515 TKL with USB ID `046d:c358` supports static colours, individual keys,
ordinary key groups and text profiles. It uses the HID++ vendor interface
(interface 2), with feature discovery for RGB Effects (`0x8071`) and Per-Key
Lighting V2 (`0x8081`). Its Linux hidraw node is located automatically; the
G515 transport uses hidraw directly with either build backend.

```sh
sudo g810-led -dv 046d -dp c358 -a ff0000
sudo g810-led -dv 046d -dp c358 -k w 00ff00
sudo g810-led -dv 046d -dp c358 -g arrows 00ffff
sudo g810-led -dv 046d -dp c358 --on-board-mode board
```

The last command releases software lighting control back to the keyboard.
Closing the program otherwise leaves software lighting active. These commands
do not save an onboard profile. Native animated effects, onboard profile
storage, logo/indicator/media/G-key groups, wireless receivers and other G515
product IDs are not supported by this implementation. Use root or an existing
appropriate hidraw access rule; this change does not add a G515 udev rule.

The direct USB implementation was tested on a G515 TKL: setting all keys red,
setting W green, and returning to keyboard-controlled lighting. The protocol
regression test can be run without hardware using `make test-g515 LIB=libusb`
(or the default hidapi build).
