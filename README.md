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
Pair the Raspberry Pi to an Android phone over Bluetooth, and add it to trusted devices.
Then run the Firmware/comm.py script.
After, run a Serial Terminal Bluetooth Android app and try to send strings over.



## GUI

You'll need to install Flask 1.0: http://flask.pocoo.org/docs/1.0/installation/

To start the server, you'll need to first start a python virtual environment.
(Instructions are also in the link above.)
With the venv started, run:

On Windows
```
$env:FLASK_APP="server.py"
flask run
```

On Linux manual
```
export FLASK_APP="server.py"
flask run
```

On Linux auto
```
make startserver
```
to start the server.
Hit up http://127.0.0.1:5000/ in a browser to see the client.

All server code is in server.py.
All client code is in templates/front.html.



## Credits

### fbtft kernel module (Framebuffer driver for ILI9341)
https://github.com/notro/fbtft

### fbcp - Framebuffer copy
https://github.com/tasanakorn/rpi-fbcp
