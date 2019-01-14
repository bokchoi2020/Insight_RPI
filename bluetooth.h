#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;
using namespace std;

typedef enum msgtype { msgERR, msgSET, msgDIR, msgSPD} msgType;

typedef struct msgStruct
{
    msgType type;
    string text;
} msgStruct;

void init_server();
msgStruct msgHandler(string s);
int getbtClient();
string bt_poll_read(int client, int time);

#endif