all: createdir main
initdisplay:
	sudo modprobe fbtft_device custom name=fb_ili9341 gpios=reset:25,dc:24,led:18 speed=90000000 rotate=90 bgr=1

startdisplay:
	sleep 2
	sudo FRAMEBUFFER=/dev/fb1 startx

copydisplay:
	sleep 2
	fbcp&

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
	#enable loading kernel module at startup
	sudo echo "options fbtft_device custom name=fb_ili9341 speed=90000000 gpios=reset:25,dc:24,led:18 rotate=270 bgr=1">/etc/modprobe.d/fbtft.conf 
	sudo echo -en "spi-bcm2835\nfbtft_device" > /etc/modules-load.d/fbtft.conf 
	sudo echo -en "Section \"Device\"\n  Identifier \"myfb\"\n  Driver \"fbdev\"\n  Option \"fbdev\" \"/dev/fb1\"\nEndSection">/usr/share/X11/xorg.conf.d/99-fbdev.conf"
	#hide mouse cursor
	sudo apt-get install unclutter
	echo -en "unclutter" > ~/.config/lxsession/LXDE-pi/autostart
	#fix screensaver issue (prevent screen off)
	echo -en "@xset s noblank\n@xset s off\n@xset -dpms" > ~/.config/lxsession/LXDE-pi/autostart

disabledisplay:
	sudo mv /etc/modprobe.d/fbtft.conf /etc/modprobe.d/fbtft.conf.bak
	sudo mv /usr/share/X11/xorg.conf.d/99-fbdev.conf /usr/share/X11/xorg.conf.d/99-fbdev.conf.bak

reenabledisplay:
	sudo mv /etc/modprobe.d/fbtft.conf.bak /etc/modprobe.d/fbtft.conf
	sudo mv /usr/share/X11/xorg.conf.d/99-fbdev.conf.bak /usr/share/X11/xorg.conf.d/99-fbdev.conf

autostart:
	sudo cp insight.desktop /etc/xdg/autostart
installservice:
	sudo cp insight.service /etc/systemd/system 
	systemctl enable insight.service

disableservice:
	systemctl disable insight.service

createdir:
	mkdir -p bin
main:
	g++ -O3 -o bin/insight.out main.cpp hc-sr04.cpp display.cpp bluetooth.c msgparser.cpp -lwiringPi -lbluetooth -Iinclude `pkg-config --cflags --libs gtk+-3.0`
clean:
	rm -rf bin/
run:
	bin/insight.out
debug:
	nemiver bin/insight.out
