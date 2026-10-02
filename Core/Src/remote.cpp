/**
******************************************************************************
 * @file    remote.cpp/h
 * @brief   Remote control. 遥控器
 ******************************************************************************
 * Copyright (c) 2026 Team JiaoLong-SJTU
 * All rights reserved.
 ******************************************************************************
 */

#include "remote.h"

#include <string.h>

constexpr uint16_t REMOTE_CONNECT_TIMEOUT = 500u; 
// Constructor 构造函数
Remote::Remote(UART_HandleTypeDef *huart): huart_(huart), connect_(REMOTE_CONNECT_TIMEOUT){
    switch_.l = RCSwitchState_e::DOWN;
    switch_.r = RCSwitchState_e::DOWN;
}

// Start UART(SBUS) receive. 打开UART接收
void Remote::init() {
    // Your code here
    //while (HAL_UARTEx_ReceiveToIdle_DMA(huart_, rx_buf, RC_FRAME_LEN)!=HAL_OK){}
    HAL_UARTEx_ReceiveToIdle_DMA(huart_, rx_buf, RC_FRAME_LEN);
    reset();
}

// Reset RC data. 重置遥控器数据
void Remote::reset() {
    // Your code here
    channel_.l_col = 1024;
    channel_.r_col = 1024;
    channel_.l_row = 1024;
    channel_.r_row = 1024;
    channel_.dial_wheel = 1024;
    switch_.l = RCSwitchState_e::DOWN;
    switch_.r = RCSwitchState_e::DOWN;
    transmitToB=false;

}

// Check for uart correspondence. 检查串口是否匹配
bool Remote::rxMsgCheck(UART_HandleTypeDef *huart){
    // Your code here
    if (huart != huart_) return false;
    else return true;

}

// Update connect status, restart UART(SBUS) receive.
// 更新连接状态，重新打开UART(SBUS)接收
void Remote::rxMsgCallback() {
    // Your code here
    handleFlag=true;
    connect_.refresh();
    if (transmitToB==true){
        transmitToB=false;
        HAL_UARTEx_ReceiveToIdle_DMA(huart_, &rx_buf[0], RC_FRAME_LEN);
    }
    else {
        transmitToB=true;
        HAL_UARTEx_ReceiveToIdle_DMA(huart_, &rx_buf[18], RC_FRAME_LEN);
    }
}

// Unpack data. 数据解包
void Remote::handle() {
    // Your code here
    if(transmitToB==false) {
        memcpy(rx_data_, &rx_buf[18], RC_FRAME_LEN);
    }
    else {
        memcpy(rx_data_, &rx_buf[0], RC_FRAME_LEN);
    }
    channel_.r_row = static_cast<uint16_t>((rx_data_[0] | (rx_data_[1] << 8)) & 0x07FF);
    channel_.r_col = static_cast<uint16_t>(((rx_data_[1] >> 3) | (rx_data_[2] << 5)) & 0x07FF);
    channel_.l_row = static_cast<uint16_t>(((rx_data_[2] >> 6) | (rx_data_[3] << 2) | (rx_data_[4] << 10)) & 0x07FF);
    channel_.l_col = static_cast<uint16_t>(((rx_data_[4] >> 1) | (rx_data_[5] << 7)) & 0x07FF);
    uint8_t s1_raw = (rx_data_[5] >> 6) & 0x03;
    uint8_t s2_raw = (rx_data_[5] >> 4) & 0x03;
    switch_.l = static_cast<RCSwitchState_e>(s1_raw);
    switch_.r = static_cast<RCSwitchState_e>(s2_raw);
    channel_.dial_wheel = static_cast<uint16_t>((rx_data_[16] | (rx_data_[17] << 8)) & 0x07FF);

}
Remote remote(&huart3);