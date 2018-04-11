/*! @file BspUsart.h
 *  @version 3.1.8
 *  @date Aug 05 2016
 *
 *  @brief
 *  Usart helper functions and ISR for board XMC4700Discovery
 *
 *  Copyright 2016 DJI. All right reserved.
 *
 * */

#ifndef _BSPUSART_H
#define _BSPUSART_H
#include <XMC4700.h>
#include "stdio.h"
void PC_UART_Config(void);
void N3_UART_Config(void);

void UARTxNVIC_Config(void);
void UartConfig(void);
void NVIC_Config(void);
void Rx_buff_Handler() ;
#endif  //_USART_H
