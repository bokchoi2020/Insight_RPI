#ifndef HCSR04_H_
#define HCSR04_H_

#include <wiringPi.h>
#include <sys/time.h>
#define ULT_REQ 17
#define LED 5
#define N_SENSOR 3

void ultsetup();
void sendULTReq();
void getAllDistance();

#endif
