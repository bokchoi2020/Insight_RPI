#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_
using namespace std;

void init_server();
int getbtClient();
string bt_poll_read(int client, int time);

#endif