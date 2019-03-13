#include <iomanip> // setprecision
#include <sstream>
#include <iostream>
#include <fstream>
#include "msgparser.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <rapidjson/istreamwrapper.h>

using namespace rapidjson;

string loadSetting()
{
    string path = "/home/pi/Insight_RPI";
    string color = "White";
    string font = "Medium";

    std::ifstream ifs { R"(/home/pi/Insight_RPI/settings.json)" };
    if ( !ifs.is_open() )
    {
        std::cerr << "Could not open file for reading!\n";
    }

    IStreamWrapper isw { ifs };

    Document doc {};
    doc.ParseStream( isw );

    StringBuffer buffer {};
    Writer<StringBuffer> writer { buffer };
    doc.Accept( writer );

    if ( doc.HasParseError() )
    {
        std::cout << "Error  : " << doc.GetParseError()  << '\n'
                  << "Offset : " << doc.GetErrorOffset() << '\n';
    }

    Value& vtype = doc["settings"];
    assert(vtype.IsArray());
    for (rapidjson::Value::ConstValueIterator itr = vtype.Begin(); itr != vtype.End(); ++itr) 
    {
        const rapidjson::Value& attribute = *itr;
        assert(attribute.IsObject()); // each attribute is an object
        /*for (rapidjson::Value::ConstMemberIterator itr2 = attribute.MemberBegin(); itr2 != attribute.MemberEnd(); ++itr2) {
            std::cout << itr2->name.GetString() << " : " << itr2->value.GetString() << std::endl;
        }*/
        string propname = attribute["propName"].GetString();

        if(propname == "color")
            color = attribute["value"].GetString();
        if(propname == "fontSize")
            font = attribute["value"].GetString();
    }

    /*color = vtype.GetString();
    vtype = doc["font"];
    font = vtype.GetString();

    ifs.close();*/
    cout<< font+color <<endl;
    return font+color;
}

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
    cout << "Type: " <<type << endl;
    if(type == "updateSetting")
    {
        FILE *fp;

        cout << "Creating file." << endl; 
        fp = fopen("settings.json", "w");
        msg.text = "-1";
        if(fp != NULL)
        {
            fputs(s.c_str(), fp);
            fclose(fp);
            msg.type = msgSET;
            msg.text = "0";
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
        cout << "Instruction: " <<instruction << endl;
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