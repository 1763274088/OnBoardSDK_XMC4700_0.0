/** @file
 *  @brief MAVLink comm protocol built from common.xml
 *  @see http://mavlink.org
 */
#pragma once
#ifndef MAVLINK_H
#define MAVLINK_H

#define MAVLINK_PRIMARY_XML_IDX 0

#ifndef MAVLINK_STX
#define MAVLINK_STX 254
#endif

#ifndef MAVLINK_ENDIAN
#define MAVLINK_ENDIAN MAVLINK_LITTLE_ENDIAN
#endif

#ifndef MAVLINK_ALIGNED_FIELDS
#define MAVLINK_ALIGNED_FIELDS 1
#endif

#ifndef MAVLINK_CRC_EXTRA
#define MAVLINK_CRC_EXTRA 1
#endif

#ifndef MAVLINK_COMMAND_24BIT
#define MAVLINK_COMMAND_24BIT 0
#endif

//#include "mavlink_helpers.h"
//#include "checksum.h"
//#include "mavlink_conversions.h"
//#include "protocol.h"
//#include "mavlink_types.h"
#include "version.h"
#include "common.h"







//#ifndef memset//由王朔添加 2018-08-24
//static inline void*memset(void*dest,int data,size_t length){
//uint32_t i;
//int*point = dest;
//for(i=0; i<length; i++) point[i]= data;
//return dest;
//}
//#endif

//#ifndef memcpy//由王朔添加 2018-08-24
//void*memcpy(void*dest,const void*src,size_t n)
//{
//unsigned char*pout =(unsigned char*)dest;
//unsigned char*pin =(unsigned char*)src;
//while(n-->0)*pout++=*pin++;
//return dest;
//}

//#include"mavlink_types.h"

//#define MAVLINK_USE_CONVENIENCE_FUNCTIONS

//#define MAVLINK_SEPARATE_HELPERS

////mavlink_system_t mavlink_system = {0,0};
//mavlink_system_t mavlink_system ={
//1,
//1
//};// System ID, 1-255, Component/Subsystem ID, 1-255
//void comm_send_ch(mavlink_channel_t chan,uint8_t buf)
//{
//chan=chan;
//USART_SendData(USART1, buf);//向串口1发送数据
//while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
//}
//#endif








#endif // MAVLINK_H
