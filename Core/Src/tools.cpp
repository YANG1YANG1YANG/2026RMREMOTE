//
// Created by yangxiao on 2026/10/2.
//
#include "tools.h"
#include "remote.h"
extern Remote remote;
extern "C" {
void remoteInit(void) {
    remote.init();
}
bool checkConnection(void) {
    return remote.connect_.check();
}
void handleData(void) {
    if (remote.handleFlag==true) {
    remote.handleFlag=false;
        remote.handle();
    }
}
}