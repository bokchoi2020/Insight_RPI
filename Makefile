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