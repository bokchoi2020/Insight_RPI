#include <iostream>
#include <wiringPi.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>     // for uint32_t
#include <sys/time.h>

using namespace std;

#define ULT_REQ 17
#define ULT_ECHO 27
#define LED 5

uint32_t startTime = 0;
uint32_t endTime = 0;
float u_distance = 0;
bool falling_flag = 0;
bool ult_rdy = 0;

void isrULT()
{
    if(!falling_flag)
    {
        startTime = micros();
        falling_flag = 1;
    }
    else
    {
        endTime = micros();
        u_distance = (endTime - startTime)/58.0;
        falling_flag = 0;
        ult_rdy = 1;
    }
}

//void (* pInterruptRise[6])() = {isrRise0, isrRise1, isrRise2, isrRise3, isrRise4, isrRise5};
//void (* pInterruptFall[6])() = {isrFall0, isrFall1, isrFall2, isrFall3, isrFall4, isrFall5};

void setup() 
{
    int i;
    //setup Pi to use BCM GPIO
    wiringPiSetupGpio();
    
    pinMode(ULT_REQ, OUTPUT);
    pinMode(ULT_ECHO, INPUT);
    pullUpDnControl(ULT_ECHO,PUD_DOWN);
    
    pinMode(LED, OUTPUT);

    // Ultrasound Request pin must start LOW
    digitalWrite(ULT_REQ, LOW);
    // set ISRs
    wiringPiISR(ULT_ECHO, INT_EDGE_BOTH, &isrULT);
    //cout << "IO, ISR setup complete. " << endl;
    
    delay(30);
}

void sendULTReq() 
{
    // Send trig pulse
    digitalWrite(ULT_REQ, HIGH);
    delayMicroseconds(10);
    digitalWrite(ULT_REQ, LOW);
    //cout << "sent req on ultrasound sensor ";
    //cout << "req time: "<< micros() <<endl;
}

int main(void)
{
    setup();

    sendULTReq();

    while(1)
    {
        if(ult_rdy)
        {
            cout <<"Distance: " << u_distance << "cm" <<endl;
            //cout <<"start time: " <<startTime <<endl;
            //cout <<"end time: "<< endTime <<endl<<endl;
            if(u_distance < 20)
                digitalWrite(LED, HIGH);
            else
                digitalWrite(LED, LOW);
            ult_rdy = 0;
            delay(400);
            sendULTReq();
        } 
    }

    return 0;
}

