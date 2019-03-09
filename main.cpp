#include <iostream>
#include "hc-sr04.h"
#include "display.h"
#include "bluetooth.h"
#include "msgparser.h"

using namespace std;

bool btConnected = false;
bool warn[N_SENSOR];

int main(int argc, char *argv[])
{
    //setup and start ultrasound
    ultsetup();
    
    //setup and start display
    setupGTKDisplay(argc, argv);
    setTime("");

    //setup and start bluetooth
    init_server();	
    int i = 0;
    //main loop
    while(1)
    {
        //i++;
        //if (i%10000 == 0) 
        //cout<<"looped"<<endl;
        //read distances        
        getAllDistance(warn);
        //show blindspot warnings
        setWarnLeft(warn[1]);
        setWarnRight(warn[0]);
        
        int btClient = getbtClient();
        
        //if device is disconnected, clear display
        if(btClient == -1 && btConnected)
        {
            setDir("Connect your device.");
            //setETA("");
            setSpeed("");
            //setDest("Connect your device.");
            setBtImg(false);
            setWarnLeft(false);
            setWarnRight(false);
            btConnected = false;
        }
        //if device connected
        else if(btClient > 0 && !btConnected)
        {
            //setDest("No destination set.");
            setDir("No Destination set.");
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
            msgStruct msg = msgParser(btMsg);
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
        updateDisplay();
    }
    return 0;
}
