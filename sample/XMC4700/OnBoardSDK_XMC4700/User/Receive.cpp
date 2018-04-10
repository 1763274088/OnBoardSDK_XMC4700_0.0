/*! @file Receive.cpp
 *
 *  @version 3.1.8
 *  @date Aug 05 2016
 *
 *  @brief
 *  This function parses Rx buffer and execute commands sent from computer.
 *
 *  Copyright 2016 DJI. All right reserved.
 *
 * */

#include "Receive.h"
#include "LocalNavigation.h"
#include "main.h"
#include "fifo.h"

/**实例化fifo数据类型的 uart_rx_fifo接收缓存*/
fifo_t uart_rx_fifo;
/**实例化fifo数据类型的 uart_tx_fifo发送缓存*/
fifo_t uart_tx_fifo;

/**定义uart_rx_fifo的接收数组缓存区*/
uint8_t uart_rx_buf[UART_RX_BUFFER_SIZE];

/**定义uart_tx_fifo的发送数组缓存区*/
uint8_t uart_tx_buf[UART_TX_BUFFER_SIZE];

/**定义基于Mavlink的消息包*/
mavlink_message_t msg;

/**定义基于Mavlink的状态*/
mavlink_status_t status;

/**定义Mavlink心跳包数据*/
mavlink_heartbeat_t     heartbeat;

/**定义基于Mavlink的optitrack数据*/
mavlink_att_pos_mocap_t att_pos_mocap; 

/**定义基于Mavlink的目标姿态数据*/
mavlink_set_attitude_target_t set_attitude_target;


extern XMC_USIC_CH_t           *pc_uart;
extern FlightData flightData;

using namespace DJI::onboardSDK;

/*
 * @brief Helper function to assemble two bytes into a float number
 */
static float32_t hex2Float(uint8_t HighByte, uint8_t LowByte)
{
  float32_t high = (float32_t) (HighByte & 0x7f);
  float32_t low  = (float32_t) LowByte;
  if (HighByte & 0x80)//MSB is 1 means a negative number
  {
    return -(high*256.0f + low)/100.0f;
  }
  else
  {
    return (high*256.0f + low)/100.0f;
  }
}

TerminalCommand myTerminal;

