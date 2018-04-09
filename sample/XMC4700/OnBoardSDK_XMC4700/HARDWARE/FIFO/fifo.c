/**
 * @file fifo.c
 * @brief 定义FIFO数据的读写以及剩余数据的判断
 * @author DavidWen
 * @version v1.0
 * @date 2017-08-02
 */

#include "fifo.h"

/* --------------------------------------------------------------------------*/
/**
 * @brief 读FIFO数据
 *
 * @param fifo 读取FIFO指针
 * @param ch 读取出的数据指针
 *
 * @returns  读取的状态，成功读取输出true错误读取输出0 
 *
 * @warning 这个函数一般配合serial_read_ch()函数使用。
 */
/* ----------------------------------------------------------------------------*/
uint8_t fifo_read_ch(fifo_t* fifo, uint8_t* ch)
{
	if(fifo->tail == fifo->head) return false;
	*ch = fifo->buf[fifo->tail];  
	
	if(++fifo->tail >= fifo->length) fifo->tail = 0;
  return true;
}

/* --------------------------------------------------------------------------*/
/**
 * @brief 写一个字节数据到FIFO
 *
 * @param fifo 待写入FIFO指针
 * @param ch 要写入的数据
 *
 * @returns  写入成功返回1，写入失败返回0 
 */
/* ----------------------------------------------------------------------------*/
uint8_t fifo_write_ch(fifo_t* fifo, uint8_t ch)
{
	uint16_t h = fifo->head;
	
	if(++h >= fifo->length) h = 0;
	if(h == fifo->tail) return false;
	
	fifo->buf[fifo->head] = ch;
	fifo->head = h;
  return true;
}

/* --------------------------------------------------------------------------*/
/**
 * @brief 看看FIFO有多少位置可以写，即看看写指针还有多少位可以追上读指针
 *
 * @param fifo 要检测的FIFO指针
 *
 * @returns  返回可写长度 
 *
 * @note  剩余字节长度大于等于2时，才可写入数据
 */
/* ----------------------------------------------------------------------------*/
uint16_t fifo_free(fifo_t* fifo)  
{
	uint16_t free;
	
	if(fifo->head >= fifo->tail) free = fifo->tail + (fifo->length - fifo->head);
	else free = fifo->tail - fifo->head;
	
  return free;
}

/* --------------------------------------------------------------------------*/
/**
 * @brief 看看FIFO变量还有多少位置可读，即看看读指针还有多少位可以追上写指针
 *
 * @param fifo 要检测的FIFO指针
 *
 * @returns  可以读的长度 
 */
/* ----------------------------------------------------------------------------*/
uint16_t fifo_used(fifo_t* fifo)
{
	uint16_t used;
	
	if(fifo->head >= fifo->tail) used = fifo->head - fifo->tail;
	else used = fifo->head + (fifo->length - fifo->tail);
	
	return used;	
}

/* --------------------------------------------------------------------------*/
/**
 * @brief 初始FIFO数据函数
 *
 * @param fifo 要初始化的FIFO指针
 * @param buf 要指定给FIFO指针的数组地址
 * @param length FIFO数组的长度
 */
/* ----------------------------------------------------------------------------*/
void fifo_init(fifo_t* fifo, uint8_t* buf, uint16_t length)  
{
	uint16_t i;
	
	fifo->buf = buf;
	fifo->length = length;
	fifo->head = 0;
	fifo->tail = 0;
	
	for(i=0; i<length; i++) fifo->buf[i] = 0;	
}

