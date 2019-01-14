initdisplay:
	sudo modprobe fbtft_device custom name=fb_ili9341 gpios=reset:25,dc:24,led:18 speed=90000000 rotate=90 bgr=1

startdisplay:
	sleep 2
	sudo FRAMEBUFFER=/dev/fb1 startx

copydisplay:
	sleep 2
	Firmware/fbcp&

startserver:
	./server.sh&

setupbluetooth:
	#Make bluetooth start in RFCOMM/SPP mode
	sudo sed -i '/^ExecStart=/a ExecStartPost=/usr/bin/sdptool add SP' /etc/systemd/system/dbus-org.bluez.service 
	sudo sed -i 's/^ExecStart=.*/& -C --noplugin=sap --compat/' /etc/systemd/system/dbus-org.bluez.service 
	#Workaround to get RFCOMM elevated privledges
	sudo usermod -G bluetooth -a pi
	sudo chgrp bluetooth /var/run/sdp
	sudo cp var-run-sdp.path /etc/systemd/system/
	sudo cp var-run-sdp.service /etc/systemd/system/
	#Load services
	sudo systemctl daemon-reload
	sudo systemctl enable var-run-sdp.path
	sudo systemctl enable var-run-sdp.service
	sudo systemctl start var-run-sdp.path

#	echo "power on"|sudo bluetoothctl
#	echo "agent on"|sudo bluetoothctl
#	echo "discoverable on"|sudo bluetoothctl
#	echo "pairable on"|sudo bluetoothctl
#	echo "scan on"|sudo bluetoothctl

rund: initdisplay copydisplay

autodisplay:
	sudo echo "fbtft_device custom name=fb_ili9341 gpios=reset:25,dc:24,led:18 speed=90000000 rotate=90 bgr=1">>/etc/modules

all: createdir main
createdir:
	mkdir -p bin
main:
	g++ -g -o bin/insight.out main.cpp hc-sr04.cpp display.cpp bluetooth.c -lwiringPi -lbluetooth -Iinclude `pkg-config --cflags --libs gtk+-3.0`
clean:
	rm -rf bin/
run:
	bin/insight.out
debug:
	nemiver bin/insight.out
