# Installation: 

## Build dependencies:
* git
* g++
* make

## Dependencies:>
* hidapi or libusb

## hidapi vs libusb:
hidapi is a newer implementation but needs more testing.</br>
hidapi is more responsive than libusb (~20ms vs ~150ms).</br>
hidapi seem not work on CentOS, writing on hidraw is not allowed.</br>
hidapi is recommended but if you encounter problem on your system, switch to libusb.</br>


## Installation by repos:
ArchLinux (aur):</br>
`yaourt -S g810-led-git` # with yaourt</br>
`pacaur -S g810-led-git` # with pacaur</br>


## Installation of dependencies:
ArchLinux:</br>
`sudo pacman -S git gcc make hidapi` # for hidapi</br>
`sudo pacman -S git gcc make libusb` # for libusb</br>
Debian :</br>
`sudo apt-get install git g++ make libhidapi-dev` # for hidapi</br>
`sudo apt-get install git g++ make libusb-1.0-0-dev` # for libusb</br>
Fedora :</br>
`sudo dnf install git make gcc-c++ hidapi-devel` # for hidapi</br>
`sudo dnf install git make gcc-c++ libusbx-devel` # for libusb</br>

## Installation:
`git clone https://github.com/MatMoul/g810-led.git`</br>
`cd g810-led`</br>
### Makefile variant:
`make bin` # for hidapi</br>
`make bin LIB=libusb` # for libusb</br>
`sudo make install`</br>
### CMake variant:
`cmake -E make_directory build
`cmake -E chdir build cmake ..`
`cmake --build build`
TODO: CMake variant for installation

## Installation of the library (For developpers):
`make lib` # for hidapi</br>
`make lib LIB=libusb` # for libusb</br>
`sudo make install-lib` to install the libg810-led library.</br>
`sudo make install-dev` to install the libg810-led library and headers for development.</br>
TODO: CMake variant for this 

## Update:
Same as install, but your profile and reboot files are preserved.</br>

## Uninstall:
`sudo make uninstall`</br>
TODO: CMake variant for this 

## Boot profiles:
On boot, the keyboard is set with the udev file /etc/udev/rules.d/g810-led.rules</br>
This file launch the profile stored in /etc/g810-led/profile</br>
To prevent your keyboard to flash 3 time when you reboot, a systemd unit (g810-led-reboot) is available for this.</br>
It launch the profile stored in /etc/g810-led/reboot</br>

Samples can be found in /etc/g810-led/samples.</br>
