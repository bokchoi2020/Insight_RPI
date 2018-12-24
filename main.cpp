#include <iostream>
#include "hc-sr04.h"
#include "display.h"
#include "bluetooth.h"

using namespace std;

int main(int argc, char *argv[])
{
    //setup and start ultrasound
    ultsetup();
    sendULTReq();
    
    //setup and start display
    setupGTKDisplay(argc, argv);
    setDest("durr");

    //setup and start bluetooth
    init_server();	

    while(1)
    {
        //delay(60);
        getAllDistance();
        
        while (gtk_events_pending())
            gtk_main_iteration();

        string temp = bt_poll_read(getbtClient(), 60);

		if(!temp.empty())
		{
			cout << temp <<endl;
			msgHandler(temp);
		}
    }

    return 0;
}
