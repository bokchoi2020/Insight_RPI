# Insight RPI Repo

## Display

Load the small ILI9341 SPI display
```
make rund
```

To let the driver autoload (untested)
```
make autodisplay
```

## Sensor, Bluetooth

Located in the Firmware folder.

### Bluetooth
To make sure that RFCOMM SPP profile is enabled for RPI:
```
make setupbluetooth
```
Pair the phone to a bluetooth device, and add it to trusted devices.
Then run the Firmware/comm.py script.
After, run a Serial Terminal Bluetooth android app and try to send strings over.



## GUI

Needs to be determined. Perhaps PyQT5 or use of HTML5 might be suited.


## Credits

### fbtft kernel module (Framebuffer driver for ILI9341)
https://github.com/notro/fbtft

### fbcp - Framebuffer copy
https://github.com/tasanakorn/rpi-fbcp
