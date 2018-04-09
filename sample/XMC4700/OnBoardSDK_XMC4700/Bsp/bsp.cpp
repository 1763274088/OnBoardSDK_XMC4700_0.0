/*! @file bsp.cpp
 *  @version 3.1.8
 *  @date Aug 05 2016
 *
 *  @brief
 *  Helper functions for board XMCDiscovery
 *
 *  Copyright 2016 DJI. All right reserved.
 *
 * */

#include "bsp.h"
#include "main.h"

void BSPinit()
{
  UartConfig();
  SystickConfig();
  Timer1Config();
  Timer2Config();
}
