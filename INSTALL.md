# Installation :</br>

## Build dependencies :</br>
* git
* g++
* make

## Dependencies :</br>
* libusb
* systemd

## Installation of dependencies :</br>
ArchLinux :</br>
`sudo pacman -S git gcc make libusb`</br>
Debian :</br>
`sudo apt-get install git g++ make libusb-1.0-0-dev`</br>
OpenSUSE :</br>
`sudo zypper in git make gcc-c++ libstdc++-devel libusb-1_0-devel`</br>
CentOS :</br>
`sudo yum install git make gcc-c++ libstdc++-devel libusb-devel`</br>

## Installation :</br>
`git clone https://github.com/MatMoul/g810-led.git`</br>
`cd g810-led`</br>
`make`</br>
`sudo make install`</br>

## Update :</br>
Same as install, but your profile and reboot files are preserved.</br>

## Uninstall :</br>
`sudo make uninstall`</br>

## Boot profiles :</br>
g810-led has 2 systemd units (g810-led and g810-led-reboot).</br>
These 2 units set the keyboard profile on boot and reboot.</br>
Profiles are stored in /etc/g810 :</br>
* /etc/g810/profile
* /etc/g810/reboot
