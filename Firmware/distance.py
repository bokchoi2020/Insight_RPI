import RPi.GPIO as GPIO
import time
import signal
import sys

def close(signal, frame):
    print("\nTurning off ultrasonic distance detection...\n")
    GPIO.cleanup() 
    sys.exit(0)

def scan_dist():
    print ("init ultrasound")
    # use Raspberry Pi board pin numbers
    GPIO.setmode(GPIO.BCM)

    # set GPIO Pins
    pinTrigger = 17
    pinEcho = 27
    pinLED = 5
    distanceLimit = 20 #limit in cm
    ledON = False

    #signal.signal(signal.SIGINT, close)

    # set GPIO input and output channels
    GPIO.setup(pinTrigger, GPIO.OUT)
    GPIO.setup(pinEcho, GPIO.IN)
    GPIO.setup(pinLED, GPIO.OUT)
    print ("begin scanning")
    while True:
        # set Trigger to HIGH
        GPIO.output(pinTrigger, True)
        # set Trigger after 0.01ms to LOW
        time.sleep(0.00001)
        GPIO.output(pinTrigger, False)
    
        startTime = time.time()
        stopTime = time.time()
    
        # save start time
        while 0 == GPIO.input(pinEcho):
            startTime = time.time()
    
        # save time of arrival
        while 1 == GPIO.input(pinEcho):
            stopTime = time.time()
    
        # time difference between start and arrival
        TimeElapsed = stopTime - startTime
        # multiply with the sonic speed (34300 cm/s)
        # and divide by 2, because there and back
        distance = (TimeElapsed * 34300) / 2
    
        if(distance < distanceLimit):
            if(not ledON):
                print("LED ON")
                print ("Distance: %.1f cm" % distance)
                GPIO.output(pinLED, True)
                ledON = True
        else:
            if(ledON):
                print("LED OFF")
                print ("Distance: %.1f cm" % distance)
                GPIO.output(pinLED, False)
                ledON = False
            
        time.sleep(1)
#if __name__ == "__main__":
#   scan_dist()