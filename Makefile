initdisplay:
	sudo modprobe fbtft_device custom name=fb_ili9341 gpios=reset:25,dc:24,led:18 speed=90000000 rotate=90 bgr=1

startdisplay:
	sleep 2
	sudo FRAMEBUFFER=/dev/fb1 startx

copydisplay:
	sleep 2
	Firmware/fbcp&

setupbluetooth:
	sudo sed -i '/^ExecStart=/a ExecStartPost=/usr/bin/sdptool add SP' /etc/systemd/system/dbus-org.bluez.service 
	sudo sed -i 's/^ExecStart=.*/& -C --noplugin=sap/' /etc/systemd/system/dbus-org.bluez.service 
#	echo "power on"|sudo bluetoothctl
#	echo "agent on"|sudo bluetoothctl
#	echo "discoverable on"|sudo bluetoothctl
#	echo "pairable on"|sudo bluetoothctl
#	echo "scan on"|sudo bluetoothctl

rund: initdisplay copydisplay

autodisplay:
	sudo echo "fbtft_device custom name=fb_ili9341 gpios=reset:25,dc:24,led:18 speed=90000000 rotate=90 bgr=1">>/etc/modules


