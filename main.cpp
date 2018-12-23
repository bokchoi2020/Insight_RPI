#include <iostream>
#include "hc-sr04.h"
#include "display.h"

using namespace std;

int main(int argc, char *argv[])
{
    //setup and start ultrasound
    ultsetup();
    sendULTReq();

    //setup and start bluetooth

    //setup and start display
    setupGTKDisplay(argc, argv);
    setDest("durr");

    while(1)
    {
        delay(60);
        getAllDistance();
        while (gtk_events_pending())
            gtk_main_iteration();
    }

    return 0;
}
