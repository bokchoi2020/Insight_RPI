#include <iostream>
#include "hc-sr04.h"
#include "display.h"
#include "bluetooth.h"

using namespace std;

bool btConnected = false;

int main(int argc, char *argv[])
{
    //setup and start ultrasound
    ultsetup();
    sendULTReq();
    
    //setup and start display
    setupGTKDisplay(argc, argv);
    setTime("");

    //setup and start bluetooth
    init_server();	

    //main loop
    while(1)
    {
        //read distances
        getAllDistance();
        
        int btClient = getbtClient();
        
        //if device is disconnected, clear display
        if(btClient == -1 && btConnected)
        {
            setDir("");
            setETA("");
            setSpeed("");
            setDest("Connect your device.");
            setBtImg(false);
            btConnected = false;
        }
        //if device connected
        else if(btClient > 0 && !btConnected)
        {
            setDest("No destination set.");
            setSpeed("- km/h");
            setBtImg(true);
            btConnected = true;
        }
        
        //poll bluetooth
        string btMsg = bt_poll_read(btClient, 60);
        //parse received message
        if(!btMsg.empty())
        {
            cout << btMsg <<endl;
            msgStruct msg = msgHandler(btMsg);
            //update display according to message.
            switch(msg.type)
            {
                case msgDIR:
                    setDir(msg.text);
                    break;
                case msgSPD:
                    setSpeed(msg.text);
            }
        }

        //update display
        while (gtk_events_pending())
            gtk_main_iteration();
    }
    return 0;
}
