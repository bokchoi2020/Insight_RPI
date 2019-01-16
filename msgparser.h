#ifndef MSG_H_
#define MSG_H_

using namespace std;

typedef enum msgtype { msgERR, msgSET, msgDIR, msgSPD, msgOTHER} msgType;

typedef struct msgStruct
{
    msgType type;
    string text;
} msgStruct;

msgStruct msgParser(string s);

#endif