all: createdir ultrasound bluetooth display
createdir:
	mkdir -p bin
ultrasound:
	g++ -g -o bin/ultrasnd.out hc-sr04.cpp -lwiringPi
bluetooth:
	g++ -g -o bin/bluetooth.out bluetooth.c -lbluetooth -Iinclude
display:
	g++ -Wall -g -o bin/display.out display.cpp `pkg-config --cflags --libs gtk+-3.0`
clean:
	rm -rf bin/
run:
	bin/ultrasnd.out
debug:
	nemiver bin/ultrasnd.out