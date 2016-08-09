# g810-led

Linux LED controller for the Logitech G810 Keyboard

Compatible with the Logitech G410 Keyboard (look at the wiki)

(The old python project has moved to https://github.com/MatMoul/g810-led-python/)

![jj](https://raw.githubusercontent.com/MatMoul/g810-led/master/pictures/logitech_g810-2.jpg)

Install and use :</br>
- look on the wiki : https://github.com/MatMoul/g810-led/wiki

Samples :</br>
`sudo g810-led -p /etc/g810/profile # Set a profile`</br>
`sudo g810-led -k logo ff0000 # Set color of a key`</br>
`sudo g810-led -a 00ff00 # Set color of all keys`</br>
`sudo g810-led -g fkeys ff00ff # Set color of a group of keys`</br>
`sudo g810-led -s color # Set keyboard power on effect`</br>
