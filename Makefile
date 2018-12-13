all: createdir ultrasound bluetooth
createdir:
	mkdir -p bin
ultrasound:
	g++ -g -o bin/ultrasnd.out hc-sr04.cpp -lwiringPi
bluetooth:
	g++ -g -o bin/bluetooth.out bluetooth.c -lbluetooth
clean:
	rm -rf bin/
run:
	bin/ultrasnd.out
debug:
	nemiver bin/ultrasnd.out