uint8_t c_temp;
void TerminalCommand::terminalCommandHandler(CoreAPI* api, Flight* flight)
{	
	while(serial_available())//如果tail还没有追上head
	{
	  c_temp = serial_read_ch();//读取fifo数据，tail加1，数据来自串口读取数组		
		
		if(mavlink_parse_char(MAVLINK_COMM_0, c_temp, &msg, &status))//解析获得的消息 
		{
			
			switch(msg.msgid)
			{
				case MAVLINK_MSG_ID_HEARTBEAT:
				{
						mavlink_msg_heartbeat_decode(&msg, &heartbeat);
						break;
				}
									
				case MAVLINK_MSG_ID_ATT_POS_MOCAP :
				{
						mavlink_msg_att_pos_mocap_decode(&msg,&att_pos_mocap);
						break;
				}
						
				case MAVLINK_MSG_ID_SET_ATTITUDE_TARGET :
				{
						mavlink_msg_set_attitude_target_decode(&msg,&set_attitude_target);
						break;
				}
				default:
						//Do nothing
						break;
			}			
			
			
		}
		
	}
	
  if (cmdReadyFlag == 1)
  {
    cmdReadyFlag = 0;
  }
  else
  { // No full command has been received yet.
    return;
  }

  if ((cmdIn[0] != 0xFA) || (cmdIn[1] != 0xFB))
  { // Command header doesn't match
    return;
  }

  switch(cmdIn[2])
  {
  case 0x00:
    api->getDroneVersion();
    break;

  case 0x01:
    User_Activate();
    break;

  case 0x02:
    api->setControl(cmdIn[3]);
    break;

  case 0x03:
    flight->setArm(cmdIn[3]);
    break;

  case 0x04:
    if (cmdIn[3] == 0x01)
    {
      flightData.flag = cmdIn[4];
      flightData.x = hex2Float(cmdIn[5], cmdIn[6]);
      flightData.y = hex2Float(cmdIn[7], cmdIn[8]);
      flightData.z = hex2Float(cmdIn[9], cmdIn[10]);
      flightData.yaw = hex2Float(cmdIn[11], cmdIn[12]);
      flight->setFlight(&flightData);
      XMC_CCU4_SLICE_StartTimer(SLICE1_PTR);
      printf("roll_or_x =%f\n", hex2Float(cmdIn[5], cmdIn[6]));
      printf("pitch_or_y =%f\n", hex2Float(cmdIn[7], cmdIn[8]));
      printf("thr_z =%f\n", hex2Float(cmdIn[9], cmdIn[10]));
      printf("yaw =%f\n\n", hex2Float(cmdIn[11], cmdIn[12]));
    }
    else if (cmdIn[3] == 0x02)
    {//for display ur hex2int
      printf("roll_or_x =%f\n", hex2Float(cmdIn[5], cmdIn[6]));
      printf("pitch_or_y =%f\n", hex2Float(cmdIn[7], cmdIn[8]));
      printf("thr_z =%f\n", hex2Float(cmdIn[9], cmdIn[10]));
      printf("yaw =%f\n\n", hex2Float(cmdIn[11], cmdIn[12]));
    }
    break;

  case 0x05:
    switch(cmdIn[3])
    {
    case 0x01:
      flight->task(Flight::TASK_GOHOME);
      break;
    case 0x02:
      flight->task(Flight::TASK_TAKEOFF);
      break;
    case 0x03:
      flight->task(Flight::TASK_LANDING);
      break;
    }
    break;

  case 0x06:
    if (cmdIn[3] == 0x00)              //0x06 0x00 to stop VRC
    {
      VRCResetData();
      XMC_CCU4_SLICE_StopTimer(SLICE0_PTR);
    }
    else if (cmdIn[3] == 0x02)            //0x06 0x01 to turn VRC to F gear
    {
      VRC_TakeControl();
      XMC_CCU4_SLICE_StartTimer(SLICE0_PTR);
    }
    else if (cmdIn[3] == 0x01)          //0x06 0x02 to reset data
    {
      VRCResetData();
      XMC_CCU4_SLICE_StartTimer(SLICE0_PTR);
    };
    break;

  case 0x07:
    if(cmdIn[3] == 0x00)
    {
      tryHotpoint(cmdIn[4], cmdIn[5], cmdIn[6]);
    }
    else
    {
      stopHotpoint();
    }
    break;
  case 0x08:
    printf("TimeStamp is %d\r\n", api->getBroadcastData().timeStamp.time);
    printf("Battery capacity remains %d percent\r\n",
        api->getBroadcastData().battery);
    break;

  case 0x09:
    if (cmdIn[3] == 0x01)
    {
      startLocalNavExample();
    }
    else if(cmdIn[3] == 0x00)
    {
      stopLocalNavExample();
    }
    break;

  default:
    break;
  }
}

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
void USIC0_5_IRQHandler(void)
{
 
    uint8_t oneByte = XMC_UART_CH_GetReceivedData(pc_uart);
	
		fifo_write_ch(&uart_rx_fifo, oneByte);
				
    if (myTerminal.rxIndex == 0)
    {
      if (oneByte == 0xFA)
      {
        myTerminal.cmdIn[myTerminal.rxIndex] = oneByte;
        myTerminal.rxIndex = 1;
      }
      else
      {;}
    }
    else
    {
      if (oneByte == 0xFE)    //receive a 0xFE would lead to a command-execution
      {
        myTerminal.cmdIn[myTerminal.rxIndex] = oneByte;
        myTerminal.rxLength = myTerminal.rxIndex + 1;
        myTerminal.rxIndex = 0;
        myTerminal.cmdReadyFlag = 1;
      }
      else
      {
        myTerminal.cmdIn[myTerminal.rxIndex] = oneByte;
        myTerminal.rxIndex++;
      }
    }
}
#ifdef __cplusplus
}
#endif //__cplusplus

/* --------------------------------------------------------------------------*/
/**
 * @brief 初始化fifo缓存区
 */
/* ----------------------------------------------------------------------------*/
void InitSoftwareFIFO(void)
{
    fifo_init(&uart_tx_fifo, uart_tx_buf, UART_TX_BUFFER_SIZE);	
    fifo_init(&uart_rx_fifo, uart_rx_buf, UART_RX_BUFFER_SIZE);
}

/* --------------------------------------------------------------------------*/
/**
 * @brief 从串口读取数据
 *
 * @returns 从串口读取的一个字节的数据 
 */
/* ----------------------------------------------------------------------------*/
uint8_t serial_read_ch(void)
{
	uint8_t ch;	
	fifo_read_ch(&uart_rx_fifo, &ch);	
	return ch;
}

/* --------------------------------------------------------------------------*/
/**
 * @brief 获取读取读取fifo数组中可读数据个数
 *
 * @returns  返回可读数据个数 
 */
/* ----------------------------------------------------------------------------*/
uint16_t serial_available(void)
{
	return fifo_used(&uart_rx_fifo);
}



