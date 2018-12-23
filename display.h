#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <string>
#include <gtk/gtk.h>

using namespace std;

void setDir(string dir);
void setTime(string time);
void setDest(string dest);
void setETA (string ETA);
int setupGTKDisplay(int argc, char *argv[]);

#endif