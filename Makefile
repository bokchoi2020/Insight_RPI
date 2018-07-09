initdisplay:
	sudo modprobe fbtft_device custom name=fb_ili9341 gpios=reset:25,dc:24,led:18 speed=90000000 rotate=90 bgr=1

startdisplay:
	sleep 2
	sudo FRAMEBUFFER=/dev/fb1 startx

copydisplay:
	sleep 2
	Firmware/fbcp&

rund: initdisplay copydisplay

autodisplay:
	sudo echo "fbtft_device custom name=fb_ili9341 gpios=reset:25,dc:24,led:18 speed=90000000 rotate=90 bgr=1">>/etc/modules


