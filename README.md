# g810-led</br>

Linux LED controller for Logitech Led Keyboards.</br>

##Compatible keyboards :</br>
- **G410 Atlas Spectrum :**</br>
- **G610 Orion (Brown and Red) :**</br>
- **G810 Orion Spectrum :**</br>
- **G910 Orion (Spark and Spectrum) :**</br>

##Install and use :</br>
- look at the wiki : https://github.com/MatMoul/g810-led/wiki</br>

##Samples :</br>
`g810-led -p /etc/g810/profile # Load a profile`</br>
`g810-led -k logo ff0000 # Set color of a key`</br>
`g810-led -a 00ff00 # Set color of all keys`</br>
`g810-led -g fkeys ff00ff # Set color of a group of keys`</br>
`g810-led -s color # Set keyboard power on effect`</br>
`g810-led -fx-color 00ff00 # Set fixed color effect`</br>
`g810-led -fx-breathing 00ff00 0a # Set breathing effect`</br>
`g810-led -fx-cycle 0a # Set color cycle effect`</br>
`g810-led -fx-hwave 0a # Set horizontal wave effect`</br>
`g810-led -fx-vwave 0a # Set vertical wave effect`</br>
`g810-led -fx-cwave 0a # Set center wave effect`</br>

##Samples with no commit :</br>
`g810-led -an 000000 # Set color of all key with no action`</br>
`g810-led -gn modifiers ff0000 # Set color of a group with no action`</br>
`g810-led -kn w ff0000 # Set color of a key with no action`</br>
`g810-led -kn a ff0000 # Set color of a key with no action`</br>
`g810-led -kn s ff0000 # Set color of a key with no action`</br>
`g810-led -kn d ff0000 # Set color of a key with no action`</br>
`g810-led -c # Commit all changes`</br>

##Samples for g610 :</br>
`g610-led -a 60 # Set intensity of all keys`</br>
`g610-led -k logo ff # Set intensity of a key`</br>
`g610-led -g fkeys aa # Set intensity of a group of keys`</br>

##Samples with pipe :</br>
`g810-led -pp < profilefile # Load a profile`</br>
`echo -e "k w ff0000\nk a ff0000\nk s ff0000\nk d ff0000\nc" | g810-led -pp # Set multiple keys`</br>
