#ifndef HCSR04_H_
#define HCSR04_H_

#define ULT_REQ 17
#define LED 5
#define N_SENSOR 2//3
#define BUFFER_DEPTH 3
void ultsetup();
void getAllDistance(bool * warn);

#endif
