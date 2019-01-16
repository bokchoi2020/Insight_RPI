#include <iomanip> // setprecision
#include <sstream>
#include <iostream>
#include "msgparser.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

msgStruct msgParser(string s)
{
    Document d;
    msgStruct msg = {.type = msgERR, .text = ""};

    if(d.Parse(s.c_str()).HasParseError())
    {
        cout<<"JSON parse error"<<endl;
        return msg;
    }
    Value& vtype = d["messageType"];

    string type = vtype.GetString();
    
    if(type == "updateSetting")
    {
        FILE *fp;

        cout << "Creating file." << endl; 
        fp = fopen("settings.json", "w");
        if(fp != NULL)
        {
            fputs(s.c_str(), fp);
            fclose(fp);
            msg.type = msgSET;
        }
        else
            cout << "Error. Could not create file." <<endl;
    }
    else if(type == "updateDir")
    {
        vtype = d["distanceUtil"];
        int dist = vtype.GetInt();

        vtype = d["instruction"];
        string instruction = vtype.GetString();
        vtype = d["maneuver"];
        string dir = vtype.GetString();
        if(dir=="f") { dir = "↑"; }
        else if(dir =="l") { dir = "↰"; }
        else if(dir =="r") { dir = "↱"; }
        else if(dir =="ul") { dir = "↶"; }
        else if(dir =="ur") { dir ="↷"; }

        msg.type = msgDIR;
        msg.text = dir + " " + to_string(dist) +"m " + instruction;
    }
    else if(type == "updateSpeed")
    {
        vtype = d["speed"];
        float speed = vtype.GetFloat();
        stringstream stream;
        stream << fixed << setprecision(0) << speed;
        msg.type = msgSPD;
        msg.text = stream.str() + " km/h";
    }

    return msg;
}