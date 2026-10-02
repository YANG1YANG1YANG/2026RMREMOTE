//
// Created by yangxiao on 2026/10/2.
//

#ifndef UART_TOOLS_H
#define UART_TOOLS_H
#include "stdbool.h"
#ifdef __cplusplus
extern "C" {
#endif
void remoteInit(void);
bool checkConnection(void);
void handleData(void);
#ifdef __cplusplus
}
#endif
#endif //UART_TOOLS_H
