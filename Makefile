initdisplay:
	sudo modprobe fbtft_device custom name=fb_ili9341 gpios=reset:25,dc:24,led:18

startdisplay:
	sudo FRAMEBUFFER=/dev/fb1 startx

rund: initdisplay startdisplay

autodisplay:
	sudo echo "fbtft_device custom name=fb_ili9341 gpios=reset:25,dc:24,led:18 speed=16000000 rotate=90 bgr=1">>/etc/modules


