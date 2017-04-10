# Installation :</br>

## Build dependencies :</br>
* git
* g++
* make

## Dependencies :</br>
* hidapi or libusb

## hidapi vs libusb :</br>
hidapi is a newer implementation but needs more testing.</br>
hidapi is more responsive than libusb (~20ms vs ~150ms).</br>
hidapi seem not work on CentOS, writing on hidraw is not allowed.</br>
hidapi is recommended but if you encounter problem on your system, switch to libusb.</br>


## Installation by repos :</br>
ArchLinux (aur) :</br>
`yaourt -S g810-led-git` # with yaourt</br>
`pacaur -S g810-led-git` # with pacaur</br>


## Installation of dependencies :</br>
ArchLinux :</br>
`sudo pacman -S git gcc make hidapi` # for hidapi</br>
`sudo pacman -S git gcc make libusb` # for libusb</br>
Debian :</br>
`sudo apt-get install git g++ make libhidapi-dev` # for hidapi</br>
`sudo apt-get install git g++ make libusb-1.0-0-dev` # for libusb</br>
Fedora :</br>
`sudo dnf install git make gcc-c++ hidapi-devel` # for hidapi</br>
`sudo dnf install git make gcc-c++ libusbx-devel` # for libusb</br>

## Installation :</br>
`git clone https://github.com/MatMoul/g810-led.git`</br>
`cd g810-led`</br>
`make` # for hidapi</br>
`make LIB=libusb` # for libusb</br>
`sudo make install`</br>
`make install-lib` to install the libg810-led library.</br>
`make install-dev` to install the libg810-led library and headers for development.</br>

## Building the library :</br>
The library is built by default when running the `make` (default target "all").</br>

To specifically build the library as a standalone component: </br>
`make lib`</br>

## Update :</br>
Same as install, but your profile and reboot files are preserved.</br>

## Uninstall :</br>
`sudo make uninstall`</br>

## Boot profiles :</br>
If your system use systemd, g810-led has 2 systemd units (g810-led and g810-led-reboot).</br>
These 2 units set the keyboard profile on boot and reboot.</br>
Profiles are stored in /etc/g810-led :</br>
* /etc/g810-led/profile
* /etc/g810-led/reboot

Samples can be found in /etc/g810-led/samples.</br>
