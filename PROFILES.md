# Profiles for your keyboard

## Boot profiles :</br>
On boot, the keyboard is set with the udev file `/etc/udev/rules.d/g810-led.rules`</br>
This file launches the profile stored in `/etc/g810-led/profile`</br>
To prevent your keyboard flashing 3 times when you reboot use the systemd unit (g810-led-reboot).</br>

A profile looks like this: 
```
# G810-LED Profile (turn all keys on)

a ffffff # Set all keys on

c # Commit changes
```
More samples can be found in [/etc/g810-led/samples.](https://github.com/MatMoul/g810-led/tree/master/sample_profiles)</br>
