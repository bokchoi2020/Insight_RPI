#include <iostream>
#include <wiringPi.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>     // for uint32_t
#include <sys/time.h>

using namespace std;

#define ULT_REQ 17
#define LED 5
#define N_SENSOR 2
int ult_echo [N_SENSOR] = { 27, 22 };

uint32_t startTime[N_SENSOR];
uint32_t endTime[N_SENSOR];
float u_distance[N_SENSOR];
bool falling_flag[N_SENSOR];
int ult_rdy[N_SENSOR];

inline void calcUltDistance(int sensor)
{
    if(digitalRead(ult_echo[sensor]))
    {
        startTime[sensor] = micros();
        falling_flag[sensor] = 1;
    }
    else
    {
        endTime[sensor] = micros();
        u_distance[sensor] = (endTime[sensor] - startTime[sensor])/58.0;
        falling_flag[sensor] = 0;
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

void (* pISR[2])() = {isrULT0, isrULT1};

void setup() 
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
        delay(60);
        for(int i = 0; i < N_SENSOR; i++)
        {
            if(ult_rdy[i] == 1)
            {
                cout <<"Distance "<<i <<": " << u_distance[i] << "cm" <<endl;
                //cout <<"start time: " <<startTime <<endl;
                //cout <<"end time: "<< endTime <<endl<<endl;
                ++ult_rdy[i];
            } 
        }

        //only send a new request once all sensors are ready
        if(ult_rdy[0] == 2 && ult_rdy[1] == 2)
        {
            ult_rdy[0] = 0;
            ult_rdy[1] = 0;
            sendULTReq();
        }

        if(u_distance[0] < 20 || u_distance[1] < 20)
                digitalWrite(LED, HIGH);
        else
                digitalWrite(LED, LOW);
    }

    return 0;
}

