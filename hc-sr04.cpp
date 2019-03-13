#include <iostream>
#include <errno.h>
#include <string.h>
#include <stdint.h>     // for uint32_t
#include <wiringPi.h>
#include <sys/time.h>
#include "hc-sr04.h"
#include <iomanip> 

using namespace std;

const int ult_echo [N_SENSOR] = { 22, 6 }; //27, 22 , 6 };
const int danger_speed = 10;
const int danger_speed_distance = 200;
const int safety_zone_radius = 20;

volatile int ult_rdy[N_SENSOR];
volatile float u_distance[N_SENSOR];
volatile float last_distance[N_SENSOR];

volatile uint32_t startTime[N_SENSOR];
volatile uint32_t endTime[N_SENSOR];

volatile uint32_t lastCallTime = 0;
bool ledON = false;

inline void calcUltDistance(int sensor)
{
    if(digitalRead(ult_echo[sensor]))
    {
        ult_rdy[sensor]  = 1;
        startTime[sensor] = micros();
    }
    else if(ult_rdy[sensor] = 1)
    {
        endTime[sensor] = micros();
        ult_rdy[sensor] = 2;
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

void (*pISR[N_SENSOR])() = {isrULT0, isrULT1};//, isrULT2};

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
        u_distance[i] = 100;
        last_distance[i] = 100;
        ult_rdy[i] = 0;
    }
        
    //cout << "IO, ISR setup complete. " << endl;
    delay(30);
    sendULTReq();
}

void getAllDistance(bool * warn)
{
    //TODO: handle micro 74 minute overflow
    for(int i = 0; i < N_SENSOR; i++)
    {
        if(ult_rdy[i] == 2)
        {
            last_distance[i] = u_distance[i];

            u_distance[i] = (endTime[i] - startTime[i])/58.0;
            //cout <<"Distance "<<i <<": " << u_distance[i] << "cm" <<endl;
            //cout <<"start time: " <<startTime <<endl;
            //cout <<"end time: "<< endTime <<endl<<endl;
            ult_rdy[i] = 3;
        } 
    }
    
    uint32_t curTime = millis();
    //only send a new request once all sensors are ready or if no activity for 200ms.
    if(ult_rdy[0] == 3 && ult_rdy[1] == 3 && ult_rdy[2] == 3 || (curTime - lastCallTime) > 200)
    {
        //if less than 60ms since last request we wait
        //cout <<"last call time:"<<lastCallTime <<endl;
        //cout <<"current time"<<curTime<<endl;
        if((curTime - lastCallTime) < 60){
            cout <<"skipped distance"<<endl;
            return;
        }
        /*
        The LED lights up if at least 1 of 2 conditions is met:
        1. any of the sensors reads less than safety_zone_radius (object is within safety zone).
        2. an object continually moves toward the rider at <2m away

        Notes:
        - The value of danger_speed depends on how frequently data is read from the sensors,
            and should be adjusted accordingly
        - obviously we can have a more sophisticated high-pass filter if we recorded
            more of the previous values, but the overhead of storing (and shifting everything
            for the new values) might be undesirable
        */

        //bool ledReq = false;
        for(int i = 0; i < N_SENSOR; i++)
        {
            //cout << "last dist: " << last_distance[i] << "sensor: " << i << endl;
            //cout << "new dist:  " << u_distance[i] << "sensor: " << i << endl;
            
            if(u_distance[i] > 400){
                u_distance[i] = 400;
                warn[i] = false;
            }
            else if(u_distance[i] < safety_zone_radius || (last_distance[i] < danger_speed_distance && last_distance[i] - u_distance[i] > danger_speed))
            {
                //ledReq = true;
                warn[i] = true;
                //break;
            }
            else
                warn[i] = false;

            //cout << fixed << setprecision(2) << u_distance[i] << ", ";
        }
        //cout << endl;
        
        ult_rdy[0] = 0;
        ult_rdy[1] = 0;
        ult_rdy[2] = 0;
        sendULTReq();
    }
}
