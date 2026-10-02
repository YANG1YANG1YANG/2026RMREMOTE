//
// Created by yangxiao on 2026/10/2.
//
#include "usart.h"
#include "main.h"
#include "string.h"
#include "remote.h"

#define SIZE 18

extern Remote remote;
extern "C" {
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t Size) {
    if (!remote.rxMsgCheck(huart)) return;
    HAL_UART_RxEventTypeTypeDef event = HAL_UARTEx_GetRxEventType(huart);
    if (event == HAL_UART_RXEVENT_IDLE || event == HAL_UART_RXEVENT_TC) {
        remote.rxMsgCallback();
    }
}
}