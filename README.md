# RPI_C
Insight is a heads-up display which provides assisted navigation and situational awareness to motorcylists. A blindspot detection system was created using ultrasound sensors with hardware interrupts (IRQs) and data filtering.  We utilized RFCOMM Bluetooth communication with a smartphone for receiving directions from Google Maps API.  A Raspberry Pi 3B was configured to process data from the blindspot system, bluetooth stack, and user display.

## Ultrasound
Uses wiringPi.h in gpioBCM mode. Remember to use wiringPiSetupGpio() instead of wiringPiSetup().

Also uses interupts with wiringPiISR. Interesting note; each pin can only have one interrupt.
Not okay to have two interrupts (one rising and one falling) on a pin.

The HC-SR04s we bought are not very reliable at high request rates. Currently using 60ms cycle delay. 

## Bluetooth
Credits to:
https://github.com/RyanGlScott/BluetoothTest/blob/master/C%20BlueZ%20Server/bluez_server.c
https://people.csail.mit.edu/albert/bluez-intro/

Establish a RFCOMM SDP Server. 
RFCOMM : Reliable data protocol 1-30 ports
SDP: Service Discovery Protocol

Works by creating a socket and binding to a port. 
Listens on the port and waits for an incoming connection.

Uses non-blocking I/O in order to use in conjuction with ultrasound.
http://beej.us/guide/bgnet/html/single/bgnet.html#blocking
http://beej.us/guide/bgnet/html/single/bgnet.html#pollman
Requires poll(), epoll(), or select().

Another method is to use multi process IPC with pipes or posix multihreading.

## JSON Parsing
Credits to:
https://github.com/Tencent/rapidjson/

Header exclusive library. Can be found in the "include" folder.
Used for parsing messages sent over bluetooth.
