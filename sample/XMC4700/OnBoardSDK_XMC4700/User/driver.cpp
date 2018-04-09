/*! @file driver.cpp
 *  @version 3.1.8
 *  @date Aug 05 2016
 *
 *  @brief
 *  Implementation of HardDriver for the XMC4700Discovery board.
 *
 *  Copyright 2016 DJI. All right reserved.
 *
 * */

#include <driver.h>
#include <XMC4700.h>
#include "xmc_gpio.h"
#include "xmc_uart.h"

extern XMC_USIC_CH_t           *n3_uart;

size_t XMC_4700::send(const uint8_t* buf, size_t len)
{
  char* p = (char*)buf;

  if (NULL == buf)
  {
    return 0;
  }

  while (len--)
  {

    XMC_UART_CH_Transmit(n3_uart, *p++);
  }
  return 1;
}

DJI::time_ms XMC_4700::getTimeStamp()
{
  return tick;
}

