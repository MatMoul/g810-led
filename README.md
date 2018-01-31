# g810-led</br>

Linux led controller for Logitech G213, G410, G610, G810, G910 and GPRO Keyboards.</br>

## Compatible keyboards :</br>
- **G213 Prodigy**</br>
- **G410 Atlas Spectrum**</br>
- **G413 Carbon**</br>
- **G610 Orion Brown**</br>
- **G610 Orion Red**</br>
- **G810 Orion Spectrum**</br>
- **G910 Orion Spark**</br>
- **G910 Orion Spectrum**</br>
- **GPRO**</br>

## Contribute and evolution :</br>
* [CONTRIBUTING.md](https://github.com/MatMoul/g810-led/blob/master/CONTRIBUTING.md)

## Install :</br>
* [INSTALL.md](https://github.com/MatMoul/g810-led/blob/master/INSTALL.md)

## Help :</br>
`ledkeyboard --help`</br>
`ledkeyboard --help-keys`</br>
`ledkeyboard --help-effects`</br>
`ledkeyboard --help-samples`</br>

## Samples :</br>
`ledkeyboard -p /etc/g810/profile # Load a profile`</br>
`ledkeyboard -k logo ff0000 # Set color of a key`</br>
`ledkeyboard -a 00ff00 # Set color of all keys`</br>
`ledkeyboard -g fkeys ff00ff # Set color of a group of keys`</br>
`ledkeyboard -s color # Set keyboard power on effect`</br>
`ledkeyboard -fx color keys 00ff00 # Set fixed color effect`</br>
`ledkeyboard -fx breathing logo 00ff00 0a # Set breathing effect`</br>
`ledkeyboard -fx cycle all 0a # Set color cycle effect`</br>
`ledkeyboard -fx hwave keys 0a # Set horizontal wave effect`</br>
`ledkeyboard -fx vwave keys 0a # Set vertical wave effect`</br>
`ledkeyboard -fx cwave keys 0a # Set center wave effect`</br>

## Samples with no commit :</br>
`ledkeyboard -an 000000 # Set color of all key with no action`</br>
`ledkeyboard -gn modifiers ff0000 # Set color of a group with no action`</br>
`ledkeyboard -kn w ff0000 # Set color of a key with no action`</br>
`ledkeyboard -kn a ff0000 # Set color of a key with no action`</br>
`ledkeyboard -kn s ff0000 # Set color of a key with no action`</br>
`ledkeyboard -kn d ff0000 # Set color of a key with no action`</br>
`ledkeyboard -c # Commit all changes`</br>

## Samples for g610 :</br>
`ledkeyboard -a 60 # Set intensity of all keys`</br>
`ledkeyboard -k logo ff # Set intensity of a key`</br>
`ledkeyboard -g fkeys aa # Set intensity of a group of keys`</br>

## Samples for g213 :</br>
`ledkeyboard -a 00ff00 # Set all keys green`</br>
`ledkeyboard -r 1 ff0000 # Set region 1 red`</br>

## Samples with pipe (for effects) :</br>
`ledkeyboard -pp < profilefile # Load a profile`</br>
`echo -e "k w ff0000\nk a ff0000\nk s ff0000\nk d ff0000\nc" | ledkeyboard -pp # Set multiple keys`</br>

## Testing unsuported keyboards :</br>
Start by retrieve the VendorID and the ProductID of your keyboard with lsusb.</br>
`lsusb`</br>
Sample return :<br>
`Bus 001 Device 001: ID 046d:c331 Logitech, Inc.`</br>
In this sample VendorID is 046d and ProductID is c331. Now test your keyboard with all supported protocol :</br>
`ledkeyboard -dv 046d -dp c331 -tuk 1 -a 000000`</br>
If your keyboard set all key to off you have found the protocol (1), if not continue.</br>
`ledkeyboard -dv 046d -dp c331 -tuk 2 -a 000000`</br>
If your keyboard set all key to off you have found the protocol (2), if not continue.</br>
`ledkeyboard -dv 046d -dp c331 -tuk 3 -a 000000`</br>
If your keyboard set all key to off you have found the protocol (3), if not, need new dump.</br>

## Building and linking against the libg810-led library :</br>
Include in implementing source files.</br>
```cpp
#include "Keyboard.h"
```
To link, simply provide `-llibledkeyboard` to the build flags.</br>

To build the g810-led application as a dynamically-linked variant, run the target:</br>
`make bin-linked`</br>

## Dumps :
Dumps of keyboards are now stored in a separate project to preserve a small download size of this project.
You can find them here : [https://github.com/MatMoul/g810-led-resources](https://github.com/MatMoul/g810-led-resources)
