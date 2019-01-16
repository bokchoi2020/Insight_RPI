#include <iostream>
#include <errno.h>
#include <string.h>
#include <stdint.h>     // for uint32_t
#include <wiringPi.h>
#include <sys/time.h>
#include "hc-sr04.h"

using namespace std;

const int ult_echo [N_SENSOR] = { 27, 22 , 6 };
volatile int ult_rdy[N_SENSOR];
volatile float u_distance[N_SENSOR];

volatile uint32_t startTime[N_SENSOR];
volatile uint32_t endTime[N_SENSOR];

volatile uint32_t lastCallTime = 0;
bool ledON = false;

inline void calcUltDistance(int sensor)
{
    if(digitalRead(ult_echo[sensor]))
    {
        startTime[sensor] = micros();
    }
    else
    {
        endTime[sensor] = micros();
        ult_rdy[sensor] = 1;
    }
}

void isrULT0()
{
    calcUltDistance(0);
}

void isrULT1()
{
    calcUltDistance(1);
}

void isrULT2()
{
    calcUltDistance(2);
}

void (*pISR[N_SENSOR])() = {isrULT0, isrULT1, isrULT2};

void sendULTReq() 
{
    // Send trig pulse
    digitalWrite(ULT_REQ, HIGH);
    delayMicroseconds(10);
    digitalWrite(ULT_REQ, LOW);
    lastCallTime = millis();
    //cout << "sent req on ultrasound sensor ";
    //cout << "req time: "<< micros() <<endl;
}

void ultsetup() 
{
    //setup Pi to use BCM GPIO
    wiringPiSetupGpio();
    
    pinMode(ULT_REQ, OUTPUT);
    pinMode(LED, OUTPUT);
    // Ultrasound Request pin must start LOW
    digitalWrite(ULT_REQ, LOW);

    //setup the sensors
    for(int i = 0; i < N_SENSOR; i++)
    {
        pinMode(ult_echo[i], INPUT);
        pullUpDnControl(ult_echo[i],PUD_DOWN);
        // set ISRs
        wiringPiISR(ult_echo[i], INT_EDGE_BOTH, pISR[i]);
    }
    
    //cout << "IO, ISR setup complete. " << endl;
    delay(30);
    sendULTReq();
}

void getAllDistance()
{
    //TODO: handle micro 74 minute overflow

    for(int i = 0; i < N_SENSOR; i++)
    {
        if(ult_rdy[i] == 1)
        {
            u_distance[i] = (endTime[i] - startTime[i])/58.0;
            cout <<"Distance "<<i <<": " << u_distance[i] << "cm" <<endl;
            //cout <<"start time: " <<startTime <<endl;
            //cout <<"end time: "<< endTime <<endl<<endl;
            ult_rdy[i] = 2;
        } 
    }
    
    uint32_t curTime = millis();
    //only send a new request once all sensors are ready or if no activity for 200ms.
    if(ult_rdy[0] == 2 && ult_rdy[1] == 2 && ult_rdy[2] == 2 || (curTime - lastCallTime) > 200)
    {
        //if less than 60ms since last request we wait
        //cout <<"last call time:"<<lastCallTime <<endl;
        //cout <<"current time"<<curTime<<endl;
        if((curTime - lastCallTime) < 60){
            cout <<"skipped distance"<<endl;
            return;
        }
        ult_rdy[0] = 0;
        ult_rdy[1] = 0;
        ult_rdy[2] = 0;
        sendULTReq();
    }

    //check if any of the sensors are less than 20cm safety zone.
    bool ledReq = false;
    for(int i = 0; i < N_SENSOR; i++)
    {
        if(u_distance[i] < 20 )
        {
            ledReq = true;
            break;
        }
    }

    if(!ledON && ledReq)
    {
        digitalWrite(LED, HIGH);
        ledON = true;
    }
    else if(ledON && !ledReq)
    {
        digitalWrite(LED, LOW); 
        ledON = false;
    }
}
