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
hidapi seems to not work on CentOS, writing to hidraw is not allowed.</br>
hidapi is recommended but if you encounter a problem on your system, switch to libusb.</br>


## Installation using repos :</br>
ArchLinux (aur) :</br>
`yaourt -S g810-led-git` # with yaourt</br>
`pacaur -S g810-led-git` # with pacaur</br>
Fedora (copr) :<br/>
`sudo dnf copr enable lkiesow/g810-led` # Enable Copr repository<br/>
`sudo dnf install g810-led`<br/>
Gentoo :<br/>
`emerge app-misc/g810-led`

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
Gentoo :<br/>
`sudo emerge dev-vcs/git dev-libs/hidapi` # for hidapi<br/>
`sudo emerge dev-vcs/git dev-libs/libusb` # for libusb<br/>

## Installation :</br>
`git clone https://github.com/MatMoul/g810-led.git`</br>
`cd g810-led`</br>
`make bin` # for hidapi</br>
`make bin LIB=libusb` # for libusb</br>
`sudo make install`</br>

## Installation of the library (For developers) :</br>
`make lib` # for hidapi</br>
`make lib LIB=libusb` # for libusb</br>
`sudo make install-lib` to install the libg810-led library.</br>
`sudo make install-dev` to install the libg810-led library and headers for development.</br>

## Update :</br>
Same as install, but your profile and reboot files are preserved.</br>

## Uninstall :</br>
`sudo make uninstall`</br>

## Boot profiles :</br>
On boot, the keyboard is set with the udev file /etc/udev/rules.d/g810-led.rules</br>
This file launches the profile stored in /etc/g810-led/profile</br>
To prevent your keyboard flashing 3 times when you reboot use the systemd unit (g810-led-reboot).</br>

Samples can be found in /etc/g810-led/samples.</br>
